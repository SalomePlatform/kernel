#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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
import abc
import asyncio
import linecache
import logging
import os
import pickle
import sys
import traceback
from pathlib import Path
from typing import TextIO

import Engines__POA
import KernelBasis
import SALOME
import SALOME__POA
import Engines
import Engines__POA
from SALOME_ContainerHelper import ScriptExecInfo

MY_CONTAINER_ENTRY_IN_GLBS = "my_container"

MY_PERFORMANCE_LOG_ENTRY_IN_GLBS = "my_log_4_this_session"

MY_KEY_TO_DETECT_FINISH = "neib av tuot"


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
  
def IsRemote(hostName):
    import socket
    return socket.gethostname() != hostName

def RemoveFileSafe( fileName ):
    if os.path.exists( fileName ):
      os.unlink( fileName )

def RetrieveRemoteFileLocallyInSameFileName( remoteHostName, fileName):
    """ To customize"""
    dn = os.path.dirname( fileName )
    import subprocess as sp
    p = sp.Popen(["scp","{}:{}".format(remoteHostName,fileName),dn])
    p.communicate()

def DestroyRemotely( remoteHostName, fileName):
    import subprocess as sp
    p = sp.Popen(["ssh","-qC","-oStrictHostKeyChecking=no","-oBatchMode=yes",remoteHostName, f"rm {fileName}"])
    p.communicate()

class CopyFileFromRemoteCtxMgr:
  def __init__(self, hostName, fileName):
    self._remoteHostName = hostName
    self._fileName = Path(fileName)
    self._isRemote = IsRemote( hostName )

  def __enter__(self):
    if not self._isRemote:
      return
    dn = self._fileName.parent
    logging.debug(f"[SALOME_PyNode] Creating directory {dn}")
    dn.mkdir(parents=True, exist_ok=True)
    RetrieveRemoteFileLocallyInSameFileName(self._remoteHostName,f"{self._fileName}")
    
  def __exit__(self, exctype, exc, tb):
    if not self._isRemote:
      return
    self._fileName.unlink()
  
class BigFileOnDiskBase(abc.ABC):
  """
  Base class in charge of managing 
  Copy or share of file accross computation Nodes
  """
  def __init__(self, fileName):
    self._file_name = fileName

  def getFileName(self):
    return self._file_name

  @abc.abstractmethod
  def get(self, visitor = None):
    """
    Method called client side of data.
    """
    raise NotImplementedError
  
  @abc.abstractmethod
  def unlink(self):
    """
    Method called client side of data.
    """
    raise NotImplementedError


class BigFileOnDiskShare(BigFileOnDiskBase):

  def get(self, visitor = None):
    return GetObjectFromFile( self._file_name, visitor )
  
  def unlink(self):
    RemoveFileSafe( self._file_name )

class BigFileOnDiskSSDNoShare(BigFileOnDiskBase):
  def __init__(self, fileName):
    import socket
    super().__init__( fileName )
    # hostname hosting data
    self._hostname = socket.gethostname()

  def get(self, visitor = None):
    logging.debug(f"[SALOME_PyNode] Directory {Path(self._file_name).parent} should be created")
    with CopyFileFromRemoteCtxMgr(self._hostname, self._file_name):
      return GetObjectFromFile( self._file_name, visitor )
    
  def unlink(self):
    if IsRemote( self._hostname ):
      DestroyRemotely(self._hostname,self._file_name)
    else:
      RemoveFileSafe( self._file_name )

BigFileOnDiskClsFromProtocol = { 0 : BigFileOnDiskShare, 1 : BigFileOnDiskSSDNoShare }

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
  """
  :param fname:
  :type fname: str
  """
  fnameEff = fname.getFileName()
  if fnameEff in DicoForProxyFile:
    DicoForProxyFile[fnameEff] += 1
  else:
    DicoForProxyFile[fnameEff] = 2
  pass

def DecrRefInFile(fname):
  """
  :param fname:
  :type fname: BigFileOnDiskBase
  """
  if fname.getFileName() not in DicoForProxyFile:
    cnt = 1
  else:
    cnt = DicoForProxyFile[fname.getFileName()]
    DicoForProxyFile[fname.getFileName()] -= 1
    if cnt == 1:
      del DicoForProxyFile[fname.getFileName()]
  if cnt == 1:
    fname.unlink()
  pass

def GetBigObjectOnDiskThreshold():
    return KernelBasis.GetBigObjOnDiskThreshold()

def ActivateProxyMecanismOrNot( sizeInByte ):
  thres = GetBigObjectOnDiskThreshold()
  if thres == -1:
    return False
  else:
    return sizeInByte > thres

class BigObjectDirHandler(abc.ABC):
  def __init__(self, directory):
    self._directory = Path(directory)
  
  @property
  def directory(self):
    return self._directory

  def __enter__(self):
    return self
    
  def __exit__(self, exctype, exc, tb):
    return

class BigObjectDirHandlerOnDiskShare(BigObjectDirHandler):
  pass

class BigObjectDirHandlerOnDiskSSDNoShare(BigObjectDirHandler):

  def __enter__(self):
    logging.debug(f"[SALOME_PyNode] Creating directory {self._directory}")
    self._directory.mkdir(parents=True, exist_ok=True)
    return self
    
  def __exit__(self, exctype, exc, tb):
    pass

BigObjectDirHandlerFromProtocol = { 0 : BigObjectDirHandlerOnDiskShare, 1 : BigObjectDirHandlerOnDiskSSDNoShare }

def GetBigObjectDirectory():
  import os
  protocol, directory = KernelBasis.GetBigObjOnDiskProtocolAndDirectory()
  if not directory:
    raise RuntimeError("An object of size higher than limit detected and no directory specified to dump it in file !")
  return protocol, os.path.expanduser( os.path.expandvars( directory ) )

def GetBigObjectFileName():
  """
  Return a filename in the most secure manner (see tempfile documentation)
  """
  import tempfile
  protocol, directory = GetBigObjectDirectory()
  with BigObjectDirHandlerFromProtocol[protocol](directory) as handler:
    with tempfile.NamedTemporaryFile(dir = handler.directory, prefix="mem_", suffix=".pckl") as f:
      ret = f.name
    return BigFileOnDiskClsFromProtocol[protocol]( ret )

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
    DumpInFile( objSerialized, self._filename.getFileName() )

  def get(self, visitor = None):
    return self._filename.get(visitor)

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
    visitor.setFileName( fileName.getFileName() )
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
def getChargeOf( p ):
  a,b = p.cpu_percent(), p.memory_info().rss
  try:
    for c in p.children():
      a += c.cpu_percent(interval=0.01) ; b += c.memory_info().rss
  except:
    pass
  return a,b
import time
with open("{}","a") as f:
  f.write( "{{}}\\n".format( "{}" ) )
  while True:
    cpu,mem_rss = getChargeOf( process )
    f.write( "{{}}\\n".format( str( cpu ) ) )
    f.write( "{{}}\\n".format( str( mem_rss  ) ) )
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
        del self._monitoring_params
        import gc
        gc.collect() # force destruction of objects even in raise context

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
  
FinalCode = """import pickle
from SALOME_PyNode import LogOfCurrentExecutionSession,MY_PERFORMANCE_LOG_ENTRY_IN_GLBS
from SALOME_PyNode import ExchangeModeServerSideFactory
from KernelBasis import VerbosityActivated,SetVerbosityLevel,SetVerbosityActivated
from salome_utils import positionVerbosityOfLoggerRegardingState
import Engines
import salome
import os
import sys
salome.salome_init()
from datetime import datetime
SetVerbosityActivated( {} )
SetVerbosityLevel( "{}" )
positionVerbosityOfLoggerRegardingState()
# WorkDir may be important to replay : "{}"
orb = salome.orb
caseDirectory = "{}"
contextExchangeMode = "{}"
bigObjExchanger = ExchangeModeServerSideFactory( contextExchangeMode )
codeFileName = os.path.join( caseDirectory, "{}" )
inputFileName = "{}"
outputFileName = "{}"
outputsKeys = {}
exec( "{{}} = LogOfCurrentExecutionSession( orb.string_to_object( \\"{}\\" ) )".format(MY_PERFORMANCE_LOG_ENTRY_IN_GLBS) )
if VerbosityActivated():
  eval( "{{}}".format(MY_PERFORMANCE_LOG_ENTRY_IN_GLBS) ).addFreestyleAndFlush( ("b4loadctx",datetime.now()) )
context = bigObjExchanger.buildContextPointEntry(caseDirectory,inputFileName)
if VerbosityActivated():
  eval( "{{}}".format(MY_PERFORMANCE_LOG_ENTRY_IN_GLBS) ).addFreestyleAndFlush( ("afterloadctx",datetime.now()) )
context[MY_PERFORMANCE_LOG_ENTRY_IN_GLBS] = eval( MY_PERFORMANCE_LOG_ENTRY_IN_GLBS )
with open(codeFileName,"r") as f:
  code = f.read()
# go for execution
if VerbosityActivated():
  eval( "{{}}".format(MY_PERFORMANCE_LOG_ENTRY_IN_GLBS) ).addFreestyleAndFlush( ("bforeexec",datetime.now()) )
exec( code , context )
if VerbosityActivated():
  eval( "{{}}".format(MY_PERFORMANCE_LOG_ENTRY_IN_GLBS) ).addFreestyleAndFlush( ("afterexec",datetime.now()) )
# filter part of context to be exported to father process
context = dict( [(k,v) for k,v in context.items() if k in outputsKeys] )
if VerbosityActivated():
  eval( "{{}}".format(MY_PERFORMANCE_LOG_ENTRY_IN_GLBS) ).addFreestyleAndFlush( ("strtdumpout",datetime.now()) )
#
bigObjExchanger.pushContext(outputFileName,context)
if VerbosityActivated():
  eval( "{{}}".format(MY_PERFORMANCE_LOG_ENTRY_IN_GLBS) ).addFreestyleAndFlush( ("afterdump",datetime.now()) )
"""

class PythonFunctionEvaluatorParams:
  def __init__(self, mainFileName, codeFileName, inContextFileName, outContextFileName):
    """
    :param outContextFileName: instance of ExchangeContextCltAbs
    """
    self._main_filename = mainFileName
    self._code_filename = codeFileName
    self._in_context_filename = inContextFileName
    self._out_context_filename = outContextFileName
  @property
  def result(self):
    return self._out_context_filename.retrieveRemoteContext()

  def destroyOnOKAndReplay(self):
    self._out_context_filename.removeContextSupport( True )
    for fileToDestroy in [self._main_filename,self._code_filename,self._in_context_filename]:
      if os.path.exists( fileToDestroy ):
        os.unlink( fileToDestroy )
  def destroyOnKOAndReplay(self, containerRef):
    """
    Called in the context of failure with replay mode activated
    """
    self._out_context_filename.removeContextSupport( False )
    # register to container files group associated to the
    containerRef.addLogFileNameGroup([self._main_filename,self._code_filename,self._in_context_filename])

  @property
  def replayCmd(self):
    return "To replay : ( cd {} && python3 {} )".format(os.path.dirname(self._main_filename),os.path.basename(self._main_filename))
  
  @property
  def cleanOperations(self):
    import os
    return "To clean files : ( cd {} && rm {} )".format( os.path.dirname(self._main_filename)," ".join( [os.path.basename(self._main_filename),os.path.basename(self._code_filename),os.path.basename(self._in_context_filename)] ) )

  def strDependingOnReturnCode(self, keepFilesToReplay, returnCode):
    if returnCode == -1:
      return f"return with non zero code ({returnCode})"
    else:
      banner = 200*"*"
      if keepFilesToReplay:
        return f"""return with non zero code ({returnCode})
{banner}
Looks like a hard crash as returnCode {returnCode} != 0
{self.replayCmd}
{self.cleanOperations}
{banner}
"""
      else:
        return f"""return with non zero code ({returnCode})
{banner}
Looks like a hard crash as returnCode {returnCode} != 0
{banner}
"""

class ExchangeContextBridgeAbs(abc.ABC):
  def __init__(self):
    pass

  @abc.abstractmethod
  def buildContextPointEntry(self, caseDir, contextEntry):
    raise RuntimeError("Must be overloaded")

  @abc.abstractmethod
  def pushContext(self, contextPointEntry, contextEntry):
    raise RuntimeError("Must be overloaded")

class ExchangeContextUsingFile( ExchangeContextBridgeAbs ):
  def buildContextPointEntry(self, caseDir, contextEntry):
    self._case_dir = caseDir
    inputFileName = os.path.join( self._case_dir, contextEntry )
    with open(inputFileName,"rb") as f:
      context = pickle.load( f )
    return context
  
  def pushContext(self, contextPointEntry, context):
    outputFileName = os.path.join( self._case_dir, contextPointEntry )
    with open(outputFileName,"wb") as f:
      pickle.dump( context, f )

class ContextExchanger_i(Engines__POA.ContextExchanger):
  """
  In TCP mode, servant hosted SALOME_Container side.
  """
  def __init__(self, inCtx):
    import salome
    self._poa = salome.orb.resolve_initial_references("RootPOA")
    self._in_ctx = inCtx
    self._out_ctx = bytes(0)

  def getPOA(self):
    return self._poa

  def getInputContext(self):
    obj = SenderByte_i(self._poa, pickle.dumps( self._in_ctx ) ) ; id_o = self._poa.activate_object(obj) ; refPtr = self._poa.id_to_reference(id_o)
    return refPtr
    
  def pushOutputContext(self, ctx):
    try:
      self._out_ctx += ctx
    except Exception as e:
      raise SALOME.SALOME_Exception( SALOME.ExceptionStruct(SALOME.INTERNAL_ERROR,str(e),"pushOutputContext",0) )

  def finishPushContext(self):
    try:
      self._output_context = pickle.loads( self._out_ctx )
    except Exception as e:
      raise SALOME.SALOME_Exception( SALOME.ExceptionStruct(SALOME.INTERNAL_ERROR,str(e),"finishPushContext",0) )

  def getOutputContext(self):
    return self._output_context

class ExchangeContextUsingTCP( ExchangeContextBridgeAbs ):
  def buildContextPointEntry(self, caseDir, contextEntry):
    import salome
    salome.salome_init()
    self._orb = salome.orb
    self._data_exchange_channel = self._orb.string_to_object( contextEntry )
    return pickle.loads( SeqByteReceiver( self._data_exchange_channel.getInputContext() ).data() )
  
  def pushContext(self, contextPointEntry, context):
    ctxBytes = pickle.dumps( context )
    size = len( ctxBytes )
    if size <= SeqByteReceiver.CHUNK_SIZE:
      self._data_exchange_channel.pushOutputContext( ctxBytes )
    else:
      EFF_CHUNK_SIZE = SeqByteReceiver.CHUNK_SIZE // 8
      iStart = 0 ; iEnd = EFF_CHUNK_SIZE
      while iStart!=iEnd and iEnd <= size:
        self._data_exchange_channel.pushOutputContext( ctxBytes[iStart:iEnd] )
        iStart = iEnd; iEnd = min(iStart + EFF_CHUNK_SIZE,size)
    self._data_exchange_channel.finishPushContext()

def ExchangeModeServerSideFactory( exchangeMode ):
  if exchangeMode == "File":
    return ExchangeContextUsingFile()
  elif exchangeMode == "TCP":
    return ExchangeContextUsingTCP()
  else:
    raise RuntimeError( f"ExchangeModeServerSideFactory : {exchangeMode} not in [File,TCP]" )

class ExchangeContextCltAbs(abc.ABC):
  def __init__(self):
    pass

  @abc.abstractmethod
  def hostInputContext(self, dirForReplayFiles, contextFileBaseName, context):
    raise RuntimeError("Must be overloaded")
  
  @abc.abstractmethod
  def setOutputContextEntryPoint(self, dirForReplayFiles, outCtxtEntryPoint):
    raise RuntimeError("Must be overloaded")
  
  @abc.abstractmethod
  def retrieveRemoteContext(self):
    raise RuntimeError("Must be overloaded")
  
  @abc.abstractmethod
  def removeContextSupport(self, isOK):
    raise RuntimeError("Must be overloaded")

class ExchangeContextUsingFileClt(ExchangeContextCltAbs):

  def __init__(self, keepFilesToReplay):
    self._keep_in_files = keepFilesToReplay

  def hostInputContext(self, dirForReplayFiles, contextFileBaseName, context):
    self._in_ctx_entry_point = os.path.join( dirForReplayFiles, contextFileBaseName)
    with open(self._in_ctx_entry_point,"wb") as contextFd:
      pickle.dump( context, contextFd)
    return os.path.basename( self._in_ctx_entry_point )
  
  def setOutputContextEntryPoint(self, dirForReplayFiles, outCtxtEntryPoint):
    self._out_ctx_entry_point = os.path.join( dirForReplayFiles, outCtxtEntryPoint )
    return os.path.basename( self._out_ctx_entry_point )

  def retrieveRemoteContext(self):
    with open(self._out_ctx_entry_point,"rb") as f:
      return pickle.load( f )
    
  def removeContextSupport(self, isOK):
    fileNamesToRm = [self._out_ctx_entry_point]
    if isOK or not self._keep_in_files:
      fileNamesToRm += [self._in_ctx_entry_point]
    for fileName in fileNamesToRm:
      if os.path.exists( fileName ):
        os.unlink( fileName )

class ExchangeContextUsingTCPClt(ExchangeContextCltAbs):

  def hostInputContext(self, dirForReplayFiles, contextFileBaseName, context):
    import salome
    self._servant = ContextExchanger_i(context)
    poa = self._servant.getPOA()
    self._id_o = poa.activate_object(self._servant) ; refPtr = poa.id_to_reference(self._id_o)
    self._portable_serv_entry = salome.orb.object_to_string( refPtr )
    return self._portable_serv_entry
  
  def setOutputContextEntryPoint(self, dirForReplayFiles, outCtxtEntryPoint):
    return self._portable_serv_entry
  
  def retrieveRemoteContext(self):
    return self._servant.getOutputContext()
  
  def removeContextSupport(self, isOK):# isOK ignored. Because in memory mode nothing to save
    poa = self._servant.getPOA()
    poa.deactivate_object(self._id_o)
    del self._servant
    import gc
    gc.collect()
    
def ExchangeModeCltSideFactory( exchangeMode, keepFilesToReplay ):
  if exchangeMode == "File":
    return ExchangeContextUsingFileClt(keepFilesToReplay)
  elif exchangeMode == "TCP":
    return ExchangeContextUsingTCPClt()
  else:
    raise RuntimeError( f"ExchangeModeCltSideFactory : {exchangeMode} not in [File,TCP]" )

def ExecCrashProofGeneric( code, context, outargsname, containerRef, instanceOfLogOfCurrentSession, keepFilesToReplay, closeEyesOnErrorAtExit):
  """
  Equivalent of exec(code,context) but executed in a separate subprocess to avoid to make the current process crash.
  
  Args:
  -----

  code (str) : python code to be executed using context
  context (dict) : context to be used for execution. This context will be updated in accordance with the execution of code.
  outargsname (list<str>) : list of arguments to be exported 
  containerRef (Engines.Container) : Container ref (retrieving the Files to created when keepFilesToReplay is set to False)
  instanceOfLogOfCurrentSession (LogOfCurrentExecutionSession) : instance of LogOfCurrentExecutionSession to build remotely the reference in order to log information
  keepFilesToReplay (bool) : if True when something goes wrong during execution all the files to replay post mortem case are kept. If False only error is reported but files to replay are destoyed.
  closeEyesOnErrorAtExit (bool) : if True in case of crash of subprocess, if MY_KEY_TO_DETECT_FINISH is displayed at the end of stdout

  Return:
  -------

  ScriptExecInfo : instance serverside

  In/Out:
  -------

  context will be modified by this method. elts in outargsname will be added and their corresponding value coming from evaluation.
  """
  import tempfile
  import pickle
  import logging
  #
  def IsConsideredAsOKRun( returnCode, closeEyesOnErrorAtExit , stderr ):
    def StdErrTreatment(closeEyesOnErrorAtExit , stderr):
      if not closeEyesOnErrorAtExit:
        return stderr
      else:
        return stderr[:-len(MY_KEY_TO_DETECT_FINISH)]
    if returnCode == 0:
      return True,StdErrTreatment(closeEyesOnErrorAtExit , stderr)
    if not closeEyesOnErrorAtExit:
      return False, stderr
    if stderr[-len(MY_KEY_TO_DETECT_FINISH):] == MY_KEY_TO_DETECT_FINISH:
      logging.debug("[IsConsideredAsOKRun] returnCode is {}, closeEyesOnErrorAtExit is {} and {!r} was detected at the end of the error message"
                    .format(returnCode, closeEyesOnErrorAtExit, MY_KEY_TO_DETECT_FINISH))
      return True,stderr[:-len(MY_KEY_TO_DETECT_FINISH)]
    else:
      logging.debug("[IsConsideredAsOKRun] returnCode is {}, closeEyesOnErrorAtExit is {} and {!r} was NOT detected at the end of the error message"
                    .format(returnCode, closeEyesOnErrorAtExit, MY_KEY_TO_DETECT_FINISH))
      return False,stderr

  zestdout = []
  zestderr = []

  async def read_stream_and_display(stream, display: TextIO, listOfLines : list, timeout=None):
      
      """[EDF32670] Read from stream line by line until EOF, display, and capture the lines."""
      output = []
      try:
        # protect against exception in case of broken pipe due to timeout
        while True:
          if stream.at_eof():
              break
          line: bytes = await stream.readline()
          tmp = line.decode()
          listOfLines.append( tmp )
          display.write( tmp )  # assume it doesn't block
          display.flush()
      except:
        pass
      return b"".join(output)

  async def read_and_display(*cmd, timeout=None):
      """Capture cmd's stdout, stderr while displaying them as they arrive
      (line by line).

      """
      global zestdout, zestderr
      # start process
      process = await asyncio.create_subprocess_exec(
        *cmd,
        limit = 1024 * 1024 * 1,  # 1 MB
        cwd = os.getcwd(),
        stdout=asyncio.subprocess.PIPE,
        stderr=asyncio.subprocess.PIPE,
        )
      # read child's stdout/stderr concurrently (capture and display)
      try:
          await asyncio.gather(
              asyncio.wait_for(read_stream_and_display(process.stdout, sys.stdout, zestdout), timeout=timeout),
              asyncio.wait_for(read_stream_and_display(process.stderr, sys.stderr, zestderr), timeout=timeout),
          )
      except asyncio.TimeoutError:
          process.kill()
          await process.wait()
          raise

      # wait for the process to exit
      await process.communicate()
      return process.returncode

  #
  def InternalExecResistant( exchangeMode, keepFilesToReplay, code, context, outargsname):
    global zestdout, zestderr
    import KernelBasis
    import salome
    salome.salome_init()
    orb = salome.orb
    iorScriptLog = orb.object_to_string( instanceOfLogOfCurrentSession._remote_handle )#ref ContainerScriptPerfLog_ptr
    ####
    EXEC_CODE_FNAME_PXF = "execsafe_"
    def RetrieveUniquePartFromPfx( fname ):
      return os.path.splitext( os.path.basename(fname)[len(EXEC_CODE_FNAME_PXF):] )[0]
    dirForReplayFiles = KernelBasis.GetDirectoryForReplayFiles()
    if not dirForReplayFiles:
      raise RuntimeError("You are in context of exec resistant you have to position Directory hosting these files properly")
    with tempfile.NamedTemporaryFile(dir=dirForReplayFiles,prefix=EXEC_CODE_FNAME_PXF,suffix=".py", mode="w", delete = False) as codeFd:
      codeFd.write( "{}\n".format( containerRef.get_startup_code() ) )
      codeFd.write( "import sys; sys.stdout.reconfigure(line_buffering=True)\n") # To allow the unbuffer of stdout and stderr in subprocess
      codeFd.write( code )
      if closeEyesOnErrorAtExit:
        codeFd.write( """
sys.stderr.write({!r})
sys.stderr.flush()""".format( MY_KEY_TO_DETECT_FINISH ) )
      codeFd.flush()
      exCtx = ExchangeModeCltSideFactory(exchangeMode, keepFilesToReplay)
      codeFileNameFull =  codeFd.name
      codeFileName = os.path.basename( codeFileNameFull )
      contextFileName = exCtx.hostInputContext(dirForReplayFiles, "contextsafe_{}.pckl".format( RetrieveUniquePartFromPfx( codeFileName  ) ), context)
      resFileName = exCtx.setOutputContextEntryPoint( dirForReplayFiles, "outcontextsafe_{}.pckl".format( RetrieveUniquePartFromPfx( codeFileName  ) ) )
      mainExecFileName = os.path.join( dirForReplayFiles, "mainexecsafe_{}.py".format( RetrieveUniquePartFromPfx( codeFileName  ) ) )
      with open(mainExecFileName,"w") as f:
        f.write( FinalCode.format( KernelBasis.VerbosityActivated() ,KernelBasis.VerbosityLevel(),
                                  os.getcwd(), dirForReplayFiles, exchangeMode, codeFileName, contextFileName, resFileName, outargsname, iorScriptLog ) )
      timeOut = KernelBasis.GetExecutionTimeOut()
      nbRetry = KernelBasis.GetNumberOfRetry()
      logging.debug( "Nb retry = {}   Timout in seconds = {}".format( nbRetry, timeOut ) )
      returnCode = -1
      zestdout = []
      zestderr = []

      for iTry in range( nbRetry ):
        if iTry > 0:
          print( f"WARNING : Retry # {iTry}. Following code has generated non zero return code ( {returnCode} ). Trying again ... \n{code}")
        logging.debug( "try # {} / {} --- ".format( iTry, nbRetry ) )
        try:
          cmd = [sys.executable, mainExecFileName]
          if timeOut > 0:
            async_args = {
              "main": asyncio.wait_for(read_and_display(*cmd), timeout=timeOut),
            }
          else:
            async_args = {
              "main": read_and_display(*cmd),
            }
          returnCode = asyncio.run(**async_args)

        except asyncio.exceptions.TimeoutError as e:
          print( f"WARNING : during retry #{iTry} timeout set to {timeOut} s has failed !")
          returnCode = 10000000000 + timeOut

        if returnCode == 0:
          if iTry >= 1:
            logging.warning( "At Retry #{} it's successful :)".format(iTry) )
          break
    stdout = "".join( zestdout ) ; zestdout = []
    stderr = "".join( zestderr ) ; zestderr = []
    return returnCode, stdout, stderr, PythonFunctionEvaluatorParams(mainExecFileName,codeFileNameFull,contextFileName,exCtx)
  
  ret = instanceOfLogOfCurrentSession._current_instance
  exchangeMode = "File"
  if not keepFilesToReplay:
    exchangeMode = "TCP"
  returnCode, stdout, stderr, evParams = InternalExecResistant( exchangeMode, keepFilesToReplay, code, context, outargsname )
  isOK, stderr = IsConsideredAsOKRun( returnCode, closeEyesOnErrorAtExit , stderr )
  if isOK:
    pcklData = instanceOfLogOfCurrentSession._remote_handle.getObj()
    if len(pcklData) > 0:
      ret = pickle.loads( pcklData )
    context.update( evParams.result )
    evParams.destroyOnOKAndReplay()
    if returnCode != 0:
      print( "WARNING : Following code has generated non zero return code ( {} ) but considered as OK\n{}".format( returnCode, code ) )
    return ret
  else:
    if keepFilesToReplay:
      evParams.destroyOnKOAndReplay( containerRef )
    else:
      evParams.destroyOnOKAndReplay()
    raise RuntimeError(f"Subprocess launched {evParams.strDependingOnReturnCode(keepFilesToReplay,returnCode)}stdout :\n{stdout}\nstderr :\n{stderr}")

def ExecCrashProofWithReplay( code, context, outargsname, containerRef, instanceOfLogOfCurrentSession ):
  return ExecCrashProofGeneric(code, context, outargsname, containerRef, instanceOfLogOfCurrentSession, keepFilesToReplay = True, closeEyesOnErrorAtExit = False)

def ExecCrashProofWithoutReplay( code, context, outargsname, containerRef, instanceOfLogOfCurrentSession ):
  return ExecCrashProofGeneric(code, context, outargsname, containerRef, instanceOfLogOfCurrentSession, keepFilesToReplay = False, closeEyesOnErrorAtExit = False)

def ExecCrashProofWithReplayFT( code, context, outargsname, containerRef, instanceOfLogOfCurrentSession ):
  return ExecCrashProofGeneric(code, context, outargsname, containerRef, instanceOfLogOfCurrentSession, keepFilesToReplay = True, closeEyesOnErrorAtExit = True)

def ExecCrashProofWithoutReplayFT( code, context, outargsname, containerRef, instanceOfLogOfCurrentSession ):
  return ExecCrashProofGeneric(code, context, outargsname, containerRef, instanceOfLogOfCurrentSession, keepFilesToReplay = False, closeEyesOnErrorAtExit = True)

def ExecLocal( code, context, outargsname, containerRef, instanceOfLogOfCurrentSession ):
  exec( code, context )
  return instanceOfLogOfCurrentSession._current_instance

class LogOfCurrentExecutionSessionAbs(abc.ABC):
  def __init__(self):
    self._current_instance = ScriptExecInfo()

  def addInfoOnLevel2(self, key, value):
    setattr(self._current_instance,key,value)

  @abc.abstractmethod
  def addFreestyleAndFlush(self, value):
    raise RuntimeError("Must be overloaded")

class LogOfCurrentExecutionSession(LogOfCurrentExecutionSessionAbs):
  def __init__(self, handleToCentralizedInst):
    super().__init__()
    self._remote_handle = handleToCentralizedInst

  def addFreestyleAndFlush(self, value):
    self._current_instance.freestyle = value
    self.finalizeAndPushToMasterAppendFreestyle()

  def finalizeAndPushToMaster(self):
    """
    Voluntary do nothing in case of problem to avoid to trouble execution
    """
    try:
      self._remote_handle.assign( pickle.dumps( self._current_instance ) )
    except:
      pass
    
  def finalizeAndPushToMasterAppendFreestyle(self):
    """
    Voluntary do nothing in case of problem to avoid to trouble execution
    """
    try:
      self._remote_handle.assignAndAppendFreestyle( pickle.dumps( self._current_instance ) )
    except:
      pass

class LogOfCurrentExecutionSessionStub(LogOfCurrentExecutionSessionAbs):
  """
  This class is to stub LogOfCurrentExecutionSession in context of replay where the server (handleToCentralizedInst) has vanished
  """
  def __init__(self, handleToCentralizedInst = None):
    super().__init__()
  def addFreestyleAndFlush(self, value):
    pass

class PyScriptNode_Abstract_i(Engines__POA.PyScriptNode,Generic,abc.ABC):
  """The implementation of the PyScriptNode CORBA IDL that executes a script"""
  def __init__(self, nodeName, code, poa, my_container, logscript):
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

  @abc.abstractmethod
  def executeNow(self, outargsname):
    raise RuntimeError("Must be overloaded")
      
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
    def executeSecondInternal(monitoringtimeresms):
      with GenericPythonMonitoringLauncherCtxMgr( CPUMemoryMonitoring( monitoringtimeresms ) ) as monitoringParams:
        currentInstance = self.executeNow( outargsname )
        cpumeminfo = ReadCPUMemInfo( monitoringParams )
      return cpumeminfo, currentInstance

    import sys
    try:
      self.addTimeInfoOnLevel2("startExecTime")
      ##
      self.addInfoOnLevel2("measureTimeResolution",self.my_container_py.monitoringtimeresms())
      self._current_execution_session.finalizeAndPushToMaster() # flush it to the server (scheduler hosting ContainerManager) in order to retrieve it in Python server
      cpumeminfo, self._current_execution_session._current_instance = executeSecondInternal( self.my_container_py.monitoringtimeresms() )
      ##
      self.addInfoOnLevel2("CPUMemDuringExec",cpumeminfo)
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

class PyScriptNode_i(PyScriptNode_Abstract_i):
  def __init__(self, nodeName, code, poa, my_container, logscript):
    super().__init__(nodeName, code, poa, my_container, logscript)

  def executeNow(self, outargsname):
    return ExecLocal(self.ccode,self.context,outargsname,self.my_container,self._current_execution_session)
    
class PyScriptNode_OutOfProcess_i(PyScriptNode_Abstract_i):
  def __init__(self, nodeName, code, poa, my_container, logscript):
    super().__init__(nodeName, code, poa, my_container, logscript)

  def executeNow(self, outargsname):
    return ExecCrashProofWithoutReplay(self.code,self.context,outargsname,self.my_container,self._current_execution_session)

class PyScriptNode_OutOfProcess_Replay_i(PyScriptNode_Abstract_i):
  def __init__(self, nodeName, code, poa, my_container, logscript):
    super().__init__(nodeName, code, poa, my_container, logscript)

  def executeNow(self, outargsname):
    return ExecCrashProofWithReplay(self.code,self.context,outargsname,self.my_container,self._current_execution_session)

class PyScriptNode_OutOfProcess_FT_i(PyScriptNode_Abstract_i):
  def __init__(self, nodeName, code, poa, my_container, logscript):
    super().__init__(nodeName, code, poa, my_container, logscript)

  def executeNow(self, outargsname):
    return ExecCrashProofWithoutReplayFT(self.code,self.context,outargsname,self.my_container,self._current_execution_session)

class PyScriptNode_OutOfProcess_Replay_FT_i(PyScriptNode_Abstract_i):
  def __init__(self, nodeName, code, poa, my_container, logscript):
    super().__init__(nodeName, code, poa, my_container, logscript)

  def executeNow(self, outargsname):
    return ExecCrashProofWithReplayFT(self.code,self.context,outargsname,self.my_container,self._current_execution_session)
