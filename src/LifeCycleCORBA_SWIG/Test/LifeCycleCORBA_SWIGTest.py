#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

#  File   : LifeCycleCORBA_SWIGTest.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$
#
import sys
import unittest
from omniORB import CORBA
import Utils_Identity
import Engines

class LifeCycleCORBA_SWIGTest(unittest.TestCase):
    def setUp(self):
        import LifeCycleCORBA
        self.lcc = LifeCycleCORBA.LifeCycleCORBA()
        pass

    def tearDown(self):
        pass

    def test001_FindOrLoad_Component_LaunchContainer(self):
        """
        get a local container (no hostname given),
        load an engine, check that the CORBA object is not null.
        check narrow
        """
        containerName = "swMyContainer"
        comp=self.lcc.FindOrLoad_Component(containerName,"SalomeTestComponent")
        self.assertNotEqual(comp,None)
        testComp=comp._narrow(Engines.TestComponent)
        self.assertNotEqual(testComp,None)
        pass

    def test002_FindOrLoad_Component_SameInstance(self):
        """
        Check FindOrLoad_Component.
        Call 2 times FindOrLoad_Component with the same parameters,
        check if we get the same engine
        """
        containerName = "swMyContainer"
        cp1=self.lcc.FindOrLoad_Component(containerName,"SalomeTestComponent")
        self.assertNotEqual(cp1,None)
        cp2=self.lcc.FindOrLoad_Component(containerName,"SalomeTestComponent")
        self.assertNotEqual(cp2,None)
        m1=cp1._narrow(Engines.TestComponent)
        self.assertNotEqual(m1,None)
        m2=cp2._narrow(Engines.TestComponent)
        self.assertNotEqual(m2,None)
        name1=m1._get_instanceName()
        name2=m2._get_instanceName()
        self.assertEqual(name1,name2)
        pass

    def test003_FindOrLoad_Component_PythonInCppContainer(self):
        """
        Check FindOrLoad_Component with Python Component on C++ Container,
        load an engine, check that the CORBA object is not null.
        check narrow
        """
        containerName = "swMyContainer"
        cp1=self.lcc.FindOrLoad_Component(containerName,"SALOME_TestComponentPy")
        self.assertNotEqual(cp1,None)
        m1=cp1._narrow(Engines.TestComponent)
        self.assertNotEqual(m1,None)
        pass

    def test004_FindOrLoad_Component_PythonSameInstance(self):
        """
        Check FindOrLoad_Component with Python Component on C++ Container,
        Call 2 times FindOrLoad_Component with the same parameters,
        check if we get the same engine,
        """
        containerName = "swMyContainer"
        cp1=self.lcc.FindOrLoad_Component(containerName,"SALOME_TestComponentPy")
        self.assertNotEqual(cp1,None)
        cp2=self.lcc.FindOrLoad_Component(containerName,"SALOME_TestComponentPy")
        self.assertNotEqual(cp2,None)
        m1=cp1._narrow(Engines.TestComponent)
        self.assertNotEqual(m1,None)
        m2=cp2._narrow(Engines.TestComponent)
        self.assertNotEqual(m2,None)
        name1=m1._get_instanceName()
        name2=m2._get_instanceName()
        self.assertEqual(name1,name2)
        pass

    def test005_FindOrLoad_Component_UnknownInCatalog(self):
        """
        Check FindOrLoad_Component with a component name not in catalog.
        See list of catalog given to module catalog server.
        Here, we work with KERNEL_SRC/resources/KERNELCatalog.xml that contains
        only KERNEL, SalomeTestComponent and SALOME_TestComponentPy
        """
        containerName = "swMyContainer"
        cp1=self.lcc.FindOrLoad_Component(containerName,"MyNewComponent")
        self.assertEqual(cp1,None)
        pass

    def test006_FindOrLoad_Component_LaunchContainerHostname(self):
        """
        Check FindOrLoad_Component with hostname given.
        get a local container : getHostName()/componentName,
        load an engine, check that the CORBA object is not null.
        check narrow
        """
        containerName = Utils_Identity.getShortHostName()
        containerName += "/swTheContainer"
        cp1=self.lcc.FindOrLoad_Component(containerName,"SalomeTestComponent")
        self.assertNotEqual(cp1,None)
        m1=cp1._narrow(Engines.TestComponent)
        self.assertNotEqual(m1,None)
        pass

    def test007_FindOrLoad_Component_SameContainer(self):
        """
        Check FindOrLoad_Component with and without local hostname given.
        We must get the same container, the same instance of component
        """
        containerName = "swAContainer"
        cp1=self.lcc.FindOrLoad_Component(containerName,"SalomeTestComponent")
        self.assertNotEqual(cp1,None)
        containerName = Utils_Identity.getShortHostName()
        containerName += "/swAContainer"
        cp2=self.lcc.FindOrLoad_Component(containerName,"SalomeTestComponent")
        self.assertNotEqual(cp2,None)
        m1=cp1._narrow(Engines.TestComponent)
        self.assertNotEqual(m1,None)
        m2=cp2._narrow(Engines.TestComponent)
        self.assertNotEqual(m2,None)
        name1=m1._get_instanceName()
        name2=m2._get_instanceName()
        self.assertEqual(name1,name2)
        c1=m1.GetContainerRef()
        self.assertNotEqual(c1,None)
        c2=m2.GetContainerRef()
        self.assertNotEqual(c2,None)
        cname1=c1._get_name()
        cname2=c2._get_name()
        self.assertEqual(cname1,cname2)
        hostname1=c1.getHostName()
        hostname2=c2.getHostName()
        self.assertEqual(hostname1,hostname2)
        pidc1=c1.getPID()
        pidc2=c2.getPID()
        self.assertEqual(pidc1,pidc2)
        pass

    def test008_FindOrLoad_Component_UnknownMachine(self):
        """
        Check FindOrLoad_Component: check behaviour when ask for an unknown
        computer. We must catch a Salome Exception with "unknown host" message
        """
        containerName = "aFarAwayContainer"
        containerName += "/swTheContainer"
        try:
            cp1=self.lcc.FindOrLoad_Component(containerName,"SalomeTestComponent")
        except RuntimeError,ex :
            self.assertEqual(ex.args[0],'unknown host')
        pass


def suite():
    return unittest.makeSuite(LifeCycleCORBA_SWIGTest,'test')

def main():
    return unittest.TextTestRunner().run(suite())

if __name__ == '__main__':
    #unittest.TextTestRunner(verbosity=2).run(suite())
    unittest.main()
