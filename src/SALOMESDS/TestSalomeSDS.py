# -*- coding: utf-8 -*-
# Copyright (C) 2007-2021  CEA/DEN, EDF R&D, OPEN CASCADE
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
# Author : Anthony Geay

import SalomeSDSClt
import SALOME
import salome
import unittest
import pickle
import gc
import time
from datetime import datetime
import multiprocessing as mp

nbOfSecWait=1.

def obj2Str(obj):
  return pickle.dumps(obj,pickle.HIGHEST_PROTOCOL)
def str2Obj(strr):
  return pickle.loads(strr)
def generateKey(varName,scopeName):
  dsm=salome.naming_service.Resolve("/DataServerManager")
  dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
  assert(not isCreated)
  t=dss.addKeyValueInVarHard(varName,obj2Str("ef"),obj2Str([11,14,100]))
  time.sleep(3)
  dss.atomicApply([t])
def work(t):
  IORNS,i,varName,scopeName=t
  if i==0:
    generateKey(varName,scopeName)
    return 0
  else:
    import TestSalomeSDSHelper0
    import os,subprocess
    fname=os.path.splitext(TestSalomeSDSHelper0.__file__)[0]+".py"
    proc = subprocess.Popen(["python3", fname, IORNS], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out,err=proc.communicate()
    if proc.returncode!=0:
      print("-------------- work -----------")
      print(out)
      print(err)
      print("~~~~~~~~~~~~~~ work ~~~~~~~~~~~")
    return proc.returncode
  
def func_test7(scopeName,cv,cv2,cv3,sharedNum):
    salome.salome_init()
    varName="a"
    zeValue={"ab":[4,5,6]}
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName) # should be suspended nbOfSecWait s by main process
    assert(not isCreated)
    ######### micro-test1 - check that all requests are suspended
    ######## Barrier
    with cv2:
      cv2.notify_all()
      sharedNum.value=True
    with cv3:
      cv3.wait()
    ####### End Barrier
    s=datetime.now()
    t0=dss.createRdWrVarTransac(varName,obj2Str(zeValue))
    dss.atomicApply([t0])
    s=(datetime.now()-s).total_seconds()
    assert(s>=0.99*nbOfSecWait and s<nbOfSecWait*1.01) # expect to wait nearly nbOfSecWait seconds
    ######### end of micro-test1
    ######### micro-test2 - after activeRequests everything work well
    s=datetime.now()
    st=dss.fetchSerializedContent(varName)
    assert(str2Obj(st)==zeValue)
    s=(datetime.now()-s).total_seconds()
    assert(s>=0. and s<0.05) # expect to be not locked
    ######### end of micro-test2
    with cv:
      cv.notify_all()
    dss.takeANap(nbOfSecWait) # emulate a DataServer occupation
    pass
  
class SalomeSDSTest(unittest.TestCase):
  
  def testList1(self):
    scopeName = "Scope0"
    a=SalomeSDSClt.CreateRdExtGlobalVar([],"a",scopeName)
    self.assertEqual(a.local_copy(),[])
    a.append(5)
    self.assertEqual(a.local_copy(),[5])
    self.assertRaises(SALOME.SALOME_Exception,a.__delitem__,0)
    a.append(["rt"])
    self.assertEqual(a.local_copy(),[5,["rt"]])
    a[1].append(8)
    self.assertEqual(a.local_copy(),[5,["rt",8]])
    a.extend(a)
    self.assertEqual(a.local_copy(),[5,["rt",8],5,["rt",8]])
    a.extend(a[3:])
    self.assertEqual(a.local_copy(),[5,["rt",8],5,["rt",8],["rt",8]])
    a[4].append(7)
    self.assertEqual(a.local_copy(),[5,["rt",8],5,["rt",8],["rt",8,7]])
    a.ptr().getMyDataScopeServer().deleteVar("a")
    del a
    import gc
    gc.collect(0)
    salome.dsm.removeDataScope(scopeName)
    pass
  
  def testDict1(self):
    scopeName = "Scope0"
    a=SalomeSDSClt.CreateRdExtGlobalVar({},"a",scopeName)
    a["ab"]=4
    self.assertEqual(a.local_copy(),{"ab":4})
    a["cd"]=[5]
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5]})
    a["cd"].append(77)
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5,77]})
    a.__setitem__("ef",["a","bb"])
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5,77],"ef":["a","bb"]})
    self.assertRaises(SALOME.SALOME_Exception,a.__setitem__,"ef",["a","bb"])
    self.assertRaises(SALOME.SALOME_Exception,a.__setitem__,"ef",["a","bb","ccc"])
    a["ef"].append("ccc")
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5,77],"ef":["a","bb","ccc"]})
    a["gh"]=a
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5,77],"ef":["a","bb","ccc"],"gh":{"ab":4,"cd":[5,77],"ef":["a","bb","ccc"]}})
    a["gh"]["cd"].append(99) ; a["cd"].append(88)
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5,77,88],"ef":["a","bb","ccc"],"gh":{"ab":4,"cd":[5,77,99],"ef":["a","bb","ccc"]}})
    a.ptr().getMyDataScopeServer().deleteVar("a")
    del a
    import gc
    gc.collect(0)
    salome.dsm.removeDataScope(scopeName)
    pass

  def testReadOnly1(self):
    scopeName = "Scope0"
    #
    a=SalomeSDSClt.CreateRdOnlyGlobalVar({"ab":4,"cd":[5,77]},"a",scopeName)
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5,77]})
    self.assertRaises(Exception,a.__getitem__,"ab")
    a.ptr().getMyDataScopeServer().deleteVar("a")
    del a
    import gc
    gc.collect(0)
    #
    salome.dsm.removeDataScope(scopeName)

  def testTransaction1(self):
    scopeName="Scope1"
    varName="a"
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dsm.cleanScopesInNS()
    if scopeName in dsm.listScopes():
      dsm.removeDataScope(scopeName)
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    self.assertTrue(isCreated)
    #
    t0=dss.createRdWrVarTransac(varName,obj2Str({"ab":[4,5,6]}))
    dss.atomicApply([t0])
    #
    self.assertEqual(dss.getAccessOfVar(varName),"RdWr")
    #
    t1=dss.addKeyValueInVarHard(varName,obj2Str("cd"),obj2Str([7,8,9,10]))
    dss.atomicApply([t1])
    #
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{'ab':[4,5,6],'cd':[7,8,9,10]})
    wk=dss.waitForKeyInVar(varName,obj2Str("cd"))
    wk.waitFor()
    self.assertEqual(str2Obj(dss.waitForMonoThrRev(wk)),[7,8,9,10])
    #
    nbProc=8
    pool=mp.Pool(processes=nbProc)
    from NamingService import NamingService
    asyncResult=pool.map_async(work,[(NamingService.IOROfNS(),i,varName,scopeName) for i in range(nbProc)])
    print("asyncResult=", asyncResult)
    self.assertEqual(asyncResult.get(),nbProc*[0]) # <- the big test is here !
    dsm.removeDataScope(scopeName)

  def testTransaction2(self):
    scopeName="Scope1"
    varName="a"
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dsm.cleanScopesInNS()
    if scopeName in dsm.listScopes():
      dsm.removeDataScope(scopeName)
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    self.assertTrue(isCreated)
    #
    t0=dss.createRdExtVarTransac(varName,obj2Str({"ab":[4,5,6]}))
    dss.atomicApply([t0])
    #
    self.assertEqual(dss.getAccessOfVar(varName),"RdExt")
    #
    self.assertRaises(SALOME.SALOME_Exception,dss.addKeyValueInVarErrorIfAlreadyExisting,varName,obj2Str("ab"),obj2Str([7,8,9,10]))#raises because ab is already a key !
    t1=dss.addKeyValueInVarErrorIfAlreadyExisting(varName,obj2Str("cd"),obj2Str([7,8,9,10]))
    dss.atomicApply([t1])
    #
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{'ab':[4,5,6],'cd':[7,8,9,10]})
    wk=dss.waitForKeyInVar(varName,obj2Str("cd"))
    wk.waitFor()
    self.assertEqual(str2Obj(dss.waitForMonoThrRev(wk)),[7,8,9,10])
    dsm.removeDataScope(scopeName)

  def testTransaction3(self):
    scopeName="Scope1"
    varName="a"
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dsm.cleanScopesInNS()
    if scopeName in dsm.listScopes():
      dsm.removeDataScope(scopeName)
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    self.assertTrue(isCreated)
    #
    t0=dss.createRdWrVarTransac(varName,obj2Str({"ab":[4,5,6]}))
    dss.atomicApply([t0])
    #
    t1=dss.addKeyValueInVarErrorIfAlreadyExisting(varName,obj2Str("cd"),obj2Str([7,8,9,10]))
    dss.atomicApply([t1])
    #
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{'ab':[4,5,6],'cd':[7,8,9,10]})
    #
    t2=dss.removeKeyInVarErrorIfNotAlreadyExisting(varName,obj2Str("ab"))
    dss.atomicApply([t2])
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{'cd':[7,8,9,10]})
    dsm.removeDataScope(scopeName)

  def testTransaction4(self):
    scopeName="Scope1"
    varName="a"
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dsm.cleanScopesInNS()
    if scopeName in dsm.listScopes():
      dsm.removeDataScope(scopeName)
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    self.assertTrue(isCreated)
    #
    t0=dss.createRdWrVarTransac(varName,obj2Str({"ab":[4,5,6]}))
    dss.atomicApply([t0])
    #
    t1=dss.addKeyValueInVarHard(varName,obj2Str("cd"),obj2Str([7,8,9,10]))
    dss.atomicApply([t1])
    #
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{'ab':[4,5,6],'cd':[7,8,9,10]})
    wk,t2=dss.waitForKeyInVarAndKillIt(varName,obj2Str("cd"))
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{'ab':[4,5,6],'cd':[7,8,9,10]})
    wk.waitFor()
    self.assertEqual(str2Obj(dss.waitForMonoThrRev(wk)),[7,8,9,10])
    dss.atomicApply([t2])
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{'ab':[4,5,6]})
    dsm.removeDataScope(scopeName)

  def testTransaction5(self):
    """ Like testTransaction2 but without transactions. """
    scopeName="Scope1"
    varName="a"
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dsm.cleanScopesInNS()
    if scopeName in dsm.listScopes():
      dsm.removeDataScope(scopeName)
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    self.assertTrue(isCreated)
    #
    t0=dss.createRdExtVarTransac(varName,obj2Str({"ab":[4,5,6]}))
    dss.atomicApply([t0])
    #
    self.assertEqual(dss.getAccessOfVar(varName),"RdExt")
    t1=dss.addMultiKeyValueSession(varName)
    self.assertEqual(dss.getAccessOfVar(varName),"RdExtInit")
    self.assertRaises(SALOME.SALOME_Exception,t1.addKeyValueInVarErrorIfAlreadyExistingNow,obj2Str("ab"),obj2Str([7,8,9,10]))#raises because ab is already a key !
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{'ab':[4,5,6]})
    wk=dss.waitForKeyInVar(varName,obj2Str("cd"))
    t1.addKeyValueInVarErrorIfAlreadyExistingNow(obj2Str("cd"),obj2Str([7,8,9,10]))
    wk.waitFor()
    self.assertEqual(str2Obj(dss.waitForMonoThrRev(wk)),[7,8,9,10])
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{'ab':[4,5,6]})# it is not a bug ! commit of t1 not done !
    dss.atomicApply([t1])
    self.assertEqual(dss.getAccessOfVar(varName),"RdExt")
    #
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{'ab':[4,5,6],'cd':[7,8,9,10]})
    wk=dss.waitForKeyInVar(varName,obj2Str("cd"))
    wk.waitFor()
    self.assertEqual(str2Obj(dss.waitForMonoThrRev(wk)),[7,8,9,10])
    keys=[str2Obj(elt) for elt in dss.getAllKeysOfVarWithTypeDict(varName)]
    self.assertEqual(set(keys),set(['ab','cd']))
    dsm.removeDataScope(scopeName)

  def testTransaction6(self):
    """ Test to test RdWr global vars with transaction"""
    scopeName="Scope1"
    varName="a"
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dsm.cleanScopesInNS()
    if scopeName in dsm.listScopes():
      dsm.removeDataScope(scopeName)
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    self.assertTrue(isCreated)
    #
    t0=dss.createWorkingVarTransac(varName,obj2Str({}))
    a=SalomeSDSClt.GetHandlerFromRef(t0.getVar())
    self.assertEqual(dss.getAccessOfVar(varName),"RdWr")
    # play
    a["ab"]=4
    self.assertEqual(a.local_copy(),{"ab":4})
    a.assign({"ab":5})
    self.assertEqual(a.local_copy(),{"ab":5})
    a.assign({"ab":4})
    a["cd"]=[5]
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5]})
    a["cd"].append(77)
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5,77]})
    a.__setitem__("ef",["a","bb"])
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5,77],"ef":["a","bb"]})
    a["ef"].append("ccc")
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5,77],"ef":["a","bb","ccc"]})
    a["gh"]=a
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5,77],"ef":["a","bb","ccc"],"gh":{"ab":4,"cd":[5,77],"ef":["a","bb","ccc"]}})
    a["gh"]["cd"].append(99) ; a["cd"].append(88)
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5,77,88],"ef":["a","bb","ccc"],"gh":{"ab":4,"cd":[5,77,99],"ef":["a","bb","ccc"]}})
    # WARNING here not problem to overwrite
    a["gh"]=7
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5,77,88],"ef":["a","bb","ccc"],"gh":7})
    # end of play
    self.assertTrue(isinstance(a,SalomeSDSClt.Dict))
    self.assertTrue(isinstance(a,SalomeSDSClt.WrappedType))# important for EEM
    # commit : RdWr->RdOnly
    dss.atomicApply([t0])
    #
    self.assertEqual(dss.getAccessOfVar(varName),"RdOnly") # after atomicApply the var is readOnly. Impossible to change its value !
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{"ab":4,"cd":[5,77,88],"ef":["a","bb","ccc"],"gh":7})
    dsm.cleanScopesInNS()
    del a # very important kill Ref before removingDataScope...
    if scopeName in dsm.listScopes():
      dsm.removeDataScope(scopeName)
    pass

  def testTransaction7(self):
    """Like testTransaction5 but after a recovery."""
    scopeName="Scope1"
    varName="a"
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dsm.cleanScopesInNS()
    if scopeName in dsm.listScopes():
      dsm.removeDataScope(scopeName)
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    self.assertTrue(isCreated)
    #
    t0=dss.createRdExtInitVarTransac(varName,obj2Str({"ab":[4,5,6]}))
    dss.atomicApply([t0])
    #
    self.assertEqual(dss.getAccessOfVar(varName),"RdExtInit")
    t1=dss.addMultiKeyValueSession(varName)
    self.assertEqual(dss.getAccessOfVar(varName),"RdExtInit")
    t1.addKeyValueInVarErrorIfAlreadyExistingNow(obj2Str("cd"),obj2Str([7,8,9,10]))
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{'ab':[4,5,6]})# it is not a bug ! commit of t1 not done !
    dss.atomicApply([t1])
    self.assertEqual(dss.getAccessOfVar(varName),"RdExt")
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{'ab':[4,5,6],'cd':[7,8,9,10]})
    dsm.removeDataScope(scopeName)
    pass

  def testTransaction8(self):
    """ EDF 16833 and EDF17719 """
    funcContent="""def comptchev(a,b):
    return "d" not in a
"""
    scopeName="ScopePP"
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dsm.cleanScopesInNS()
    if scopeName in dsm.listScopes():
        dsm.removeDataScope(scopeName)
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    self.assertTrue(isCreated)

    value={"a":1,"b":2}
    value2={'a':1,'c':3,'b':2}
    value3={'a':1,'c':3,'b':2,'d':4}

    varName="abc"
    t0=dss.createRdExtVarFreeStyleTransac(varName,obj2Str(value),funcContent) # sha1 is the key used to compare the initial value
    dss.atomicApply([t0])
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),value)
    t1=dss.addMultiKeyValueSession(varName)
    t1.addKeyValueInVarErrorIfAlreadyExistingNow(obj2Str("c"),obj2Str(3))
    dss.atomicApply([t1])
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),value2)
    t2=dss.createRdExtVarFreeStyleTransac(varName,obj2Str(value),funcContent) # func says OK this is the same (even if it is not the case) as original one -> OK
    dss.atomicApply([t2])
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),value2) # value2 remains untouched
    t3=dss.addMultiKeyValueSession(varName)
    t3.addKeyValueInVarErrorIfAlreadyExistingNow(obj2Str("d"),obj2Str(4))
    dss.atomicApply([t3])
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),value3)
    t4=dss.createRdExtVarFreeStyleTransac(varName,obj2Str(value),funcContent)
    self.assertRaises(SALOME.SALOME_Exception,dss.atomicApply,[t4]) # d is in dict pointed by var. Func returns false -> rejected
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),value3)
    dsm.removeDataScope(scopeName)
    pass
  
  def testTransaction9(self):
    """ EDF 16833 and EDF17719 : use case 2. Trying to createRdExt during add key session"""
    funcContent="""def comptchev(a,b):
    return a==b
"""
    scopeName="ScopePP"
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dsm.cleanScopesInNS()
    if scopeName in dsm.listScopes():
        dsm.removeDataScope(scopeName)
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    self.assertTrue(isCreated)

    value={"a":1,"b":2}
    value2={'a':1,'c':3,'b':2}

    varName="abc"
    t0=dss.createRdExtVarFreeStyleTransac(varName,obj2Str(value),funcContent)
    dss.atomicApply([t0])
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),value)
    t1=dss.addMultiKeyValueSession(varName)
    t2=dss.createRdExtVarFreeStyleTransac(varName,obj2Str(value),funcContent)
    dss.atomicApply([t2])
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),value)
    t1.addKeyValueInVarErrorIfAlreadyExistingNow(obj2Str("c"),obj2Str(3))
    dss.atomicApply([t1])
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),value2)
    dsm.removeDataScope(scopeName)
    pass

    
  def testLockToDump(self):
    """ Test to check that holdRequests method. This method wait for clean server status and hold it until activeRequests is called.
    Warning this method expects a not overloaded machine to be run because test is based on ellapse time.
    """
    scopeName="Scope1"
    varName="ab"
    zeObj={"ab":[5,6]}
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dsm.cleanScopesInNS()
    if scopeName in dsm.listScopes():
        dsm.removeDataScope(scopeName)
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    self.assertTrue(isCreated)
    cv=mp.Condition(mp.Lock())
    cv2=mp.Condition(mp.Lock()) # sharedNum & cv2 & cv3 for the barrier
    cv3=mp.Condition(mp.Lock())
    sharedNum=mp.Value('b',False)
    p=mp.Process(target=func_test7,args=(scopeName,cv,cv2,cv3,sharedNum))
    p.start()
    #
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    self.assertTrue(not isCreated)
    t0=dss.createRdWrVarTransac(varName,obj2Str(zeObj))
    dss.atomicApply([t0])
    rs=dss.getRequestSwitcher()
    self.assertTrue(not isCreated)
    ######## Barrier
    with cv2:
      if not sharedNum.value:
        cv2.wait()
      sharedNum.value=False
      pass
    with cv3:
      cv3.notify_all()
    ####### End Barrier
    rs.holdRequests() # The aim of the test
    self.assertEqual(rs.listVars(),[varName]) # call whereas holdRequest is called
    time.sleep(nbOfSecWait)
    rs.activeRequests() # The aim of the test
    ######### micro-test3 - check that holdRequests is able to wait for a non finished job
    with cv:
      cv.wait()
      s=datetime.now()
      time.sleep(0.01) # let main proc the priority
      rs.holdRequests() # the aim of the test is here. main process is occupied 1s -> holdRequests is Expected to wait
      s=(datetime.now()-s).total_seconds()
      self.assertTrue(str2Obj(rs.fetchSerializedContent(varName))==zeObj) # call whereas holdRequest is called
      rs.activeRequests()
      self.assertTrue(s>=0.99*nbOfSecWait and s<nbOfSecWait*1.01) # expect to be not locked
    # finishing
    p.join()
    dsm.removeDataScope(scopeName)
    pass
  
  def testShutdownScopes(self):
    """ Test shutdownScopes.
    """
    salome.salome_init()
    dss, isCreated = salome.dsm.giveADataScopeTransactionCalled('scope0')
    t0 = dss.createRdExtVarTransac('a', obj2Str({}))
    dss.atomicApply([t0])
    salome.dsm.shutdownScopes() # no exception expected
    pass

  pass

if __name__=="__main__":
  with salome.SessionContextManager():
    unittest.main()


