#!/bin/bash
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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
# ssh cli76ce ${APPLI}/runRemote.sh cli76cd 2810 SALOME_Container myContainerName
# --- local arguments
#     $0 : ${APPLI}/runRemote.sh: from arg name, rebuild and export $APPLI variable
#     $1 : computer name for CORBA name service (where SALOME was launched)
#     $2 : port for CORBA name service
#     $3 : WORKINGDIR (if $3 == WORKINDIR a working dir is given in $4. If not the working dir is $HOME)
#     $4 : if $3 == WORKINGDIR, the path to the workingdir
#     $5 (or $3 if no workingdir given) and following : local command to execute, with args
# --- retrieve APPLI path, relative to $HOME, set ${APPLI}

APPLI_HOME=$(dirname "$0")
export APPLI=$("${APPLI_HOME}/getAppliPath.py")

# --- set the SALOME environment (prerequisites, MODULES_ROOT_DIR...)
# Sourcing files with parameters works with bash, not with dash. This is why
# we must use bash for this script.

. "${HOME}/${APPLI}/envd" "${HOME}/${APPLI}"

# --- set the OMNIORB_CONFIG file and environment relative to this run of SALOME
export NSHOST=$1
export NSPORT=$2

# Get (in bash) the configuration filename from (Python) ORBConfigFile return values
RETURN_VALUES=$("${KERNEL_ROOT_DIR}/bin/salome/envSalome.py" python "${KERNEL_ROOT_DIR}/bin/salome/ORBConfigFile.py" "${OMNIORB_USER_PATH}" "${NSHOST}" "${NSPORT}" "with_username=${USER}")
export OMNIORB_CONFIG=$(echo "${RETURN_VALUES}" | cut -d' ' -f1)

#go to the requested working directory if any
if test "x$3" == "xWORKINGDIR"; then
  if test "x$4" = "x\$TEMPDIR"; then
    #create a temp working dir and change to it
    WDIR=$(mktemp -d) && {
      cd "$WDIR"
    }
  else
    if test -d "$4"; then
      #the dir exists, go to it
      cd "$4"
    else
      if test -a "$4"; then
        # It's a file do nothing
        echo "$4 is an existing file. Can't use it as a working directory"
      else
        #It does not exists, create it
        mkdir -p "$4" && {
          cd "$4"
        }
      fi
    fi
  fi
  shift 4
else
  shift 2
fi

# --- execute the command in the SALOME environment


# suppress --rcfile option because of problem on Mandriva2006 - B Secher mai 2007
#${KERNEL_ROOT_DIR}/bin/salome/envSalome.py /bin/sh --rcfile $HOME/$APPLI/.bashrc -c "$*"
"${KERNEL_ROOT_DIR}/bin/salome/envSalome.py" /bin/sh -c "$*"
