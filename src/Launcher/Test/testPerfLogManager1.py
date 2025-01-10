#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2024-2025  CEA/DEN, EDF R&D
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
import pylauncher
import SALOME_PyNode
import KernelBasis

import glob
import pickle
import tempfile
import logging
from datetime import datetime

def flush():
    import sys
    sys.stdout.flush()
    sys.stderr.flush()

def unProxyfy( objs ):
    """
    objs is a list of SenderByte
    """
    ret = []
    fileNamesProxyOut = []
    for ret2 in objs:
        ret3 = pickle.loads( SALOME_PyNode.SeqByteReceiver(ret2).data() )
        ret4 = ret3
        fileNameProxyOut = None
        if SALOME_PyNode.GetBigObjectOnDiskThreshold() != -1:
            if isinstance( ret3, SALOME_PyNode.BigObjectOnDiskBase ):
                ret4 = ret3.get()
            else:
                raise RuntimeError("Oooops")
            ret3.unlinkOnDestructor()
            fileNameProxyOut = ret3.getFileName()
        logging.debug("Prxy file : {}".format( fileNameProxyOut ))
        ret.append( ret3 )
        fileNamesProxyOut.append( fileNameProxyOut )
    return ret, fileNamesProxyOut

class testPerfLogManager1(unittest.TestCase):
    def testPerfLogManager0(self):
        """
        [EDF29150] : This test checks measure performance methods
        """
        hostname = "localhost"
        cp = pylauncher.GetRequestForGiveContainer(hostname,"container_test")
        salome.logm.clear()
        #PROXY_THRES = "-1"
        PROXY_THRES = 1
        with SALOME_PyNode.GenericPythonMonitoringLauncherCtxMgr( SALOME_PyNode.FileSystemMonitoring(1000,os.path.dirname( salome.__file__ )) ) as monitoringParamsForFileMaster:
            with SALOME_PyNode.GenericPythonMonitoringLauncherCtxMgr( SALOME_PyNode.CPUMemoryMonitoring(1000) ) as monitoringParamsMaster:
                with tempfile.TemporaryDirectory() as tmpdirnameMonitoring:
                    monitoringFile = os.path.join( str( tmpdirnameMonitoring ), "zeHtop.pckl" )
                    monitoringFileTwo = os.path.join( str( tmpdirnameMonitoring ), "zeHtopTwo.pckl" )
                    iorLogm = os.path.join( str( tmpdirnameMonitoring ), "logm.ior" )
                    with open(iorLogm,"w") as f:
                        f.write( salome.orb.object_to_string(salome.logm) )
                    logging.debug("Monitoring file : {}".format(monitoringFile))
                    with tempfile.TemporaryDirectory() as tmpdirname:
                        with salome.LogManagerLaunchMonitoringFileCtxMgr(250,monitoringFile) as monitoringParams:
                            pyFileContainingCodeOfMonitoring = monitoringParams.pyFileName.filename
                            logging.debug("Python file containing code of monitoring : {}".format(pyFileContainingCodeOfMonitoring))
                            val_for_big_obj = str( tmpdirname )
                            # Override environement for all containers launched
                            salome.cm.SetBigObjOnDiskDirectory(val_for_big_obj)
                            salome.cm.SetBigObjOnDiskThreshold(PROXY_THRES)
                            salome.cm.SetOverrideEnvForContainersSimple(env = [])
                            salome.cm.SetDeltaTimeBetweenCPUMemMeasureInMilliSecond( 250 )
                            cont = salome.cm.GiveContainer(cp)
                            logging.debug("{} {}".format(40*"*",cont.getPID()))
                            script_st = """
import logging
import sys
import KernelBasis
from datetime import datetime
cst = KernelBasis.GetTimeAdjustmentCst()
logging.debug("constant = {}".format(cst))
nbcore = 3
my_log_4_this_session.addFreestyleAndFlush( ("a",datetime.now()) ) # exemple of custom tracking
print("coucou {} {}".format(len(zeinput0),len(zeinput1)))
logging.debug("debug or not debug")
ob = [ [ bytes(3000000) ] ]
pihm, ts = KernelBasis.HeatMarcel(1 * nbcore * cst,nbcore)
print("Time ellapse spent : {} s".format(ts))
ob2 = [ [ bytes(100000) ] ]
pihm, ts = KernelBasis.HeatMarcel(1 * nbcore * cst,nbcore)
print("Time ellapse spent : {} s".format(ts))
sys.stderr.write("fake error message\\n")
"""
                            poa = salome.orb.resolve_initial_references("RootPOA")
                            zeinput0 = [ bytes(100000000) ]
                            if SALOME_PyNode.GetBigObjectOnDiskThreshold() != -1:
                                zeinput0 = SALOME_PyNode.ProxyfyPickeled( zeinput0 )
                                zeinput0.unlinkOnDestructor()
                            obj = SALOME_PyNode.SenderByte_i(poa,pickle.dumps( (["zeinput0"],{"zeinput0": [zeinput0], "zeinput1": [ [zeinput0], [zeinput0] ] }) ))
                            id_o = poa.activate_object(obj)
                            refPtr = poa.id_to_reference(id_o)
                            pyscript2 = cont.createPyScriptNode("testScript2",script_st)
                            pyscript2.executeFirst(refPtr)
                            ret2 = pyscript2.executeSecond(["ob","ob2"])# generate a DeprecationWarning: PY_SSIZE_T_CLEAN will be required for '#' formats on debian11 ?
                            ret3, fileNamesProxyOut = unProxyfy( ret2 )
                            logging.getLogger().debug("test logging 1")
                            logging.debug("test logging 2")
                            logging.debug( salome.orb.object_to_string( salome.logm ) )
                            a = salome.logm.NaiveFetch()
                            logging.debug(a)
                            fs = a[0][1][0].get().freestyle
                            self.assertEqual(len(fs),1)
                            self.assertEqual(fs[0][0],"a")
                            self.assertTrue( isinstance( fs[0][1], datetime ) )
                            logging.debug(a[0][1][0])
                            logging.debug( a[0][1][0].get()._input_hdd_mem._data[0]._data[0]._hdd_mem ) # important
                            logging.debug( a[0][1][0].get()._input_hdd_mem._data[1]._data[0]._data[0]._hdd_mem ) # important
                            fileNameProxyIn = a[0][1][0].get()._input_hdd_mem._data[0]._data[0]._file_name
                            logging.debug( fileNameProxyIn )
                            del zeinput0
                            del ret3
                            import gc ; gc.collect()
                            if fileNameProxyIn is not None:
                                if os.path.exists(fileNameProxyIn):
                                    raise RuntimeError("Oooops 2")
                            for fileNameProxyOut in fileNamesProxyOut:
                                if fileNameProxyOut is not None:
                                    if os.path.exists(fileNameProxyOut.getFileName()):
                                        raise RuntimeError("Oooops 3")
                            # execution #2 inside last
                            script_st2 = """
import logging
b = 7+a
logging.debug("Execution 2")
import time
time.sleep(1)
"""
                            obj2 = SALOME_PyNode.SenderByte_i(poa,pickle.dumps((["a"],{"a":3})))
                            id2_o = poa.activate_object(obj2)
                            refPtr2 = poa.id_to_reference(id2_o)
                            pyscript2.assignNewCompiledCode(script_st2)
                            pyscript2.executeFirst(refPtr2)
                            ret2_0 = pyscript2.executeSecond(["b"])
                            ret2_1, fileNamesProxyOut2 = unProxyfy( ret2_0 )
                            logging.debug( fileNamesProxyOut2 )
                            a = salome.logm.NaiveFetch()
                            del ret2_1
                            import gc ; gc.collect()
                            for fileNameProxyOut in fileNamesProxyOut2:
                                if fileNameProxyOut is not None:
                                    if os.path.exists(fileNameProxyOut.getFileName()):
                                        raise RuntimeError("Oooops 3")
                            #
                            fname = os.path.join(str( tmpdirname ),"perf.log")
                            salome.logm.DumpInFile( fname )
                            logManagerInst0 = salome.LogManagerLoadFromFile( fname )
                            logging.debug( logManagerInst0[0][1][0].get()._input_hdd_mem._data[1]._data[0]._data[0]._hdd_mem ) # important
                            logManagerInst = salome.logm.Fetch(True)
                            logManagerInst2 = salome.logm.Fetch(True)
                            logging.debug( salome.LogManagerLoadFromIORFile( iorLogm )[0][1][0].get() )
                            salome.logm.putStructInFileAtomic(False,monitoringFileTwo)
                            logging.debug( salome.LogManagerLoadFromFile(monitoringFileTwo)[0][1][0].get() )
                            logging.debug( logManagerInst[0][1][0].get()._input_hdd_mem._data[1]._data[0]._data[0]._hdd_mem ) # important
                            self.assertTrue( logManagerInst2[0][1][0].get() is None )
                            self.assertTrue( logManagerInst[0][1][1].get()._output_hdd_mem._data[0]._file_name == fileNamesProxyOut2[0].getFileName() )
                            logging.debug( logManagerInst[0][1][1].log() )
                            # 2 files because a backup file is stored in case of unexpected kill during 
                            self.assertEqual( len( glob.glob("{}*".format(monitoringFile) ) ) , 2 )
                            # leaving MonitoringFile Manager -> backup file is killed
                            pass
                    #self.assertEqual(monitoringFileSafe, monitoringFile)
                    self.assertEqual( len( glob.glob("{}*".format(monitoringFile) ) ) , 1 )
                    logging.debug( salome.LogManagerLoadFromFile(monitoringFile)[0][1][0].get() )
                    del monitoringParams
                    import gc ; gc.collect()
                    self.assertFalse( os.path.exists(pyFileContainingCodeOfMonitoring) )
                    cont.Shutdown()
                fnToTest0 = monitoringParamsMaster.outFileName.filename
                cpumeminfo = SALOME_PyNode.ReadCPUMemInfo( monitoringParamsMaster )
                self.assertTrue( isinstance(monitoringParamsMaster.outFileName,SALOME_PyNode.FileDeleter) )
                del monitoringParamsMaster
                import gc
                gc.collect()
                self.assertTrue( os.path.exists( fnToTest0 ) )
            dirInfo = SALOME_PyNode.ReadInodeSizeInfo( monitoringParamsForFileMaster )
            self.assertTrue( isinstance(monitoringParamsForFileMaster.outFileName,SALOME_PyNode.FileDeleter) )
            self.assertFalse( os.path.exists( fnToTest0 ) )
        logging.debug( cpumeminfo )
        logging.debug( dirInfo )
        
    def testPerfLogManager1(self):
        """
        [EDF29150] : Similar than testPerfLogManager1 but with out fileName precised for CPU/Mem monitoring and FS monitoring
        """
        hostname = "localhost"
        cp = pylauncher.GetRequestForGiveContainer(hostname,"container_test_three")
        salome.logm.clear()
        #PROXY_THRES = -1
        PROXY_THRES = 1
        with tempfile.TemporaryDirectory() as tmpdirnameMonitoring:
            fsMonitoringFile = os.path.join( str( tmpdirnameMonitoring ), "zeFS.txt" )
            cpuMemMonitoringFile = os.path.join( str( tmpdirnameMonitoring ), "zeCPUMem.txt" )
            with SALOME_PyNode.GenericPythonMonitoringLauncherCtxMgr( SALOME_PyNode.FileSystemMonitoring(1000,os.path.dirname( salome.__file__ ),fsMonitoringFile) ) as monitoringParamsForFileMaster:
                with SALOME_PyNode.GenericPythonMonitoringLauncherCtxMgr( SALOME_PyNode.CPUMemoryMonitoring(1000,cpuMemMonitoringFile) ) as monitoringParamsMaster:
                    monitoringFile = os.path.join( str( tmpdirnameMonitoring ), "zeHtop.pckl" )
                    monitoringFileTwo = os.path.join( str( tmpdirnameMonitoring ), "zeHtopTwo.pckl" )
                    iorLogm = os.path.join( str( tmpdirnameMonitoring ), "logm.ior" )
                    with open(iorLogm,"w") as f:
                        f.write( salome.orb.object_to_string(salome.logm) )
                    logging.debug("Monitoring file : {}".format(monitoringFile))
                    with tempfile.TemporaryDirectory() as tmpdirname:
                        with salome.LogManagerLaunchMonitoringFileCtxMgr(250,monitoringFile) as monitoringParams:
                            pyFileContainingCodeOfMonitoring = monitoringParams.pyFileName.filename
                            logging.debug("Python file containing code of monitoring : {}".format(pyFileContainingCodeOfMonitoring))
                            val_for_big_obj = str( tmpdirname )
                            salome.cm.SetBigObjOnDiskDirectory(val_for_big_obj)
                            salome.cm.SetBigObjOnDiskThreshold(PROXY_THRES)
                            # Override environement for all containers launched
                            salome.cm.SetOverrideEnvForContainersSimple(env = [])
                            salome.cm.SetDeltaTimeBetweenCPUMemMeasureInMilliSecond( 250 )
                            cont = salome.cm.GiveContainer(cp)
                            logging.debug("{} {}".format(40*"*",cont.getPID()))
                            script_st = """
import logging
import sys
import KernelBasis
from datetime import datetime
cst = KernelBasis.GetTimeAdjustmentCst()
logging.debug("constant = {}".format(cst))
nbcore = 3
my_log_4_this_session.addFreestyleAndFlush( ("a",datetime.now()) ) # exemple of custom tracking
print("coucou {} {}".format(len(zeinput0),len(zeinput1)))
logging.debug("debug or not debug")
ob = [ [ bytes(3000000) ] ]
pihm, ts = KernelBasis.HeatMarcel(1 * nbcore * cst,nbcore)
print("Time ellapse spent : {} s".format(ts))
ob2 = [ [ bytes(100000) ] ]
pihm, ts = KernelBasis.HeatMarcel(1 * nbcore * cst,nbcore)
print("Time ellapse spent : {} s".format(ts))
sys.stderr.write("fake error message\\n")
"""
                            poa = salome.orb.resolve_initial_references("RootPOA")
                            zeinput0 = [ bytes(100000000) ]
                            if SALOME_PyNode.GetBigObjectOnDiskThreshold() != -1:
                                zeinput0 = SALOME_PyNode.ProxyfyPickeled( zeinput0 )
                                zeinput0.unlinkOnDestructor()
                            obj = SALOME_PyNode.SenderByte_i(poa,pickle.dumps( (["zeinput0"],{"zeinput0": [zeinput0], "zeinput1": [ [zeinput0], [zeinput0] ] }) ))
                            id_o = poa.activate_object(obj)
                            refPtr = poa.id_to_reference(id_o)
                            pyscript2 = cont.createPyScriptNode("testScript2",script_st)
                            pyscript2.executeFirst(refPtr)
                            ret2 = pyscript2.executeSecond(["ob","ob2"])# generate a DeprecationWarning: PY_SSIZE_T_CLEAN will be required for '#' formats on debian11 ?
                            ret3, fileNamesProxyOut = unProxyfy( ret2 )
                            logging.getLogger().debug("test logging 1")
                            logging.debug("test logging 2")
                            logging.debug( salome.orb.object_to_string( salome.logm ) )
                            a = salome.logm.NaiveFetch()
                            logging.debug(a)
                            cont.Shutdown()
                cpumeminfo = SALOME_PyNode.ReadCPUMemInfo( monitoringParamsMaster )
                self.assertTrue( isinstance(monitoringParamsMaster.outFileName,SALOME_PyNode.FileHolder) )
            dirInfo = SALOME_PyNode.ReadInodeSizeInfo( monitoringParamsForFileMaster )
            self.assertTrue( isinstance(monitoringParamsForFileMaster.outFileName,SALOME_PyNode.FileHolder) )
            self.assertTrue( os.path.exists( fsMonitoringFile ) )
            self.assertTrue( os.path.exists( cpuMemMonitoringFile ) )
        logging.debug( cpumeminfo )
        logging.debug( dirInfo )
        self.assertFalse( os.path.exists( fsMonitoringFile ) )
        self.assertFalse( os.path.exists( cpuMemMonitoringFile ) )

    def testEasyNamingService(self):
        """
        [EDF29150] : This test checks measure performance methods
        """
        hostname = "localhost"
        cp = pylauncher.GetRequestForGiveContainer(hostname,"container_test_two")
        salome.logm.clear()
        PROXY_THRES = 1
        with tempfile.TemporaryDirectory() as tmpdirname:
            ior_ns_file = os.path.join(tmpdirname,"ns.ior")
            val_for_big_obj = str( tmpdirname )
            salome.cm.SetBigObjOnDiskDirectory(val_for_big_obj)
            salome.cm.SetBigObjOnDiskThreshold(PROXY_THRES)
            salome.cm.SetOverrideEnvForContainersSimple(env = [])
            salome.cm.SetDeltaTimeBetweenCPUMemMeasureInMilliSecond( 250 )
            salome.naming_service.DumpIORInFile( ior_ns_file )
            cont = salome.cm.GiveContainer(cp)
            script_st = """
from SALOME_Embedded_NamingService_ClientPy import SALOME_Embedded_NamingService_ClientPy
ior_ns_file = "{ior_ns_file}"
ns = SALOME_Embedded_NamingService_ClientPy.BuildFromIORFile( ior_ns_file )
ret = ns.repr()
""".format(**locals())
            cont = salome.cm.GiveContainer(cp)
            pyscript2 = cont.createPyScriptNode("testScript3",script_st)
            retCoarse = pickle.loads( pyscript2.execute( ["ret"], pickle.dumps( ([],{} ) ) ) )
            ret = retCoarse[0]
            self.assertTrue( isinstance(ret,list) and isinstance(ret[0],str) )
            cont.Shutdown()


    def testSSDCopyMethod(self):
        """
        [EDF30157] : This test focuses on protocol of data using SSD local disks
        """
        import gc
        hostname = "localhost"
        cp0 = pylauncher.GetRequestForGiveContainer(hostname,"container_test_ssd_0")
        cp1 = pylauncher.GetRequestForGiveContainer(hostname,"container_test_ssd_1")
        salome.logm.clear()
        PROXY_THRES = 1
        poa = salome.orb.resolve_initial_references("RootPOA")
        with tempfile.TemporaryDirectory() as tmpdirname:
            val_for_big_obj = str( tmpdirname )
            salome.cm.SetBigObjOnDiskDirectory( "@1@{}".format( val_for_big_obj) ) # <- key point is here tell KERNEL that directory is considered as local 
            salome.cm.SetBigObjOnDiskThreshold(PROXY_THRES)
            salome.cm.SetOverrideEnvForContainersSimple(env = [])
            salome.cm.SetDeltaTimeBetweenCPUMemMeasureInMilliSecond( 250 )
            cont0 = salome.cm.GiveContainer(cp0)
            cont1 = salome.cm.GiveContainer(cp1)
            #
            script_st0 = """ret0 = bytes(zeLength)"""
            #
            pyscript0 = cont0.createPyScriptNode("testScript0",script_st0)
            szOfArray = 3000000
            obj = SALOME_PyNode.SenderByte_i(poa,pickle.dumps( (["zeLength"],{"zeLength": szOfArray }) ))
            id_o = poa.activate_object(obj)
            refPtr = poa.id_to_reference(id_o)
            pyscript0.executeFirst(refPtr)
            ret0 = pyscript0.executeSecond(["ret0"])
            ret0_prxy = pickle.loads( SALOME_PyNode.SeqByteReceiver(ret0[0]).data() )
            self.assertTrue( isinstance( ret0_prxy.getFileName(), SALOME_PyNode.BigFileOnDiskSSDNoShare) ) # <- Key point is here
            self.assertTrue( isinstance( ret0_prxy.get(), bytes ) )
            self.assertEqual( len(ret0_prxy.get()), szOfArray )
            ret0_prxy.unlinkOnDestructor()
            #
            script_st1 = """ret1 = len(ret0)"""
            pyscript1 = cont1.createPyScriptNode("testScript1",script_st1)
            obj1 = SALOME_PyNode.SenderByte_i(poa,pickle.dumps( (["ret0"],{"ret0": ret0_prxy }) ))
            id_o1 = poa.activate_object(obj1)
            refPtr1 = poa.id_to_reference(id_o1)
            pyscript1.executeFirst(refPtr1)
            ret1 = pyscript1.executeSecond(["ret1"])
            ret1_prxy = pickle.loads( SALOME_PyNode.SeqByteReceiver(ret1[0]).data() )
            ret1_prxy.unlinkOnDestructor()
            self.assertEqual( ret1_prxy.get(), szOfArray )
            #
            del ret0_prxy
            del ret1_prxy
            #
            cont0.Shutdown()
            cont1.Shutdown()
            gc.collect()
            self.assertTrue( len( glob.glob( os.path.join(tmpdirname,"*") ) ) == 0 )
        pass

if __name__ == '__main__':
    from salome_utils import positionVerbosityOfLoggerRegardingState,setVerboseLevel,setVerbose
    salome.standalone()
    salome.salome_init()
    setVerbose(True)
    setVerboseLevel(logging.DEBUG)
    positionVerbosityOfLoggerRegardingState()
    unittest.main()
