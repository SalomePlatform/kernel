# -*- coding: utf-8 -*-
# Copyright (C) 2007-2014  CEA/DEN, EDF R&D
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
import gc

class SalomeSDS2Test(unittest.TestCase):
  
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
    a._var_ptr.getMyDataScopeServer().deleteVar("a")
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
    a._var_ptr.getMyDataScopeServer().deleteVar("a")
    pass

  def testReadOnly1(self):
    a=SalomeSDSClt.CreateRdOnlyGlobalVar({"ab":4,"cd":[5,77]},"a","Scope0")
    self.assertEqual(a.local_copy(),{"ab":4,"cd":[5,77]})
    self.assertRaises(AttributeError,a.__getitem__,"ab")
    a._var_ptr.getMyDataScopeServer().deleteVar("a")

  def setUp(self):
    salome.salome_init()
    pass
  
  pass

unittest.main()

