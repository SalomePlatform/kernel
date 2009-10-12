#  -*- coding: iso-8859-1 -*-

import unittest
import salome
import LifeCycleCORBA
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
    params=LifeCycleCORBA.MachineParameters(hostname="m3")
    machineList=rm.GetFittingResources(params)
    self.assertEqual(machineList, ["m3"])

  def test1(self):
    """OS required"""
    params=LifeCycleCORBA.MachineParameters(OS="Linux")
    machineList=rm.GetFittingResources(params)
    self.assertEqual(machineList, [])

  def test2(self):
    """component add required"""
    params=LifeCycleCORBA.MachineParameters(componentList=["add"])
    machineList=rm.GetFittingResources(params)
    self.assertEqual(machineList, ['claui2c6', 'm1', 'm2'])

  def test3(self):
    """component tutu required"""
    machineList=rm.GetFittingResources(LifeCycleCORBA.MachineParameters(componentList=["tutu"]))
    self.assertEqual(machineList, ['m1', 'm2', 'm3'])

  def test4(self):
    """component tata required"""
    machineList=rm.GetFittingResources(LifeCycleCORBA.MachineParameters(componentList=["tata"]))
    self.assertEqual(machineList, ['m1', 'm2'])

  def test5(self):
    """component titi required"""
    machineList=rm.GetFittingResources(LifeCycleCORBA.MachineParameters(componentList=["titi"]))
    self.assertEqual(machineList, ['m1', 'm2'])

  def test6(self):
    """component toto required"""
    machineList=rm.GetFittingResources(LifeCycleCORBA.MachineParameters(componentList=["toto"]))
    self.assertEqual(machineList, ['claui2c6', 'm1', 'm2'])

  def test7(self):
    """components add and toto required"""
    machineList=rm.GetFittingResources(LifeCycleCORBA.MachineParameters(componentList=["add","toto"]))
    self.assertEqual(machineList, ['claui2c6', 'm1', 'm2'])

  def test8(self):
    """components add and toto required"""
    machineDef=rm.GetMachineParameters('claui2c6')
    self.assertEqual(machineDef.componentList, ['toto', 'add'])

  def test10(self):
    """policy altcycl"""
    machineList=rm.GetFittingResources(LifeCycleCORBA.MachineParameters(componentList=["add","toto"]))
    self.assertEqual(rm.Find('altcycl',machineList), "claui2c6")
    self.assertEqual(rm.Find('altcycl',machineList), "m1")
    self.assertEqual(rm.Find('altcycl',machineList), "m2")
    self.assertEqual(rm.Find('altcycl',machineList), "claui2c6")
    self.assertEqual(rm.Find('altcycl',machineList), "m1")
    self.assertEqual(rm.Find('altcycl',machineList), "m2")

  def test11(self):
    """policy cycl"""
    machineList=rm.GetFittingResources(LifeCycleCORBA.MachineParameters(componentList=["add","toto"]))
    self.assertEqual(rm.Find('cycl',machineList), "claui2c6")
    self.assertEqual(rm.Find('cycl',machineList), "m1")
    self.assertEqual(rm.Find('cycl',machineList), "m2")
    self.assertEqual(rm.Find('cycl',machineList), "claui2c6")
    self.assertEqual(rm.Find('cycl',machineList), "m1")
    self.assertEqual(rm.Find('cycl',machineList), "m2")

  def test12(self):
    """policy first"""
    machineList=rm.GetFittingResources(LifeCycleCORBA.MachineParameters(componentList=["add","toto"]))
    self.assertEqual(rm.Find('first',machineList), "claui2c6")
    self.assertEqual(rm.Find('first',machineList), "claui2c6")

  def test13(self):
    """policy best"""
    machineList=rm.GetFittingResources(LifeCycleCORBA.MachineParameters(componentList=["add","toto"]))
    self.assertEqual(rm.Find('best',machineList), "claui2c6")
    self.assertEqual(rm.Find('best',machineList), "m1")
    self.assertEqual(rm.Find('best',machineList), "m2")
    self.assertEqual(rm.Find('best',machineList), "claui2c6")
    self.assertEqual(rm.Find('best',machineList), "m1")
    self.assertEqual(rm.Find('best',machineList), "m2")

if __name__ == '__main__':
  unittest.main()

