#!/bin/bash

# Copyright (C) 2005  OPEN CASCADE, CEA, EDF R&D, LEG
#           PRINCIPIA R&D, EADS CCR, Lip6, BV, CEDRAT
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either 
# version 2.1 of the License.
# 
# This library is distributed in the hope that it will be useful 
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
#
# --- run command in SALOME environment from remote call, ssh or rsh
#     - SALOME configuration is defined by :
#        - list of MODULE_ROOT_DIR
#        - prerequisite environment
#        - specific configuration for a particular SALOME application
#     - These elements of configuration are defined locally on each computer
#       in the directory APPLI:
#     - A SALOME application distributed on several computers needs APPLI
#       directories on the same path ($APPLI) relative to $HOME directory
#       of the user, on each computer.
#
# --- call example (from cli76cd to cli76ce):
# ssh cli76ce ${APPLI}/runRemote.sh cli76cd 2810 SALOME_Container myContainerName
#
# --- local arguments
#     $0 : ${APPLI}/runRemote.sh: from arg name, rebuild and export $APPLI variable
#     $1 : computer name for CORBA name service (where SALOME was launched)
#     $2 : port for CORBA name service
#     $3 : working directory
#     $4 and following : local command to execute, with args
#

# --- retrieve APPLI path, relative to $HOME, set ${APPLI}

APPLI_HOME=`dirname $0`
export APPLI=`${APPLI_HOME}/getAppliPath.py`

# --- set the SALOME environment (prerequisites, MODULES_ROOT_DIR...)

. ${HOME}/${APPLI}/envd ${HOME}/${APPLI}

# --- set the OMNIORB_CONFIG file and environment relative to this run of SALOME

OMNIORB_CONFIG=${HOME}/${APPLI}/.omniORB_$1_$2.cfg
export OMNIORB_CONFIG
NSHOST=$1
export NSHOST
NSPORT=$2
export NSPORT
initref="NameService=corbaname::"$1":$2"
echo "ORBInitRef $initref" > $OMNIORB_CONFIG

#go to the requested working directory if any
if test "x$3" != x; then
  if test "x$3" = "x\$TEMPDIR"; then
    #create a temp working dir and change to it
    WDIR=`mktemp -d` && {
      cd $WDIR
    }
  else
    if test -d $3; then
      #the dir exists, go to it
      cd $3
    else
      if test -a $3; then
        # It's a file do nothing
        echo $3 "is an existing file. Can't use it as a working directory"
      else
        #It does not exists, create it
        mkdir -p $3 && {
          cd $3
        }
      fi
    fi
  fi
fi

# --- execute the command in the SALOME environment

shift 3

# suppress --rcfile option because of problem on Mandriva2006 - B Secher mai 2007
#${KERNEL_ROOT_DIR}/bin/salome/envSalome.py /bin/sh --rcfile $HOME/$APPLI/.bashrc -c "$*"
${KERNEL_ROOT_DIR}/bin/salome/envSalome.py /bin/sh -c "$*"
