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

omniNames -start -logdir ${BaseDir}/logs/${Username} &

# In LifeCycleCORBA, FactoryServer is started with rsh on the requested
#    computer if this Container does not exist. Default is localhost.
#    Others Containers are started with start_impl method of FactoryServer Container.
# For using rsh it is necessary to have in the ${HOME} directory a .rhosts file
# Warning : on RedHat the file /etc/hosts contains by default a line like :
# 127.0.0.1               bordolex bordolex.paris1.matra-dtv.fr localhost.localdomain localhost  
#   (bordolex is the station name). omniNames on bordolex will be accessible from other
#   computers only if the computer name is removed on that line like :
#   127.0.0.1               bordolex.paris1.matra-dtv.fr localhost.localdomain localhost

# To start dynamically Containers on several computers you need to
# put in the ${OMNIORB_CONFIG} file a computer name instead of "localhost"
# example : ORBInitRef NameService=corbaname::dm2s0017

# If you need to use several omniNames running on the same computer, you have to :
#1. put in your ${OMNIORB_CONFIG} file a computer name and port number
# example : ORBInitRef NameService=corbaname::dm2s0017:1515
#2. start omninames with this port number in runNS.sh
# example : omniNames -start 1515 -logdir ${BaseDir}/logs/${Username} &

echo ok
echo "to list contexts and objects bound int the context with the specified name : showNS "
