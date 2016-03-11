# -*- coding: iso-8859-1 -*-
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

# Author: Guillaume Boulant (EDF/R&D)

## \defgroup service service
#  \{ 
#  \details Helper for using %SALOME kernel services
#  \}

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
from deprecation import is_called_by_sphinx
if not is_called_by_sphinx() and salome.lcc is None:
    try:
        salome.salome_init()
    except Exception, e:
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

## Get a %SALOME CORBA component from its name
#  \param componentName is the name of the component as declared in the XML
#  %SALOME catalog. %A loadable library with name lib<componentName>Engine.so
#  is supposed to be reachable. This library is supposed to provide a
#  factory function with the prototype:
#
#  \code
#  PortableServer::ObjectId * <componentName>Engine_factory( CORBA::ORB_ptr orb,
#                                                            PortableServer::POA_ptr poa,
#                                                            PortableServer::ObjectId* contId,
#                                                            const char *instanceName,
#                                                            const char *interfaceName );
#  \endcode
#
#  \param corbaModule is the name of the IDL module that contains the
#  definition of the interface of the component. This name corresponds
#  to the namespace of the servant classes.
#
#  \param containerType specified the container in which the servants are
#  executed.
#  \ingroup service
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

# Note that an alternative (and maybe better) method to get a component
# is to use the module catalog. Here, we just use the catalog to get
# the list of components defined in the current session.
import SALOME_ModuleCatalog

## Get the list of names of all %SALOME componenents register in
#  the catalog.
#  \ingroup service
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

## Get a study manager to create and manage %SALOME studies
#  \ingroup service
def getStudyManager():
    """Get a study manager to create and manage SALOME studies"""
    return salome.myStudyManager

import SALOMEDS
## Get a study manager to create and manage SALOME studies. 
#  \warning you should use instead the variable salome.myStudyManager. 
#  This function is given for illustration of usage of the naming service
#  \ingroup service
def __getStudyManager_demo():
    """
    Get a study manager to create and manage SALOME studies. WARN: you
    should use instead the variable salome.myStudyManager. This
    function is given for illustration of usage of the naming service
    """
    naming_service = SALOME_NamingServicePy_i( orb )
    obj = naming_service.Resolve( '/myStudyManager' )
    studyManager = obj._narrow( SALOMEDS.StudyManager)
    return studyManager


#
# ==============================================================================
# Helper functions for manipulating objects, sobjects and entry
# ==============================================================================
#

# - the SObject is an item in a study (Study Object).
# - the entry is the identifier of an item.
# - the ID is the entry
# - the object (geom object or smesh object) is a CORBA servant
#   embedded in the SALOME component container and with a reference in
#   the SALOME study, so that it can be retrieved.

# __GBO__ WARN: theses functions are already defined in
# salome_study.py, but without the possibility to specify the
# underlying study (in salome_study.py, the study is the default study
# binded to the salome.myStudy attribute). TODO: see if it can be
# extends to the prototype (with the study as an argument) below and
# resorb the functions below.

def IDToObject(id, study=None):
    myObj = None
    if study is None:
        myStudy = salome.myStudy
    else:
        myStudy = study
    mySO = myStudy.FindObjectID(id);
    if mySO is not None:
        ok, anAttr = mySO.FindAttribute("AttributeIOR")
        if ok:
            AtIOR = anAttr._narrow(SALOMEDS.AttributeIOR)
            if AtIOR.Value() != "":
                myObj = salome.orb.string_to_object(AtIOR.Value())
    return myObj

def ObjectToSObject(obj, study=None):
    mySO = None

    if study is None:
        myStudy = salome.myStudy
    else:
        myStudy = study

    if obj is not None:
        ior =  salome.orb.object_to_string(obj)
        if ior != "":
            mySO = myStudy.FindObjectIOR(ior)
    return mySO

def ObjectToID(obj, study=None):
    mySO = ObjectToSObject(obj,study)
    if mySO:
        return mySO.GetID()
    return ""

def IDToSObject(id, study=None):
    if study is None:
        myStudy = salome.myStudy
    else:
        myStudy = study

    mySO = myStudy.FindObjectID(id);
    return mySO

def SObjectToID(sobject):
    if sobject is None: return None
    return sobject.GetID()


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

def TEST_createObject():
    """
    WARNING: for this test, we need GEOM (used to create an object) 
    """
    import GEOM
    from salome.geom import geomBuilder
    geompy = geomBuilder.New(salome.myStudy)

    box = geompy.MakeBoxDXDYDZ(200, 200, 200)
    id = geompy.addToStudy( box, 'box' )
    return id

def TEST_objectsManipulation():
    myEntry = TEST_createObject()

    mySObject = IDToSObject(myEntry)
    entry     = SObjectToID(mySObject)

    if str(entry) != str(myEntry):
        return False


    myObject = IDToObject(myEntry)
    print myObject
    if myObject is None:
        return False

    return True

if __name__ == "__main__":
    import unittester
    unittester.run("services","TEST_getComponent")
    unittester.run("services","TEST_getComponentList")
    unittester.run("services","TEST_objectsManipulation")
