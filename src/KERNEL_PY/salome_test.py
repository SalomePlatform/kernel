#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2024  CEA, EDF, OPEN CASCADE
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

import importlib
import sys
import unittest

def main(modules=None):
    suite = unittest.TestSuite()
    loader = unittest.defaultTestLoader

    import salome
    salome.salome_init()

    if not modules:
        from salome.kernel.services import getComponentList
        modules = getComponentList()
        if salome.hasDesktop():
            import SalomePyQt
            sg = SalomePyQt.SalomePyQt()
            gui_modules = sg.getComponents()
            modules += [i for i in gui_modules if i not in modules]
    elif not isinstance(modules, (list, tuple)):
        modules = [modules]
    for module in modules:
        test_name = '{}_test'.format(module.lower())
        try:
            importlib.import_module(test_name)
            test_module = sys.modules[test_name]
            suite.addTest(loader.loadTestsFromModule(test_module))
        except ImportError:
            pass
    runner = unittest.TextTestRunner()
    return runner.run(suite)

main()
