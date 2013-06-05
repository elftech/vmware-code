"""The Ubuntu One Music Store Rhythmbox plugin."""
# Copyright (C) 2009 Canonical, Ltd.
#
# This library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License
# version 3.0 as published by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License version 3.0 for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library. If not, see
# <http://www.gnu.org/licenses/>.
#
# Authored by Stuart Langridge <stuart.langridge@canonical.com>

 
import gtk, gobject, os, urllib, gconf, stat, urlparse, gio
import gst, gst.pbutils
import aptdaemon.client
from aptdaemon.enums import *
from aptdaemon.gtkwidgets import AptErrorDialog, \
                                 AptProgressBar
import rb, rhythmdb
from ubuntuone.gtkwidgets import MusicStore as U1MusicStore
import xdg.BaseDirectory
import dbus.exceptions

import gettext
from gettext import lgettext as _
gettext.bindtextdomain("rhythmbox-ubuntuone-music-store", "/usr/share/locale")
gettext.textdomain("rhythmbox-ubuntuone-music-store")

MUSIC_STORE_WIDGET = U1MusicStore() # keep this around for later
U1LIBRARYPATH = MUSIC_STORE_WIDGET.get_library_location()
RB_LIBRARY_LOCATIONS = "/apps/rhythmbox/library_locations"
U1_CONFIG_PATH = "/apps/rhythmbox/plugins/umusicstore"
U1_FIRST_TIME_FLAG_ENTRY = U1_CONFIG_PATH + "/first_time_flag"

class U1MusicStorePlugin (rb.Plugin):
    """The Ubuntu One Music Store."""

    def __init__(self):
        rb.Plugin.__init__(self)

    def activate(self, shell):
        """Plugin startup."""
        self.db = shell.get_property("db")
        group = rb.rb_source_group_get_by_name ("stores")
        if not group:
            group = rb.rb_source_group_register ("stores",
                                           "Stores",
                                           rb.SOURCE_GROUP_CATEGORY_FIXED)
        
        icon_file_name = self.find_file("musicstore_icon.png")
        icon = gtk.gdk.pixbuf_new_from_file_at_size(icon_file_name, 22, 22)
        self.entry_type = self.db.entry_register_type("U1EntryType")
        self.source = gobject.new (U1Source,
                                   shell=shell,
                                   entry_type=self.entry_type,
                                   source_group=group,
                                   icon=icon,
                                   plugin=self)
        shell.register_entry_type_for_source(self.source, self.entry_type)
        self.shell = shell
        self.source.connect("preview-mp3", self.play_preview_mp3)
        self.source.connect("play-library", self.play_library)
        self.source.connect("download-finished", self.download_finished)
        self.source.connect("url-loaded", self.url_loaded)
            
        # Do these every time
        shell.append_source(self.source, None) # Add the source to the list
        self.add_u1_library()

	# Select the source if it's the first time
        conf_client = gconf.client_get_default ()
        if not conf_client.get_bool(U1_FIRST_TIME_FLAG_ENTRY):
            shell.props.sourcelist.select(self.source)
            conf_client.set_bool(U1_FIRST_TIME_FLAG_ENTRY, True)

    def deactivate(self, shell):
        """Plugin shutdown."""
        # remove source
        self.source.delete_thyself()
        # remove the library, if it's empty
        try:
            filecount = len(os.listdir(self.U1_LIBRARY_SYMLINK))
        except OSError:
            # symlink is dangling
            # so they never downloaded anything
            filecount = 0
        if filecount == 0:
            client = gconf.client_get_default()
            libraries = client.get_list(RB_LIBRARY_LOCATIONS, gconf.VALUE_STRING)
            if self.u1_library_path_url in libraries:
                client.notify_remove(self.library_adder)
                libraries.remove(self.u1_library_path_url)
                client.set_list(RB_LIBRARY_LOCATIONS, gconf.VALUE_STRING, libraries)
        # delete held references
        del self.db
        del self.source
        del self.shell

    def url_loaded(self, source, url):
        """A URL is loaded in the plugin"""
        print "URL loaded:", url
        if urlparse.urlparse(url).scheme == "https":
            pass
        else:
            pass
    
    def _udf_path_to_library_uri(self, path):
        """Calculate the path in the library. 
        Since the library is accessed via the created symlink, but the path 
        passed to us is to a file in the actual music store UDF, we need to 
        work out the path inside the library, i.e., what the path to that file
        is via the symlink."""
        
        if path.startswith(U1LIBRARYPATH):
            subpath = path[len(U1LIBRARYPATH):]
        else:
            subpath = path
        if subpath.startswith("/"): subpath = subpath[1:]
        library_path = os.path.join(self.U1_LIBRARY_SYMLINK, subpath)
        # convert path to URI. Don't use urllib for this; Python and
        # glib escape URLs differently. gio does it the glib way.
        library_uri = gio.File(library_path).get_uri()
        return library_uri

    def download_finished(self, source, path):
        """A file is finished downloading"""
        library_uri = self._udf_path_to_library_uri(path)
        # Import the URI
        if not self.shell.props.db.entry_lookup_by_location(library_uri):
            self.db.add_uri(library_uri)

    def play_library(self, source, path):
        """Switch to and start playing a song from the library"""
        uri = self._udf_path_to_library_uri(path)
        entry = self.shell.props.db.entry_lookup_by_location(uri)
        if not entry: 
            print "couldn't find entry", uri
            return
        libsrc = self.shell.props.library_source
        genre_view, artist_view, album_view = libsrc.get_property_views()
        song_view = libsrc.get_entry_view()
        artist = self.shell.props.db.entry_get(entry, rhythmdb.PROP_ARTIST)
        album = self.shell.props.db.entry_get(entry, rhythmdb.PROP_ALBUM)
        self.shell.props.sourcelist.select(libsrc)
        artist_view.set_selection([artist])
        album_view.set_selection([album])
        song_view.scroll_to_entry(entry)
        player = self.shell.get_player()
        player.stop()
        player.play()
        player.play_entry(entry, libsrc)

    def play_preview_mp3(self, source, url, title):
        """Play a passed mp3; signal handler for preview-mp3 signal."""
        # create an entry, don't save it, and play it
        entry = self.shell.props.db.entry_lookup_by_location(url)
        if entry is None:
            entry = self.shell.props.db.entry_new(self.entry_type, url)
        self.shell.props.db.set(entry, rhythmdb.PROP_TITLE, title)
        player = self.shell.get_player()
        player.stop()
        player.play()
        player.play_entry(entry, self.source)
        # FIXME delete this entry when it finishes playing. Don't know how yet.

    def add_u1_library(self):
        """Add the U1 library if not listed in RB and re-add if changed."""
        u1_library_path_folder = xdg.BaseDirectory.save_data_path("ubuntuone")
        # Ensure that we can write to the folder, because syncdaemon creates it
        # with no write permissions
        os.chmod(u1_library_path_folder,
            os.stat(u1_library_path_folder)[stat.ST_MODE] | stat.S_IWUSR)
        # Translators: this is the name under Music for U1 music in Rhythmbox
        u1_library_path = os.path.join(u1_library_path_folder, _("Purchased from Ubuntu One"))
        if not os.path.islink(u1_library_path):
            if not os.path.exists(u1_library_path):
                print "Attempting to symlink %s to %s" % (U1LIBRARYPATH, 
                    u1_library_path)
                os.symlink(U1LIBRARYPATH, u1_library_path)
            else:
                # something that isn't a symlink already exists. That's not
                # supposed to happen.
                # Write a warning and carry on.
                print ("Warning: library location %s existed. It should have "
                    "been a symlink to %s, and it wasn't. This isn't supposed "
                    "to happen. Carrying on anyway, on the assumption that "
                    "you know what you're doing. If this is a problem, then "
                    "delete or rename %s.") % (u1_library_path, U1LIBRARYPATH,
                    u1_library_path)
        self.u1_library_path_url = "file://%s" % urllib.quote(u1_library_path)
        self.U1_LIBRARY_SYMLINK = u1_library_path
        client = gconf.client_get_default()
        self._add_u1_library_if_not_present(client)
        self._remove_old_u1_library_if_present(client)
        # Watch for changes to the gconf key and re-add the library
        self.library_adder = client.notify_add(RB_LIBRARY_LOCATIONS, 
            self._add_u1_library_if_not_present)

    def _add_u1_library_if_not_present(self, client, *args, **kwargs):
        """Check for the U1 library and add to libraries list."""
        libraries = client.get_list(RB_LIBRARY_LOCATIONS, gconf.VALUE_STRING)
        if self.u1_library_path_url not in libraries:
            libraries.append(self.u1_library_path_url)
            client.set_list(RB_LIBRARY_LOCATIONS, gconf.VALUE_STRING, libraries)

    def _remove_old_u1_library_if_present(self, client, *args, **kwargs):
        """Check for the old U1 library and remove it from libraries list."""
        libraries = client.get_list(RB_LIBRARY_LOCATIONS, gconf.VALUE_STRING)
        removed = False
        # originally, this was the library path. Someone might have this.
        old_path = "file://%s" % os.path.expanduser("~/.ubuntuone/musicstore")
        if old_path in libraries:
            libraries.remove(old_path)
            removed = True
        # Then, this was the library path, which we put into gconf unescaped
        actual_udf_path_unescaped = "file://%s" % U1LIBRARYPATH
        if actual_udf_path_unescaped in libraries:
            libraries.remove(actual_udf_path_unescaped)
            removed = True
        # In theory, no-one should have the escaped path, but let's check
        actual_udf_path_escaped = "file://%s" % U1LIBRARYPATH
        if actual_udf_path_escaped in libraries:
            libraries.remove(actual_udf_path_escaped)
            removed = True
        # Also, remove any library which is in .local/share and *isn't*
        # the current library. This caters for people who got the library
        # created under one name (say, English) and then had it created
        # under another (say, after a translation to Dutch)
        u1_library_path_folder = xdg.BaseDirectory.save_data_path("ubuntuone")
        u1_library_path_folder_url = "file://%s" % urllib.quote(u1_library_path_folder)
        symlink_url = "file://" + urllib.quote(self.U1_LIBRARY_SYMLINK)
        for l in libraries:
            if l.startswith(u1_library_path_folder_url) and l != symlink_url:
                libraries.remove(l)
                # and delete the symlink itself
                symlink_to_remove = urllib.unquote(l[7:])
                os.unlink(symlink_to_remove)
                removed = True
        if removed:
            client.set_list(RB_LIBRARY_LOCATIONS, gconf.VALUE_STRING, libraries)

class U1Source(rb.Source):
    """A Rhythmbox source widget for the U1 Music Store."""
    # gproperties required so that rb.Source is instantiable
    __gproperties__ = {
      'plugin': (rb.Plugin, 'plugin', 'plugin', 
                    gobject.PARAM_WRITABLE|gobject.PARAM_CONSTRUCT_ONLY),
    }
    # we have the preview-mp3 signal; we receive it from the widget, and re-emit
    # it so that the plugin gets it, because the plugin actually plays the mp3
    __gsignals__ = {
      "preview-mp3": (gobject.SIGNAL_RUN_FIRST, gobject.TYPE_NONE, (str, str)),
      "play-library": (gobject.SIGNAL_RUN_FIRST, gobject.TYPE_NONE, (str,)),
      "download-finished": (gobject.SIGNAL_RUN_FIRST, gobject.TYPE_NONE, (str,)),
      "url-loaded": (gobject.SIGNAL_RUN_FIRST, gobject.TYPE_NONE, (str,)),
    }
    
    def __init__(self):
        rb.Source.__init__(self, name=_("Ubuntu One"))
        self.__activated = False

    def do_impl_activate(self):
        """Source startup."""
        if self.__activated: return
        self.__activated = True
        self.test_can_play_mp3()
        rb.Source.do_impl_activate (self)

    def test_can_play_mp3(self):
        """Can the user play mp3s? Start a GStreamer pipeline to check."""
        mp3pth = os.path.realpath(os.path.join(
            os.path.split(__file__)[0], "empty.mp3"))
        uri = "file://%s" % urllib.quote("%s" % mp3pth)
        self.install_pipeline = gst.parse_launch(
          'uridecodebin uri=%s ! fakesink' % uri)
        bus = self.install_pipeline.get_bus()
        bus.add_signal_watch()
        bus.connect("message::element", self._got_element_msg)
        bus.connect("message::eos", self._got_end_of_stream)
        self.install_pipeline.set_state(gst.STATE_PLAYING)

    def _got_element_msg(self, bus, msg):
        """Handler for element messages from the check-mp3 pipeline.
           GStreamer throws a "plugin-missing" element message if the
           user does not have the right codecs to play a file."""
        plugin_missing = gst.pbutils.is_missing_plugin_message(msg)
        if plugin_missing:
            self.install_pipeline.set_state(gst.STATE_NULL)
            self.install_mp3_playback()

    def _got_end_of_stream(self, bus, msg):
        """Handler for end of stream from the check-mp3 pipeline.
           If we reach the end of the stream, mp3 playback is enabled."""
        self.install_pipeline.set_state(gst.STATE_NULL)
        if os.environ.has_key("U1INSTALLMP3ANYWAY"):
            # override the decision not to install the package
            self.install_mp3_playback()
        else:
            self.add_music_store_widget()

    def install_mp3_playback(self):
        """Use aptdaemon to install the Fluendo mp3 playback codec package."""
        self.install_box = gtk.Alignment(xscale=0.0, yscale=0.0, xalign=0.5,
            yalign=0.5)
        self.install_vbox = gtk.VBox()
        self.install_label_head = gtk.Label()
        self.install_label_head.set_use_markup(True)
        not_installed = _("MP3 plugins are not installed")
        self.install_label_head.set_markup('<span weight="bold" size="larger">'
            '%s</span>' % not_installed)
        self.install_label_head.set_alignment(0.0, 0.5)
        self.install_label_body = gtk.Label()
        self.install_label_body.set_text(_('To listen to your purchased songs'
            ', you need to install MP3 plugins. Click below to install them.'))
        self.install_label_body.set_alignment(0.0, 0.5)
        self.install_label_eula = gtk.Label()
        # EULA text copied from /var/lib/dpkg/info/gstreamer0.10-fluendo-plugins-mp3-partner.templates
        # The partner package shows the EULA itself on installations, but
        #  aptdaemon installations work like DEBIAN_FRONTEND=noninteractive
        # so we show the EULA here. (This also avoids a popup window.)
        # EULA text is not translatable; do not wrap it with gettext!
        self.install_label_eula.set_markup(
            "<small>MPEG Layer-3 audio decoding technology notice\n"
            "MPEG Layer-3 audio decoding technology licensed "
            "from Fraunhofer IIS and Thomson\n"
            "This product cannot be installed in product other than Personal "
            "Computers sold for general purpose usage, and not for set-top "
            "boxes, embedded PC, PC which are sold and customized for "
            "mainly audio or multimedia playback and/or registration, "
            "unless the seller has received a license by Fraunhofer IIS"
            "and Thomson and pay the relevant royalties to them.</small>")
        self.install_label_eula.set_alignment(0.0, 0.5)
        self.install_label_eula.set_size_request(400,200)
        self.install_label_eula.set_line_wrap(True)
        self.install_hbtn = gtk.HButtonBox()
        self.install_hbtn.set_layout(gtk.BUTTONBOX_END)
        self.install_button = gtk.Button(label=_("Install MP3 plugins"))
        self.install_button.connect("clicked", self._start_mp3_install)
        self.install_hbtn.add(self.install_button)
        self.install_vbox.pack_start(self.install_label_head, expand=False)
        self.install_vbox.pack_start(self.install_label_body, expand=False,
            padding=12)
        self.install_vbox.pack_start(self.install_hbtn, expand=False)
        self.install_vbox.pack_start(self.install_label_eula, expand=False,
            padding=12)
        self.install_box.add(self.install_vbox)
        self.install_box.show_all()
        self.add(self.install_box)

    def _start_mp3_install(self, btn):
        """Add the 'partner' repository and update the package list from it."""
        self.ac = aptdaemon.client.AptClient()
        try:
            self.ac.add_repository("deb","http://archive.canonical.com/", "lucid", ["partner"])
        except dbus.exceptions.DBusException, e:
            if e.get_dbus_name() == "org.freedesktop.PolicyKit.Error.NotAuthorized":
                # user cancelled, so exit from here so they can press the button again
                return
        self.ac.update_cache(reply_handler=self._finish_updating_packages,
                        error_handler=self._on_error)
    
    def _finish_updating_packages(self, transaction):
        """Now that partner is added, install our mp3 codec package."""
        self.update_progress = AptProgressBar(transaction)
        self.update_progress.show()
        self.install_label_head.set_text("")
        self.install_label_body.set_text(_("Finding MP3 plugins"))
        self.install_label_eula.hide()
        self.install_hbtn.hide()
        self.install_vbox.pack_start(self.update_progress, expand=False)
        transaction.run(reply_handler=lambda: True, 
                        error_handler=self._on_error)
        self.ac.install_packages(["gstreamer0.10-fluendo-plugins-mp3-partner"],
                        reply_handler=self._run_transaction,
                        error_handler=self._on_error)

    def _run_transaction(self, transaction):
        """Show progress of aptdaemon package installation."""
        self.update_progress.hide()
        self.install_progress = AptProgressBar(transaction)
        self.install_progress.show()
        self.install_label_head.set_text("")
        self.install_label_body.set_text(_("Installing MP3 plugins"))
        self.install_label_eula.hide()
        self.install_hbtn.hide()
        self.install_vbox.pack_start(self.install_progress, expand=False)
        transaction.run(reply_handler=lambda: True, 
                        error_handler=self._on_error)
        transaction.connect("finished", self._finished)

    def _finished(self, trans, exit_code):
        """Aptdaemon package installation finished; show music store."""
        if exit_code == 0 or exit_code == 2: # 0: success, 2: already installed
            self.remove(self.install_box)
            gst.update_registry()
            self.add_music_store_widget()
        else:
            self._on_error("Could not find the "
                "gstreamer0.10-fluendo-plugins-mp3-partner package.")

    def _on_error(self, error):
        """Error handler for aptdaemon."""
        print error
        problem_installing = _("There was a problem installing, sorry")
        self.install_label_head.set_markup('<span weight="bold" size="larger">'
            '%s</span>' % problem_installing)
        self.install_label_body.set_text(_('Check your internet connection and '
            'try again.'))
        if getattr(self, "install_progress"):
            self.install_progress.hide()
        self.install_hbtn.show()

    def add_music_store_widget(self):
        """Display the music store widget in Rhythmbox."""
        self.browser = MUSIC_STORE_WIDGET
        self.add(self.browser)
        self.show_all()
        self.browser.set_no_show_all(True)
        self.browser.set_property("visible", True)
        self.browser.connect("preview-mp3", self.re_emit_preview)
        self.browser.connect("play-library", self.re_emit_playlibrary)
        self.browser.connect("download-finished", self.re_emit_downloadfinished)
        self.browser.connect("url-loaded", self.re_emit_urlloaded)

    def do_impl_can_pause(self): 
        """Implementation can pause.
           If we don't handle this, Rhythmbox segfaults."""
        return True # so we can pause, else we segfault

    def re_emit_preview(self, widget, url, title):
        """Handle the preview-mp3 signal and re-emit it to the rb.Plugin."""
        self.emit("preview-mp3", url, title)

    def re_emit_playlibrary(self, widget, path):
        """Handle the play-library signal and re-emit it to the rb.Plugin."""
        self.emit("play-library", path)

    def re_emit_downloadfinished(self, widget, path):
        """Handle the download-finished signal and re-emit it to the rb.Plugin."""
        self.emit("download-finished", path)

    def re_emit_urlloaded(self, widget, url):
        """Handle the url-loaded signal and re-emit it to the rb.Plugin."""
        self.emit("url-loaded", url)

    def do_set_property(self, property, value):
        """Allow property settings to handle the plugin call."""
        if property.name == 'plugin':
            self.__plugin = value
        else:
            raise AttributeError, 'unknown property %s' % property.name


