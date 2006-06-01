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
import orbmodule
import TestKiller
import addToKillList

# get SALOME environment :

args, modules_list, modules_root_dir = runSalome.get_config()
runSalome.set_env(args, modules_list, modules_root_dir)

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

# launch notify server

myServer=runSalome.NotifyServer(args,modules_root_dir)
myServer.run()

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

myCmServer = runSalome.ContainerManagerServer(args)
myCmServer.setpath(modules_list,modules_root_dir)
myCmServer.run()
clt.waitNS("/ContainerManager")

# execute Unit Test

command = ['TestLifeCycleCORBA']
ret = os.spawnvp(os.P_WAIT, command[0], command)

# kill containers created by the Container Manager

import Engines
containerManager = clt.waitNS("/ContainerManager",Engines.ContainerManager)
containerManager.Shutdown()

# kill Test process

addToKillList.killList()

TestKiller.killProcess(runSalome.process_id)
