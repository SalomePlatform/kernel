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
import multiprocessing as mp

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
    self.assertEqual(str2Obj(wk.waitFor()),[7,8,9,10])
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
    self.assertEqual(str2Obj(wk.waitFor()),[7,8,9,10])

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
    self.assertEqual(str2Obj(wk.waitFor()),[7,8,9,10])
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
    self.assertEqual(str2Obj(wk.waitFor()),[7,8,9,10])
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{'ab':[4,5,6]})# it is not a bug ! commit of t1 not done !
    dss.atomicApply([t1])
    self.assertEqual(dss.getAccessOfVar(varName),"RdExt")
    #
    self.assertEqual(str2Obj(dss.fetchSerializedContent(varName)),{'ab':[4,5,6],'cd':[7,8,9,10]})
    wk=dss.waitForKeyInVar(varName,obj2Str("cd"))
    self.assertEqual(str2Obj(wk.waitFor()),[7,8,9,10])
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

  def setUp(self):
    salome.salome_init()
    pass
  
  pass

unittest.main()

