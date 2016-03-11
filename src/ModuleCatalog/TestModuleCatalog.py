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

import batchmode_salome
import SALOME_ModuleCatalog

print
print "======================================================================"
print "           XML Catalog file generation from idl file"
print "======================================================================"

import os
os.system('runIDLparser -Wbcatalog=x \
                 ${KERNEL_ROOT_DIR}/idl/salome/SALOME_TestModuleCatalog.idl')

print "======================================================================"
print "           Get Catalog "
print "======================================================================"
obj = batchmode_salome.naming_service.Resolve('Kernel/ModulCatalog')
catalog = obj._narrow(SALOME_ModuleCatalog.ModuleCatalog)
catalog.GetComponentList()

print 
print "======================================================================"
print "           Import xml file "
print "======================================================================"
catalog.ImportXmlCatalogFile("x.xml")

name = "AddComponent"
print 
print "======================================================================"
print "           Dump component <", name, "> "
print "======================================================================"
C = catalog.GetComponent(name)

print "name       : ", C._get_componentname()
print "username   : ", C._get_componentusername()
print "type       : ", C._get_component_type()
print "constraint : ", C._get_constraint()
print "icon       : ", C._get_component_icone()

for iL in C.GetInterfaceList():
    I = C.GetInterface(iL)
    print "interface  : ", I.interfacename
    for S in I.interfaceservicelist:
        print "  service : ", S.ServiceName
        print "    ", len(S.ServiceinParameter), "in params : "
        for iP in S.ServiceinParameter:
            print '      ' + iP.Parametername + '(' + iP.Parametertype + ')'
            pass
        print "    ", len(S.ServiceoutParameter), "out params : "
        for iP in S.ServiceoutParameter:
            print '      ' + iP.Parametername + '(' + iP.Parametertype + ')'
            pass
        print "    ", len(S.ServiceinDataStreamParameter), "in datastream params : "
        for iP in S.ServiceinDataStreamParameter:
            print '      ' + iP.Parametername + '(' + str(iP.Parametertype) + ', ' + \
                  str(iP.Parameterdependency) + ')'
            pass
        print "    ", len(S.ServiceoutDataStreamParameter), "out datastream params : "
        for iP in S.ServiceoutDataStreamParameter:
            print '      ' + iP.Parametername + '(' + str(iP.Parametertype) + ', ' + \
                  str(iP.Parameterdependency) + ')'
            pass
        pass
    pass

