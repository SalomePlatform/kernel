import unittest
import salome
import Engines
import LifeCycleCORBA
salome.salome_init()
cm= salome.lcc.getContainerManager()
rm= salome.lcc.getResourcesManager()

class TestContainerManager(unittest.TestCase):
  """
Test with catalog :
<!DOCTYPE ResourcesCatalog>
<resources>
   <machine hostname="claui2c6" >
   </machine>
   <machine hostname="clt10br" >
   </machine>
</resources>
"""
  def setUp(self):
    pass

  def test0(self):
    """"""
    p=LifeCycleCORBA.MachineParameters(container_name="MyContainer",mode="start",
                                       policy="best",componentList=["PYHELLO"])
    co=cm.StartContainer( p )
    print "Container:",co,co.getHostName(), co.getPID(),co._get_name()
    self.assertEqual(co._get_name(), "/Containers/claui2c6/MyContainer")
    co=cm.StartContainer( p )
    self.assertEqual(co._get_name(), "/Containers/clt10br/MyContainer")

  def test1(self):
    """"""
    p=LifeCycleCORBA.MachineParameters(container_name="MyContainer",mode="get",
                                       policy="best",componentList=["PYHELLO"])
    co=cm.StartContainer( p )
    print "Container:",co,co.getHostName(), co.getPID(),co._get_name()
    self.assertEqual(co._get_name(), "/Containers/claui2c6/MyContainer")
    co=cm.StartContainer( p )
    self.assertEqual(co._get_name(), "/Containers/clt10br/MyContainer")

  def test2(self):
    """"""
    p=LifeCycleCORBA.MachineParameters(container_name="MyContainer",mode="getorstart",
                                       policy="best",componentList=["PYHELLO"])
    co=cm.StartContainer( p )
    print "Container:",co,co.getHostName(), co.getPID(),co._get_name()
    self.assertEqual(co._get_name(), "/Containers/claui2c6/MyContainer")
    co=cm.StartContainer( p )
    self.assertEqual(co._get_name(), "/Containers/clt10br/MyContainer")


if __name__ == '__main__':
  unittest.main()

