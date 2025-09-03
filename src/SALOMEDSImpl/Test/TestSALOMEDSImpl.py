#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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

import sys, os,signal,string,subprocess
import subprocess
from salome.kernel import runSalomeOld_impl
from salome.kernel import orbmodule
from salome.kernel import TestKiller
from salome.kernel import setenv_impl

# get SALOME environment :

args, modules_list, modules_root_dir = setenv_impl.get_config()
setenv_impl.set_env(args, modules_list, modules_root_dir)

# launch CORBA naming server

clt=orbmodule.client()

# launch CORBA logger server

myServer=runSalomeOld_impl.LoggerServer(args)
myServer.run()
clt.waitLogger("Logger")

# execute Unit Test

command = ['./TestSALOMEDSImpl']
ret = subprocess.call(command)

# kill Test process

TestKiller.killProcess(runSalomeOld_impl.process_id)
TestKiller.closeSalome()
exit(ret)
