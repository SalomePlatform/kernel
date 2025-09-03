#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2023-2025  CEA/DEN, EDF R&D
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
from salome.kernel import salome
from salome.kernel import Engines
import pickle
import tempfile
from salome.kernel import pylauncher
from salome.kernel import KernelBasis

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
from salome.kernel import KernelBasis
_,a = KernelBasis.GetBigObjOnDiskProtocolAndDirectory()
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
            from salome.kernel import SALOME_PyNode
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
            self.assertTrue( val_for_big_obj == os.path.dirname( ret3.getFileName().getFileName() ) )# very important part of test
            self.assertTrue( ret3.get() == list(range(100)) )
            fn = ret3.getFileName().getFileName()
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

    def testWorkingDirectoryForward(self):
        """
        [EDF30062] test of forward of current directory overriding entry in Catalog
        """
        KernelBasis.SetForwardCurrentDirectoryStatus( True ) # key point
        hostname = "localhost"
        cp = pylauncher.GetRequestForGiveContainer(hostname,"container_test")

        with tempfile.TemporaryDirectory() as tmpdirname:
            os.chdir( str( tmpdirname ) )
            cp = pylauncher.GetRequestForGiveContainer("localhost","gg")
            cont = salome.cm.GiveContainer(cp)

            pyscript2 = cont.createPyScriptNode("testScript","""import os
ret = os.getcwd()
""")
            #
            from salome.kernel import SALOME_PyNode
            poa = salome.orb.resolve_initial_references("RootPOA")
            obj = SALOME_PyNode.SenderByte_i(poa,pickle.dumps( ([],{}) ))
            id_o = poa.activate_object(obj)
            refPtr = poa.id_to_reference(id_o)
            #
            pyscript2.executeFirst(refPtr)
            ret2 = pyscript2.executeSecond(["ret"])
            #
            ret2 = ret2[0]
            ret3 = pickle.loads( SALOME_PyNode.SeqByteReceiver(ret2).data() )
            self.assertEqual(ret3,str(tmpdirname)) # key point
            #
            cont.Shutdown()
        ################
        KernelBasis.SetForwardCurrentDirectoryStatus( False ) # key point
        cp = pylauncher.GetRequestForGiveContainer(hostname,"container_test")

        with tempfile.TemporaryDirectory() as tmpdirname:
            os.chdir( str( tmpdirname ) )
            cp = pylauncher.GetRequestForGiveContainer("localhost","gg")
            cont = salome.cm.GiveContainer(cp)

            pyscript2 = cont.createPyScriptNode("testScript","""import os
ret = os.getcwd()
""")
            #
            from salome.kernel import SALOME_PyNode
            poa = salome.orb.resolve_initial_references("RootPOA")
            obj = SALOME_PyNode.SenderByte_i(poa,pickle.dumps( ([],{}) ))
            id_o = poa.activate_object(obj)
            refPtr = poa.id_to_reference(id_o)
            #
            pyscript2.executeFirst(refPtr)
            ret2 = pyscript2.executeSecond(["ret"])
            #
            ret2 = ret2[0]
            ret3 = pickle.loads( SALOME_PyNode.SeqByteReceiver(ret2).data() )
            self.assertNotEqual(ret3,str(tmpdirname)) # key point
            #
            cont.Shutdown()

    def testAccessOfResourcesAcrossWorkers(self):
        """
        [EDF30157] To ease testing advanced configuration. This test checks that
        """
        ### start of catalog manipulation in memory
        rmcpp = pylauncher.RetrieveRMCppSingleton()
        res0 = pylauncher.CreateContainerResource("zFakeHost","/home/appli/fakeappli","ssh")
        rmcpp.AddResourceInCatalogNoQuestion(res0)#<- key point is here
        res1 = pylauncher.CreateContainerResource("zzFakeHost","/home/appli/zzfakeappli","ssh")
        rmcpp.AddResourceInCatalogNoQuestion(res1)#<- key point is here
        ### end of catalog manipulation in memory

        ### start to check effectivity of manipulation locally
        machines = salome.rm.ListAllResourceEntriesInCatalog()
        localStructure = { machine : salome.rm.GetResourceDefinition2( machine ) for machine in machines }
        ### end of check effectivity of manipulation locally

        cp = pylauncher.GetRequestForGiveContainer("localhost","gg")
        with salome.ContainerLauncherCM(cp) as cont:
            pyscript = cont.createPyScriptNode("testScript","""
from salome.kernel import salome
salome.salome_init()
machines = salome.rm.ListAllResourceEntriesInCatalog()
structure = { machine : salome.rm.GetResourceDefinition2( machine ) for machine in machines }
""") # retrieve the content remotely and then return it back to current process
            from salome.kernel import SALOME_PyNode
            import pickle
            poa = salome.orb.resolve_initial_references("RootPOA")
            obj = SALOME_PyNode.SenderByte_i(poa,pickle.dumps( ([],{}) ))
            id_o = poa.activate_object(obj)
            refPtr = poa.id_to_reference(id_o)
            #
            pyscript.executeFirst(refPtr)
            ret = pyscript.executeSecond(["structure"])
            ret = ret[0]
            retPy = pickle.loads( SALOME_PyNode.SeqByteReceiver(ret).data() )

            self.assertTrue( len(localStructure) == len(retPy) )
            self.assertTrue( "zFakeHost" in [elt for elt in localStructure])
            self.assertTrue( localStructure["zFakeHost"].applipath == "/home/appli/fakeappli")
            self.assertTrue( "zzFakeHost" in [elt for elt in localStructure])
            self.assertTrue( localStructure["zzFakeHost"].applipath == "/home/appli/zzfakeappli")
            for k in localStructure:
                a = pylauncher.FromEngineResourceDefinitionToCPP( localStructure[k] )
                self.assertTrue( isinstance(a,pylauncher.ResourceDefinition_cpp) )
                b = pylauncher.FromEngineResourceDefinitionToCPP( retPy[k] )
                self.assertTrue( isinstance(b,pylauncher.ResourceDefinition_cpp) )
                self.assertTrue( a==b ) #<- key point is here
                a1 = pylauncher.ToEngineResourceDefinitionFromCPP( a )
                b1 = pylauncher.ToEngineResourceDefinitionFromCPP( b )
                a2 = pylauncher.FromEngineResourceDefinitionToCPP( a1 )
                b2 = pylauncher.FromEngineResourceDefinitionToCPP( b1 )
                self.assertTrue( a2==b2 )

    def testMultiProcessCriticalSection0(self):
        """
        [EDF30382] : Synchro mecanism to ease test of extreme situations
        """
        from datetime import datetime
        from threading import Thread
        import contextlib
        from salome.kernel import SALOME_PyNode
        import time

        def func(pyscript,b):
            poa = salome.orb.resolve_initial_references("RootPOA")
            obj = SALOME_PyNode.SenderByte_i(poa,pickle.dumps( (["b"],{"b":b} ) )) ; id_o = poa.activate_object(obj) ; refPtr = poa.id_to_reference(id_o)
            pyscript.executeFirst(refPtr)
            ret = pyscript.executeSecond([])
            retPy = [ pickle.loads( SALOME_PyNode.SeqByteReceiver( elt ).data() ) for elt in ret]
            return retPy

        salome.salome_init()
        rmcpp = pylauncher.RetrieveRMCppSingleton()
        hostname = "localhost"
        cps = [ pylauncher.GetRequestForGiveContainer(hostname,contName) for contName in ["container_test_lock","container_test_lock_2"]]
        with contextlib.ExitStack() as stack:
            conts = [stack.enter_context(salome.ContainerLauncherCM(cp)) for cp in cps] # Context Manager to automatically cleanup launched containers
            b = salome.Barrier(3)
            pyscript = conts[0].createPyScriptNode("testScript","""
from datetime import datetime
from salome.kernel import salome
import time
time.sleep( 2.0 )
b.barrier()
print("go barrier")
print("after barrier T0 : {}".format(datetime.now()))
with salome.LockGuardCM("SSD"):
    print("Start CS T0 : {}".format(datetime.now()))
    time.sleep(5)
    print("End CS T0 : {}".format(datetime.now()))
""")
            pyscript2 = conts[1].createPyScriptNode("testScript","""
from datetime import datetime
from salome.kernel import salome
import time
time.sleep( 4.0 )
b.barrier()
print("go barrier")
print("after barrier T1 : {}".format(datetime.now()))
with salome.LockGuardCM("SSD"):
    print("Start CS T1 : {}".format(datetime.now()))
    time.sleep(5)
    print("End CS T1 : {}".format(datetime.now()))
""")
            ts = [Thread( target=func, args=(ps,b) ) for ps in [pyscript,pyscript2]]
            [t.start() for t in ts]
            st0 = datetime.now()
            time.sleep( 1.0 )
            b.barrier() # wait everybody
            print("go barrier")
            print("after barrier Master : {}".format(datetime.now()))
            with salome.LockGuardCM("SSD"):
                print("In critical section")
            [t.join() for t in ts]
            zedelt = datetime.now() - st0
            assert( zedelt.total_seconds() > 14.0 ) # expected max(1,2,4)+5+5 = 14s
            pass
    pass


if __name__ == '__main__':
    salome.standalone()
    salome.salome_init()
    unittest.main()
