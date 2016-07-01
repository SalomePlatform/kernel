#  -*- coding: iso-8859-1 -*-
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

import unittest
import salome
import LifeCycleCORBA
import SALOME
salome.salome_init()
cm= salome.lcc.getContainerManager()
rm= salome.lcc.getResourcesManager()

class TestResourceManager(unittest.TestCase):
  """
Test with CatalogResources.xml:
<!DOCTYPE ResourcesCatalog>
<resources>
   <machine hostname="m1" />
   <machine hostname="m2" />
   <machine hostname="m3" >
     <modules moduleName="tutu" />
   </machine>
   <machine hostname="m4" >
     <component name="sub" moduleName="tata" />
   </machine>
   <machine hostname="claui2c6" >
     <modules moduleName="toto" />
     <component name="add" moduleName="titi" />
   </machine>
</resources>
"""
  def setUp(self):
    pass

  def test0(self):
    """host required"""
    params=LifeCycleCORBA.ResourceParameters(hostname="m3")
    machineList=rm.GetFittingResources(params)
    self.assertEqual(machineList, ["m3"])

  def test1(self):
    """OS required"""
    params=LifeCycleCORBA.ResourceParameters(OS="Linux")
    self.assertRaises(SALOME.SALOME_Exception,rm.GetFittingResources,params)

  def test2(self):
    """component add required.
    If a resource  doesn't have any module or component defined in the catalog,
    it means that it has every module and component and it fits for any request.
    """
    params=LifeCycleCORBA.ResourceParameters(componentList=["add"])
    machineList=rm.GetFittingResources(params)
    self.assertEqual(machineList, ['claui2c6', 'localhost', 'm1', 'm2'])

  def test3(self):
    """component tutu required"""
    params=LifeCycleCORBA.ResourceParameters(componentList=["tutu"])
    machineList=rm.GetFittingResources(params)
    self.assertEqual(machineList, ['localhost', 'm1', 'm2', 'm3'])

  def test4(self):
    """component tata required"""
    params=LifeCycleCORBA.ResourceParameters(componentList=["tata"])
    machineList=rm.GetFittingResources(params)
    self.assertEqual(machineList, ['localhost', 'm1', 'm2'])

  def test5(self):
    """component titi required"""
    params=LifeCycleCORBA.ResourceParameters(componentList=["titi"])
    machineList=rm.GetFittingResources(params)
    self.assertEqual(machineList, ['localhost', 'm1', 'm2'])

  def test6(self):
    """component toto required"""
    params=LifeCycleCORBA.ResourceParameters(componentList=["toto"])
    machineList=rm.GetFittingResources(params)
    self.assertEqual(machineList, ['claui2c6', 'localhost', 'm1', 'm2'])

  def test7(self):
    """components add and toto required"""
    params=LifeCycleCORBA.ResourceParameters(componentList=["add","toto"])
    machineList=rm.GetFittingResources(params)
    self.assertEqual(machineList, ['claui2c6', 'localhost', 'm1', 'm2'])

  def test8(self):
    """components add and toto required"""
    machineDef=rm.GetResourceDefinition('claui2c6')
    self.assertEqual(machineDef.componentList, ['toto', 'add'])

  def test10(self):
    """policy altcycl"""
    params=LifeCycleCORBA.ResourceParameters(componentList=["add","toto"])
    machineList=rm.GetFittingResources(params)
    self.assertEqual(rm.Find('altcycl',machineList), "claui2c6")
    self.assertEqual(rm.Find('altcycl',machineList), "localhost")
    self.assertEqual(rm.Find('altcycl',machineList), "m1")
    self.assertEqual(rm.Find('altcycl',machineList), "m2")
    self.assertEqual(rm.Find('altcycl',machineList), "claui2c6")
    self.assertEqual(rm.Find('altcycl',machineList), "localhost")
    self.assertEqual(rm.Find('altcycl',machineList), "m1")
    self.assertEqual(rm.Find('altcycl',machineList), "m2")

  def test11(self):
    """policy cycl"""
    params=LifeCycleCORBA.ResourceParameters(componentList=["add","toto"])
    machineList=rm.GetFittingResources(params)
    self.assertEqual(rm.Find('cycl',machineList), "claui2c6")
    self.assertEqual(rm.Find('cycl',machineList), "localhost")
    self.assertEqual(rm.Find('cycl',machineList), "m1")
    self.assertEqual(rm.Find('cycl',machineList), "m2")
    self.assertEqual(rm.Find('cycl',machineList), "claui2c6")
    self.assertEqual(rm.Find('cycl',machineList), "localhost")
    self.assertEqual(rm.Find('cycl',machineList), "m1")
    self.assertEqual(rm.Find('cycl',machineList), "m2")

  def test12(self):
    """policy first"""
    params=LifeCycleCORBA.ResourceParameters(componentList=["add","toto"])
    machineList=rm.GetFittingResources(params)
    self.assertEqual(rm.Find('first',machineList), "claui2c6")
    self.assertEqual(rm.Find('first',machineList), "claui2c6")

  def test13(self):
    """policy best"""
    params=LifeCycleCORBA.ResourceParameters(componentList=["add","toto"])
    machineList=rm.GetFittingResources(params)
    self.assertEqual(rm.Find('best',machineList), "claui2c6")
    self.assertEqual(rm.Find('best',machineList), "localhost")
    self.assertEqual(rm.Find('best',machineList), "m1")
    self.assertEqual(rm.Find('best',machineList), "m2")
    self.assertEqual(rm.Find('best',machineList), "claui2c6")
    self.assertEqual(rm.Find('best',machineList), "localhost")
    self.assertEqual(rm.Find('best',machineList), "m1")
    self.assertEqual(rm.Find('best',machineList), "m2")

if __name__ == '__main__':
  #suite = unittest.TestLoader().loadTestsFromTestCase(TestResourceManager)
  #unittest.TextTestRunner().run(suite)
  unittest.main()
