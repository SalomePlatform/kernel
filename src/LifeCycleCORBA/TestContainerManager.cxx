// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  SALOME TestContainer : test of container creation and its life cycle
//  File   : TestContainer.cxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$
//
#include "utilities.h"
#include <iostream>
#ifndef WIN32
#include <unistd.h>
#endif
#include <SALOMEconfig.h>
#include "SALOME_NamingService.hxx"
#include "SALOME_ResourcesManager.hxx"
#include "SALOME_ContainerManager.hxx"
#include "SALOME_LifeCycleCORBA.hxx"
#include "SALOME_ResourcesManager.hxx"
#include "NamingService_WaitForServerReadiness.hxx"
#include "OpUtil.hxx"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "Utils_SALOME_Exception.hxx"
#include "Utils_CommException.hxx"
#include "Basics_DirUtils.hxx"

int main (int argc, char * argv[])
{
  std::map<std::string, int> cycle;
  std::map<std::string, int> first;
  Engines::Container_ptr cont;
  Engines::EngineComponent_ptr compo;
  bool error = false;
  bool bestImplemented;
  int status;

  // Initializing omniORB
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  CORBA::ORB_ptr orb = init( argc , argv ) ;

  SALOME_NamingService *_NS=new SALOME_NamingService(orb);

  CORBA::Object_var obj = _NS->Resolve(SALOME_ContainerManager::_ContainerManagerNameInNS);
  ASSERT( !CORBA::is_nil(obj));
  Engines::ContainerManager_var _ContManager=Engines::ContainerManager::_narrow(obj);
  obj = _NS->Resolve(SALOME_ResourcesManager::_ResourcesManagerNameInNS);
  ASSERT( !CORBA::is_nil(obj));
  Engines::ResourcesManager_var _ResManager=Engines::ResourcesManager::_narrow(obj);

  Engines::ContainerParameters p;
  p.resource_params.componentList.length(2);
  p.resource_params.componentList[0] = "MED";
  p.resource_params.componentList[1] = "GEOM";

  p.resource_params.hostname = "";
  p.resource_params.OS = "LINUX";
  p.resource_params.mem_mb = 1000;
  p.resource_params.cpu_clock = 1000;
  p.resource_params.nb_proc_per_node = 1;
  p.resource_params.nb_node = 1;
  p.isMPI = false;

  char st[10];
  for(int i=0;i<10;i++){
    sprintf(st,"cycl_%d",i);
    p.container_name = CORBA::string_dup(st);
    p.resource_params.policy="cycl";
    cont = _ContManager->GiveContainer(p);
    if(CORBA::is_nil(cont)) error = true;
  }

  for(int i=0;i<10;i++){
    sprintf(st,"first_%d",i);
    p.container_name = CORBA::string_dup(st);
    p.resource_params.policy="first";
    cont = _ContManager->GiveContainer(p);
    if(CORBA::is_nil(cont)) error = true;
  }

  p.container_name = CORBA::string_dup("best");
  p.resource_params.policy="best";
  cont = _ContManager->GiveContainer(p);
  if(CORBA::is_nil(cont)) bestImplemented = false;
  else bestImplemented = true;

  SALOME_LifeCycleCORBA LCC(_NS);
  p.container_name = CORBA::string_dup("FactoryServer");
  compo = LCC.FindOrLoad_Component(p,"SMESH");
  if(CORBA::is_nil(compo)) error = true;
  compo = LCC.FindOrLoad_Component(p,"SMESH");
  if(CORBA::is_nil(compo)) error = true;

  _NS->Change_Directory("/Containers");

  std::vector<std::string> vec = _NS->list_directory_recurs();
  std::list<std::string> lstCont;
  for(std::vector<std::string>::iterator iter = vec.begin();iter!=vec.end();iter++){
    CORBA::Object_var obj=_NS->Resolve((*iter).c_str());
    Engines::Container_var cont=Engines::Container::_narrow(obj);
    if(!CORBA::is_nil(cont)){
      cycle[cont->getHostName()]=0;
      first[cont->getHostName()]=0;
      lstCont.push_back((*iter));
    }
  }
  for(std::list<std::string>::iterator iter=lstCont.begin();iter!=lstCont.end();iter++){
    CORBA::Object_var obj=_NS->Resolve((*iter).c_str());
    Engines::Container_var cont=Engines::Container::_narrow(obj);
    if(!CORBA::is_nil(cont)){
      std::string basename = Kernel_Utils::GetBaseName(cont->name());
      if(basename.compare(0,4,"cycl")==0)
        cycle[cont->getHostName()]++;
      if(basename.compare(0,5,"first")==0)
        first[cont->getHostName()]++;
    }
  }
  _ContManager->ShutdownContainers();

  int cmin=10;
  int cmax=0;
  int fmin=10;
  int fmax=0;
  int nbpmax;
  for(std::map<std::string,int>::iterator iter=cycle.begin();iter!=cycle.end();iter++){
    if(strcmp((*iter).first.c_str(),"localhost")!=0){
      Engines::ResourceDefinition *p = _ResManager->GetResourceDefinition((*iter).first.c_str());
      int nbproc = p->nb_node * p->nb_proc_per_node;
      if(cycle[(*iter).first]/nbproc<cmin) cmin=cycle[(*iter).first]/nbproc;
      if(cycle[(*iter).first]/nbproc>cmax) cmax=cycle[(*iter).first]/nbproc;
      if(first[(*iter).first]/nbproc<fmin) fmin=first[(*iter).first]/nbproc;
      if(first[(*iter).first]/nbproc>fmax){
        fmax=first[(*iter).first]/nbproc;
        nbpmax = nbproc;
      }
    }
  }
  std::string msg;
  if( ((cmax-cmin) <= 1) && (fmax == 10/nbpmax) && !error ){
    if(bestImplemented)
      msg = "TEST OK";
    else
      msg = "TEST OK but FindBest not implemented!";
    status=0;
  }
  else{
    msg ="TEST KO";
    status=1;
  }
  std::cout << msg << std::endl;

  return status;
}
