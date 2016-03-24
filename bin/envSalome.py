#! /usr/bin/env python
#  -*- coding: iso-8859-1 -*-
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


###############################################
############### IMPORTANT NOTE ################
###############################################
# The runConsole script is obsolete.          #
# Please consider the new salome.py launcher. #
###############################################


import os
import sys
import setenv

kernel_root=os.getenv("KERNEL_ROOT_DIR")
kernel_root = os.path.realpath(kernel_root)
sys.path[:0]=[os.path.join(kernel_root,"bin","salome")]

argv = sys.argv[1:]
sys.argv = argv[1:]

if argv[0] == "-exec":
  #keep options only for execution
  del argv[0]
  sys.argv = []

setenv.main(True);

if sys.platform == "win32":
    if len(argv) >= 2:
        # Fix a problem of spaces in argv[1]
        argv[1] = '"%s"'%(argv[1])
        pass
    pass

os.execvp(argv[0],argv)
