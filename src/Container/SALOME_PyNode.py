#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2024  CEA, EDF, OPEN CASCADE
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

#  File   : SALOME_PyNode.py
#  Author : Christian CAREMOLI, EDF
#  Module : SALOME
#  $Header$
#
import sys,traceback
import linecache
import pickle
import Engines__POA
import SALOME__POA
import SALOME
import logging
import os
import sys
from SALOME_ContainerHelper import ScriptExecInfo

MY_CONTAINER_ENTRY_IN_GLBS = "my_container"

MY_PERFORMANCE_LOG_ENTRY_IN_GLBS = "my_log_4_this_session"

class Generic(SALOME__POA.GenericObj):
  """A Python implementation of the GenericObj CORBA IDL"""
  def __init__(self,poa):
    self.poa=poa
    self.cnt=1

  def Register(self):
    #print("Register called : %d"%self.cnt)
    self.cnt+=1

  def UnRegister(self):
    #print("UnRegister called : %d"%self.cnt)
    self.cnt-=1
    if self.cnt <= 0:
      oid=self.poa.servant_to_id(self)
      self.poa.deactivate_object(oid)

  def Destroy(self):
    print("WARNING SALOME::GenericObj::Destroy() function is obsolete! Use UnRegister() instead.")
    self.UnRegister()

  def __del__(self):
    #print("Destuctor called")
    pass

class PyNode_i (Engines__POA.PyNode,Generic):
  """The implementation of the PyNode CORBA IDL"""
  def __init__(self, nodeName,code,poa,my_container):
    """Initialize the node : compilation in the local context"""
    Generic.__init__(self,poa)
    self.nodeName=nodeName
    self.code=code
    self.my_container=my_container._container
    linecache.cache[nodeName]=0,None,code.split('\n'),nodeName
    ccode=compile(code,nodeName,'exec')
    self.context={}
    self.context[MY_CONTAINER_ENTRY_IN_GLBS] = self.my_container
    exec(ccode, self.context)

  def getContainer(self):
    return self.my_container

  def getCode(self):
    return self.code

  def getName(self):
    return self.nodeName

  def defineNewCustomVar(self,varName,valueOfVar):
    self.context[varName] = pickle.loads(valueOfVar)
    pass

  def executeAnotherPieceOfCode(self,code):
    """Called for initialization of container lodging self."""
    try:
      ccode=compile(code,self.nodeName,'exec')
      exec(ccode, self.context)
    except Exception:
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"","PyScriptNode (%s) : code to be executed \"%s\"" %(self.nodeName,code),0))

  def execute(self,funcName,argsin):
    """Execute the function funcName found in local context with pickled args (argsin)"""
    try:
      argsin,kws=pickle.loads(argsin)
      func=self.context[funcName]
      argsout=func(*argsin,**kws)
      argsout=pickle.dumps(argsout,-1)
      return argsout
    except Exception:
      exc_typ,exc_val,exc_fr=sys.exc_info()
      l=traceback.format_exception(exc_typ,exc_val,exc_fr)
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"".join(l),"PyNode: %s, function: %s" % (self.nodeName,funcName),0))

class SenderByte_i(SALOME__POA.SenderByte,Generic):
  def __init__(self,poa,bytesToSend):
    Generic.__init__(self,poa)
    self.bytesToSend = bytesToSend

  def getSize(self):
    return len(self.bytesToSend)

  def sendPart(self,n1,n2):
    return self.bytesToSend[n1:n2]

SALOME_FILE_BIG_OBJ_DIR = "SALOME_FILE_BIG_OBJ_DIR"
    
SALOME_BIG_OBJ_ON_DISK_THRES_VAR = "SALOME_BIG_OBJ_ON_DISK_THRES"

# default is 50 MB
SALOME_BIG_OBJ_ON_DISK_THRES_DFT = 50000000

DicoForProxyFile = { }

def GetSizeOfBufferedReader(f):
  """
  This method returns in bytes size of a file openned.

  Args:
  ----
      f (io.IOBase): buffered reader returned by open
      
  Returns
  -------
      int: number of bytes
  """
  import io
  pos = f.tell()
  f.seek(0,io.SEEK_END)
  pos2 = f.tell()
  f.seek(pos,io.SEEK_SET)
  return pos2-pos

def GetObjectFromFile(fname, visitor = None):
  with open(fname,"rb") as f:
    if visitor:
      visitor.setHDDMem( GetSizeOfBufferedReader(f) )
      visitor.setFileName( fname )
    obj = pickle.load(f)
  return obj

def DumpInFile(obj,fname):
  with open(fname,"wb") as f:
    f.write( obj )

def IncrRefInFile(fname):
  if fname in DicoForProxyFile:
    DicoForProxyFile[fname] += 1
  else:
    DicoForProxyFile[fname] = 2
  pass

def DecrRefInFile(fname):
  if fname not in DicoForProxyFile:
    cnt = 1
  else:
    cnt = DicoForProxyFile[fname]
    DicoForProxyFile[fname] -= 1
    if cnt == 1:
      del DicoForProxyFile[fname]
  if cnt == 1:
    if os.path.exists(fname):
      os.unlink( fname )
  pass

def GetBigObjectOnDiskThreshold():
  import os
  if SALOME_BIG_OBJ_ON_DISK_THRES_VAR in os.environ:
    return int( os.environ[SALOME_BIG_OBJ_ON_DISK_THRES_VAR] )
  else:
    return SALOME_BIG_OBJ_ON_DISK_THRES_DFT

def ActivateProxyMecanismOrNot( sizeInByte ):
  thres = GetBigObjectOnDiskThreshold()
  if thres == -1:
    return False
  else:
    return sizeInByte > thres

def GetBigObjectDirectory():
  import os
  if SALOME_FILE_BIG_OBJ_DIR not in os.environ:
    raise RuntimeError("An object of size higher than limit detected and no directory specified to dump it in file !")
  return os.path.expanduser( os.path.expandvars( os.environ[SALOME_FILE_BIG_OBJ_DIR] ) )

def GetBigObjectFileName():
  """
  Return a filename in the most secure manner (see tempfile documentation)
  """
  import tempfile
  with tempfile.NamedTemporaryFile(dir=GetBigObjectDirectory(),prefix="mem_",suffix=".pckl") as f:
    ret = f.name
  return ret

class BigObjectOnDiskBase:
  def __init__(self, fileName, objSerialized):
    """
    :param fileName: the file used to dump into.
    :param objSerialized: the object in pickeled form
    :type objSerialized: bytes
    """
    self._filename = fileName
    # attribute _destroy is here to tell client side or server side
    # only client side can be with _destroy set to True. server side due to risk of concurrency
    # so pickled form of self must be done with this attribute set to False.
    self._destroy = False
    self.__dumpIntoFile(objSerialized)

  def getDestroyStatus(self):
    return self._destroy

  def incrRef(self):
    if self._destroy:
      IncrRefInFile( self._filename )
    else:
      # should never happen !
      RuntimeError("Invalid call to incrRef !")

  def decrRef(self):
    if self._destroy:
      DecrRefInFile( self._filename )
    else:
      # should never happen !
      RuntimeError("Invalid call to decrRef !")

  def unlinkOnDestructor(self):
    self._destroy = True

  def doNotTouchFile(self):
    """
    Method called slave side. The life cycle management of file is client side not slave side.
    """
    self._destroy = False

  def __del__(self):
    if self._destroy:
      DecrRefInFile( self._filename )

  def getFileName(self):
    return self._filename
  
  def __dumpIntoFile(self, objSerialized):
    DumpInFile( objSerialized, self._filename )

  def get(self, visitor = None):
    obj = GetObjectFromFile( self._filename, visitor )
    return obj

  def __float__(self):
    return float( self.get() )
    
  def __int__(self):
    return int( self.get() )
    
  def __str__(self):
    obj = self.get()
    if isinstance(obj,str):
        return obj
    else:
        raise RuntimeError("Not a string")
      
class BigObjectOnDisk(BigObjectOnDiskBase):
  def __init__(self, fileName, objSerialized):
    BigObjectOnDiskBase.__init__(self, fileName, objSerialized)
    
class BigObjectOnDiskListElement(BigObjectOnDiskBase):
  def __init__(self, pos, length, fileName):
    self._filename = fileName
    self._destroy = False
    self._pos = pos
    self._length = length

  def get(self, visitor = None):
    fullObj = BigObjectOnDiskBase.get(self, visitor)
    return fullObj[ self._pos ]
    
  def __getitem__(self, i):
    return self.get()[i]

  def __len__(self):
    return len(self.get())
    
class BigObjectOnDiskSequence(BigObjectOnDiskBase):
  def __init__(self, length, fileName, objSerialized):
    BigObjectOnDiskBase.__init__(self, fileName, objSerialized)
    self._length = length

  def __getitem__(self, i):
    return BigObjectOnDiskListElement(i, self._length, self.getFileName())

  def __len__(self):
    return self._length

class BigObjectOnDiskList(BigObjectOnDiskSequence):
  def __init__(self, length, fileName, objSerialized):
    BigObjectOnDiskSequence.__init__(self, length, fileName, objSerialized)
    
class BigObjectOnDiskTuple(BigObjectOnDiskSequence):
  def __init__(self, length, fileName, objSerialized):
    BigObjectOnDiskSequence.__init__(self, length, fileName, objSerialized)

def ProxyfyPickeled( obj, pickleObjInit = None, visitor = None ):
  """
  This method return a proxy instance of pickled form of object given in input.

  Args:
  ----
      obj (pickelable type) : object to be proxified
      pickleObjInit (bytes) : Optionnal. Original pickeled form of object to be proxyfied if already computed. If not this method generate it

  Returns
  -------
      BigObjectOnDiskBase: proxy instance
  """
  pickleObj = pickleObjInit
  if pickleObj is None:
    pickleObj = pickle.dumps( obj , pickle.HIGHEST_PROTOCOL )
  fileName = GetBigObjectFileName()
  if visitor:
    visitor.setHDDMem( len(pickleObj) )
    visitor.setFileName(fileName)
  if isinstance( obj, list):
    proxyObj = BigObjectOnDiskList( len(obj), fileName, pickleObj )
  elif isinstance( obj, tuple):
    proxyObj = BigObjectOnDiskTuple( len(obj), fileName , pickleObj )
  else:
    proxyObj = BigObjectOnDisk( fileName , pickleObj )
  return proxyObj

def SpoolPickleObject( obj, visitor = None ):
  import pickle
  with InOutputObjVisitorCM(visitor) as v:
    pickleObjInit = pickle.dumps( obj , pickle.HIGHEST_PROTOCOL )
    if not ActivateProxyMecanismOrNot( len(pickleObjInit) ):
      return pickleObjInit
    else:
      proxyObj = ProxyfyPickeled( obj, pickleObjInit, v.visitor() )
      pickleProxy = pickle.dumps( proxyObj , pickle.HIGHEST_PROTOCOL )
      return pickleProxy

from SALOME_ContainerHelper import InOutputObjVisitorCM, InOutputObjVisitor

def UnProxyObjectSimple( obj, visitor = None ):
  """
  Method to be called in Remote mode. Alterate the obj _status attribute. 
  Because the slave process does not participate in the reference counting
  
  Args:
  ----
      visitor (InOutputObjVisitor): A visitor to keep track of amount of memory on chip and those on HDD

  """
  with InOutputObjVisitorCM(visitor) as v:
    logging.debug( "UnProxyObjectSimple {}".format(type(obj)) )
    if isinstance(obj,BigObjectOnDiskBase):
      obj.doNotTouchFile()
      return obj.get( v )
    elif isinstance( obj, list):
      retObj = []
      for elt in obj:
        retObj.append( UnProxyObjectSimple(elt,v.visitor()) )
      return retObj
    else:
      return obj

def UnProxyObjectSimpleLocal( obj ):
  """
  Method to be called in Local mode. Do not alterate the PyObj counter
  """
  if isinstance(obj,BigObjectOnDiskBase):
    return obj.get()
  elif isinstance( obj, list):
    retObj = []
    for elt in obj:
      retObj.append( UnProxyObjectSimpleLocal(elt) )
    return retObj
  else:
    return obj
  
class FileHolder:
  def __init__(self, fileName):
    self._filename = fileName
  @property
  def filename(self):
    return self._filename
  
class FileDeleter(FileHolder):
  def __init__(self, fileName):
    super().__init__( fileName )
  def __del__(self):
    import os
    if os.path.exists( self._filename ):
      os.unlink( self._filename )

class MonitoringInfo:
  def __init__(self, pyFileName, intervalInMs, outFileName, pid):
    self._py_file_name = pyFileName
    self._interval_in_ms = intervalInMs
    self._out_file_name = outFileName
    self._pid = pid

  @property
  def pyFileName(self):
    return self._py_file_name

  @property
  def pid(self):
    return self._pid
  
  @pid.setter
  def pid(self, value):
    self._pid = value

  @property
  def outFileName(self):
    return self._out_file_name
  
  @property
  def intervalInMs(self):
    return self._interval_in_ms
  
def FileSystemMonitoring(intervalInMs, dirNameToInspect, outFileName = None):
    """
    This method loops indefinitely every intervalInMs milliseconds to scan 
    number of inodes and size of content recursively included into the in input directory.

    Args:
    ----

    outFileName (str) : name of file inside the results will be written. If None a new file is generated

    See also CPUMemoryMonitoring
    """
    global orb
    import os
    dirNameToInspect2 = os.path.abspath( os.path.expanduser(dirNameToInspect) )
    import tempfile
    import logging
    import KernelBasis
    # outFileNameSave stores the content of outFileName during phase of dumping
    with tempfile.NamedTemporaryFile(prefix="fs_monitor_",suffix=".txt") as f:
      outFileNameSave = f.name
    with tempfile.NamedTemporaryFile(prefix="fs_monitor_",suffix=".py") as f:
      tempPyFile = f.name
    tempOutFile = outFileName
    if tempOutFile is None:
      tempOutFile = "{}.txt".format( os.path.splitext( tempPyFile )[0] )
    with open(tempPyFile,"w") as f:
        f.write("""
import subprocess as sp
import re
import os
import time
import datetime
with open("{tempOutFile}","a") as f:
  f.write( "{{}}\\n".format( "{dirNameToInspect2}" ) )
  f.write( "{{}}\\n".format( "{intervalInMs}" ) )
  while(True):
    nbinodes = -1
    try:
      nbinodes = sp.check_output("{{}} | wc -l".format( " ".join(["find","{dirNameToInspect2}"]),  ), shell = True).decode().strip()
    except:
      pass
    szOfDirStr = "fail"
    try:
      st = sp.check_output(["du","-sh","{dirNameToInspect2}"]).decode()
      szOfDirStr = re.split("[\s]+",st)[0]
    except:
      pass
    f.write( "{{}}\\n".format( str( datetime.datetime.now().timestamp() ) ) )
    f.write( "{{}}\\n".format( str( nbinodes  ) ) )
    f.write( "{{}}\\n".format( str( szOfDirStr ) ) )
    f.flush()
    time.sleep( {intervalInMs} / 1000.0 )
""".format( **locals()))
    logging.debug( "File for FS monitoring dump file : {}".format(tempPyFile) )
    pyFileName = FileDeleter( tempPyFile )
    if outFileName is None:
      outFileName = FileDeleter( tempOutFile )
    else:
      outFileName = FileHolder(outFileName)
    return MonitoringInfo(pyFileName, intervalInMs, outFileName, None)

def CPUMemoryMonitoring( intervalInMs, outFileName = None ):
  """
  Launch a subprocess monitoring self process.
  This monitoring subprocess is a python process lauching every intervalInMs ms evaluation of
  CPU usage and RSS memory of the calling process.
  Communication between subprocess and self is done by file.

  Args:
  ----
    outFileName (str) : name of file inside the results will be written. If None a new file is generated

  See also FileSystemMonitoring
  """
  import KernelBasis
  def BuildPythonFileForCPUPercent( intervalInMs, outFileName):
    import os
    import tempfile
    with tempfile.NamedTemporaryFile(prefix="cpu_mem_monitor_",suffix=".py") as f:
      tempPyFile = f.name
    tempOutFile = outFileName
    if tempOutFile is None:
      tempOutFile = "{}.txt".format( os.path.splitext( tempPyFile )[0] )
    pid = os.getpid()
    with open(tempPyFile,"w") as f:
      f.write("""import psutil
pid = {}
process = psutil.Process( pid )
import time
with open("{}","a") as f:
  f.write( "{{}}\\n".format( "{}" ) )
  while True:
    f.write( "{{}}\\n".format( str( process.cpu_percent() ) ) )
    f.write( "{{}}\\n".format( str( process.memory_info().rss  ) ) )
    f.flush()
    time.sleep( {} / 1000.0 )
""".format(pid, tempOutFile, intervalInMs, intervalInMs))
    if outFileName is None:
      autoOutFile = FileDeleter(tempOutFile)
    else:
      autoOutFile = FileHolder(tempOutFile)
    return FileDeleter(tempPyFile),autoOutFile
  pyFileName, outFileName = BuildPythonFileForCPUPercent( intervalInMs, outFileName )
  return MonitoringInfo(pyFileName, intervalInMs, outFileName, None)

class GenericPythonMonitoringLauncherCtxMgr:
    def __init__(self, monitoringParams):
        """
        Args:
        ----
            monitoringParams (MonitoringInfo)
        """
        self._monitoring_params = monitoringParams
    def __enter__(self):
        import KernelBasis
        pid = KernelBasis.LaunchMonitoring(self._monitoring_params.pyFileName.filename)
        self._monitoring_params.pid = pid
        return self._monitoring_params
    
    def __exit__(self,exctype, exc, tb):
        StopMonitoring( self._monitoring_params )

def StopMonitoring( monitoringInfo ):
  """
  Kill monitoring subprocess.

  Args:
  ----
      monitoringInfo (MonitoringInfo): info returned by LaunchMonitoring
  """
  import KernelBasis
  KernelBasis.StopMonitoring(monitoringInfo.pid)

class CPUMemInfo:
  def __init__(self, intervalInMs, cpu, mem_rss):
    """
    Args:
    ----
    intervalInMs (int)
    cpu (list<float>)  CPU usage
    mem_rss (list<int>) rss memory usage
    """
    self._interval_in_ms = intervalInMs
    self._data = [(a,b) for a,b in zip(cpu,mem_rss)]
  def __str__(self):
    st = """Interval in ms : {self.intervalInMs}
Data : ${self.data}
""".format( **locals() )
    return st
  @property
  def intervalInMs(self):
    return self._interval_in_ms
  @property
  def data(self):
    """
    list of triplets. First param of pair is cpu usage 
                      Second param of pair is memory usage
    """
    return self._data

def ReadCPUMemInfoInternal( fileName ):
  intervalInMs = 0
  cpu = [] ; mem_rss = []
  if os.path.exists( fileName ):
    try:
      with open(fileName, "r") as f:
        coarseData = [ elt.strip() for elt in f.readlines() ]
      intervalInMs = int( coarseData[0] )
      coarseData = coarseData[1:]
      cpu = [float(elt) for elt in coarseData[::2]]
      mem_rss = [ int(elt) for elt in coarseData[1::2]]
    except:
      pass
  return CPUMemInfo(intervalInMs,cpu,mem_rss)

def ReadCPUMemInfo( monitoringInfo ):
  """
  Retrieve CPU/Mem data of monitoring.

  Args:
  ----
      monitoringInfo (MonitoringInfo): info returned by LaunchMonitoring
  
  Returns
  -------
    CPUMemInfo instance
  """
  return ReadCPUMemInfoInternal( monitoringInfo.outFileName.filename )

class InodeSizeInfo:
  def __init__(self, dirNameMonitored, intervalInMs, timeStamps, nbInodes, volumeOfDir):
    """
    Args:
    ----
    timeStamps (list<datetimestruct>)
    nbInodes (list<int>)
    volumeOfDir (list<str>)
    """
    self._dir_name_monitored = dirNameMonitored
    self._interval_in_ms = intervalInMs
    self._data = [(t,a,b) for t,a,b in zip(timeStamps,nbInodes,volumeOfDir)]
  def __str__(self):
    st = """Filename monitored : {self.dirNameMonitored}
Interval in ms : ${self.intervalInMs}
Data : ${self.data}
""".format( **locals() )
    return st
  @property
  def dirNameMonitored(self):
    return self._dir_name_monitored
  @property
  def intervalInMs(self):
    return self._interval_in_ms
  @property
  def data(self):
    """
    list of triplets. First param of triplet is datetimestruct
                                      Second param of triplet is #inodes.
                                      Thirst param of triplet is size.
    """
    return self._data

def ReadInodeSizeInfoInternal( fileName ):
  import datetime
  import os
  with open(fileName, "r") as f:
    coarseData = [ elt.strip() for elt in f.readlines() ]
  dirNameMonitored = coarseData[0] ; intervalInMs = int( coarseData[1] ) ; coarseData = coarseData[2:]
  tss = [ datetime.datetime.fromtimestamp( float(elt) ) for elt in coarseData[::3] ]
  nbInodes = [int(elt) for elt in coarseData[1::3]]
  volumeOfDir = coarseData[2::3]
  return InodeSizeInfo(dirNameMonitored,intervalInMs,tss,nbInodes,volumeOfDir)

def ReadInodeSizeInfo( monitoringInfo ):
  """
  Retrieve nb of inodes and size of monitoring

  Args:
  ----
      monitoringInfo (MonitoringInfo): info returned by LaunchMonitoring

  Returns
  -------
    InodeSizeInfo
  """
  return ReadInodeSizeInfoInternal( monitoringInfo.outFileName.filename )

class SeqByteReceiver:
  # 2GB limit to trigger split into chunks
  CHUNK_SIZE = 2000000000
  def __init__(self,sender):
    self._obj = sender
  def __del__(self):
    self._obj.UnRegister()
    pass
  def data(self):
    size = self._obj.getSize()
    if size <= SeqByteReceiver.CHUNK_SIZE:
      return self.fetchOneShot( size )
    else:
      return self.fetchByChunks( size )
  def fetchOneShot(self,size):
    return self._obj.sendPart(0,size)
  def fetchByChunks(self,size):
      """
      To avoid memory peak parts over 2GB are sent using EFF_CHUNK_SIZE size.
      """
      data_for_split_case = bytes(0)
      EFF_CHUNK_SIZE = SeqByteReceiver.CHUNK_SIZE // 8
      iStart = 0 ; iEnd = EFF_CHUNK_SIZE
      while iStart!=iEnd and iEnd <= size:
        part = self._obj.sendPart(iStart,iEnd)
        data_for_split_case = bytes(0).join( [data_for_split_case,part] )
        iStart = iEnd; iEnd = min(iStart + EFF_CHUNK_SIZE,size)
      return data_for_split_case

class LogOfCurrentExecutionSession:
  def __init__(self, handleToCentralizedInst):
    self._remote_handle = handleToCentralizedInst
    self._current_instance = ScriptExecInfo()

  def addFreestyleAndFlush(self, value):
    self._current_instance.freestyle = value
    self.finalizeAndPushToMaster()

  def addInfoOnLevel2(self, key, value):
    setattr(self._current_instance,key,value)

  def finalizeAndPushToMaster(self):
    self._remote_handle.assign( pickle.dumps( self._current_instance ) )

class PyScriptNode_i (Engines__POA.PyScriptNode,Generic):
  """The implementation of the PyScriptNode CORBA IDL that executes a script"""
  def __init__(self, nodeName,code,poa,my_container,logscript):
    """Initialize the node : compilation in the local context"""
    Generic.__init__(self,poa)
    self.nodeName=nodeName
    self.code=code
    self.my_container_py = my_container
    self.my_container=my_container._container
    linecache.cache[nodeName]=0,None,code.split('\n'),nodeName
    self.ccode=compile(code,nodeName,'exec')
    self.context={}
    self.context[MY_CONTAINER_ENTRY_IN_GLBS] = self.my_container
    self._log_script = logscript
    self._current_execution_session = None
    sys.stdout.flush() ; sys.stderr.flush() # flush to correctly capture log per execution session
      
  def __del__(self):
    # force removal of self.context. Don t know why it s not done by default
    self.removeAllVarsInContext()
    pass

  def getContainer(self):
    return self.my_container

  def getCode(self):
    return self.code

  def getName(self):
    return self.nodeName

  def defineNewCustomVar(self,varName,valueOfVar):
    self.context[varName] = pickle.loads(valueOfVar)
    pass

  def executeAnotherPieceOfCode(self,code):
    """Called for initialization of container lodging self."""
    try:
      ccode=compile(code,self.nodeName,'exec')
      exec(ccode, self.context)
    except Exception:
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"","PyScriptNode (%s) : code to be executed \"%s\"" %(self.nodeName,code),0))

  def assignNewCompiledCode(self,codeStr):
    try:
      self.code=codeStr
      self.ccode=compile(codeStr,self.nodeName,'exec')
    except Exception:
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"","PyScriptNode.assignNewCompiledCode (%s) : code to be executed \"%s\"" %(self.nodeName,codeStr),0))

  def executeSimple(self, key, val):
    """
    Same as execute method except that no pickelization mecanism is implied here. No output is expected
    """
    try:
      self.context.update({ "env" : [(k,v) for k,v in zip(key,val)]})
      exec(self.ccode,self.context)
    except Exception:
      exc_typ,exc_val,exc_fr=sys.exc_info()
      l=traceback.format_exception(exc_typ,exc_val,exc_fr)
      print("".join(l)) ; sys.stdout.flush() # print error also in logs of remote container
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"".join(l),"PyScriptNode: %s" % (self.nodeName),0))
    
  def execute(self,outargsname,argsin):
    """Execute the script stored in attribute ccode with pickled args (argsin)"""
    try:
      argsname,kws=pickle.loads(argsin)
      self.context.update(kws)
      exec(self.ccode, self.context)
      argsout=[]
      for arg in outargsname:
        if arg not in self.context:
          raise KeyError("There is no variable %s in context" % arg)
        argsout.append(self.context[arg])
      argsout=pickle.dumps(tuple(argsout),-1)
      return argsout
    except Exception:
      exc_typ,exc_val,exc_fr=sys.exc_info()
      l=traceback.format_exception(exc_typ,exc_val,exc_fr)
      print("".join(l)) ; sys.stdout.flush() # print error also in logs of remote container
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"".join(l),"PyScriptNode: %s, outargsname: %s" % (self.nodeName,outargsname),0))

  def executeFirst(self,argsin):
    """ Same than first part of self.execute to reduce memory peak."""
    def ArgInMananger(self,argsin):
      argsInPy = SeqByteReceiver( argsin )
      data = argsInPy.data()
      self.addInfoOnLevel2("inputMem",len(data))
      _,kws=pickle.loads(data)
      return kws
    try:
      self.beginOfCurrentExecutionSession()
      self.addTimeInfoOnLevel2("startInputTime")
      # to force call of SeqByteReceiver's destructor
      kws = ArgInMananger(self,argsin)
      vis = InOutputObjVisitor()
      for elt in kws:
        # fetch real data if necessary
        kws[elt] = UnProxyObjectSimple( kws[elt],vis)
      self.addInfoOnLevel2("inputHDDMem",vis)
      self.context.update(kws)
      self.addTimeInfoOnLevel2("endInputTime")
    except Exception:
      exc_typ,exc_val,exc_fr=sys.exc_info()
      l=traceback.format_exception(exc_typ,exc_val,exc_fr)
      print("".join(l)) ; sys.stdout.flush() # print error also in logs of remote container
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"".join(l),"PyScriptNode:First %s" % (self.nodeName),0))

  def executeSecond(self,outargsname):
    """ Same than second part of self.execute to reduce memory peak."""
    import sys
    try:
      self.addTimeInfoOnLevel2("startExecTime")
      ##
      self.addInfoOnLevel2("measureTimeResolution",self.my_container_py.monitoringtimeresms())
      with GenericPythonMonitoringLauncherCtxMgr( CPUMemoryMonitoring( self.my_container_py.monitoringtimeresms() ) ) as monitoringParams:
        exec(self.ccode, self.context)
        cpumeminfo = ReadCPUMemInfo( monitoringParams )
      ##
      self.addInfoOnLevel2("CPUMemDuringExec",cpumeminfo)
      del monitoringParams
      self.addTimeInfoOnLevel2("endExecTime")
      self.addTimeInfoOnLevel2("startOutputTime")
      argsout=[]
      for arg in outargsname:
        if arg not in self.context:
          raise KeyError("There is no variable %s in context" % arg)
        argsout.append(self.context[arg])
      ret = [ ]
      outputMem = 0
      vis = InOutputObjVisitor()
      for arg in argsout:
        # the proxy mecanism is catched here
        argPickle = SpoolPickleObject( arg, vis )
        retArg = SenderByte_i( self.poa,argPickle )
        id_o = self.poa.activate_object(retArg)
        retObj = self.poa.id_to_reference(id_o)
        ret.append( retObj._narrow( SALOME.SenderByte ) )
        outputMem += len(argPickle)
      self.addInfoOnLevel2("outputMem",outputMem)
      self.addInfoOnLevel2("outputHDDMem",vis)
      self.addTimeInfoOnLevel2("endOutputTime")
      self.endOfCurrentExecutionSession()
      return ret
    except Exception:
      exc_typ,exc_val,exc_fr=sys.exc_info()
      l=traceback.format_exception(exc_typ,exc_val,exc_fr)
      print("".join(l)) ; sys.stdout.flush() # print error also in logs of remote container
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"".join(l),"PyScriptNode:Second %s, outargsname: %s" % (self.nodeName,outargsname),0))

  def listAllVarsInContext(self):
      import re
      pat = re.compile("^__([a-z]+)__$")
      return [elt for elt in self.context if not pat.match(elt) and elt != MY_CONTAINER_ENTRY_IN_GLBS]
      
  def removeAllVarsInContext(self):
      for elt in self.listAllVarsInContext():
        del self.context[elt]

  def getValueOfVarInContext(self,varName):
    try:
      return pickle.dumps(self.context[varName],-1)
    except Exception:
      exc_typ,exc_val,exc_fr=sys.exc_info()
      l=traceback.format_exception(exc_typ,exc_val,exc_fr)
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"".join(l),"PyScriptNode: %s" %self.nodeName,0))
    pass
  
  def assignVarInContext(self, varName, value):
    try:
      self.context[varName][0] = pickle.loads(value)
    except Exception:
      exc_typ,exc_val,exc_fr=sys.exc_info()
      l=traceback.format_exception(exc_typ,exc_val,exc_fr)
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"".join(l),"PyScriptNode: %s" %self.nodeName,0))
    pass

  def callMethodOnVarInContext(self, varName, methodName, args):
    try:
      return pickle.dumps( getattr(self.context[varName][0],methodName)(*pickle.loads(args)),-1 )
    except Exception:
      exc_typ,exc_val,exc_fr=sys.exc_info()
      l=traceback.format_exception(exc_typ,exc_val,exc_fr)
      raise SALOME.SALOME_Exception(SALOME.ExceptionStruct(SALOME.BAD_PARAM,"".join(l),"PyScriptNode: %s" %self.nodeName,0))
    pass

  def beginOfCurrentExecutionSession(self):
    self._current_execution_session = LogOfCurrentExecutionSession( self._log_script.addExecutionSession() )
    self.context[MY_PERFORMANCE_LOG_ENTRY_IN_GLBS] = self._current_execution_session
  
  def endOfCurrentExecutionSession(self):
    self._current_execution_session.finalizeAndPushToMaster()
    self._current_execution_session = None

  def addInfoOnLevel2(self, key, value):
    self._current_execution_session.addInfoOnLevel2(key, value)
      
  def addTimeInfoOnLevel2(self, key):
    from datetime import datetime
    self._current_execution_session.addInfoOnLevel2(key,datetime.now())
