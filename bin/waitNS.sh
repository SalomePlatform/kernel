#! /bin/sh
status=1
while [ $status -ne 0 ]; do
  ls $HOME/$APPLI/.omniORB_last.cfg >& /dev/null
  status=$?
  sleep 1
  echo -n "#"
done
portNumber=`grep InitRef $HOME/$APPLI/.omniORB_last.cfg | grep -v grep | awk 'BEGIN{FS=":"}{print $NF}'`
status=1
while [ $status -ne 0 ]; do
  ps -efw | grep $USER | grep omniNames | grep -v grep | grep $portNumber
  status=$?
  sleep 1
  echo -n "="
done
./runSession waitNS.py