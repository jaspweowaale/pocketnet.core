#!/bin/bash

git reset --hard
git clean -xdf --exclude=debian
git pull
dch -v 0.18.13
dh_make -p pocketnetcore_0.18.13 --createorig -c apache
dpkg-buildpackage