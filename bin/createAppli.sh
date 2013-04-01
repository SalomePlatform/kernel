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

comName=`which $0`
rep=`dirname $comName`

if [ $# -ne 1 ]; then
  echo "--- usage:"
  echo $0 AppliName
  echo "--- In which AppliName is a directory to create with SALOME application scripts"
elif [ -d $1 ]; then
  echo $1 "directory already exists, nothing done"
else
  mkdir -p $1
  mkdir -p $1/USERS
  cp -r $rep/appliskel/* $1
  cp $rep/appliskel/.bashrc $1
  chmod +x $1/*.sh $1/run* $1/envd
fi
