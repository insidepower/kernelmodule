#!/bin/bash
# generate kernel header
# run this script inside kernel source Directory

#set -ex

PWDDIR=`pwd`
#LNX=`pwd | sed -r -e "s,[^/]+/,,g"`
#LNX=`echo $LNX | sed -r -e "s,[/],,g"`
#LNX=`echo $LNX | sed -r -e "s,linux,linux-headers,g"`

LNX=linux-headers-$(cat include/config/kernel.release)

TMPDIR="/tmp/$LNX"

if [ -a "$TMPDIR" ]; then
    echo -e "remove old linux headers...\n"
    rm -Rd $TMPDIR
fi

echo -e "copy files...\n"

mkdir $TMPDIR

cp .config $TMPDIR
cp Module.symvers $TMPDIR
cp .missing-syscalls.d $TMPDIR

cp -R scripts $TMPDIR

find .					\
    -name "include"			\
    -type d > _files

cp -R --parents `cat ./_files` $TMPDIR

find .					\
    -path "scripts" -prune -o		\
    -path "include" -prune -o		\
    -name "Kbuild*"			\
    -type f > _files

cp --parents `cat ./_files` $TMPDIR

find .					\
    -path "scripts" -prune -o		\
    -path "include" -prune -o		\
    -name "Kconfig*"			\
    -type f > _files

cp --parents `cat ./_files` $TMPDIR

find .					\
    -path "scripts" -prune -o		\
    -path "include" -prune -o		\
    -name "Makefile*"			\
    -type f > _files

cp --parents `cat ./_files` $TMPDIR

find .					\
    -path "scripts" -prune -o		\
    -path "include" -prune -o		\
    -name "*.lds"			\
    -type f > _files

cp --parents `cat ./_files` $TMPDIR

rm ./_files

(
echo -e "tar files...\n"
cd /tmp
tar cjf $PWDDIR/$LNX.tar.bz2 $LNX
)
