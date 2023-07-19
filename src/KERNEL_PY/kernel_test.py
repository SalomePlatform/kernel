#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2018-2023  CEA, EDF, OPEN CASCADE
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

import unittest

class TestKernel(unittest.TestCase):

    def setUp(self):
        import salome
        salome.salome_init()

    def processGuiEvents(self):
        import salome
        if salome.sg.hasDesktop():
            salome.sg.updateObjBrowser();
            import SalomePyQt
            SalomePyQt.SalomePyQt().processEvents()

    def test_modulecatalog(self):
        """Quick test for Kernel module: check module catalogue"""
        print()
        print('Testing Kernel module: check module catalogue')

        import salome

        # ---- get module catalogue
        print('... Get module catalogue')
        import SALOME_ModuleCatalog
        obj = salome.naming_service.Resolve('/Kernel/ModulCatalog')
        self.assertIsNotNone(obj)
        catalog = obj._narrow(SALOME_ModuleCatalog.ModuleCatalog)
        self.assertIsNotNone(catalog)

        # ---- ping catalog
        catalog.ping()

        # ---- check some catalog's features
        components = catalog.GetComponentList()
        self.assertTrue('KERNEL' in components)
        info = catalog.GetComponentInfo('KERNEL')
        self.assertEqual(info.name, 'KERNEL')
        self.assertEqual(info.type, SALOME_ModuleCatalog.OTHER)
            
    def test_dataserver(self):
        """Quick test for Kernel module: check data server"""
        print()
        print('Testing Kernel module: check data server')
        
        import salome

        print('... Initialize study builder')
        builder = salome.myStudy.NewBuilder()
        self.assertIsNotNone(builder)

        print('... Create new component')
        scomponent = builder.NewComponent('TEST')
        self.assertIsNotNone(scomponent)
        self.assertEqual(scomponent.ComponentDataType(), 'TEST')
        self.assertEqual(scomponent.Depth(), 2)

        print('...... Assign AttributeName')
        scomponent.SetAttrString('AttributeName', 'Test')
        self.assertEqual(scomponent.GetName(), 'Test')
        self.assertEqual(salome.myStudy.GetObjectPath(scomponent), '/Test')
        self.processGuiEvents()

        print('... Create new object')
        sobject = builder.NewObject(scomponent)
        self.assertIsNotNone(sobject)
        self.assertEqual(sobject.Tag(), 1)
        self.assertEqual(sobject.Depth(), 3)

        print('...... Get father')
        father = sobject.GetFatherComponent()
        self.assertIsNotNone(father)
        self.assertEqual(father.GetName(), scomponent.GetName())
        father = sobject.GetFather()
        self.assertIsNotNone(father)
        self.assertEqual(father.GetName(), scomponent.GetName())

        print('...... Assign AttributeName')
        sobject.SetAttrString('AttributeName', 'Object')
        self.assertEqual(sobject.GetName(), 'Object')
        self.assertEqual(salome.myStudy.GetObjectPath(sobject), '/Test/Object')

        print('...... Assign AttributeInteger')
        attr = builder.FindOrCreateAttribute(sobject, 'AttributeInteger')
        self.assertIsNotNone(attr)
        attr.SetValue(123)
        self.assertEqual(attr.Value(), 123)
        self.processGuiEvents()

if __name__ == '__main__':
    unittest.main()
