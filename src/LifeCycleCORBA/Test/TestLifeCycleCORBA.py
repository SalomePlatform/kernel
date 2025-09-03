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
from salome.kernel import setenv_impl
from salome.kernel import orbmodule
from pathlib import Path
from salome.kernel import TestKiller
from salome.kernel import addToKillList_impl

# get SALOME environment :

args, modules_list, modules_root_dir = setenv_impl.get_config()
setenv_impl.set_env(args, modules_list, modules_root_dir)

# set environment for trace in logger
# (with file, servers may be killed before the write to the file...)

#os.environ["SALOME_trace"] = "file:/tmp/traceUnitTest.log"
#os.environ["SALOME_trace"] = "local"
os.environ["SALOME_trace"] = "with_logger"

# launch CORBA naming server

clt=orbmodule.client()

# launch CORBA logger server

myServer=runSalomeOld_impl.LoggerServer(args)
myServer.run()
clt.waitLogger("Logger")

# launch registry server

myServer=runSalomeOld_impl.RegistryServer(args)
myServer.run()
clt.waitNS("/Registry")

# launch module catalog server

cataServer=runSalomeOld_impl.CatalogServer(args)
cataServer.setpath(modules_list,modules_root_dir)
cataServer.run()
clt.waitNS("/Kernel/ModulCatalog")

# launch launcher server

myCmServer = runSalomeOld_impl.LauncherServer(args)
myCmServer.setpath(modules_list,modules_root_dir)
myCmServer.run()
clt.waitNS("/SalomeLauncher")

# execute Unit Test

command = ['./TestLifeCycleCORBA']
ret = subprocess.call(command)

# kill containers created by the Container Manager

from salome.kernel import Engines
launcher = clt.waitNS("/SalomeLauncher",Engines.SalomeLauncher)

launcher.Shutdown()

# kill Test process

addToKillList_impl.killList()

TestKiller.killProcess(runSalomeOld_impl.process_id)
TestKiller.closeSalome()
exit(ret)
