#!/bin/sh
#father script
echo "this is the father"
FILM="A Few Good Men"
echo "I like the film: $FILM"
#call the child script
./child.sh
echo "back to father"
echo "and the film is :$FILM"
