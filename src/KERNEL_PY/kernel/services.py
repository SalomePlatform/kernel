# -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2011  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
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

# Author: Guillaume Boulant (EDF/R&D)

#
# WARNING: development notes
#
# This python module is aimed to be a (yet another?) end user
# interface to manipulate the KERNEL SALOME services. Note that it
# does not replace the salome module (i.e. what you get when typing
# "import salome", actually implemented in the file __init__.py of the
# salome python package). It provides instead functions that help to
# use the salome module in a end user context (development of domain
# specific tools by programmers that are not ten years experienced in
# SALOME development).

import salome
if salome.lcc is None:
    try:
        salome.salome_init()
    except RuntimeError, e:
        print e

# Note that the salome module provides you with standard SALOME
# objects: CORBA broker (orb): salome.orb lyfe cycle (lcc) :
# salome.lcc naming service : salome.naming_service study manager :
# salome.myStudyManager The default study : salome.myStudy
#
# Alternatively, you may obtain these objects directly with the
# following instructions:
#
#   from omniORB import CORBA
#   from LifeCycleCORBA import LifeCycleCORBA
#   orb = CORBA.ORB_init( [''], CORBA.ORB_ID )
#   lcc = LifeCycleCORBA( orb )
# 
#   from SALOME_NamingServicePy import SALOME_NamingServicePy_i
#   naming_service = SALOME_NamingServicePy_i( orb )
#
# (See salome.py page in the KERNEL documentation)

#
# ==============================================================================
# Helper functions for SALOME components
# ==============================================================================
#

#
# componantName is the name of the component as declared in the XML
# SALOME catalog. A loadable library with name lib<componentName>Engine.so
# is supposed to be reachable. This library is supposed to provide a
# factory function with the prototype:
#
#   PortableServer::ObjectId * <componentName>Engine_factory(
#                                             CORBA::ORB_ptr orb,
#    	                                      PortableServer::POA_ptr poa,
#					      PortableServer::ObjectId * contId,
#					      const char *instanceName,
#					      const char *interfaceName);
#
# corbaModule is the name of the IDL module that contains the
# definition of the interface of the component. This name corresponds
# to the namespace of the servant classes.
#
# containerType specified the container in which the servants are
# executed.
#
def getComponent(componentName = "SalomeTestComponent",
                 corbaModule   = "Engines",
                 containerType = "FactoryServer"):
    """
    Get a SALOME CORBA component from its name
    """
    print "INF: getting component %s from CORBA module %s ..."%(componentName,corbaModule)
    __import__(corbaModule)
    component=salome.lcc.FindOrLoadComponent(containerType,componentName)
    if component is None:
        print "ERR: the SALOME component "+componentName+" can't be reached"
    print "INF: component %s obtained from CORBA module %s"%(componentName,corbaModule)
    return component

# Not that an alternative (and maybe better) method to get a component
# is to use the module catalog. Here, we just use the catalog to get
# the list of components defined in the current session.
import SALOME_ModuleCatalog
def getComponentList():
    """
    Get the list of names of all SALOME componenents register in
    the catalog.
    """
    obj = salome.naming_service.Resolve('Kernel/ModulCatalog')
    catalog = obj._narrow(SALOME_ModuleCatalog.ModuleCatalog)
    if not catalog:
        raise RuntimeError, "Can't accesss module catalog"
    return catalog.GetComponentList()

import SALOMEDS
def getStudyManager():
    """
    Get a study manager to create and manage SALOME studies. WARN: you
    should use instead the variable salome.myStudyManager. This
    function is given for illustration of usage of the naming service
    """
    import SALOMEDS
    naming_service = SALOME_NamingServicePy_i( orb )
    obj = naming_service.Resolve( '/myStudyManager' )
    studyManager = obj._narrow( SALOMEDS.StudyManager)
    return studyManager


#
# ==============================================================================
# Basic use cases and unit tests
# ==============================================================================
#

def TEST_getComponent():
    component=getComponent(componentName = "SalomeTestComponent")
    
    ref_string = 'TestComponent_i : L = 3'
    res_string = component.Coucou(3)
    if ref_string != ref_string:
        return False
    return True

def TEST_getComponentList():
    componentList=getComponentList()
    if 'SalomeTestComponent' not in componentList:
        return False
    return True

if __name__ == "__main__":
    import unittester
    unittester.run("services","TEST_getComponent")
    unittester.run("services","TEST_getComponentList")
