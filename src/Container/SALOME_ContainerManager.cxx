#include "SALOME_ContainerManager.hxx"
#include "SALOME_NamingService.hxx"
#include "OpUtil.hxx"
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define TIME_OUT_TO_LAUNCH_CONT 21

using namespace std;

const char *SALOME_ContainerManager::_ContainerManagerNameInNS="ContainerManager";

SALOME_ContainerManager::SALOME_ContainerManager(CORBA::ORB_ptr orb)
{
  _NS=new SALOME_NamingService(orb);
  PortableServer::POA_var root_poa=PortableServer::POA::_the_root_poa();
  PortableServer::ObjectId_var id=root_poa->activate_object(this);
  CORBA::Object_var obj=root_poa->id_to_reference(id);
  Engines::ContainerManager_var refContMan = Engines::ContainerManager::_narrow(obj);
  _NS->Register(refContMan,_ContainerManagerNameInNS);
}

SALOME_ContainerManager::~SALOME_ContainerManager()
{
  delete _NS;
}

void SALOME_ContainerManager::Shutdown()
{
  ShutdownContainers();
  PortableServer::ObjectId_var oid = _default_POA()->servant_to_id(this);
  _default_POA()->deactivate_object(oid);
  _remove_ref();
  
}

void SALOME_ContainerManager::ShutdownContainers()
{
  _NS->Change_Directory("/Containers");
  vector<string> vec=_NS->list_directory_recurs();
  for(vector<string>::iterator iter=vec.begin();iter!=vec.end();iter++)
    {
      CORBA::Object_var obj=_NS->Resolve((*iter).c_str());
      Engines::Container_var cont=Engines::Container::_narrow(obj);
      if(!CORBA::is_nil(cont))
	cont->Shutdown();
    }
}

Engines::Container_ptr SALOME_ContainerManager::FindOrStartContainer(const char *containerName, const Engines::MachineList& possibleComputers)
{
  Engines::Container_ptr ret=FindContainer(containerName,possibleComputers);
  if(!CORBA::is_nil(ret))
    return ret;
  // Container doesn't exist try to launch it ...
  vector<string> vector;
  string theMachine=_LoadManager.FindBest(possibleComputers);
  string command;
  if(theMachine==GetHostname())
    command=_ResManager.BuildCommandToLaunchLocalContainer(containerName);
  else
    command=_ResManager.BuildTempFileToLaunchRemoteContainer(theMachine,containerName);
  _ResManager.RmTmpFile();
  int status=system(command.c_str());
  if (status == -1) {
    MESSAGE("SALOME_LifeCycleCORBA::StartOrFindContainer rsh failed (system command status -1)");
    return Engines::Container::_nil();
  }
  else if (status == 217) {
    MESSAGE("SALOME_LifeCycleCORBA::StartOrFindContainer rsh failed (system command status 217)");
    return Engines::Container::_nil();
  }
  else {
    int count=TIME_OUT_TO_LAUNCH_CONT;
    while ( CORBA::is_nil(ret) && count ) {
      sleep( 1 ) ;
      count-- ;
      if ( count != 10 )
	MESSAGE( count << ". Waiting for FactoryServer on " << theMachine);
      string containerNameInNS=BuildContainerNameInNS(containerName,theMachine.c_str());
      CORBA::Object_var obj = _NS->Resolve(containerNameInNS.c_str());
      ret=Engines::Container::_narrow(obj);
    }
    if ( CORBA::is_nil(ret) ) {
      MESSAGE("SALOME_LifeCycleCORBA::StartOrFindContainer rsh failed");
    }
    return ret;
  }
}

Engines::MachineList *SALOME_ContainerManager::GetFittingResources(const Engines::MachineParameters& params, const char *componentName)
{
  vector<string> vec=_ResManager.GetFittingResources(params,componentName);
  Engines::MachineList *ret=new Engines::MachineList;
  ret->length(vec.size());
  for(unsigned int i=0;i<vec.size();i++)
    {
      (*ret)[i]=(vec[i]).c_str();
    }
  return ret;
}

char* SALOME_ContainerManager::FindBest(const Engines::MachineList& possibleComputers)
  {
    string theMachine=_LoadManager.FindBest(possibleComputers);
    return CORBA::string_dup(theMachine.c_str());
  }

Engines::Container_ptr SALOME_ContainerManager::FindContainer(const char *containerName,const char *theMachine)
{
  string containerNameInNS(BuildContainerNameInNS(containerName,theMachine));
  CORBA::Object_var obj = _NS->Resolve(containerNameInNS.c_str());
  if( !CORBA::is_nil(obj) )
    return Engines::Container::_narrow(obj);
  else
    return Engines::Container::_nil();
}

Engines::Container_ptr SALOME_ContainerManager::FindContainer(const char *containerName,const Engines::MachineList& possibleComputers)
{
  for(unsigned int i=0;i<possibleComputers.length();i++)
    {
      Engines::Container_ptr cont=FindContainer(containerName,possibleComputers[i]);
      if( !CORBA::is_nil(cont) )
	return cont;
    }
  return Engines::Container::_nil();
}

string SALOME_ContainerManager::BuildContainerNameInNS(const char *containerName,const char *machineName)
{
  string containerNameInNS("/Containers/");
  containerNameInNS+=machineName;
  containerNameInNS+="/";
  containerNameInNS+=containerName;
  return containerNameInNS;
}
