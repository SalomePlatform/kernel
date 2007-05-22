//  SALOME TestContainer : test of container creation and its life cycle
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : TestContainer.cxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$

#include "utilities.h"
#include <iostream>
#include <unistd.h>
#include <SALOMEconfig.h>
#include "SALOME_NamingService.hxx"
#include "SALOME_ContainerManager.hxx"
#include "SALOME_LifeCycleCORBA.hxx"
#include "NamingService_WaitForServerReadiness.hxx"
#include "OpUtil.hxx"
#include "Utils_ORB_INIT.hxx"
#include "Utils_SINGLETON.hxx"
#include "Utils_SALOME_Exception.hxx"
#include "Utils_CommException.hxx"
using namespace std;

int main (int argc, char * argv[])
{
  map<string, int> cycle;
  map<string, int> first;
  Engines::Container_ptr cont;
  Engines::Component_ptr compo;
  bool error = false;
  bool bestImplemented;

  // Initializing omniORB
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  CORBA::ORB_ptr orb = init( argc , argv ) ;

  SALOME_NamingService *_NS=new SALOME_NamingService(orb);

  CORBA::Object_var obj = _NS->Resolve(SALOME_ContainerManager::_ContainerManagerNameInNS);
  ASSERT( !CORBA::is_nil(obj));
  Engines::ContainerManager_var _ContManager=Engines::ContainerManager::_narrow(obj);

  Engines::MachineParameters p;

  p.hostname = "";
  p.OS = "LINUX";
  p.mem_mb = 1000;
  p.cpu_clock = 1000;
  p.nb_proc_per_node = 1;
  p.nb_node = 1;
  p.isMPI = false;

  char st[10];
  for(int i=0;i<10;i++){
    sprintf(st,"cycl_%d",i);
    p.container_name = CORBA::string_dup(st);
    cont = _ContManager->StartContainer(p,Engines::P_CYCL);
    if(CORBA::is_nil(cont)) error = true;
  }

  for(int i=0;i<10;i++){
    sprintf(st,"first_%d",i);
    p.container_name = CORBA::string_dup(st);
    cont = _ContManager->StartContainer(p,Engines::P_FIRST);
    if(CORBA::is_nil(cont)) error = true;
  }

  p.container_name = CORBA::string_dup("best");
  cont = _ContManager->StartContainer(p,Engines::P_BEST);
  if(CORBA::is_nil(cont)) bestImplemented = false;
  else bestImplemented = true;

  SALOME_LifeCycleCORBA LCC(_NS);
  compo = LCC.FindOrLoad_Component("FactoryServer","GEOM");
  if(CORBA::is_nil(compo)) error = true;
  compo = LCC.FindOrLoad_Component("FactoryServer","GEOM");
  if(CORBA::is_nil(compo)) error = true;

  _NS->Change_Directory("/Containers");

  vector<string> vec = _NS->list_directory_recurs();
  list<string> lstCont;
  for(vector<string>::iterator iter = vec.begin();iter!=vec.end();iter++){
    CORBA::Object_var obj=_NS->Resolve((*iter).c_str());
    Engines::Container_var cont=Engines::Container::_narrow(obj);
    if(!CORBA::is_nil(cont)){
      cycle[cont->getHostName()]=0;
      first[cont->getHostName()]=0;
      lstCont.push_back((*iter));
    }
  }
  for(list<string>::iterator iter=lstCont.begin();iter!=lstCont.end();iter++){
    CORBA::Object_var obj=_NS->Resolve((*iter).c_str());
    Engines::Container_var cont=Engines::Container::_narrow(obj);
    if(!CORBA::is_nil(cont)){
      if(strncmp(basename(cont->name()),"cycl",4)==0)
	cycle[cont->getHostName()]++;
      if(strncmp(basename(cont->name()),"first",5)==0)
	first[cont->getHostName()]++;
    }
  }
  _ContManager->ShutdownContainers();

  int cmin=10;
  int cmax=0;
  int fmin=10;
  int fmax=0;
  for(map<string,int>::iterator iter=cycle.begin();iter!=cycle.end();iter++){
    if(strcmp((*iter).first.c_str(),"localhost")!=0){
      if(cycle[(*iter).first]<cmin) cmin=cycle[(*iter).first];
      if(cycle[(*iter).first]>cmax) cmax=cycle[(*iter).first];
      if(first[(*iter).first]<fmin) fmin=first[(*iter).first];
      if(first[(*iter).first]>fmax) fmax=first[(*iter).first];
    }
  }
  if( ((cmax-cmin) <= 1) && (fmax == 10) && !error ){
    string msg;
    if(bestImplemented)
      msg = "TEST OK";
    else
      msg = "TEST OK but FindBest not implemented!";
    MESSAGE(msg);
    return 0;
  }
  else{
    MESSAGE("TEST KO");
    return 1;
  }
}
