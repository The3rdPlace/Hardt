#!/bin/bash

# Get credentials
if [ -f .publish ]; then
  . .publish
else
  echo "Please create the file .publish with this content:"
  echo "PUBUSER=you"
  echo "PUBPASS=pass"
  echo "PUBHOST=ftp.where.com"
  echo "PUBROOT=put/here"
  exit 1
fi

MAJOR=$(cat hardt/libhardt/include/hardt.h | grep MAJORVERSION | cut -d ' ' -f 3)
MINOR=$(cat hardt/libhardt/include/hardt.h | grep MINORVERSION | cut -d ' ' -f 3)
BUILD=$(cat hardt/libhardt/include/hardt.h | grep BUILDNO | cut -d ' ' -f 3)

# Archives
if [ -f ../dist/hardt_${MAJOR}.${MINOR}-${BUILD}_amd64.deb ]; then
  ncftpput -u $PUBUSER -p $PUBPASS $PUBHOST $PUBROOT/. ../dist/hardt_${MAJOR}.${MINOR}-${BUILD}_amd64.deb
fi
ncftpput -u $PUBUSER -p $PUBPASS $PUBHOST $PUBROOT/. ../dist/hardt_${MAJOR}.${MINOR}.orig.tar.gz

# Documentation
ncftpput -u $PUBUSER -p  $PUBPASS -R $PUBHOST $PUBROOT/. docs/html/*
