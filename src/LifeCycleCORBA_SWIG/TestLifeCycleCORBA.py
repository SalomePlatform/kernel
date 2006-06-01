# Copyright (C) 2005  OPEN CASCADE, CEA, EDF R&D, LEG
#           PRINCIPIA R&D, EADS CCR, Lip6, BV, CEDRAT
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either 
# version 2.1 of the License.
# 
# This library is distributed in the hope that it will be useful 
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
    param={}
    #param['hostname']='cli76cc'
    param['hostname']=host
    param['container_name']='FactoryServer'
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
    myContainerparamsCPP = Engines.MachineParameters( 'myContainer' , host , 'osf' , 0 , 0 , 0 , 0 , 0 )
    computerlistCPP = [host]
    containerCPP = ContainerManager.FindOrStartContainer( myContainerparamsCPP , computerlistCPP )
    containerCPP.ping()
    ComponentparamsCPP={}
    ComponentparamsCPP['hostname']=host
    ComponentparamsCPP['container_name']='myContainer'
    compCPP=lcc.FindOrLoad_Component(ComponentparamsCPP,'SalomeTestComponent')
    compCPP.Coucou(1)
    engineCPP=lcc.FindComponent(ComponentparamsCPP,'SalomeTestComponent')
    engineCPP.Coucou(1)
except :
    print 'ContainerManager.FindOrStartContainer( myContainerparams , computerlist ) C++ failed'

try :
    myContainerparamsPy = Engines.MachineParameters( 'myContainerPy' , host , 'osf' , 0 , 0 , 0 , 0 , 0 )
    computerlistPy = [host]
    containerPy = ContainerManager.FindOrStartContainer( myContainerparamsPy , computerlistPy )
    containerPy.ping()
    ComponentparamsPy={}
    ComponentparamsPy['hostname']=host
    ComponentparamsPy['container_name']='myContainerPy'
    compPy=lcc.FindOrLoad_Component(ComponentparamsPy,'SALOME_TestComponentPy')
    compPy.Coucou(1)
    enginePy=lcc.FindComponent(ComponentparamsPy,'SALOME_TestComponentPy')
    enginePy.Coucou(1)
except :
    print 'ContainerManager.FindOrStartContainer( myContainerparams , computerlist ) Python failed'

