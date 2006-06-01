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

# --- retrieve APPLI path, relative to $HOME, set ${APPLI}
#     on sarge, "which" gives not allways the absolute path...
     
comName=`which $0`
aa=${comName:0:1}
if test x$aa == x\/; then
  mycom=${comName}
elif test x$aa == x\.; then
  mycom=${PWD}/${comName:2}
else
  mycom=${PWD}/${comName}
fi
APPLI=`echo ${HOME} \`dirname $mycom\` | awk ' { print substr($2,length($1)+2) } '`
#echo $APPLI
export APPLI
