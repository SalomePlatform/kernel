#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2023-2024  CEA, EDF
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

from collections import defaultdict

import pickle

class ScriptExecInfo:
    @classmethod
    def GetRepresentationOfTimeDelta(cls,endTime, startTime):
       if endTime is None and startTime is None:
          return "not measured"
       td = endTime - startTime
       import time
       ts_of_td = time.gmtime(td.total_seconds())
       return "{}.{:06d}".format(time.strftime("%H:%M:%S",ts_of_td),td.microseconds)
    
    @classmethod
    def MemRepr(cls,memInByte):
      m = int( memInByte )
      UNITS=["B","kB","MB","GB"]
      remain = 0
      oss = ""
      for i in range( len(UNITS) ):
          if m<1024:
              oss = "{:03d}".format( int( (remain/1024)*1000 ) )
              oss = "{}.{} {}".format(m,oss,UNITS[i])
              return oss
          else:
              if i!=3:
                  remain = m%1024
                  m//=1024
      return "{} {}".format(m,UNITS[3])
    
    @classmethod
    def SpeedRepr(cls,memInBytePerS):
       return "{}/s".format( cls.MemRepr(memInBytePerS) )

    def __init__(self):
      self._measure_time_resolution_ms = None
      self._cpu_mem_during_exec = None
      self._start_exec_time = None
      self._end_exec_time = None
      self._start_input_time = None
      self._end_input_time = None
      self._start_output_time = None
      self._end_output_time = None
      self._input_mem = 0
      self._input_hdd_mem = None
      self._output_mem = 0
      self._output_hdd_mem = None
      self._start_pos_log = None
      self._stop_pos_log = None
      self._freestyle_log = []

    @property
    def freestyle(self):
       return self._freestyle_log
    
    @freestyle.setter
    def freestyle(self, value):
       self._freestyle_log = [ value ]

    def preappendFreestyle(self, value):
       self._freestyle_log = value + self._freestyle_log

    def appendFreestyle(self, value):
       self._freestyle_log += value

    @property
    def measureTimeResolution(self):
      return self._measure_time_resolution_ms
    
    @measureTimeResolution.setter
    def measureTimeResolution(self, value):
      self._measure_time_resolution_ms = value

    @property
    def tracePosStart(self):
      return self._start_pos_log
    
    @tracePosStart.setter
    def tracePosStart(self,value):
      self._start_pos_log = value

    @property
    def tracePosStop(self):
      return self._stop_pos_log
    
    @tracePosStop.setter
    def tracePosStop(self,value):
      self._stop_pos_log = value
      
    @property
    def CPUMemDuringExec(self):
      return self._cpu_mem_during_exec
    
    @CPUMemDuringExec.setter
    def CPUMemDuringExec(self,value):
      self._cpu_mem_during_exec = value

    @property
    def CPUMemDuringExecStr(self):
      return [(a,ScriptExecInfo.MemRepr(b)) for a,b in self._cpu_mem_during_exec.data]

    @property
    def inputMem(self):
      return self._input_mem
    
    @inputMem.setter
    def inputMem(self,value):
      self._input_mem = value
       
    @property
    def inputMemStr(self):
      return ScriptExecInfo.MemRepr( self.inputMem )
    
    @property
    def outputMem(self):
      return self._output_mem
    
    @outputMem.setter
    def outputMem(self,value):
      self._output_mem = value
       
    @property
    def outputMemStr(self):
      return ScriptExecInfo.MemRepr( self.outputMem )
    
    def inputReadHDDSize(self):
       return self.inputHDDMem.getSizeOfFileRead()
    
    def inputReadHDDSizeRepr(self):
       return ScriptExecInfo.MemRepr( self.inputReadHDDSize() )
    
    def inputReadHDDSpeed(self):
       return self.inputReadHDDSize() / ( self.endInputTime - self.startInputTime ).total_seconds()
    
    def inputReadHDDSpeedRepr(self):
       return ScriptExecInfo.SpeedRepr( self.inputReadHDDSpeed() )
    
    def outputWriteHDDSize(self):
       return self.outputHDDMem.getSizeOfFileRead()
    
    def outputWriteHDDSizeRepr(self):
       return ScriptExecInfo.MemRepr( self.outputWriteHDDSize() )

    def outputWriteHDDSpeed(self):
      return self.outputWriteHDDSize() / ( self.endOutputTime - self.startOutputTime ).total_seconds()
    
    def outputWriteHDDSpeedRepr(self):
      return ScriptExecInfo.SpeedRepr( self.outputWriteHDDSpeed() )
    
    @property
    def inputHDDMem(self):
      return self._input_hdd_mem
    
    @inputHDDMem.setter
    def inputHDDMem(self,value):
      self._input_hdd_mem = value

    @property
    def inputHDDMemStr(self):
      if self._input_hdd_mem is None:
         return "not computed"
      return " ".join( [ ScriptExecInfo.MemRepr( elt.getSizeOfFileRead() ) for elt in self._input_hdd_mem] )
    
    @property
    def outputHDDMem(self):
      return self._output_hdd_mem
    
    @outputHDDMem.setter
    def outputHDDMem(self,value):
      self._output_hdd_mem = value

    @property
    def outputHDDMemStr(self):
      if self._output_hdd_mem is None:
         return "not computed"
      return " ".join( [ ScriptExecInfo.MemRepr( elt.getSizeOfFileRead() ) for elt in self._output_hdd_mem] )

    @property
    def startInputTime(self):
      return self._start_input_time
    
    @startInputTime.setter
    def startInputTime(self,value):
      self._start_input_time = value

    @property
    def endInputTime(self):
      return self._end_input_time
    
    @endInputTime.setter
    def endInputTime(self,value):
      self._end_input_time = value

    @property
    def startExecTime(self):
      return self._start_exec_time
    
    @startExecTime.setter
    def startExecTime(self,value):
      self._start_exec_time = value

    @property
    def endExecTime(self):
      return self._end_exec_time
    
    @endExecTime.setter
    def endExecTime(self,value):
      self._end_exec_time = value

    @property
    def execTime(self):
      if ( self.endExecTime is not None ) and (self.startExecTime is not None):
        return self.endExecTime - self.startExecTime
      return None
    
    @property
    def fullExecTime(self):
      return self.endOutputTime - self.startInputTime

    @property
    def startOutputTime(self):
      return self._start_output_time
    
    @startOutputTime.setter
    def startOutputTime(self,value):
      self._start_output_time = value

    @property
    def endOutputTime(self):
      return self._end_output_time
    
    @endOutputTime.setter
    def endOutputTime(self,value):
      self._end_output_time = value

    @property
    def execTimeStr(self):
       return ScriptExecInfo.GetRepresentationOfTimeDelta(self.endExecTime,self.startExecTime)
    
    @property
    def inputTimeStr(self):
       return ScriptExecInfo.GetRepresentationOfTimeDelta(self.endInputTime,self.startInputTime)
    
    @property
    def outputTimeStr(self):
       return ScriptExecInfo.GetRepresentationOfTimeDelta(self.endOutputTime,self.startOutputTime)

    def __str__(self):
      CPUMemDuringExecForStr = self.CPUMemDuringExecStr
      if len( CPUMemDuringExecForStr ) > 30:
         CPUMemDuringExecForStr = "{} ...".format( str(CPUMemDuringExecForStr[:30]) )
      else:
         CPUMemDuringExecForStr = str( CPUMemDuringExecForStr )
      return """start exec time = {self.startExecTime}
end exec time = {self.endExecTime}
exec_time = {self.execTimeStr}
Measure time resolution = {self.measureTimeResolution} ms
CPU and mem monitoring = {CPUMemDuringExecForStr}
input unpickling and ev load from disk time = {self.inputTimeStr}
output serialization and ev write to disk time = {self.outputTimeStr}
input memory size before exec (MemoryPeak 2x) = {self.inputMemStr}
input memory size from HDD = {self.inputHDDMemStr}
output memory size after exec (MemoryPeak 2x) = {self.outputMemStr}
output memory size from HDD = {self.outputHDDMemStr}
Start position in log = {self.tracePosStart}
End position in log = {self.tracePosStop}""".format(**locals())

class ScriptExecInfoDeco:
  def __init__(self, eff, father):
    self._eff = eff
    self._father = father
  @property
  def father(self):
    return self._father
  def get(self):
    return self._eff
  def __getitem__(self,i):
    return self._eff[i]
  def __str__(self):
    return self._eff.__str__()
  def __repr__(self):
    return self._eff.__repr__()
  def log(self):
    with open(self.father.father.logfile,"rb") as f:
       cont = f.read()
    return cont[self._eff.tracePosStart:self._eff.tracePosStop].decode()
  @property
  def nodeName(self):
    return self.father.get().nodeName
  @property
  def code(self):
    return self.father.code
  @property
  def ns_entry(self):
    return self.father.father.ns_entry
  @property
  def computingNode(self):
    return self.father.computingNode
  
  @property
  def freestyle(self):
      return self.get().freestyle
  
  @property
  def measureTimeResolution(self):
    return self.get().measureTimeResolution
    
  @property
  def CPUMemDuringExec(self):
    return self.get().CPUMemDuringExec

  @property
  def inputMem(self):
    return self.get().inputMem
  
  @property
  def outputMem(self):
    return self.get().outputMem
  
  @property
  def inputHDDMem(self):
    return self.get().inputHDDMem
  
  @property
  def outputHDDMem(self):
    return self.get().outputHDDMem
  
  def inputReadHDDSize(self):
    return self.get().inputReadHDDSize()
  
  def inputReadHDDSizeRepr(self):
    return self.get().inputReadHDDSizeRepr()
  
  def inputReadHDDSpeed(self):
    return self.get().inputReadHDDSpeed()
  
  def inputReadHDDSpeedRepr(self):
    return self.get().inputReadHDDSpeedRepr()
  
  def outputWriteHDDSize(self):
    return self.get().outputWriteHDDSize()
  
  def outputWriteHDDSizeRepr(self):
    return self.get().outputWriteHDDSizeRepr()
  
  def outputWriteHDDSpeed(self):
    return self.get().outputWriteHDDSpeed()
  
  def outputWriteHDDSpeedRepr(self):
    return self.get().outputWriteHDDSpeedRepr()

  @property
  def startInputTime(self):
    return self.get().startInputTime

  @property
  def endInputTime(self):
    return self.get().endInputTime

  @property
  def startExecTime(self):
    return self.get().startExecTime

  @property
  def endExecTime(self):
    return self.get().endExecTime

  @property
  def execTime(self):
    return self.get().execTime
  
  @property
  def fullExecTime(self):
     return self.get().fullExecTime

  @property
  def startOutputTime(self):
    return self.get().startOutputTime

  @property
  def endOutputTime(self):
    return self.get().endOutputTime

class ScriptInfoAbstract:
  def __init__(self, scriptPtr):
      self._node_name = scriptPtr.getName()
      self._code = scriptPtr.getCode()
      self._exec = [pickle.loads(elt.getObj()) for elt in scriptPtr.listOfExecs()]

  @property
  def execs(self):
      return self._exec

  @property
  def nodeName(self):
      return self._node_name

  @property
  def code(self):
      return self._code
  
  @code.setter
  def code(self,value):
      self._code = value

  def __len__(self):
      return len( self._exec )
  
  def __getitem__(self,i):
      return self._exec[i]

  def __str__(self):
      return """name = {self.nodeName}\ncode = {self.code}\nexecs = {self.execs}""".format(**locals())
  
  def __repr__(self):
      return """ScriptInfo \"{self.nodeName}\"""".format(**locals())
  
class ScriptInfoClt(ScriptInfoAbstract):
  def __init__(self, scriptPtr):
      def unPickledSafe( dataPickled ):
        if len(dataPickled) > 0:
          return pickle.loads(dataPickled)
        else:
           return None  
      self._node_name = scriptPtr.getName()
      self._code = scriptPtr.getCode()
      self._exec = [unPickledSafe(elt.getObj()) for elt in scriptPtr.listOfExecs()]

class ScriptInfo(ScriptInfoAbstract):
  def __init__(self, nodeName, code, execs):
      self._node_name = nodeName
      self._code = code
      self._exec = execs

class ScriptInfoDeco:
  def __init__(self, eff, father):
    self._eff = eff
    self._father = father
  @property
  def father(self):
    return self._father
  def get(self):
    return self._eff
  @property
  def nodeName(self):
    return self.get().nodeName
  @property
  def code(self):
    return self.get().code
  @property
  def ns_entry(self):
    return self.father.ns_entry
  @property
  def computingNode(self):
    return self.father.computingNode
  def __getitem__(self,i):
    return ScriptExecInfoDeco( self._eff[i], self )
  def __len__(self):
    return self._eff.__len__()
  def __str__(self):
    return self._eff.__str__()
  def __repr__(self):
    return self._eff.__repr__()

class ContainerLogInfoAbstract:
  
  @property
  def log(self):
    with open(self.logfile,"rb") as f:
       cont = f.read()
    return cont.decode()

  @property
  def ns_entry(self):
    return self._ns_entry
  
  @property
  def logfile(self):
    return self._log_file
  
  @property
  def computingNode(self):
    return ContainerLogInfoAbstract.ComputingNodeFromNSEntry( self.ns_entry )
  
  def __len__(self):
    return len( self._scripts )
  
  def __getitem__(self,i):
    return ScriptInfoDeco( self._scripts[i], self)
  
  def __str__(self):
    return """NS entry = {self.ns_entry} LogFile = {self.logfile}""".format(**locals())
  
  @classmethod
  def ComputingNodeFromNSEntry(cls, nsEntry):
    return nsEntry.split("/")[2]
  
class ContainerLogInfoClt(ContainerLogInfoAbstract):
  def __init__(self,contLogPtr):
    self._log_file = contLogPtr.getLogFile()
    self._ns_entry = contLogPtr.getContainerEntryInNS()
    self._scripts = [ScriptInfoClt(elt) for elt in contLogPtr.listOfScripts()]
    
class ContainerLogInfo(ContainerLogInfoAbstract):
  def __init__(self, nsEntry, logFile, scripts):
     self._log_file = logFile
     self._ns_entry = nsEntry
     self._scripts = scripts

from abc import ABC, abstractmethod

class InOutputObjVisitorAbstract(ABC):
  def __init__(self):
      self._cur_obj = None
      self._data = []

  def enter(self):
      self._cur_obj = ObjMemModel()
      return self._cur_obj
  
  def leave(self):
      self._data.append( self._cur_obj )
      self._cur_obj = None

  def getSizeOfFileRead(self):
      return sum( [elt.getSizeOfFileRead() for elt in self._data] )
      
  def visitor(self):
      return self

  def setHDDMem(self, v):
      pass
  
  def setFileName(self, fileName):
      pass

  @abstractmethod
  def getRepr(self):
      pass

class InOutputObjVisitorIter:
  def __init__(self, visitor):
      self._visitor = visitor
      self._current = 0

  def __next__(self):
      if self._current >= len(self._visitor._data):
            raise StopIteration
      else:
        ret = self._visitor._data[ self._current ]
        self._current += 1
        return ret

class InOutputObjVisitor(InOutputObjVisitorAbstract):
  def __init__(self):
      super().__init__()
      
  def getRepr(self):
      return self.getSizeOfFileRead()
  
  def __iter__(self):
     return InOutputObjVisitorIter(self)

class ObjMemModel(InOutputObjVisitorAbstract):
  def __init__(self):
      super().__init__()
      self._hdd_mem = 0
      self._file_name = None
      
  def setHDDMem(self, v):
      self._hdd_mem = v
      del self._data

  def setFileName(self, fileName):
      self._file_name = fileName
      pass
      
  def getSizeOfFileRead(self):
      if hasattr(self,"_data"):
        return super().getSizeOfFileRead()
      else:
        return self._hdd_mem
  
  def getRepr(self):
      return self.getSizeOfFileRead()

class FakeObjVisitor:
  def setHDDMem(self, v):
      pass
  
  def setFileName(self, fileName):
      pass
    
  def visitor(self):
      return None

class InOutputObjVisitorCM:
  def __init__(self, visitor):
     self._visitor = visitor
  def __enter__(self):
      if self._visitor:
        r = self._visitor.enter()
        return r
      else:
        return FakeObjVisitor()
  def __exit__(self,exctype, exc, tb):
      if self._visitor:
        self._visitor.leave()
      pass
  
class OffsetType:
  def __init__(self,i):
    self._i = i
  def __int__(self):
    return self._i
  def __iadd__(self,delta):
    self._i += delta
    return self

def unserializeInt(structData, offset):
    from ctypes import c_int
    sz_of_cint = 4
    sz = c_int.from_buffer_copy( structData[int(offset):int(offset)+sz_of_cint] ).value
    offset += sz_of_cint
    return sz

def unserializeString(structData,offset):
    sz = unserializeInt(structData,offset)
    ret = structData[int(offset):int(offset)+sz].decode()
    offset += sz
    return ret

def unserializeContainerScriptExecPerfLog(structData, offset):
    import pickle
    sz = unserializeInt(structData,offset)
    inst = None
    if sz > 0:
        inst = pickle.loads( structData[int(offset):int(offset)+sz] )
    offset += sz
    return inst

def unserializeContainerScriptPerfLog(structData, offset):
    name = unserializeString(structData,offset)
    code = unserializeString(structData,offset)
    numberOfSessions = unserializeInt(structData,offset)
    sessions = []
    for _ in range(numberOfSessions):
        session = unserializeContainerScriptExecPerfLog(structData,offset)
        sessions.append( session )
    return ScriptInfo(name,code,sessions)

def unserializeContainerPerfLog(structData, offset):
    nsEntry = unserializeString(structData,offset)
    logFile = unserializeString(structData,offset)
    scripts = []
    nbScripts = unserializeInt(structData,offset)
    for _ in range(nbScripts):
        script = unserializeContainerScriptPerfLog(structData,offset)
        scripts.append( script )
    return ContainerLogInfo(nsEntry,logFile,scripts)

def unserializeLogManager(structData):
    offset = OffsetType(0)
    numberOfScripts = unserializeInt(structData,offset)
    logManagerInst = []
    for _ in range(numberOfScripts):
        containerPerfLogInst = unserializeContainerPerfLog(structData,offset)
        logManagerInst.append( containerPerfLogInst )
    if int(offset) != len(structData):
        raise RuntimeError("Something went wrong during unserialization phase.")
    return logManagerInst

def ListAllExecContainIn( listOfContainerLogInfo ):
    """
    For all ContainerLogInfo contained in listOfContainerLogInfo extract all ScriptExecInfo contained recursively
    in it. This method filters all "side" executions like those positionning environment for exemple.

    See also : ListAllExecFinishedContainIn

    Args:
    -----

    listOfContainerLogInfo (list<ContainerLogInfo>) : instance typically returned by salome.LogManagerLoadFromFile

    Returns
    -------

    list<ScriptExecInfoDeco> : all ScriptExecInfoDeco instance contained recursively in all input ContainerLogInfo instances

    """
    allexecs = sum( [sum( [[myexec for myexec in ps] for ps in cont],[] ) for cont in listOfContainerLogInfo], [] )
    return [elt for elt in allexecs if elt.get() is not None]

def ListAllExecFinishedContainIn( listOfContainerLogInfo ):
    """
    For all ContainerLogInfo contained in listOfContainerLogInfo extract all ScriptExecInfo contained recursively
    in it. This method filters all "side" executions like those positionning environment for exemple and those not finished.

    See also : ListAllExecContainIn

    Args:
    -----

    listOfContainerLogInfo (list<ContainerLogInfo>) : instance typically returned by salome.LogManagerLoadFromFile

    Returns
    -------

    list<ScriptExecInfoDeco> : all ScriptExecInfoDeco instance contained recursively in all input ContainerLogInfo instances

    """
    beforeFiltering = ListAllExecContainIn( listOfContainerLogInfo )
    return [elt for elt in beforeFiltering if elt.get().execTime is not None]

def IsExecTimeHigherThan( execInstDeco, limitDuration ):
    """
    Example of Usage :

    [elt for elt in allexecs if IsExecTimeHigherThan(elt,datetime.timedelta(hours=1))]
    
    Args:
    -----

    execInstDeco (ScriptExecInfoDeco)
    limitDuration (datetime.timedelta)  Ex (datetime.timedelta(hours=1))

    """
    if execInstDeco.get() is not None:
        return execInstDeco.get().execTime > limitDuration
    else:
        return False
    
def GetMaxTimeExec( listOfFinishedExecs ):
    """
    Returns the instance among listOfFinishedExecs that spends the max time to be executed

    Args:
    -----

    listOfFinishedExecs ( list<ScriptExecInfoDeco> ) : instance of ScriptExecInfoDeco that have finished to be executed
    Typically returned by ListAllExecFinishedContainIn
    
    Returns
    -------

    ScriptExecInfoDeco :
    """
    return listOfFinishedExecs[ max([(i,elt.execTime) for i,elt in enumerate( listOfFinishedExecs) ],key = lambda x : x[1])[0] ]

def GetMinTimeExec( listOfFinishedExecs ):
    """
    Returns the instance among listOfFinishedExecs that spends the min time to be executed

    Args:
    -----

    listOfFinishedExecs ( list<ScriptExecInfoDeco> ) : instance of ScriptExecInfoDeco that have finished to be executed
    Typically returned by ListAllExecFinishedContainIn
    
    Returns
    -------

    ScriptExecInfoDeco :
    """
    return listOfFinishedExecs[ min([(i,elt.execTime) for i,elt in enumerate( listOfFinishedExecs) ],key = lambda x : x[1])[0] ]

class DistriutionClass:
    def __init__(self, begin, end, listOfExecs):
        self._begin = begin
        self._end = end
        self._list_of_execs = sorted( listOfExecs, key = lambda x : x.execTime)
    def __getitem__(self, *args):
        return self._list_of_execs.__getitem__( *args )
    @property
    def begin(self):
        return self._begin
    @property
    def end(self):
        return self._end
    @property
    def listOfExecs(self):
        return self._list_of_execs
    @property
    def size(self):
        return len( self.listOfExecs )
    @property
    def reprGlobal(self):
        return f"[{self.begin}->{self.end}] : {self.size} execs"

class DistributionOfExecs:
    """
    Class in charge to distribute execution log instance into equi-classes
    """
    def __init__(self, listOfFinishedExecs, nbOfClasses):
        self._list_of_finished_execs = listOfFinishedExecs
        self._nb_of_classes = nbOfClasses
        self._classes = DistributionOfExecs.ComputeDistributionOfExecTime(self._list_of_finished_execs,self._nb_of_classes)
    
    def __getitem__(self, *args):
        return self._classes.__getitem__( *args )
    def reprOfExecs(self):
        import numpy as np
        cs = np.cumsum( [elt.size for elt in self._classes] )
        ret = []
        for i,(elt,cum_nb) in enumerate( zip(self._classes,cs) ):
            ratio = (cum_nb / cs[-1])*100.0
            ret.append( f"- For class {i} - {elt.reprGlobal} ( {ratio:.1f}% )" )
        return "\n".join( ret )
    @classmethod
    def ComputeDistributionOfExecTime( cls, listOfFinishedExecs, nbOfClasses ):
        maxt = GetMaxTimeExec( listOfFinishedExecs )
        mint = GetMinTimeExec( listOfFinishedExecs )
        deltaTime = ( maxt.execTime - mint.execTime ) / nbOfClasses
        res = []
        for i in range( nbOfClasses ):
            begin = mint.execTime + i * deltaTime
            end = mint.execTime + (i+1) * deltaTime
            res.append( DistriutionClass(begin,end,[elt for elt in listOfFinishedExecs if elt.execTime >=begin and elt.execTime <= end]) )
        return res
