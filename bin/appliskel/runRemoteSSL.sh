#!/bin/bash
# Copyright (C) 2021-2022  CEA/DEN, EDF R&D
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
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
# --- call example (from cli76cd to cli76ce):
# ssh cli76ce ${APPLI}/runRemoteSSL.sh IOR_OF_CURSTOM_NS SALOME_Container myContainerName
# --- local arguments
#     $0 : ${APPLI}/runRemote.sh: from arg name, rebuild and export $APPLI variable
#     --noenvd : optional argument. If present, the envd file is NOT sourced.
#     $1 : IOR_OF_CURSTOM_NS of reference to Engines::EmbeddedNamingService servant
#     $2 : WORKINGDIR (if $2 == WORKINDIR a working dir is given in $3. If not the working dir is $HOME)
#     $3 : if $2 == WORKINGDIR, the path to the workingdir
#     $4 (or $2 if no workingdir given) and following : local command to execute, with args
# --- retrieve APPLI path, relative to $HOME, set ${APPLI}

APPLI_HOME=$(dirname "$0")
export APPLI=$("${APPLI_HOME}/getAppliPath.py")

# --- set the SALOME environment (prerequisites, MODULES_ROOT_DIR...)
# Sourcing files with parameters works with bash, not with dash. This is why
# we must use bash for this script.

if [ "$1" == "--noenvd" ]
then
  shift
else
  . "${HOME}/${APPLI}/envd" "${HOME}/${APPLI}"
fi

#go to the requested working directory if any
if test "x$2" == "xWORKINGDIR"; then
  if test "x$3" = "x\$TEMPDIR"; then
    #create a temp working dir and change to it
    WDIR=$(mktemp -d) && {
      cd "$WDIR"
    }
  else
    if test -d "$3"; then
      #the dir exists, go to it
      cd "$3"
    else
      if test -a "$3"; then
        # It's a file do nothing
        echo "$3 is an existing file. Can't use it as a working directory"
      else
        #It does not exists, create it
        mkdir -p "$3" && {
          cd "$3"
        }
      fi
    fi
  fi
  shift 3
else
  shift 1
fi

# --- execute the command in the SALOME environment

"${KERNEL_ROOT_DIR}/bin/salome/envSalome.py" /bin/sh -c "$* "
