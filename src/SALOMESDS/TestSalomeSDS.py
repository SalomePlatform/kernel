# -*- coding: utf-8 -*-
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
import cPickle
import gc
import time
from datetime import datetime
import multiprocessing as mp

nbOfSecWait=1.

def obj2Str(obj):
  return cPickle.dumps(obj,cPickle.HIGHEST_PROTOCOL)
def str2Obj(strr):
  return cPickle.loads(strr)
def generateKey(varName,scopeName):
  dsm=salome.naming_service.Resolve("/DataServerManager")
  dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
  assert(not isCreated)
  t=dss.addKeyValueInVarHard(varName,obj2Str("ef"),obj2Str([11,14,100]))
  time.sleep(3)
  dss.atomicApply([t])
def work(t):
  i,varName,scopeName=t
  if i==0:
    generateKey(varName,scopeName)
    return 0
  else:
    import TestSalomeSDSHelper0
    import os,subprocess
    fname=os.path.splitext(TestSalomeSDSHelper0.__file__)[0]+".py"
    proc=subprocess.Popen(["python",fname],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    out,err=proc.communicate()
    if proc.returncode!=0:
      print out
      print err
    return proc.returncode
  
def func_test7(scopeName,l,l2,cv):
    salome.salome_init()
    varName="a"
    zeValue={"ab":[4,5,6]}
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName) # should be suspended nbOfSecWait s by main process
    assert(not isCreated)
    l.release() # tell manager that I'm ready
    l2.acquire() # wait for manager to start micro-test1
    ######### micro-test1 - check that all requests are suspended
    s=datetime.now()
    t0=dss.createRdWrVarTransac(varName,obj2Str(zeValue))
    s=(datetime.now()-s).total_seconds()
    assert(s>=0.99*nbOfSecWait and s<nbOfSecWait*1.01) # expect to wait nearly nbOfSecWait seconds
    dss.atomicApply([t0])
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
    a=SalomeSDSClt.CreateRdExtGlobalVar([],"a","Scope0")
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
    pass
  
  def testDict1(self):
    a=SalomeSDSClt.CreateRdExtGlobalVar({},"a","Scope0")
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
    pass

  def testReadOnly1(self):
    a=SalomeSDSClt.CreateRdOnlyGlobalVar({"ab":4,"cd":[5,77]},"a","Scope0")
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5,77]})
    self.assertRaises(Exception,a.__getitem__,"ab")
    a.ptr().getMyDataScopeServer().deleteVar("a")

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
    asyncResult=pool.map_async(work,[(i,varName,scopeName) for i in xrange(nbProc)])
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
    self.assertEqual(keys,['ab','cd'])

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
    pass

  def testLockToDump(self):
    """ Test to check that holdRequests method. This method wait for clean server status and hold it until activeRequests is called.
    Warning this method expects a not overloaded machine to be run because test is based on ellapse time.
    """
    scopeName="Scope1"
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dsm.cleanScopesInNS()
    if scopeName in dsm.listScopes():
        dsm.removeDataScope(scopeName)
    # l is for main process sync. to be sure to launch test when sub process is ready
    # l2 lock is for sub process sync.
    l=mp.Lock(); l2=mp.Lock()
    l.acquire() ; l2.acquire()
    cv=mp.Condition(mp.Lock())
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    self.assertTrue(isCreated)
    p=mp.Process(target=func_test7,args=(scopeName,l,l2,cv))
    p.start()
    l.acquire()
    rs=dss.getRequestSwitcher() ; rs.holdRequests() # The aim of the test
    l2.release() # tell slave process that it's ready for micro-test1
    time.sleep(nbOfSecWait)
    rs.activeRequests() # The aim of the test
    ######### micro-test3 - check that holdRequests is able to wait for a non finished job
    with cv:
      cv.wait()
      s=datetime.now()
      time.sleep(0.01) # let main proc the priority
      rs.holdRequests() # the aim of the test is here. main process is occupied 1s -> holdRequests is Expected to wait
      s=(datetime.now()-s).total_seconds()
      rs.activeRequests()
      self.assertTrue(s>=0.99*nbOfSecWait and s<nbOfSecWait*1.01) # expect to be not locked
    # finishing
    p.join()
    pass

  def setUp(self):
    salome.salome_init()
    pass
  
  pass

if __name__=="__main__":
  unittest.main()

