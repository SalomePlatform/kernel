// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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
//  File   : SALOME_ParallelContainer_i.cxx
//  Author : André RIBES, EDF

#include "SALOME_ParallelContainer_i.hxx"
#include "SALOME_Component_i.hxx"
#include "SALOME_FileRef_i.hxx"
#include "SALOME_FileTransfer_i.hxx"
#include "SALOME_NamingService.hxx"
#include "OpUtil.hxx"
#include "utilities.h"
#include "Basics_Utils.hxx"

#include <string.h>
#include <stdio.h>
#ifndef WIN32
#include <sys/time.h>
#include <dlfcn.h>
#include <unistd.h>
#else
#include <signal.h>
#include <process.h>
#include <direct.h>
int SIGUSR1 = 1000;
#endif

#include <paco_omni.h>

#include <Python.h>
#include "Container_init_python.hxx"


bool _Sleeping = false ;

extern "C" {void ActSigIntHandler() ; }
#ifndef WIN32
extern "C" {void SigIntHandler(int, siginfo_t *, void *) ; }
#else
extern "C" {void SigIntHandler( int ) ; }
#endif

/*! \class Engines_Parallel_Container_i
 *  \brief C++ implementation of Engines::Container interface for parallel
 *  container implemented with PaCO++
 */

//=============================================================================
/*! 
 *  Construtor
 */
//=============================================================================

Engines_Parallel_Container_i::Engines_Parallel_Container_i (CORBA::ORB_ptr orb, 
                                                            char * ior, 
                                                            int rank,
                                                            PortableServer::POA_ptr poa,
                                                            std::string containerName,
                                                            bool isServantAloneInProcess) :
  InterfaceParallel_impl(orb,ior,rank), 
  Engines::PACO_Container_serv(orb,ior,rank),
  Engines::PACO_Container_base_serv(orb,ior,rank),
  Engines::Container_serv(orb,ior,rank),
  Engines::Container_base_serv(orb,ior,rank),
  _numInstance(0),_isServantAloneInProcess(isServantAloneInProcess)
{
  // Members init
  _pid = getpid();
  _hostname = Kernel_Utils::GetHostname();
  _orb = CORBA::ORB::_duplicate(orb);
  _poa = PortableServer::POA::_duplicate(poa);

  // Add CORBA object to the poa
  _id = _poa->activate_object(this);
  this->_remove_ref();
  CORBA::Object_var container_node = _poa->id_to_reference(*_id);

  // Adding this servant to SALOME
  _NS = new SALOME_NamingService();
  _NS->init_orb(_orb);
  _containerName = _NS->BuildContainerNameForNS(containerName.c_str(), _hostname.c_str());

  // Ajout du numero de noeud
  char node_number[12];
  sprintf(node_number, "%d", getMyRank());
  _containerName = _containerName + node_number;

  // Init Python container part
  CORBA::String_var sior =  _orb->object_to_string(container_node);
  std::string myCommand="pyCont = SALOME_Container.SALOME_Container_i('";
  myCommand += _containerName + "','";
  myCommand += sior;
  myCommand += "')\n";
  Py_ACQUIRE_NEW_THREAD;
  PyRun_SimpleString("import SALOME_Container\n");
  PyRun_SimpleString((char*)myCommand.c_str());
  Py_RELEASE_NEW_THREAD;

  // Init FileTransfer service
  fileTransfer_i* aFileTransfer = new fileTransfer_i();
  _fileTransfer = aFileTransfer->_this();
  aFileTransfer->_remove_ref();

  // Some signal handlers
  ActSigIntHandler();
}

//=============================================================================
/*! 
 *  Destructor
 */
//=============================================================================

Engines_Parallel_Container_i::~Engines_Parallel_Container_i()
{
  MESSAGE("Container_i::~Container_i()");
  if (_id)
    delete _id;
  if(_NS)
    delete _NS;
}

//=============================================================================
//! Get container name
/*! 
 *  CORBA attribute: Container name (see constructor)
 */
//=============================================================================

char* Engines_Parallel_Container_i::name()
{
  return CORBA::string_dup(_containerName.c_str()) ;
}

//=============================================================================
//! Get container working directory
/*! 
 *  CORBA attribute: Container working directory 
 */
//=============================================================================

char* 
Engines_Parallel_Container_i::workingdir()
{
  char wd[256];
  getcwd (wd,256);
  return CORBA::string_dup(wd) ;
}

//=============================================================================
//! Get container log file name
/*! 
 *  CORBA attribute: Container log file name
 */
//=============================================================================

char* 
Engines_Parallel_Container_i::logfilename()
{
  return CORBA::string_dup(_logfilename.c_str()) ;
}

//! Set container log file name
void 
Engines_Parallel_Container_i::logfilename(const char* name)
{
  _logfilename=name;
}

//=============================================================================
//! Get container host name
/*! 
 *  CORBA method: Get the hostName of the Container (without domain extensions)
 */
//=============================================================================

char* Engines_Parallel_Container_i::getHostName()
{
  MESSAGE("Warning: getHostName of a parallel container returns the hostname of the first servant node");
  return CORBA::string_dup(_hostname.c_str()) ;
}

//=============================================================================
//! Get container PID
/*! 
 *  CORBA method: Get the PID (process identification) of the Container
 */
//=============================================================================

CORBA::Long Engines_Parallel_Container_i::getPID()
{
  MESSAGE("Warning: getPID of a parallel container returns the PID of the first servant node");
  return _pid;
}

//=============================================================================
//! Ping the servant to check it is still alive
/*! 
 *  CORBA method: check if servant is still alive
 */
//=============================================================================

void Engines_Parallel_Container_i::ping()
{
  MESSAGE("Engines_Parallel_Container_i::ping() my pid is "<< _pid);
}

//=============================================================================
//! Shutdown the container
/*! 
 *  CORBA method, oneway: Server shutdown. 
 *  - Container name removed from naming service,
 *  - servant deactivation,
 *  - orb shutdown if no other servants in the process 
 */
//=============================================================================

void Engines_Parallel_Container_i::Shutdown()
{
  MESSAGE("Engines_Parallel_Container_i::Shutdown()");

  /* For each seq component contained in this container
  * tell it to self-destroy
  */
  std::map<std::string, Engines::EngineComponent_var>::iterator itm;
  for (itm = _listInstances_map.begin(); itm != _listInstances_map.end(); itm++)
  {
    try
    {
      itm->second->destroy();
    }
    catch(const CORBA::Exception& e)
    {
      // ignore this entry and continue
    }
    catch(...)
    {
      // ignore this entry and continue
    }
  }

  // Destroy each parallel component node...
  std::map<std::string, PortableServer::ObjectId *>::iterator i;
  for (i = _par_obj_inst_map.begin(); i != _par_obj_inst_map.end(); i++)
    _poa->deactivate_object(*(i->second));

  _NS->Destroy_FullDirectory(_containerName.c_str());
  _NS->Destroy_Name(_containerName.c_str());

  if(_isServantAloneInProcess)
  {
    MESSAGE("Effective Shutdown of container Begins...");
    if(!CORBA::is_nil(_orb))
      _orb->shutdown(0);
  }
}


//=============================================================================
//! load a new component class
/*! 
 *  CORBA method: load a new component class (Python or C++ implementation)
 *  \param componentName like COMPONENT
 *                          try to make a Python import of COMPONENT,
 *                          then a lib open of libCOMPONENTEngine.so
 *  \return true if dlopen successfull or already done, false otherwise
 */
//=============================================================================

bool
Engines_Parallel_Container_i::load_component_Library(const char* componentName, CORBA::String_out reason)
{
  reason=CORBA::string_dup("");

  MESSAGE("Begin of load_component_Library : " << componentName)
  bool ret = false;
  std::string aCompName = componentName;
#ifndef WIN32
  std::string impl_name = string ("lib") + aCompName + string("Engine.so");
#else
  std::string impl_name = aCompName + string("Engine.dll");
#endif

  _numInstanceMutex.lock(); // lock to be alone 

  // Check if already loaded or imported in the container
  if (_toRemove_map.count(impl_name) != 0) _toRemove_map.erase(impl_name);
  if (_library_map.count(impl_name) != 0)
  {
    MESSAGE("Library " << impl_name << " already loaded");
    ret = true;
  }
  if (_library_map.count(aCompName) != 0)
  {
    MESSAGE("Python component already imported");
    ret = true;
  }

  // --- try dlopen C++ component
  if (!ret)
  {
    MESSAGE("Try to load C++ component");
    void* handle;
#ifndef WIN32
    handle = dlopen( impl_name.c_str() , RTLD_LAZY | RTLD_GLOBAL ) ;
#else
    handle = dlopen( impl_name.c_str() , 0 ) ;
#endif
    if ( handle )
    {
      _library_map[impl_name] = handle;
      MESSAGE("Library " << impl_name << " loaded");
      ret = true;
    }
    else
    {
      std::cerr << "Can't load shared library : " << impl_name << std::endl;
      std::cerr << "error of dlopen: " << dlerror() << std::endl;
    }
  }

  // --- try import Python component
  if (!ret)
  {
    MESSAGE("Try to import Python component "<<componentName);
    Py_ACQUIRE_NEW_THREAD;
    PyObject *mainmod = PyImport_AddModule("__main__");
    PyObject *globals = PyModule_GetDict(mainmod);
    PyObject *pyCont = PyDict_GetItemString(globals, "pyCont");
    PyObject *result = PyObject_CallMethod(pyCont,
                                           (char*)"import_component",
                                           (char*)"s",componentName);
    std::string ret_p= PyString_AsString(result);
    Py_XDECREF(result);
    Py_RELEASE_NEW_THREAD;

    if (ret_p=="") // import possible: Python component
    {
      _library_map[aCompName] = (void *)pyCont; // any non O value OK
      MESSAGE("import Python: " << aCompName <<" OK");
      ret = true;
    }
    else
    {
      std::cerr << "Error in importing Python component : " << aCompName << std::endl;
    }
  }

  _numInstanceMutex.unlock();
  return ret;
}

//=============================================================================
//! Create a new component instance
/*! 
 *  CORBA method: Creates a new servant instance of a component.
 *  The servant registers itself to naming service and Registry.
 *  \param genericRegisterName  Name of the component instance to register
 *                         in Registry & Name Service (without _inst_n suffix)
 *  \param studyId         0 for multiStudy instance, 
 *                         study Id (>0) otherwise
 *  \return a loaded component
 */
//=============================================================================
Engines::EngineComponent_ptr
Engines_Parallel_Container_i::create_component_instance(const char*genericRegisterName,
                                                        CORBA::Long studyId)
{
  Engines::FieldsDict_var env = new Engines::FieldsDict;
  char* reason;
  Engines::EngineComponent_ptr compo = create_component_instance_env(genericRegisterName,studyId,env, reason);
  CORBA::string_free(reason);
  return compo;
}

//=============================================================================
//! Create a new component instance
/*! 
 *  CORBA method: Creates a new servant instance of a component.
 *  The servant registers itself to naming service and Registry.
 *  \param genericRegisterName  Name of the component instance to register
 *                         in Registry & Name Service (without _inst_n suffix)
 *  \param studyId         0 for multiStudy instance, 
 *                         study Id (>0) otherwise
 *  \param env             dict of environment variables
 *  \return a loaded component
 */
//=============================================================================

Engines::EngineComponent_ptr
Engines_Parallel_Container_i::create_component_instance_env(const char*genericRegisterName,
                                                            CORBA::Long studyId,
                                                            const Engines::FieldsDict& env,
                                                            CORBA::String_out reason)
{
  MESSAGE("Begin of create_component_instance in node : " << getMyRank());
  reason=CORBA::string_dup("");

  if (studyId < 0)
  {
    INFOS("studyId must be > 0 for mono study instance, =0 for multiStudy");
    return Engines::EngineComponent::_nil() ;
  }

  std::string aCompName = genericRegisterName;
#ifndef WIN32
  std::string impl_name = string ("lib") + aCompName +string("Engine.so");
#else
  std::string impl_name = aCompName +string("Engine.dll");
#endif

  _numInstanceMutex.lock();
  _numInstance++;

  // Test if the component lib is loaded
  std::string type_of_lib("Not Loaded");
  void* handle = _library_map[impl_name];
  if (handle)
    type_of_lib = "cpp";
  if (_library_map.count(aCompName) != 0 and !handle)
    type_of_lib = "python";
  
  if (type_of_lib == "Not Loaded")
  {
    std::cerr << "Component library is not loaded or imported ! lib was : " << aCompName << std::endl;
    _numInstanceMutex.unlock();
    return Engines::EngineComponent::_nil();
  }

  Engines::EngineComponent_var iobject = Engines::EngineComponent::_nil();
  if (type_of_lib == "cpp")
    iobject = createCPPInstance(aCompName, handle, studyId);
  else
    iobject = createPythonInstance(aCompName, studyId);

  _numInstanceMutex.unlock();
  return iobject._retn();
}

//=============================================================================
//! Find an existing (in the container) component instance
/*! 
 *  CORBA method: Finds a servant instance of a component
 *  \param registeredName  Name of the component in Registry or Name Service,
 *                         without instance suffix number
 *  \param studyId         0 if instance is not associated to a study, 
 *                         >0 otherwise (== study id)
 *  \return the first instance found with same studyId
 */
//=============================================================================

Engines::EngineComponent_ptr Engines_Parallel_Container_i::find_component_instance( const char* registeredName,
                                                                              CORBA::Long studyId)
{
  Engines::EngineComponent_var anEngine = Engines::EngineComponent::_nil();
  std::map<std::string,Engines::EngineComponent_var>::iterator itm =_listInstances_map.begin();
  while (itm != _listInstances_map.end())
  {
    std::string instance = (*itm).first;
    SCRUTE(instance);
    if (instance.find(registeredName) == 0)
    {
      anEngine = (*itm).second;
      if (studyId == anEngine->getStudyId())
      {
        return anEngine._retn();
      }
    }
    itm++;
  }
  return anEngine._retn();  
}

char* Engines_Parallel_Container_i::create_python_service_instance(const char* CompName,
								   CORBA::String_out reason)
{
  // not implemented
  reason=CORBA::string_dup("");
  return CORBA::string_dup("");
}

//=============================================================================
//! Find or create a new component instance
/*! 
 *  CORBA method: find or create an instance of the component (servant),
 *  load a new component class (dynamic library) if required,
 *  ---- FOR COMPATIBILITY WITH 2.2 ---- 
 *  ---- USE ONLY FOR MULTISTUDY INSTANCES ! --------
 *  The servant registers itself to naming service and Registry.
 *  \param genericRegisterName  Name of the component to register
 *                              in Registry & Name Service
 *  \param componentName       Name of the constructed library of the component
 *  \return a loaded component
 */
//=============================================================================

Engines::EngineComponent_ptr Engines_Parallel_Container_i::load_impl( const char* genericRegisterName,
                                                                const char* componentName )
{
  Engines::EngineComponent_var iobject = Engines::EngineComponent::_nil();
  char* reason;
  if (load_component_Library(genericRegisterName,reason))
    iobject = find_or_create_instance(genericRegisterName);
  CORBA::string_free(reason);
  return iobject._retn();
}


//=============================================================================
//! Remove the component instance from container
/*! 
 *  CORBA method: Stops the component servant, and deletes all related objects
 *  \param component_i     Component to be removed
 */
//=============================================================================

void Engines_Parallel_Container_i::remove_impl(Engines::EngineComponent_ptr component_i)
{
  ASSERT(!CORBA::is_nil(component_i));
  std::string instanceName = component_i->instanceName();
  _numInstanceMutex.lock() ; // lock to be alone (stl container write)
  // Test if the component is in this container
  std::map<std::string, Engines::EngineComponent_var>::iterator itm;
  itm = _listInstances_map.find(instanceName);
  if (itm != _listInstances_map.end())
  {
    MESSAGE("Unloading component " << instanceName);
    _listInstances_map.erase(instanceName);
    component_i->destroy() ;
    _NS->Destroy_Name(instanceName.c_str());
  }
  else
    std::cerr << "WARNING !!!! component instance was not in this container !!!" << std::endl;
  _numInstanceMutex.unlock() ;
}

//=============================================================================
//! Unload component libraries from the container
/*! 
 *  CORBA method: Discharges unused libraries from the container.
 */
//=============================================================================

void Engines_Parallel_Container_i::finalize_removal()
{
  MESSAGE("Finalize removal : dlclose");
  MESSAGE("WARNING FINALIZE DOES CURRENTLY NOTHING !!!");

  // (see decInstanceCnt, load_component_Library)
  //std::map<std::string, void *>::iterator ith;
  //for (ith = _toRemove_map.begin(); ith != _toRemove_map.end(); ith++)
  //{
  //  void *handle = (*ith).second;
  //  std::string impl_name= (*ith).first;
  //  if (handle)
  //  {
  //    SCRUTE(handle);
  //    SCRUTE(impl_name);
  //    dlclose(handle);                // SALOME unstable after ...
  //    _library_map.erase(impl_name);
  //  }
  //}

  _numInstanceMutex.lock(); // lock to be alone
  _toRemove_map.clear();
  _numInstanceMutex.unlock();
}

//=============================================================================
//! Kill the container
/*! 
 *  CORBA method: Kill the container process with exit(0).
 *  To remove :  never returns !
 */
//=============================================================================

bool Engines_Parallel_Container_i::Kill_impl()
{
  MESSAGE("Engines_Parallel_Container_i::Kill() my pid is "<< _pid 
          << " my containerName is " << _containerName.c_str() 
          << " my machineName is " << _hostname.c_str());
  INFOS("===============================================================");
  INFOS("= REMOVE calls to Kill_impl in C++ container                  =");
  INFOS("===============================================================");
  //exit( 0 ) ;
  ASSERT(0);
  return false;
}

//=============================================================================
//! Get or create a file reference object associated to a local file (to transfer it)
/*! 
 *  CORBA method: get or create a fileRef object associated to a local file
 *  (a file on the computer on which runs the container server), which stores
 *  a list of (machine, localFileName) corresponding to copies already done.
 * 
 *  \param  origFileName absolute path for a local file to copy on other
 *          computers
 *  \return a fileRef object associated to the file.
 */
//=============================================================================

Engines::fileRef_ptr
Engines_Parallel_Container_i::createFileRef(const char* origFileName)
{
  std::string origName(origFileName);
  Engines::fileRef_var theFileRef = Engines::fileRef::_nil();

  if (origName[0] != '/')
  {
    INFOS("path of file to copy must be an absolute path begining with '/'");
    return Engines::fileRef::_nil();
  }

  if (CORBA::is_nil(_fileRef_map[origName]))
  {
    CORBA::Object_var obj=_poa->id_to_reference(*_id);
    Engines::Container_var pCont = Engines::Container::_narrow(obj);
    fileRef_i* aFileRef = new fileRef_i(pCont, origFileName);
    theFileRef = Engines::fileRef::_narrow(aFileRef->_this());
    _numInstanceMutex.lock() ; // lock to be alone (stl container write)
    _fileRef_map[origName] = theFileRef;
    _numInstanceMutex.unlock() ;
  }

  theFileRef =  Engines::fileRef::_duplicate(_fileRef_map[origName]);
  ASSERT(! CORBA::is_nil(theFileRef));
  return theFileRef._retn();
}

//=============================================================================
/*! 
 *  CORBA method:
 *  \return a reference to the fileTransfer object
 */
//=============================================================================

Engines::fileTransfer_ptr
Engines_Parallel_Container_i::getFileTransfer()
{
  Engines::fileTransfer_var aFileTransfer
    = Engines::fileTransfer::_duplicate(_fileTransfer);
  return aFileTransfer._retn();
}


Engines::Salome_file_ptr 
Engines_Parallel_Container_i::createSalome_file(const char* origFileName) 
{
  string origName(origFileName);
  if (CORBA::is_nil(_Salome_file_map[origName]))
  {
    Salome_file_i* aSalome_file = new Salome_file_i();
    try 
    {
      aSalome_file->setLocalFile(origFileName);
      aSalome_file->recvFiles();
    }
    catch (const SALOME::SALOME_Exception& e)
    {
      return Engines::Salome_file::_nil();
    }

    Engines::Salome_file_var theSalome_file = Engines::Salome_file::_nil();
    theSalome_file = Engines::Salome_file::_narrow(aSalome_file->_this());
    _numInstanceMutex.lock() ; // lock to be alone (stl container write)
    _Salome_file_map[origName] = theSalome_file;
    _numInstanceMutex.unlock() ;
  }

  Engines::Salome_file_ptr theSalome_file =  
    Engines::Salome_file::_duplicate(_Salome_file_map[origName]);
  ASSERT(!CORBA::is_nil(theSalome_file));
  return theSalome_file;
}


//=============================================================================
//! Finds an already existing component instance or create a new instance
/*! 
 *  C++ method: Finds an already existing servant instance of a component, or
 *              create an instance.
 *  ---- USE ONLY FOR MULTISTUDY INSTANCES ! --------
 *  \param genericRegisterName    Name of the component instance to register
 *                                in Registry & Name Service,
 *                                (without _inst_n suffix, like "COMPONENT")
 *  \return a loaded component
 * 
 *  example with names:
 *  aGenRegisterName = COMPONENT (= first argument)
 *  impl_name = libCOMPONENTEngine.so (= second argument)
 *  _containerName = /Containers/cli76ce/FactoryServer
 *  factoryName = COMPONENTEngine_factory
 *  component_registerBase = /Containers/cli76ce/FactoryServer/COMPONENT
 *
 *  instanceName = COMPONENT_inst_1
 *  component_registerName = /Containers/cli76ce/FactoryServer/COMPONENT_inst_1
 */
//=============================================================================

Engines::EngineComponent_ptr
Engines_Parallel_Container_i::find_or_create_instance(std::string genericRegisterName)
{
  Engines::EngineComponent_var iobject = Engines::EngineComponent::_nil();
  try
  {
    std::string aGenRegisterName = genericRegisterName;
    // --- find a registered instance in naming service, or create
    std::string component_registerBase = _containerName + "/" + aGenRegisterName;
    CORBA::Object_var obj = _NS->ResolveFirst(component_registerBase.c_str());
    if (CORBA::is_nil( obj ))
    {
      iobject = create_component_instance(genericRegisterName.c_str(), 
                                          0); // force multiStudy instance here !
    }
    else
    { 
      iobject = Engines::EngineComponent::_narrow(obj) ;
      Engines_Component_i *servant = dynamic_cast<Engines_Component_i*>(_poa->reference_to_servant(iobject));
      ASSERT(servant)
      int studyId = servant->getStudyId();
      ASSERT (studyId >= 0);
      if (studyId != 0)  // monoStudy instance: NOK
      {
        iobject = Engines::EngineComponent::_nil();
        INFOS("load_impl & find_component_instance methods "
              << "NOT SUITABLE for mono study components");
      }
    }
  }
  catch (...)
  {
    INFOS( "Container_i::load_impl catched" ) ;
  }
  return iobject._retn();
}

//=============================================================================
//! Create a new Python component instance 
/*! 
 *  C++ method: create a servant instance of a component.
 *  \param genericRegisterName    Name of the component instance to register
 *                                in Registry & Name Service,
 *                                (without _inst_n suffix, like "COMPONENT")
 *  \param handle                 loaded library handle
 *  \param studyId                0 for multiStudy instance, 
 *                                study Id (>0) otherwise
 *  \return a loaded component
 * 
 *  example with names:
 *  aGenRegisterName = COMPONENT (= first argument)
 *  _containerName = /Containers/cli76ce/FactoryServer
 *  factoryName = COMPONENTEngine_factory
 *  component_registerBase = /Containers/cli76ce/FactoryServer/COMPONENT
 *  instanceName = COMPONENT_inst_1
 *  component_registerName = /Containers/cli76ce/FactoryServer/COMPONENT_inst_1
 */
//=============================================================================
Engines::EngineComponent_ptr
Engines_Parallel_Container_i::createPythonInstance(std::string genericRegisterName, int studyId)
{

  Engines::EngineComponent_var iobject = Engines::EngineComponent::_nil();

  int numInstance = _numInstance;
  char aNumI[12];
  sprintf( aNumI , "%d" , numInstance ) ;
  std::string instanceName = genericRegisterName + "_inst_" + aNumI ;
  std::string component_registerName = _containerName + "/" + instanceName;

  Py_ACQUIRE_NEW_THREAD;
  PyObject *mainmod = PyImport_AddModule("__main__");
  PyObject *globals = PyModule_GetDict(mainmod);
  PyObject *pyCont = PyDict_GetItemString(globals, "pyCont");
  PyObject *result = PyObject_CallMethod(pyCont,
                                         (char*)"create_component_instance",
                                         (char*)"ssl",
                                         genericRegisterName.c_str(),
                                         instanceName.c_str(),
                                         studyId);
  const char *ior;
  const char *error;
  PyArg_ParseTuple(result,"ss", &ior, &error);
  string iors = ior;
  Py_DECREF(result);
  Py_RELEASE_NEW_THREAD;

  if( iors!="" )
  {
    CORBA::Object_var obj = _orb->string_to_object(iors.c_str());
    iobject = Engines::EngineComponent::_narrow(obj);
    _listInstances_map[instanceName] = iobject;
  }
  else
    std::cerr << "createPythonInstance ior is empty ! Error in creation" << std::endl;

  return iobject._retn();
}

//=============================================================================
//! Create a new CPP component instance 
/*! 
 *  C++ method: create a servant instance of a component.
 *  \param genericRegisterName    Name of the component instance to register
 *                                in Registry & Name Service,
 *                                (without _inst_n suffix, like "COMPONENT")
 *  \param handle                 loaded library handle
 *  \param studyId                0 for multiStudy instance, 
 *                                study Id (>0) otherwise
 *  \return a loaded component
 * 
 *  example with names:
 *  aGenRegisterName = COMPONENT (= first argument)
 *  _containerName = /Containers/cli76ce/FactoryServer
 *  factoryName = COMPONENTEngine_factory
 *  component_registerBase = /Containers/cli76ce/FactoryServer/COMPONENT
 *  instanceName = COMPONENT_inst_1
 *  component_registerName = /Containers/cli76ce/FactoryServer/COMPONENT_inst_1
 */
//=============================================================================
Engines::EngineComponent_ptr
Engines_Parallel_Container_i::createCPPInstance(std::string genericRegisterName,
                                                void *handle,
                                                int studyId)
{
  MESSAGE("Entering Engines_Parallel_Container_i::createCPPInstance");

  // --- find the factory

  std::string aGenRegisterName = genericRegisterName;
  std::string factory_name = aGenRegisterName + string("Engine_factory");

  typedef  PortableServer::ObjectId * (*FACTORY_FUNCTION_2)
    (CORBA::ORB_ptr,
     PortableServer::POA_ptr, 
     PortableServer::ObjectId *, 
     const char *, 
     const char *) ;

  FACTORY_FUNCTION_2 Component_factory = NULL;
#ifndef WIN32
  Component_factory = (FACTORY_FUNCTION_2)dlsym( handle, factory_name.c_str() );
#else
  Component_factory = (FACTORY_FUNCTION_2)GetProcAddress( (HINSTANCE)handle, factory_name.c_str() );
#endif

  if (!Component_factory)
  {
    INFOS("Can't resolve symbol: " + factory_name);
#ifndef WIN32
    INFOS("dlerror() result is : " << dlerror());
#endif
    return Engines::EngineComponent::_nil() ;
  }

  // --- create instance
  Engines::EngineComponent_var iobject = Engines::EngineComponent::_nil() ;
  try
  {
    int numInstance = _numInstance;
    char aNumI[12];
    sprintf( aNumI , "%d" , numInstance );
    std::string instanceName = aGenRegisterName + "_inst_" + aNumI;
    std::string component_registerName =
      _containerName + "/" + instanceName;

    // --- Instanciate required CORBA object

    PortableServer::ObjectId *id; //not owner, do not delete (nore use var)
    id = (Component_factory) ( _orb, _poa, _id, instanceName.c_str(),
                               aGenRegisterName.c_str() );
    if (id == NULL)
    {
      INFOS("Factory function returns NULL !");
      return iobject._retn();
    }

    // --- get reference & servant from id
    CORBA::Object_var obj = _poa->id_to_reference(*id);
    iobject = Engines::EngineComponent::_narrow(obj);

    Engines_Component_i *servant = 
      dynamic_cast<Engines_Component_i*>(_poa->reference_to_servant(iobject));
    ASSERT(servant);
    servant->_remove_ref(); // compensate previous id_to_reference 
    _listInstances_map[instanceName] = iobject;
    _cntInstances_map[aGenRegisterName] += 1;
#if defined(_DEBUG_) || defined(_DEBUG)
    bool ret_studyId = servant->setStudyId(studyId);
    ASSERT(ret_studyId);
#else
    servant->setStudyId(studyId);
#endif

    // --- register the engine under the name
    //     containerName(.dir)/instanceName(.object)
    _NS->Register(iobject , component_registerName.c_str());
    MESSAGE( component_registerName.c_str() << " bound" );
  }
  catch (...)
  {
    INFOS( "Container_i::createInstance exception catched" );
  }
  return iobject._retn();
}

void
Engines_Parallel_Container_i::create_paco_component_node_instance(const char* componentName,
                                                                  const char* proxy_containerName,
                                                                  CORBA::Long studyId)
{
  // Init de la méthode
  char * proxy_ior;
  Engines::EngineComponent_PaCO_var work_node;
  std::string aCompName = componentName;
  std::string _proxy_containerName = proxy_containerName;

#ifndef WIN32
  string impl_name = string ("lib") + aCompName +string("Engine.so");
#else
  string impl_name = aCompName +string("Engine.dll");
#endif
  void* handle = _library_map[impl_name];
  _numInstanceMutex.lock() ; // lock on the instance number
  _numInstance++ ;
  int numInstance = _numInstance ;
  _numInstanceMutex.unlock() ;
  char aNumI[12];
  sprintf( aNumI , "%d" , numInstance ) ;
  string instanceName = aCompName + "_inst_" + aNumI ;

  // Step 1 : Get proxy !
  string component_registerName = _proxy_containerName + "/" + instanceName;
  CORBA::Object_var temp = _NS->Resolve(component_registerName.c_str());
  Engines::EngineComponent_var obj_proxy = Engines::EngineComponent::_narrow(temp);
  if (CORBA::is_nil(obj_proxy))
  {
    INFOS("Proxy reference from NamingService is nil !");
    INFOS("Proxy name was : " << component_registerName);
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    es.text = "Proxy reference from NamingService is nil !";
    throw SALOME::SALOME_Exception(es);
  }
  proxy_ior = _orb->object_to_string(obj_proxy);

  // Get factory
  string factory_name = aCompName + string("Engine_factory");
  FACTORY_FUNCTION Component_factory = (FACTORY_FUNCTION) dlsym(handle, factory_name.c_str());
  if (!Component_factory)
  {
    INFOS("Can't resolve symbol : " + factory_name);
#ifndef WIN32
    INFOS("dlerror() result is : " << dlerror());
#endif
    std::string ex_text = "Can't resolve symbol : " + factory_name;
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    es.text = CORBA::string_dup(ex_text.c_str());
    throw SALOME::SALOME_Exception(es);
  }

  try
  {
    char aNumI2[12];
    sprintf(aNumI2 , "%d" , getMyRank()) ;
    std::string instanceName = aCompName + "_inst_" + aNumI + "_work_node_" + aNumI2;
    std::string component_registerName = _containerName + "/" + instanceName;

    // --- Instanciate work node
    PortableServer::ObjectId *id ; //not owner, do not delete (nore use var)
    id = (Component_factory) (_orb, proxy_ior, getMyRank(), _poa, _id, instanceName.c_str(), componentName);
    CORBA::string_free(proxy_ior);

    // --- get reference & servant from id
    CORBA::Object_var obj = _poa->id_to_reference(*id);
    work_node = Engines::EngineComponent_PaCO::_narrow(obj) ;
    if (CORBA::is_nil(work_node))
    {
      INFOS("work_node reference from factory is nil !");
      SALOME::ExceptionStruct es;
      es.type = SALOME::INTERNAL_ERROR;
      es.text = "work_node reference from factory is nil !";
      throw SALOME::SALOME_Exception(es);
    }
    work_node->deploy();
    _NS->Register(work_node, component_registerName.c_str());
    _par_obj_inst_map[instanceName] = id;
    MESSAGE(component_registerName.c_str() << " bound" );
  }
  catch (...)
  {
    INFOS("Container_i::create_paco_component_node_instance exception catched");
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    es.text = "Container_i::create_paco_component_node_instance exception catched";
    throw SALOME::SALOME_Exception(es);
  }
}

//=============================================================================
//! Decrement component instance reference count
/*! 
 *
 */
//=============================================================================

void Engines_Parallel_Container_i::decInstanceCnt(std::string genericRegisterName)
{
  if(_cntInstances_map.count(genericRegisterName) !=0 )
  {
    std::string aGenRegisterName =genericRegisterName;
    MESSAGE("Engines_Parallel_Container_i::decInstanceCnt " << aGenRegisterName);
    ASSERT(_cntInstances_map[aGenRegisterName] > 0); 
    _numInstanceMutex.lock(); // lock to be alone
    // (see finalize_removal, load_component_Library)
    _cntInstances_map[aGenRegisterName] -= 1;
    SCRUTE(_cntInstances_map[aGenRegisterName]);
    if (_cntInstances_map[aGenRegisterName] == 0)
    {
      std::string impl_name =
        Engines_Component_i::GetDynLibraryName(aGenRegisterName.c_str());
      SCRUTE(impl_name);
      void* handle = _library_map[impl_name];
      ASSERT(handle);
      _toRemove_map[impl_name] = handle;
    }
    _numInstanceMutex.unlock();
  }
}

//=============================================================================
//! Indicate if container is a python one
/*! 
 *  Retrieves only with container naming convention if it is a python container
 */
//=============================================================================

bool Engines_Parallel_Container_i::isPythonContainer(const char* ContainerName)
{
  bool ret=false;
  return ret;
}


// Cette méthode permet de tenir à jour le compteur des
// instances pour le container parallèle.
// En effet losrque l'on charge un composant séquentielle seul
// le compteur du noeud 0 est augmenté, il faut donc tenir les autres 
// noeuds à jour.
void
Engines_Parallel_Container_i::updateInstanceNumber()
{
  if (getMyRank() != 0)
  {
    _numInstanceMutex.lock();
    _numInstance++;
    _numInstanceMutex.unlock();
  }
}

/*! \brief copy a file from a remote host (container) to the local host
 * \param container the remote container
 * \param remoteFile the file to copy locally from the remote host into localFile
 * \param localFile the local file
 */
void 
Engines_Parallel_Container_i::copyFile(Engines::Container_ptr container, const char* remoteFile, const char* localFile)
{
  Engines::fileTransfer_var fileTransfer = container->getFileTransfer();

  FILE* fp;
  if ((fp = fopen(localFile,"wb")) == NULL)
    {
      INFOS("file " << localFile << " cannot be open for writing");
      return;
    }

  CORBA::Long fileId = fileTransfer->open(remoteFile);
  if (fileId > 0)
    {
      Engines::fileBlock* aBlock;
      int toFollow = 1;
      int ctr=0;
      while (toFollow)
        {
          ctr++;
          SCRUTE(ctr);
          aBlock = fileTransfer->getBlock(fileId);
          toFollow = aBlock->length();
          SCRUTE(toFollow);
          CORBA::Octet *buf = aBlock->get_buffer();
          fwrite(buf, sizeof(CORBA::Octet), toFollow, fp);
          delete aBlock;
        }
      fclose(fp);
      MESSAGE("end of transfer");
      fileTransfer->close(fileId);
    }
  else
    {
      INFOS("open reference file for copy impossible");
    }
}

/*! \brief create a PyNode object to execute remote python code
 * \param nodeName the name of the node
 * \param code the python code to load
 * \return the PyNode
 */
Engines::PyNode_ptr 
Engines_Parallel_Container_i::createPyNode(const char* nodeName, const char* code)
{
  INFOS("Python component not yet implemented");
  Engines::PyNode_var node= Engines::PyNode::_nil();
  return node._retn();
}

Engines::PyNode_ptr Engines_Parallel_Container_i::getDefaultPyNode(const char *nodeName)
{
  INFOS("Python component not yet implemented");
  return Engines::PyNode::_nil();
}

Engines::PyScriptNode_ptr 
Engines_Parallel_Container_i::createPyScriptNode(const char* nodeName, const char* cod)
{
  INFOS("Python script node not yet implemented");
  Engines::PyScriptNode_var node= Engines::PyScriptNode::_nil();
  return node._retn();
}

Engines::PyScriptNode_ptr Engines_Parallel_Container_i::getDefaultPyScriptNode(const char *nodeName)
{
  INFOS("Python script node not yet implemented");
  return Engines::PyScriptNode::_nil();
}

//=============================================================================
/*! 
 *  
 */
//=============================================================================

void ActSigIntHandler()
{
#ifndef WIN32
  struct sigaction SigIntAct ;
  SigIntAct.sa_sigaction = &SigIntHandler ;
  SigIntAct.sa_flags = SA_SIGINFO ;
#endif

  // DEBUG 03.02.2005 : the first parameter of sigaction is not a mask of signals
  // (SIGINT | SIGUSR1) :
  // it must be only one signal ===> one call for SIGINT 
  // and an other one for SIGUSR1

#ifndef WIN32
  if ( sigaction( SIGINT , &SigIntAct, NULL ) ) {
    perror("SALOME_Container main ") ;
    exit(0) ;
  }
  if ( sigaction( SIGUSR1 , &SigIntAct, NULL ) ) {
    perror("SALOME_Container main ") ;
    exit(0) ;
  }
  if ( sigaction( SIGUSR2 , &SigIntAct, NULL ) )
  {
    perror("SALOME_Container main ") ;
    exit(0) ;
  }

  //PAL9042 JR : during the execution of a Signal Handler (and of methods called through Signal Handlers)
  //             use of streams (and so on) should never be used because :
  //             streams of C++ are naturally thread-safe and use pthread_mutex_lock ===>
  //             A stream operation may be interrupted by a signal and if the Handler use stream we
  //             may have a "Dead-Lock" ===HangUp
  //==INFOS is commented
  //  INFOS(pthread_self() << "SigIntHandler activated") ;
#else  
  signal( SIGINT, SigIntHandler );
  signal( SIGUSR1, SigIntHandler );
#endif

}

void SetCpuUsed();
void CallCancelThread();

#ifndef WIN32
void SigIntHandler(int what , siginfo_t * siginfo ,
                   void * toto ) {
  //PAL9042 JR : during the execution of a Signal Handler (and of methods called through Signal Handlers)
  //             use of streams (and so on) should never be used because :
  //             streams of C++ are naturally thread-safe and use pthread_mutex_lock ===>
  //             A stream operation may be interrupted by a signal and if the Handler use stream we
  //             may have a "Dead-Lock" ===HangUp
  //==MESSAGE is commented
  //  MESSAGE(pthread_self() << "SigIntHandler what     " << what << endl
  //          << "              si_signo " << siginfo->si_signo << endl
  //          << "              si_code  " << siginfo->si_code << endl
  //          << "              si_pid   " << siginfo->si_pid) ;
  if ( _Sleeping ) {
    _Sleeping = false ;
    //     MESSAGE("SigIntHandler END sleeping.") ;
    return ;
  }
  else {
    ActSigIntHandler() ;
    if ( siginfo->si_signo == SIGUSR1 ) {
      SetCpuUsed() ;
    }
    else if ( siginfo->si_signo == SIGUSR2 )
    {
      CallCancelThread() ;
    }
    else {
      _Sleeping = true ;
      //      MESSAGE("SigIntHandler BEGIN sleeping.") ;
      int count = 0 ;
      while( _Sleeping ) {
        sleep( 1 ) ;
        count += 1 ;
      }
      //      MESSAGE("SigIntHandler LEAVE sleeping after " << count << " s.") ;
    }
    return ;
  }
}
#else // Case WIN32
void SigIntHandler( int what ) {
  MESSAGE( pthread_self() << "SigIntHandler what     " << what << endl );
  if ( _Sleeping ) {
    _Sleeping = false ;
    MESSAGE("SigIntHandler END sleeping.") ;
    return ;
  }
  else {
    ActSigIntHandler() ;
    if ( what == SIGUSR1 ) {
      SetCpuUsed() ;
    }
    else {
      _Sleeping = true ;
      MESSAGE("SigIntHandler BEGIN sleeping.") ;
      int count = 0 ;
      while( _Sleeping ) {
        Sleep( 1000 ) ;
        count += 1 ;
      }
      MESSAGE("SigIntHandler LEAVE sleeping after " << count << " s.") ;
    }
    return ;
  }
}
#endif
