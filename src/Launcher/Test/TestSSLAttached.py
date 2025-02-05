#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (C) 2021-2025  CEA, EDF
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

import os
import sys
import salome
import Engines

salome.salome_init_without_session()
rp=Engines.ResourceParameters(name="localhost",
                                hostname="localhost",
                                can_launch_batch_jobs=False,
                                can_run_containers=True,
                                OS="Linux",
                                componentList=[],
                                nb_proc=1,
                                mem_mb=1000,
                                cpu_clock=1000,
                                nb_node=1,
                                nb_proc_per_node=1,
                                policy="first",
                                resList=[])

cp=Engines.ContainerParameters(container_name="container_test",
                                 mode="start",
                                 workingdir=os.getcwd(),
                                 nb_proc=1,
                                 isMPI=False,
                                 parallelLib="",
                                 resource_params=rp)

cm=salome.naming_service.Resolve("/ContainerManager")

cont_prov=cm.GiveContainer(cp)
import CORBA
import pickle
orb=CORBA.ORB_init([''])

# Testing for CM
ref_cm = orb.object_to_string(salome.cm)
pyscript = cont_prov.createPyScriptNode("testCM","""import salome\nsalome.salome_init()\nthe_cm=salome.cm""")
pyscript.execute([],pickle.dumps(([],{})))
cm_to_test = orb.object_to_string(pickle.loads(pyscript.execute(["the_cm"],pickle.dumps(([],{}))))[0])
if cm_to_test!=ref_cm:
    raise AssertionError("The ContainerManager of salome_init launched in SALOME_Container_No_NS_Serv is not those in current process !")
pyscriptt = cont_prov.createPyScriptNode("testCM2","""import salome\nsalome.salome_init()\nthe_cm=salome.naming_service.Resolve("/ContainerManager")""")
pyscriptt.execute([],pickle.dumps(([],{})))
cm_to_testt = orb.object_to_string(pickle.loads(pyscriptt.execute(["the_cm"],pickle.dumps(([],{}))))[0])
if cm_to_testt!=ref_cm:
    raise AssertionError("The ContainerManager of salome_init launched in SALOME_Container_No_NS_Serv is not those in current process !")
del pyscript,pyscriptt,ref_cm,cm_to_test,cm_to_testt

# Testing for RM
ref_rm = orb.object_to_string(salome.lcc.getResourcesManager())
pyscript4 = cont_prov.createPyScriptNode("testRM","""import salome\nsalome.salome_init()\nthe_rm=salome.lcc.getResourcesManager()""")
pyscript4.execute([],pickle.dumps(([],{})))
rm_to_test = orb.object_to_string(pickle.loads(pyscript4.execute(["the_rm"],pickle.dumps(([],{}))))[0])
if rm_to_test!=ref_rm:
    raise AssertionError("The ResourcesManager of salome_init launched in SALOME_Container_No_NS_Serv is not those in current process !")
pyscript44 = cont_prov.createPyScriptNode("testRM2","""import salome\nsalome.salome_init()\nthe_rm=salome.naming_service.Resolve("/ResourcesManager")""")
pyscript44.execute([],pickle.dumps(([],{})))
rm_to_testt = orb.object_to_string(pickle.loads(pyscript44.execute(["the_rm"],pickle.dumps(([],{}))))[0])
if rm_to_testt!=ref_rm:
    raise AssertionError("The ResourcesManager of salome_init launched in SALOME_Container_No_NS_Serv is not those in current process !")
del pyscript4,pyscript44,ref_rm,rm_to_test,rm_to_testt

# Testing for DSM
ref_dsm = orb.object_to_string(salome.dsm)
pyscript2 = cont_prov.createPyScriptNode("testDSM","""import salome\nsalome.salome_init()\nthe_dsm=salome.dsm""")
dsm_to_test = orb.object_to_string(pickle.loads(pyscript2.execute(["the_dsm"],pickle.dumps(([],{}))))[0])
if dsm_to_test!=ref_dsm:
    raise AssertionError("The DataServerManager of salome_init launched in SALOME_Container_No_NS_Serv is not those in current process !")
pyscript22 = cont_prov.createPyScriptNode("testDSM","""import salome\nsalome.salome_init()\nthe_dsm=salome.naming_service.Resolve("/DataServerManager")""")
dsm_to_test_2 = orb.object_to_string(pickle.loads(pyscript22.execute(["the_dsm"],pickle.dumps(([],{}))))[0])
if dsm_to_test_2!=ref_dsm:
    raise AssertionError("The DataServerManager of salome_init launched in SALOME_Container_No_NS_Serv is not those in current process !")
del pyscript2,pyscript22,ref_dsm,dsm_to_test,dsm_to_test_2

# Testing for ESM
ref_esm = orb.object_to_string(salome.esm)
pyscript3 = cont_prov.createPyScriptNode("testESM","""import salome\nsalome.salome_init()\nthe_esm=salome.esm""")
esm_to_test = orb.object_to_string(pickle.loads(pyscript3.execute(["the_esm"],pickle.dumps(([],{}))))[0])
if esm_to_test!=ref_esm:
    raise AssertionError("The ExternalServerLauncher of salome_init launched in SALOME_Container_No_NS_Serv is not those in current process !")
pyscript33 = cont_prov.createPyScriptNode("testDSM","""import salome\nsalome.salome_init()\nthe_esm=salome.naming_service.Resolve("/ExternalServers")""")
esm_to_test_2 = orb.object_to_string(pickle.loads(pyscript33.execute(["the_esm"],pickle.dumps(([],{}))))[0])
if esm_to_test_2!=ref_esm:
    raise AssertionError("The ExternalServerLauncher of salome_init launched in SALOME_Container_No_NS_Serv is not those in current process !")
del pyscript3,pyscript33,ref_esm,esm_to_test,esm_to_test_2

# End of test
cont_prov.Shutdown()
