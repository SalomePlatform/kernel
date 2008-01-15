#! /bin/sh
status=1
while [ $status -ne 0 ]; do
  ls $HOME/$APPLI/.omniORB_last.cfg >& /dev/null
  status=$?
  sleep 1
  echo -n "#"
done
./runSession waitNS.py