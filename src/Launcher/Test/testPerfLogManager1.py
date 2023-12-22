#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2024  CEA/DEN, EDF R&D
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

import pickle
import tempfile
import logging

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
        #PROXY_THRES = "-1"
        PROXY_THRES = "1"
        with tempfile.TemporaryDirectory() as tmpdirname:
            val_for_big_obj = str( tmpdirname )
            # Override environement for all containers launched
            salome.cm.SetOverrideEnvForContainersSimple(env = [("SALOME_FILE_BIG_OBJ_DIR",val_for_big_obj),("SALOME_BIG_OBJ_ON_DISK_THRES",PROXY_THRES)])
            salome.cm.SetDeltaTimeBetweenCPUMemMeasureInMilliSecond( 250 )
            cont = salome.cm.GiveContainer(cp)
            logging.debug("{} {}".format(40*"*",cont.getPID()))
            script_st = """
import logging
import sys
import KernelBasis
cst = KernelBasis.GetTimeAdjustmentCst()
logging.debug("constant = {}".format(cst))
nbcore = 3
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
                    if os.path.exists(fileNameProxyOut):
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
                    if os.path.exists(fileNameProxyOut):
                        raise RuntimeError("Oooops 3")
            #
            fname = os.path.join(str( tmpdirname ),"perf.log")
            salome.logm.DumpInFile( fname )
            logManagerInst0 = salome.logm.LoadFromFile( fname )
            logging.debug( logManagerInst0[0][1][0].get()._input_hdd_mem._data[1]._data[0]._data[0]._hdd_mem ) # important
            logManagerInst = salome.logm.Fetch(True)
            logManagerInst2 = salome.logm.Fetch(True)
            logging.debug( logManagerInst[0][1][0].get()._input_hdd_mem._data[1]._data[0]._data[0]._hdd_mem ) # important
            self.assertTrue( logManagerInst2[0][1][0].get() is None )
            self.assertTrue( logManagerInst[0][1][1].get()._output_hdd_mem._data[0]._file_name == fileNamesProxyOut2[0] )
            logging.debug( logManagerInst[0][1][1].log() )
            cont.Shutdown()

if __name__ == '__main__':
    from salome_utils import positionVerbosityOfLoggerRegardingState,setVerboseLevel,setVerbose
    salome.standalone()
    salome.salome_init()
    setVerboseLevel(logging.DEBUG)
    positionVerbosityOfLoggerRegardingState()
    unittest.main()
