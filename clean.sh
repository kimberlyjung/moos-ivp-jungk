#!/bin/bash

rm -rf build/*
rm -rf lib/*
rm -rf bin/p*
rm -rf  MOOSLog_*  LOG_* XLOG_* ./MOOSLOG_* ./LOG_* ./XLOG_*
rm -rf   *~ \#* targ_* *.moos++
rm -rf   .LastOpenedMOOSLogDirectory
rm -rf *.alog *.blog *.xlog *.ylog *._bhv

find . -name '.DS_Store'  -print -exec rm -rfv {} \;
find . -name '*~'  -print -exec rm -rfv {} \;
find . -name '#*'  -print -exec rm -rfv {} \;
find . -name '*.moos++'  -print -exec rm -rfv {} \;
find . -name 'MOOSLog*'  -print -exec rm -rfv {} \;

