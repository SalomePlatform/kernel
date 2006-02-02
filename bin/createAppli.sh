#!/bin/sh

comName=`which $0`
rep=`dirname $comName`

if [ $# -ne 1 ]
  echo "--- usage:"
  echo $0 AppliName
  echo "--- In which AppliName is a directory to create with SALOME application scripts"
elif [ -d $1]
  echo $1 "directory already exists, nothing done"
else
  mkdir -p $1
  cp -r $rep/appliskel/* $1
  cp $rep/appliskel/.bashrc $1
  chmod +x $1/*.sh $1/run* $1/envd
fi