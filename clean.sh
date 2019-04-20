#!/bin/bash

rm -rf build/*
rm -rf lib/*
rm -rf bin/p*
<<<<<<< HEAD
rm -rf  MOOSLog_*  LOG_* XLOG_* ./MOOSLOG_* ./LOG_* ./XLOG_*
rm -f   *~ \#* targ_* *.moos++
rm -f   .LastOpenedMOOSLogDirectory

=======
>>>>>>> 693f7b764985091ea4ef263f3bcb5b162ba54cb7

find . -name '.DS_Store'  -print -exec rm -rfv {} \;
find . -name '*~'  -print -exec rm -rfv {} \;
find . -name '#*'  -print -exec rm -rfv {} \;
find . -name '*.moos++'  -print -exec rm -rfv {} \;

#find . -name 'MOOSLog*'  -print -exec rm -rfv {} \;

