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

#from TestLifeCycleCORBA import *
#
import os
import Engines
import LifeCycleCORBA

import Utils_Identity
host  = Utils_Identity.getShortHostName()
#host = os.getenv( 'HOST' )

lcc = LifeCycleCORBA.LifeCycleCORBA()

try :
    obj=lcc.FindOrLoad_Component("FactoryServer","SalomeTestComponent")
    comp=obj._narrow(Engines.TestComponent)
    comp.Coucou(1)
    param = LifeCycleCORBA.ContainerParameters()
    param.resource_params.hostname = host
    param.container_name = 'FactoryServer'
    comp=lcc.FindOrLoad_Component(param,'SalomeTestComponent')
    engine=lcc.FindComponent(param,'SalomeTestComponent')
    engine.Coucou(1)
except :
    print 'lcc.FindOrLoad_Component("FactoryServer","SalomeTestComponent") failed'

import sys
import CORBA
import CosNaming
orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)
obj = orb.resolve_initial_references("NameService")
rootContext = obj._narrow(CosNaming.NamingContext)
context_name=[]
context_name.append(CosNaming.NameComponent( 'ContainerManager' , 'object' ) )
ContainerManager = rootContext.resolve( context_name )

try :
    myContainerparamsCPP = LifeCycleCORBA.ContainerParameters()
    param.resource_params.hostname = host
    param.container_name = 'myContainer'
    computerlistCPP = [host]
    containerCPP = ContainerManager.FindOrStartContainer( myContainerparamsCPP , computerlistCPP )
    containerCPP.ping()
    ComponentparamsCPP={}
    ComponentparamsCPP['hostname']=host
    ComponentparamsCPP['container_name']='myContainer'
    compCPP=lcc.FindOrLoad_Component(myContainerparamsCPP,'SalomeTestComponent')
    compCPP.Coucou(1)
    engineCPP=lcc.FindComponent(myContainerparamsCPP,'SalomeTestComponent')
    engineCPP.Coucou(1)
except :
    print 'ContainerManager.FindOrStartContainer( myContainerparams , computerlist ) C++ failed'

try :
    myContainerparamsPy = LifeCycleCORBA.ContainerParameters()
    param.resource_params.hostname = host
    param.container_name = 'myContainerPy'
    computerlistPy = [host]
    containerPy = ContainerManager.FindOrStartContainer( myContainerparamsPy , computerlistPy )
    containerPy.ping()
    compPy=lcc.FindOrLoad_Component(myContainerparamsPy,'SALOME_TestComponentPy')
    compPy.Coucou(1)
    enginePy=lcc.FindComponent(myContainerparamsPy,'SALOME_TestComponentPy')
    enginePy.Coucou(1)
except :
    print 'ContainerManager.FindOrStartContainer( myContainerparams , computerlist ) Python failed'

