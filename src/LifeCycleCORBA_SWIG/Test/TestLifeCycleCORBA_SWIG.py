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

#  File   : TestLifeCycleCORBA_SWIG.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$
#
import sys, os,signal,string,commands
import subprocess
import runSalome
import setenv
import orbmodule
import TestKiller
import addToKillList

# get SALOME environment :

args, modules_list, modules_root_dir = setenv.get_config()
setenv.set_env(args, modules_list, modules_root_dir)

# set environment for trace in logger
# (with file, servers may be killed before the write to the file...)

#os.environ["SALOME_trace"] = "file:/tmp/traceUnitTest.log"
#os.environ["SALOME_trace"] = "local"
os.environ["SALOME_trace"] = "with_logger"

# launch CORBA naming server

clt=orbmodule.client()

# launch CORBA logger server

myServer=runSalome.LoggerServer(args)
myServer.run()
clt.waitLogger("Logger")

# launch registry server

myServer=runSalome.RegistryServer(args)
myServer.run()
clt.waitNS("/Registry")

# launch module catalog server

cataServer=runSalome.CatalogServer(args)
cataServer.setpath(modules_list,modules_root_dir)
cataServer.run()
clt.waitNS("/Kernel/ModulCatalog")

# launch container manager server

myCmServer = runSalome.LauncherServer(args)
myCmServer.setpath(modules_list,modules_root_dir)
myCmServer.run()
clt.waitNS("/SalomeLauncher")

# execute Unit Test

command = ['TestLifeCycleCORBA']
ret = subprocess.call(command)

import LifeCycleCORBA_SWIGTest
import unittest
unittest.TextTestRunner(verbosity=2).run(LifeCycleCORBA_SWIGTest.suite())

# kill containers created by the Container Manager

import Engines
launcher = clt.waitNS("/SalomeLauncher",Engines.SalomeLauncher)
launcher.Shutdown()

# kill Test process

addToKillList.killList()

TestKiller.killProcess(runSalome.process_id)
TestKiller.closeSalome()
exit(ret)
