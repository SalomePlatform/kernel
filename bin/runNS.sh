#!/bin/sh 
# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

#log files localization
#
BaseDir=/tmp
Username=`/usr/bin/whoami`

# kill OmniNames if exists

#killall -q -9 omniNames

# clear log files

mkdir -m 777 ${BaseDir}/logs 
mkdir ${BaseDir}/logs/${Username}
touch ${BaseDir}/logs/${Username}/dummy
\rm -f ${BaseDir}/logs/${Username}/omninames* ${BaseDir}/logs/${Username}/dummy ${BaseDir}/logs/${Username}/*.log

echo "Name Service... "
aSedCommand="s/.*NameService=corbaname::`hostname`:\([[:digit:]]*\)/\1/"
aPort=`cat $OMNIORB_CONFIG | grep NameService | sed -e"$aSedCommand"`
omniNames -start $aPort -logdir ${BaseDir}/logs/${Username} &

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
