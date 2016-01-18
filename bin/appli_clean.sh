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

#clean appli

DELCOM="rm -rf bin lib share doc idl env.d envd USERS getAppliPath.py runRemote.sh SalomeApp.xml update_catalogs.py kill_remote_containers.py salome *.pyc *~ .bashrc"
DOIT="false"

if [ $# -gt 0 ]
then
  if [ "x$1" == "x-f" ]
  then
     DOIT="true"
  fi
fi

if [ $DOIT == "false" ]
then
  echo "Do you want to delete a SALOME application in this directory ? " `pwd`
  echo "command to execute: " $DELCOM
  echo "enter yes or no"
  read ANSWER
  echo "ANSWER="$ANSWER
  if [ "x$ANSWER" == "xyes" ]
  then
    DOIT="true"
  fi
fi

if [ $DOIT == "true" ]
then
  $DELCOM
fi
