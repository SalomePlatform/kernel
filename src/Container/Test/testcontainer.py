#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2022  CEA/DEN, EDF R&D, OPEN CASCADE
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

import unittest
from os import getcwd
from Engines import ContainerParameters, ResourceParameters
import SALOME
import salome

from time import sleep

class TestResourceManager(unittest.TestCase):
    def getContainer(self, name):
        rp = ResourceParameters(name="localhost",
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
        cp = ContainerParameters(container_name=name,
                                 mode="start",
                                 workingdir=getcwd(),
                                 nb_proc=1,
                                 isMPI=False,
                                 parallelLib="",
                                 resource_params=rp)
        cm = salome.naming_service.Resolve("/ContainerManager")
        return cm.GiveContainer(cp)

    def checkLoads(self, cont, loads):
        self.assertEqual(len(loads), cont.getNumberOfCPUCores())
        for load in loads:
            self.assertTrue(0.0 <= load <= 1.0)

    def test1(self):
        # Check loadOfCPUCores
        cont = self.getContainer("test_container_1")
        loads1 = cont.loadOfCPUCores()
        self.checkLoads(cont, loads1)
        sleep(1)
        loads2 = cont.loadOfCPUCores()
        self.checkLoads(cont, loads2)
        self.assertNotEqual(loads1, loads2)
        cont.Shutdown()

    def test2(self):
        # Check custom script
        cont = self.getContainer("test_container_2")
        import multiprocessing as mp
        ref_load = [min(0.1*(i+1),1.0) for i in range(mp.cpu_count())]
        cont.setPyScriptForCPULoad('cpu_loads = {}'.format(ref_load))
        loads1 = cont.loadOfCPUCores()
        self.assertEqual(loads1, ref_load)
        cont.resetScriptForCPULoad()
        loads2 = cont.loadOfCPUCores()
        self.checkLoads(cont, loads2)
        cont.Shutdown()

    def test3(self):
        # Check bad script
        cont = self.getContainer("test_container_3")
        cont.setPyScriptForCPULoad("bla-bla-bla")
        self.assertRaises(Exception, cont.loadOfCPUCores)
        cont.Shutdown()

    def test4(self):
        # check memory sizes
        cont = self.getContainer("test_container_4")
        memory_total = cont.getTotalPhysicalMemory()
        memory_in_use = cont.getTotalPhysicalMemoryInUse()
        memory_by_me = cont.getTotalPhysicalMemoryInUseByMe()
        self.assertGreater(memory_total, memory_in_use)
        self.assertGreater(memory_in_use, memory_by_me)
        cont.Shutdown()
    
    def test5(self):
        """
        Test checking memory consumption of container
        """
        cont = self.getContainer("test_container_5")
        memory_by_me_start = cont.getTotalPhysicalMemoryInUseByMe()
        import pickle
        psn = cont.createPyScriptNode("n","""b = bytearray(10485760)""")# create 10MB byte array abroad
        psn.execute([],pickle.dumps(((),{})))
        memory_by_me_end = cont.getTotalPhysicalMemoryInUseByMe()
        self.assertGreater(memory_by_me_end,memory_by_me_start)
        self.assertIn(memory_by_me_end-memory_by_me_start,[10,11,12])# test elevation of memory
        cont.Shutdown()

if __name__ == '__main__':
    salome.standalone()
    salome.salome_init()
    unittest.main()
