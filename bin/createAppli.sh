#!/bin/sh

comName=`which $0`
rep=`dirname $comName`

cp -r $rep/appliskel $1
chmod +x $1/*.sh $1/run* $1/envd
