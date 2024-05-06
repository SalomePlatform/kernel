#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2023-2024  CEA/DEN, EDF R&D
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
import os
import salome
import Engines
import pickle
import tempfile
import pylauncher

class TestProxy(unittest.TestCase):
    def testProxy(self):
        """
        [EDF27816] : This test checks two things :
        - Capability of ContainerManager to launch a Container with overriden environement
        - Management of proxy creation to manage big obj exchange between process (here between Container and the current process)
        """
        hostname = "localhost"
        cp = pylauncher.GetRequestForGiveContainer(hostname,"container_test")

        with tempfile.TemporaryDirectory() as tmpdirname:
            val_for_jj = "3333"
            val_for_big_obj = str( tmpdirname )
            val_for_thres = 100 # force proxy file
            # Override environement for all containers launched
            salome.cm.SetBigObjOnDiskDirectory(val_for_big_obj)
            salome.cm.SetBigObjOnDiskThreshold(val_for_thres)
            salome.cm.SetOverrideEnvForContainersSimple(env = [("jj",val_for_jj)])
            cont = salome.cm.GiveContainer(cp)
            ## Time to test it
            script_st = """import os
import KernelBasis
a = KernelBasis.GetBigObjOnDiskDirectory()
b = os.environ["jj"]
c = KernelBasis.GetBigObjOnDiskThreshold()
j = a,b,c"""
            pyscript = cont.createPyScriptNode("testScript",script_st)
            a,b,c = pickle.loads(pyscript.execute(["j"],pickle.dumps(([],{}))))[0]
            self.assertTrue( a == val_for_big_obj )
            self.assertTrue( b == val_for_jj )
            self.assertTrue( c == val_for_thres )
            # check environment using POSIX API in the container process
            for k,v in [("jj",val_for_jj)]:
                assert( {elt.key:elt.value.value() for elt in cont.get_os_environment()}[k] == v )
            #
            import SALOME_PyNode
            poa = salome.orb.resolve_initial_references("RootPOA")
            obj = SALOME_PyNode.SenderByte_i(poa,pickle.dumps( ([],{}) ))
            id_o = poa.activate_object(obj)
            refPtr = poa.id_to_reference(id_o)
            script_st2 = """ob = list( range(100) )""" # size of pickled ob must be greater than val_for_thres
            pyscript2 = cont.createPyScriptNode("testScript2",script_st2)
            pyscript2.executeFirst(refPtr)
            ret2 = pyscript2.executeSecond(["ob"])
            self.assertTrue( len(ret2) == 1)
            ret2 = ret2[0]
            ret3 = pickle.loads( SALOME_PyNode.SeqByteReceiver(ret2).data() )
            self.assertTrue( isinstance( ret3, SALOME_PyNode.BigObjectOnDiskList ) )
            self.assertTrue( val_for_big_obj == os.path.dirname( ret3.getFileName() ) )# very important part of test
            self.assertTrue( ret3.get() == list(range(100)) )
            fn = ret3.getFileName()
            self.assertTrue( os.path.exists( fn ) )
            ret3.unlinkOnDestructor()
            del ret3
            import gc
            gc.collect(0)
            self.assertTrue( not os.path.exists( fn ) ) # at destruction of ret3 the corresponding pckl file must be destructed
            cont.Shutdown()

    def testExecCodeAtInit(self):
        """
        [EDF28648] : allow initialisation script at startup
        """
        import os
        with tempfile.NamedTemporaryFile() as tmpFileName:
            tmpFileName.close()
            salome.cm.SetCodeOnContainerStartUp("""
with open("{}","w") as f:
  f.write("coucou")
""".format(tmpFileName.name) )# injection of python code expected to be executed at startup
            cp = pylauncher.GetRequestForGiveContainer("localhost","gg")
            cont = salome.cm.GiveContainer(cp) # code is expected to be executed in process abroad -> file is supposed to contain coucou
            cont.Shutdown()
            salome.cm.SetCodeOnContainerStartUp("") # no more startup code for other tests
            with open(tmpFileName.name,"r") as f:
                self.assertTrue(f.read()=="coucou")
            os.unlink( tmpFileName.name ) # context manager do not clean file

if __name__ == '__main__':
    salome.standalone()
    salome.salome_init()
    unittest.main()
