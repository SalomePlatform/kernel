#!/bin/sh

#log files localization

BaseDir=/tmp
Username=`/usr/bin/whoami`

# kill OmniNames if exists

#killall -q -9 omniNames

# clear log files

mkdir -m 775 ${BaseDir}/logs 
mkdir ${BaseDir}/logs/${Username}
touch ${BaseDir}/logs/${Username}/dummy
\rm -f ${BaseDir}/logs/${Username}/omninames* ${BaseDir}/logs/${Username}/dummy ${BaseDir}/logs/${Username}/*.log

echo "Name Service... "

omniNames -start $1 -logdir ${BaseDir}/logs/${Username} &

# In LifeCycleCORBA, FactoryServer is started with rsh on the requested
#    computer if this Container does not exist. Default is localhost.
#    Others Containers are started with start_impl of FactoryServer Container.

# To start dynamically Containers on several computers you need to :
#1. define your ~/.omniORB.cfg file with a computer name and port number
# example : ORBInitRef NameService=corbaname::dm2s0017:1515
#2. start omninames with this port number in runNS.sh
# example : omniNames -start 1515 -logdir ${BaseDir}/logs/${Username} &

echo ok
echo "to list contexts and objects bound int the context with the specified name : showNS "
