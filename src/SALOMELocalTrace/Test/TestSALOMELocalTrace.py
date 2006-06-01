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

import sys, os,signal,string,commands
import runSalome

# get SALOME environment :
# here we need KERNEL_ROOT_DIR, PATH, LD_LIBRARY_PATH

args, modules_list, modules_root_dir = runSalome.get_config()
runSalome.set_env(args, modules_list, modules_root_dir)

# execute Unit Test

command = ['TestSALOMELocalTrace']
ret = os.spawnvp(os.P_WAIT, command[0], command)

# no process to kill
