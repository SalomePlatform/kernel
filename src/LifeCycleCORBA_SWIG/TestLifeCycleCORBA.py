
import Engines
import LifeCycleCORBA

lcc = LifeCycleCORBA.LifeCycleCORBA()

#obj=lcc.FindOrLoad_Component("FactoryServer","SalomeTestComponent")
#comp=obj._narrow(Engines.TestComponent)
#comp.Coucou(1)

param={}
param['hostname']='cli76ce'
param['container_name']='myContainer'
smesh=lcc.FindOrLoad_Component(param,'SMESH')

container=lcc.FindContainer('myContainer')
engine=lcc.FindComponent(param,'SMESH')
geom=lcc.LoadComponent(param,'GEOM')
