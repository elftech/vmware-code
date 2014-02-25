#!/bin/bash
make>/dev/null
./simple_load
make clean>/dev/null
ls /dev/|grep simple
cat /proc/devices|grep simple
echo "*********************************************"
echo ' 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>

int main(int argc, char **argv)
{
//    char *fname="/dev/simpler";
    char *fname="/dev/simplen";
    FILE *f;
    unsigned long offset=0x10000, len=0x1000;
    void *address;

    if (!(f=fopen(fname,"r"))) {
        fprintf(stderr, "%s: %s: %s\n", argv[0], fname, strerror(errno));
        exit(1);
    }

    address=mmap(0, len, PROT_READ, MAP_FILE | MAP_PRIVATE, fileno(f), offset);

    if (address == (void *)-1) {
        fprintf(stderr,"%s: mmap(): %s\n",argv[0],strerror(errno));
        exit(1);
    }
    fclose(f);
    fprintf(stderr, "mapped \"%s\" from %lu (0x%08lx) to %lu (0x%08lx)\n",
            fname, offset, offset, offset+len, offset+len);

    fwrite(address, 1, len, stdout);
    return 0;
}
'>mmapDevSmple.c 
gcc mmapDevSmple.c -o mmapDevSmple.o
./mmapDevSmple.o | od -Ax -t x1
#OR:We can use the following exited file to test /dev/simpler
#gcc mmapDevSimple.c -o mmapDevSimple.o
#./mmapDevSimple.o | od -Ax -t x1
#rm mmapDevSimple.o 
echo "*******************************************************"
echo "CLEANING"
echo "*******************************************************"
rm mmapDevSmple.o mmapDevSmple.c
./simple_unload

cat /var/log/messages |tail -10
