#!/usr/bin/env python

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

import os
import sys
import setenv

kernel_root=os.getenv("KERNEL_ROOT_DIR")
sys.path[:0]=[kernel_root+"/bin/salome"]

#import runSalome

argv = sys.argv[1:]
sys.argv = argv[1:]

#sys.argv = [sys.argv[0]]
#if len(argv) > 3:
#    sys.argv += argv[3:]
#if len(argv) > 2:
#    sys.argv = argv[2:]
    
#args, modules_list, modules_root_dir = setenv.get_config()
#runSalome.set_env(args, modules_list, modules_root_dir)
setenv.main();

os.execvp(argv[0],argv)
