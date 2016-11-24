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

//  SALOME Container : implementation of container and engine for Kernel
//  File   : Container_i.cxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA 
//  Module : SALOME
//  $Header$
//#define private public
//
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#ifndef WIN32
#include <sys/time.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/wait.h>
#else
#include <signal.h>
#include <process.h>
#include <direct.h>
int SIGUSR1 = 1000;
#endif

#include "utilities.h"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)
#include CORBA_SERVER_HEADER(SALOME_Exception)
#include <pthread.h>  // must be before Python.h !
#include "SALOME_Container_i.hxx"
#include "SALOME_Component_i.hxx"
#include "SALOME_FileRef_i.hxx"
#include "SALOME_FileTransfer_i.hxx"
#include "Salome_file_i.hxx"
#include "SALOME_NamingService.hxx"
#include "Basics_Utils.hxx"

#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#endif

#include <Python.h>
#include "Container_init_python.hxx"

bool _Sleeping = false ;

// // Needed by multi-threaded Python --- Supervision
int _ArgC ;
char ** _ArgV ;

extern "C" {void ActSigIntHandler() ; }
#ifndef WIN32
extern "C" {void SigIntHandler(int, siginfo_t *, void *) ; }
#else
extern "C" {void SigIntHandler( int ) ; }
#endif

#ifndef WIN32
#define LIB "lib"
#define ENGINESO "Engine.so"
#else
#define LIB ""
#define ENGINESO "Engine.dll"
#endif

#ifdef WIN32
#define SEP ';'
#define SLASH '\\'
#else
#define SEP ':'
#define SLASH '/'
#endif

std::map<std::string, int> Engines_Container_i::_cntInstances_map;
std::map<std::string, void *> Engines_Container_i::_library_map;
std::map<std::string, void *> Engines_Container_i::_toRemove_map;
omni_mutex Engines_Container_i::_numInstanceMutex ;

static PyObject* _pyCont;

int checkifexecutable(const std::string&);
int findpathof(const std::string& path, std::string&, const std::string&);

/*! \class Engines_Container_i
 *  \brief C++ implementation of Engines::Container interface
 *
 */


//=============================================================================
/*! 
*  Default constructor, not for use
*/
//=============================================================================

Engines_Container_i::Engines_Container_i () :
_numInstance(0),_id(0),_NS(0)
{
}

//=============================================================================
/*! 
*  Construtor to use
*/
//=============================================================================

Engines_Container_i::Engines_Container_i (CORBA::ORB_ptr orb, 
                                          PortableServer::POA_ptr poa,
                                          char *containerName ,
                                          int argc , char* argv[],
                                          bool activAndRegist,
                                          bool isServantAloneInProcess
                                          ) :
  _numInstance(0),_isServantAloneInProcess(isServantAloneInProcess),_id(0),_NS(0)
{
  _pid = (long)getpid();

  if(activAndRegist)
    ActSigIntHandler() ;

  _argc = argc ;
  _argv = argv ;

  std::string hostname = Kernel_Utils::GetHostname();
#ifndef WIN32
  MESSAGE(hostname << " " << getpid() << 
    " Engines_Container_i starting argc " <<
    _argc << " Thread " << pthread_self() ) ;
#else
  MESSAGE(hostname << " " << _getpid() << 
    " Engines_Container_i starting argc " << _argc<< " Thread " << pthread_self().p ) ;
#endif

  int i = 0 ;
  while ( _argv[ i ] )
  {
    MESSAGE("           argv" << i << " " << _argv[ i ]) ;
    i++ ;
  }

  if ( argc < 2 )
  {
    INFOS("SALOME_Container usage : SALOME_Container ServerName");
    ASSERT(0) ;
  }
  SCRUTE(argv[1]);
  _isSupervContainer = false;

  _orb = CORBA::ORB::_duplicate(orb) ;
  _poa = PortableServer::POA::_duplicate(poa) ;

  // Pour les containers paralleles: il ne faut pas enregistrer et activer
  // le container generique, mais le container specialise

  if(activAndRegist)
  {
    _id = _poa->activate_object(this);
    _NS = new SALOME_NamingService();
    _NS->init_orb( _orb ) ;
    CORBA::Object_var obj=_poa->id_to_reference(*_id);
    Engines::Container_var pCont 
      = Engines::Container::_narrow(obj);
    _remove_ref();

    _containerName = _NS->BuildContainerNameForNS(containerName,
      hostname.c_str());
    SCRUTE(_containerName);
    _NS->Register(pCont, _containerName.c_str());
    MESSAGE("Engines_Container_i::Engines_Container_i : Container name "
      << _containerName);

    // Python: 
    // import SALOME_Container
    // pycont = SALOME_Container.SALOME_Container_i(containerIORStr)

    CORBA::String_var sior =  _orb->object_to_string(pCont);
    std::string myCommand="pyCont = SALOME_Container.SALOME_Container_i('";
    myCommand += _containerName + "','";
    myCommand += sior;
    myCommand += "')\n";
    SCRUTE(myCommand);

    //[RNV]: Comment the PyEval_AcquireLock() and PyEval_ReleaseLock() because this 
    //approach leads to the deadlock of the main thread of the application on Windows platform
    //in case if cppContainer runs in the standalone mode. The problem with the PyThreadState 
    //described by ABN seems not reproduced, to be checked carefully later...
    PyGILState_STATE gstate = PyGILState_Ensure();
    
    //// [ABN]: using the PyGILState* API here is unstable. omniORB logic is invoked
    //// by the Python code executed below, and in some (random) cases, the Python code
    //// execution ends with a PyThreadState which was not the one we have here.
    //// (TODO: understand why ...)
    //// To be on the safe side we get and load the thread state ourselves:    
    //PyEval_AcquireLock();  // get GIL
    //PyThreadState * mainThreadState = PyThreadState_Get();
    //PyThreadState_Swap(mainThreadState);

#ifdef WIN32
    // mpv: this is temporary solution: there is a unregular crash if not
    //Sleep(2000);
    //
    // first element is the path to Registry.dll, but it's wrong
    PyRun_SimpleString("import sys\n");
    PyRun_SimpleString("sys.path = sys.path[1:]\n");
#endif
    PyRun_SimpleString("import SALOME_Container\n");
    PyRun_SimpleString((char*)myCommand.c_str());
    PyObject *mainmod = PyImport_AddModule("__main__");
    PyObject *globals = PyModule_GetDict(mainmod);
    _pyCont = PyDict_GetItemString(globals, "pyCont");

    //PyThreadState_Swap(NULL);
    //PyEval_ReleaseLock();
    PyGILState_Release(gstate);

    fileTransfer_i* aFileTransfer = new fileTransfer_i();
    CORBA::Object_var obref=aFileTransfer->_this();
    _fileTransfer = Engines::fileTransfer::_narrow(obref);
    aFileTransfer->_remove_ref();
  }
}

//=============================================================================
/*! 
*  Destructor
*/
//=============================================================================

Engines_Container_i::~Engines_Container_i()
{
  MESSAGE("Container_i::~Container_i()");
  if(_id)
    delete _id;
  if(_NS)
    delete _NS;
  for(std::map<std::string,Engines::PyNode_var>::iterator it=_dftPyNode.begin();it!=_dftPyNode.end();it++)
    {
      Engines::PyNode_var tmpVar((*it).second);
      if(!CORBA::is_nil(tmpVar))
        tmpVar->UnRegister();
    }
  for(std::map<std::string,Engines::PyScriptNode_var>::iterator it=_dftPyScriptNode.begin();it!=_dftPyScriptNode.end();it++)
    {
      Engines::PyScriptNode_var tmpVar((*it).second);
      if(!CORBA::is_nil(tmpVar))
        tmpVar->UnRegister();
    }
}

//=============================================================================
//! Get container name
/*! 
*  CORBA attribute: Container name (see constructor)
*/
//=============================================================================

char* Engines_Container_i::name()
{
  return CORBA::string_dup(_containerName.c_str()) ;
}

//=============================================================================
//! Get container working directory
/*! 
*  CORBA attribute: Container working directory 
*/
//=============================================================================

char* Engines_Container_i::workingdir()
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

char* Engines_Container_i::logfilename()
{
  return CORBA::string_dup(_logfilename.c_str()) ;
}

//! Set container log file name
void Engines_Container_i::logfilename(const char* name)
{
  _logfilename=name;
}

//=============================================================================
//! Get container host name
/*! 
*  CORBA method: Get the hostName of the Container (without domain extensions)
*/
//=============================================================================

char* Engines_Container_i::getHostName()
{
  std::string s = Kernel_Utils::GetHostname();
  //  MESSAGE("Engines_Container_i::getHostName " << s);
  return CORBA::string_dup(s.c_str()) ;
}

//=============================================================================
//! Get container PID
/*! 
*  CORBA method: Get the PID (process identification) of the Container
*/
//=============================================================================

CORBA::Long Engines_Container_i::getPID()
{
  return (CORBA::Long)getpid();
}

//=============================================================================
//! Ping the servant to check it is still alive
/*! 
*  CORBA method: check if servant is still alive
*/
//=============================================================================
void Engines_Container_i::ping()
{
  MESSAGE("Engines_Container_i::ping() pid "<< getpid());
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
void Engines_Container_i::Shutdown()
{
  MESSAGE("Engines_Container_i::Shutdown()");

  // Clear registered temporary files
  clearTemporaryFiles();

  /* For each component contained in this container
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
  _listInstances_map.clear();

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
//! load a component implementation
/*! 
*  CORBA method
*  \param componentName         component name
*  \param reason                explains error when load fails
*  \return true if dlopen successfull or already done, false otherwise
*/
//=============================================================================
bool
Engines_Container_i::load_component_Library(const char* componentName, CORBA::String_out reason)
{

  //=================================================================
  // --- C++ implementation section 
  //=================================================================
  std::string retso;
  if(load_component_CppImplementation(componentName,retso))
    {
      reason=CORBA::string_dup("");
      return true;
    }
  else if(retso != "ImplementationNotFound")
    {
      reason=CORBA::string_dup(retso.c_str());
      return false;
    }

  retso="Component ";
  retso+=componentName;
  retso+=": Can't find C++ implementation ";
  retso+=std::string(LIB) + componentName + ENGINESO;

  //=================================================================
  // --- Python implementation section 
  //=================================================================
  std::string retpy;
  if(load_component_PythonImplementation(componentName,retpy))
    {
      reason=CORBA::string_dup("");
      return true;
    }
  else if(retpy != "ImplementationNotFound")
    {
      reason=CORBA::string_dup(retpy.c_str());
      return false;
    }
  
  retpy="Component ";
  retpy+=componentName;
  retpy+=": Can't find python implementation ";
  retpy+=componentName;
  retpy+="(.py)";

  //=================================================================
  // -- Executable implementation section
  //=================================================================
  std::string retex;
  if(load_component_ExecutableImplementation(componentName,retex))
    {
      reason=CORBA::string_dup("");
      return true;
    }
  else if(retex != "ImplementationNotFound")
    {
      reason=CORBA::string_dup(retex.c_str());
      return false;
    }

  retex="Component ";
  retex+=componentName;
  retex+=": Can't find executable implementation ";
  retex+=componentName;
  retex+=".exe";

  std::string ret="Component implementation not found: ";
  ret += componentName ;
  ret += '\n' ;
  ret += retso+ '\n' ;
  ret += retpy+ '\n' ;
  ret += retex+ '\n' ;

  std::cerr << ret << std::endl;
  reason=CORBA::string_dup(ret.c_str());

  return false;
}

//=============================================================================
//! try to load a C++ component implementation
/*! 
*  C++ method: 
*  \param componentName      the name of the component (COMPONENT, for example)
*  \param reason             explains error when load fails
*  \return true if loading is successfull or already done, false otherwise
*/
//=============================================================================
bool
Engines_Container_i::load_component_CppImplementation(const char* componentName, std::string& reason)
{
  std::string aCompName(componentName);
  std::string impl_name = std::string(LIB) + aCompName + ENGINESO;
  SCRUTE(impl_name);

  _numInstanceMutex.lock(); // lock to be alone
  // (see decInstanceCnt, finalize_removal))
  if (_toRemove_map.count(impl_name) != 0) _toRemove_map.erase(impl_name);
  if (_library_map.count(impl_name) != 0)
    {
      MESSAGE("Library " << impl_name << " already loaded");
      _numInstanceMutex.unlock();
      reason="";
      return true;
    }
  _numInstanceMutex.unlock();

#ifndef WIN32
  void* handle;
  handle = dlopen( impl_name.c_str() , RTLD_NOW | RTLD_GLOBAL ) ;
  if ( !handle )
    {
      //not loadable. Try to find the lib file in LD_LIBRARY_PATH
      std::string path;
      char* p=getenv("LD_LIBRARY_PATH");
      if(p)path=p;
      path=path+SEP+"/usr/lib"+SEP+"/lib";

      std::string pth;
      if(findpathof(path, pth, impl_name))
        {
          //found but not loadable
          reason="Component ";
          reason+=aCompName;
          reason+=": C++ implementation found ";
          reason+=pth;
          reason+=" but it is not loadable. Error:\n";
          reason+=dlerror();
          std::cerr << reason << std::endl;
          return false;
        }
      else
        {
          //not found
          //continue with other implementation
          reason="ImplementationNotFound";
          return false;
        }
    }
#else
  HINSTANCE handle;
  handle = LoadLibrary( impl_name.c_str() );
  if ( !handle )
    {
      reason="ImplementationNotFound";
    }
#endif

  if ( handle )
    {
      _numInstanceMutex.lock();
      _library_map[impl_name] = handle;
      _numInstanceMutex.unlock();
      reason="";
      return true;
    }

  return false;

}
//=============================================================================
//! try to load a Python component implementation
/*! 
*  C++ method: 
*  \param componentName         name of the component
*  \param reason                explains error when load fails
*  \return true if loading is successfull or already done, false otherwise
*/
//=============================================================================
bool
Engines_Container_i::load_component_PythonImplementation(const char* componentName, std::string& reason)
{
  std::string aCompName(componentName);

  _numInstanceMutex.lock() ; // lock to be alone (stl container write)
  if (_library_map.count(aCompName) != 0)
    {
      _numInstanceMutex.unlock() ;
      reason="";
      return true; // Python Component, already imported
    }
  _numInstanceMutex.unlock() ;

  PyGILState_STATE gstate = PyGILState_Ensure();
  PyObject *result = PyObject_CallMethod(_pyCont,
                                         (char*)"import_component",
                                         (char*)"s",componentName);

  reason=PyString_AsString(result);
  Py_XDECREF(result);
  SCRUTE(reason);
  PyGILState_Release(gstate);

  if (reason=="")
    {
      //Python component has been loaded (import componentName)
      _numInstanceMutex.lock() ; // lock to be alone (stl container write)
      _library_map[aCompName] = (void *)_pyCont; // any non O value OK
      _numInstanceMutex.unlock() ;
      MESSAGE("import Python: "<< aCompName <<" OK");
      return true;
    }
  else if(reason=="ImplementationNotFound")
    {
      //Python implementation has not been found. Continue with other implementation
      reason="ImplementationNotFound";
    }
  else
    {
      //Python implementation has been found but loading has failed
      std::cerr << reason << std::endl;
    }
  return false;

}
//=============================================================================
//! try to load a Executable component implementation
/*! 
*  C++ method: 
*  \param componentName        name of the component
*  \param reason               explains error when load fails
*  \return true if loading is successfull or already done, false otherwise
*/
//=============================================================================
bool
Engines_Container_i::load_component_ExecutableImplementation(const char* componentName, std::string& reason)
{
  std::string aCompName(componentName);
  std::string executable=aCompName+".exe";

  std::string path;
  std::string pth;

  char* p=getenv("PATH");
  if(p)path=p;

  if (findpathof(path, pth, executable))
    {
      if(checkifexecutable(pth))
        {
          _numInstanceMutex.lock() ; // lock to be alone (stl container write)
          _library_map[executable] = (void *)1; // any non O value OK
          _numInstanceMutex.unlock() ;
          MESSAGE("import executable: "<< pth <<" OK");
          reason="";
          return true;
        }
      reason="Component ";
      reason+=aCompName;
      reason+=": implementation found ";
      reason+=pth;
      reason+=" but it is not executable";
      std::cerr << reason << std::endl;
    }
  else
    {
      reason="ImplementationNotFound";
    }
  return false;
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
Engines_Container_i::create_component_instance(const char*genericRegisterName,
                                               CORBA::Long studyId)
{
  Engines::FieldsDict_var env = new Engines::FieldsDict;
  char* reason;
  Engines::EngineComponent_ptr compo =
    create_component_instance_env(genericRegisterName, studyId, env, reason);
  CORBA::string_free(reason);
  return compo;
}

//=============================================================================
//! Create a new component instance with environment variables specified
/*! 
*  CORBA method: Creates a new servant instance of a component.
*  The servant registers itself to naming service and Registry.
*  \param genericRegisterName  Name of the component instance to register
*                         in Registry & Name Service (without _inst_n suffix)
*  \param studyId         0 for multiStudy instance, 
*                         study Id (>0) otherwise
*  \param env             dict of env variables
*  \param reason          explains error when create_component_instance_env fails
*  \return a loaded component
*/
//=============================================================================
Engines::EngineComponent_ptr
Engines_Container_i::create_component_instance_env(const char*genericRegisterName,
                                                   CORBA::Long studyId,
                                                   const Engines::FieldsDict& env,
                                                   CORBA::String_out reason)
{
  if (studyId < 0)
    {
      INFOS("studyId must be > 0 for mono study instance, =0 for multiStudy");
      reason=CORBA::string_dup("studyId must be > 0 for mono study instance, =0 for multiStudy");
      return Engines::EngineComponent::_nil() ;
    }

  std::string error;
  if (_library_map.count(genericRegisterName) != 0)
    {
      // It's a Python component
      Engines::EngineComponent_ptr compo = createPythonInstance(genericRegisterName, studyId, error);
      reason=CORBA::string_dup(error.c_str());
      return compo;
    }

  std::string impl_name = std::string(LIB) + genericRegisterName + ENGINESO;
  if (_library_map.count(impl_name) != 0)
    {
      // It's a C++ component
      void* handle = _library_map[impl_name];
      Engines::EngineComponent_ptr compo = createInstance(genericRegisterName, handle, studyId, error);
      reason=CORBA::string_dup(error.c_str());
      return compo;
    }

  impl_name = std::string(genericRegisterName) + ".exe";
  if (_library_map.count(impl_name) != 0)
    {
      //It's an executable component
      Engines::EngineComponent_ptr compo = createExecutableInstance(genericRegisterName, studyId, env, error);
      reason=CORBA::string_dup(error.c_str());
      return compo;
    }

  error="load_component_Library has probably not been called for component: ";
  error += genericRegisterName;
  INFOS(error);
  reason=CORBA::string_dup(error.c_str());
  return Engines::EngineComponent::_nil() ;
}

//=============================================================================
//! Create a new component instance (Executable implementation)
/*! 
*  \param CompName               Name of the component instance
*  \param studyId                0 for multiStudy instance, 
*                                study Id (>0) otherwise
*  \param env                    dict of env variables
*  \param reason                 explains error when creation fails
*  \return a loaded component
*
*   This component is implemented in an executable with name genericRegisterName.exe
*   It must register itself in Naming Service. The container waits some time (10 s max)
*   it's registration.
*/
//=============================================================================
Engines::EngineComponent_ptr
Engines_Container_i::createExecutableInstance(std::string CompName, int studyId,
                                              const Engines::FieldsDict& env,
                                              std::string& reason)
{
  Engines::EngineComponent_var iobject = Engines::EngineComponent::_nil() ;

  _numInstanceMutex.lock() ; // lock on the instance number
  _numInstance++ ;
  int numInstance = _numInstance ;
  _numInstanceMutex.unlock() ;

  char aNumI[12];
  sprintf( aNumI , "%d" , numInstance ) ;
  std::string instanceName = CompName + "_inst_" + aNumI ;
  std::string component_registerName = _containerName + "/" + instanceName;

  //check if an entry exist in naming service
  CORBA::Object_var nsobj = _NS->Resolve(component_registerName.c_str());
  if ( !CORBA::is_nil(nsobj) )
  {
    // unregister the registered component
    _NS->Destroy_Name(component_registerName.c_str());
    //kill or shutdown it ???
  }

  // first arg container ior string
  // second arg container name
  // third arg instance name

  Engines::Container_var pCont= _this();
  CORBA::String_var sior =  _orb->object_to_string(pCont);

  std::string command;
  command="mkdir -p ";
  command+=instanceName;
  command+=";cd ";
  command+=instanceName;
  command+=";";
  command+=CompName ;
  command+=".exe";
  command+=" ";
  command+= sior; // container ior string
  command+=" ";
  command+=_containerName; //container name
  command+=" ";
  command+=instanceName; //instance name
  command+=" &";
  MESSAGE("SALOME_Container::create_component_instance command=" << command);

#ifndef WIN32
  // use fork/execl instead of system to get finer control on env variables
  int status;
  pid_t pid = fork();
  if(pid == 0) // child
    {
      for (CORBA::ULong i=0; i < env.length(); i++)
        {
          if (env[i].value.type()->kind() == CORBA::tk_string)
            {
              const char* value;
              env[i].value >>= value;
              std::string s(env[i].key);
              s+='=';
              s+=value;
              putenv(strdup(s.c_str()));
            }
        }

      execl("/bin/sh", "sh", "-c", command.c_str() , (char *)0);
      status=-1;
    }
  else if(pid < 0)       // failed to fork
    {
      status=-1;
    }
  else            //parent
    {
      pid_t tpid;
      do
        {
          tpid = wait(&status);
        } while (tpid != pid);
    }
#else
  // launch component with a system call
  int status=system(command.c_str());
#endif

  if (status == -1)
  {
    reason="SALOME_Container::create_component_instance system failed (system command status -1)";
    MESSAGE(reason);
    return Engines::EngineComponent::_nil();
  }
#ifndef WIN32
  else if (WEXITSTATUS(status) == 217)
  {
    reason="SALOME_Container::create_component_instance system failed (system command status 217)";
    MESSAGE(reason);
    return Engines::EngineComponent::_nil();
  }
#endif
  else
  {
    int count=20;
    if (getenv("TIMEOUT_TO_WAIT_EXE_COMPONENT") != 0)
    {
      std::string new_count_str = getenv("TIMEOUT_TO_WAIT_EXE_COMPONENT");
      int new_count;
      std::istringstream ss(new_count_str);
      if (!(ss >> new_count))
      {
        INFOS("[Container] TIMEOUT_TO_WAIT_EXE_COMPONENT should be an int");
      }
      else
        count = new_count;
    }
    INFOS("[Container] waiting " << count << " second steps exe component ");
    CORBA::Object_var obj = CORBA::Object::_nil() ;
    while ( CORBA::is_nil(obj) && count )
    {
#ifndef WIN32
      sleep( 1 ) ;
#else
      Sleep(1000);
#endif
      count-- ;
      MESSAGE( count << ". Waiting for component " << CompName);
      obj = _NS->Resolve(component_registerName.c_str());
    }

    if(CORBA::is_nil(obj))
    {
      reason="SALOME_Container::create_component_instance failed";
      MESSAGE(reason);
      return Engines::EngineComponent::_nil();
    }
    else
    {
      MESSAGE("SALOME_Container::create_component_instance successful");
      iobject = Engines::EngineComponent::_narrow(obj);
      _listInstances_map[instanceName] = iobject;
      return iobject._retn();
    }
  }
}


//=============================================================================
//! Create a new component instance (Python implementation)
/*! 
*  \param CompName               Name of the component instance
*  \param studyId                0 for multiStudy instance, 
*                                study Id (>0) otherwise
*  \param reason                 explains error when creation fails
*  \return a loaded component
*/
//=============================================================================
Engines::EngineComponent_ptr
Engines_Container_i::createPythonInstance(std::string CompName, int studyId,
                                          std::string& reason)
{
  Engines::EngineComponent_var iobject = Engines::EngineComponent::_nil() ;

  _numInstanceMutex.lock() ; // lock on the instance number
  _numInstance++ ;
  int numInstance = _numInstance ;
  _numInstanceMutex.unlock() ;

  char aNumI[12];
  sprintf( aNumI , "%d" , numInstance ) ;
  std::string instanceName = CompName + "_inst_" + aNumI ;
  std::string component_registerName = _containerName + "/" + instanceName;

  PyGILState_STATE gstate = PyGILState_Ensure();
  PyObject *result = PyObject_CallMethod(_pyCont,
                                         (char*)"create_component_instance",
                                         (char*)"ssl",
                                         CompName.c_str(),
                                         instanceName.c_str(),
                                         studyId);
  const char *ior;
  const char *error;
  PyArg_ParseTuple(result,"ss", &ior, &error);
  std::string iors = ior;
  reason=error;
  Py_DECREF(result);
  PyGILState_Release(gstate);

  if( iors!="" )
    {
      CORBA::Object_var obj = _orb->string_to_object(iors.c_str());
      iobject = Engines::EngineComponent::_narrow( obj ) ;
      _listInstances_map[instanceName] = iobject;
    }
  return iobject._retn();
}

char *
Engines_Container_i::create_python_service_instance(const char * CompName,
                                                    CORBA::String_out reason)
{
  CORBA::Object_var object = CORBA::Object::_nil();

  _numInstanceMutex.lock() ; // lock on the instance number
  _numInstance++ ;
  int numInstance = _numInstance ;
  _numInstanceMutex.unlock() ;

  char aNumI[12];
  sprintf( aNumI , "%d" , numInstance ) ;
  std::string instanceName = std::string(CompName) + "_inst_" + aNumI ;
  std::string component_registerName = _containerName + "/" + instanceName;

  PyGILState_STATE gstate = PyGILState_Ensure();
  PyObject *result = PyObject_CallMethod(_pyCont,
                                         (char*)"create_component_instance",
                                         (char*)"ssl",
                                         CompName,
                                         instanceName.c_str(),
                                         0);
  const char *ior;
  const char *error;
  PyArg_ParseTuple(result,"ss", &ior, &error);
  reason = CORBA::string_dup(error);
  char * _ior = CORBA::string_dup(ior);
  Py_DECREF(result);
  PyGILState_Release(gstate);

  return _ior;
}


//=============================================================================
//! Create a new component instance (C++ implementation)
/*! 
*  C++ method: create a servant instance of a component.
*  \param genericRegisterName    Name of the component instance to register
*                                in Registry & Name Service,
*                                (without _inst_n suffix, like "COMPONENT")
*  \param handle                 loaded library handle
*  \param studyId                0 for multiStudy instance, 
*                                study Id (>0) otherwise
*  \param reason                 explains error when creation fails
*  \return a loaded component
* 
*  example with names:
*    - aGenRegisterName = COMPONENT (= first argument)
*    - _containerName = /Containers/cli76ce/FactoryServer
*    - factoryName = COMPONENTEngine_factory
*    - component_registerBase = /Containers/cli76ce/FactoryServer/COMPONENT
*    - instanceName = COMPONENT_inst_1
*    - component_registerName = /Containers/cli76ce/FactoryServer/COMPONENT_inst_1
*/
//=============================================================================
Engines::EngineComponent_ptr
Engines_Container_i::createInstance(std::string genericRegisterName,
                                    void *handle,
                                    int studyId,
                                    std::string& reason)
{
  // --- find the factory

  std::string aGenRegisterName = genericRegisterName;
  std::string factory_name = aGenRegisterName + std::string("Engine_factory");
  SCRUTE(factory_name) ;

  typedef PortableServer::ObjectId* (*FACTORY_FUNCTION) (CORBA::ORB_ptr,
                                                         PortableServer::POA_ptr, 
                                                         PortableServer::ObjectId *, 
                                                         const char *, 
                                                         const char *) ;

#ifndef WIN32
  FACTORY_FUNCTION Component_factory = (FACTORY_FUNCTION)dlsym( handle, factory_name.c_str() );
#else
  FACTORY_FUNCTION Component_factory = (FACTORY_FUNCTION)GetProcAddress( (HINSTANCE)handle, factory_name.c_str() );
#endif

  if ( !Component_factory )
  {
    MESSAGE( "Can't resolve symbol: " + factory_name );
#ifndef WIN32
    reason=dlerror();
    MESSAGE(reason);
#endif
    return Engines::EngineComponent::_nil() ;
  }

  // --- create instance

  Engines::EngineComponent_var iobject = Engines::EngineComponent::_nil() ;

  try
  {
    _numInstanceMutex.lock() ; // lock on the instance number
    _numInstance++ ;
    int numInstance = _numInstance ;
    _numInstanceMutex.unlock() ;

    char aNumI[12];
    sprintf( aNumI , "%d" , numInstance ) ;
    std::string instanceName = aGenRegisterName + "_inst_" + aNumI ;
    std::string component_registerName =
      _containerName + "/" + instanceName;

    // --- Instanciate required CORBA object

    PortableServer::ObjectId *id ; //not owner, do not delete (nore use var)
    id = (Component_factory) ( _orb, _poa, _id, instanceName.c_str(),
                                                aGenRegisterName.c_str() ) ;
    if (id == NULL)
      {
        reason="Can't get ObjectId from factory";
        INFOS(reason);
        return iobject._retn();
      }

    // --- get reference & servant from id

    CORBA::Object_var obj = _poa->id_to_reference(*id);
    iobject = Engines::EngineComponent::_narrow( obj ) ;

    Engines_Component_i *servant =
      dynamic_cast<Engines_Component_i*>(_poa->reference_to_servant(iobject));
    ASSERT(servant);
    //SCRUTE(servant->_refcount_value());
    _numInstanceMutex.lock() ; // lock to be alone (stl container write)
    _listInstances_map[instanceName] = iobject;
    _cntInstances_map[aGenRegisterName] += 1;
    _numInstanceMutex.unlock() ;
    SCRUTE(aGenRegisterName);
    SCRUTE(_cntInstances_map[aGenRegisterName]);
    servant->setStudyId(studyId);
    servant->_remove_ref(); // do not need servant any more (remove ref from reference_to_servant)
    //SCRUTE(servant->_refcount_value());

    // --- register the engine under the name
    //     containerName(.dir)/instanceName(.object)

    _NS->Register( iobject , component_registerName.c_str() ) ;
    MESSAGE( component_registerName.c_str() << " bound" ) ;
  }
  catch (...)
  {
    reason="Container_i::createInstance exception catched";
    INFOS(reason) ;
  }
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
Engines::EngineComponent_ptr
Engines_Container_i::find_component_instance( const char* registeredName,
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

//=============================================================================
//! Remove the component instance from container
/*!
*  CORBA method: Stops the component servant, and deletes all related objects
*  \param component_i     Component to be removed
*/
//=============================================================================

void Engines_Container_i::remove_impl(Engines::EngineComponent_ptr component_i)
{
  ASSERT(! CORBA::is_nil(component_i));
  std::string instanceName = component_i->instanceName() ;
  MESSAGE("unload component " << instanceName);
  _numInstanceMutex.lock() ; // lock to be alone (stl container write)
  _listInstances_map.erase(instanceName);
  _numInstanceMutex.unlock() ;
  component_i->destroy() ;
  _NS->Destroy_Name(instanceName.c_str());
}

//=============================================================================
//! Unload component libraries from the container
/*!
*  CORBA method: Discharges unused libraries from the container.
*/
//=============================================================================
void Engines_Container_i::finalize_removal()
{
  MESSAGE("finalize unload : dlclose");
  _numInstanceMutex.lock(); // lock to be alone
  // (see decInstanceCnt, load_component_Library)
  std::map<std::string, void *>::iterator ith;
  for (ith = _toRemove_map.begin(); ith != _toRemove_map.end(); ith++)
  {
    void *handle = (*ith).second;
    std::string impl_name= (*ith).first;
    if (handle)
    {
      SCRUTE(handle);
      SCRUTE(impl_name);
      //        dlclose(handle);                // SALOME unstable after ...
      //        _library_map.erase(impl_name);
    }
  }
  _toRemove_map.clear();
  _numInstanceMutex.unlock();
}

//=============================================================================
//! Decrement component instance reference count
/*!
*
*/
//=============================================================================
void Engines_Container_i::decInstanceCnt(std::string genericRegisterName)
{
  if(_cntInstances_map.count(genericRegisterName)==0)
    return;
  std::string aGenRegisterName =genericRegisterName;
  MESSAGE("Engines_Container_i::decInstanceCnt " << aGenRegisterName);
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

//=============================================================================
//! Find or create a new component instance
/*!
*  CORBA method: find or create an instance of the component (servant),
*  load a new component class (dynamic library) if required,
*
*  ---- FOR COMPATIBILITY WITH 2.2 ----
*
*  ---- USE ONLY FOR MULTISTUDY INSTANCES ! --------
*
*  The servant registers itself to naming service and Registry.
*  \param genericRegisterName  Name of the component to register
*                              in Registry & Name Service
*  \param componentName       Name of the constructed library of the component
*  \return a loaded component
*/
//=============================================================================

Engines::EngineComponent_ptr
Engines_Container_i::load_impl( const char* genericRegisterName,
                                const char* componentName )
{
  char* reason;
  std::string impl_name = std::string(LIB) + genericRegisterName + ENGINESO;
  Engines::EngineComponent_var iobject = Engines::EngineComponent::_nil() ;
  if (load_component_Library(genericRegisterName,reason))
    iobject = find_or_create_instance(genericRegisterName, impl_name);
  CORBA::string_free(reason);
  return iobject._retn();
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
*  \param componentLibraryName   like "libCOMPONENTEngine.so"
*  \return a loaded component
*
*  example with names:
*    - aGenRegisterName = COMPONENT (= first argument)
*    - impl_name = libCOMPONENTEngine.so (= second argument)
*    - _containerName = /Containers/cli76ce/FactoryServer
*    - factoryName = COMPONENTEngine_factory
*    - component_registerBase = /Containers/cli76ce/FactoryServer/COMPONENT
*    - instanceName = COMPONENT_inst_1
*    - component_registerName = /Containers/cli76ce/FactoryServer/COMPONENT_inst_1
*/
//=============================================================================

Engines::EngineComponent_ptr
Engines_Container_i::find_or_create_instance(std::string genericRegisterName,
                                             std::string componentLibraryName)
{
  std::string aGenRegisterName = genericRegisterName;
  std::string impl_name = componentLibraryName;
  if (_library_map.count(impl_name) == 0)
  {
    INFOS("shared library " << impl_name <<" must be loaded before creating instance");
    return Engines::EngineComponent::_nil() ;
  }
  else
  {
    // --- find a registered instance in naming service, or create

    void* handle = _library_map[impl_name];
    std::string component_registerBase =
      _containerName + "/" + aGenRegisterName;
    Engines::EngineComponent_var iobject = Engines::EngineComponent::_nil() ;
    std::string reason;
    try
    {
      CORBA::Object_var obj =
        _NS->ResolveFirst( component_registerBase.c_str());
      if ( CORBA::is_nil( obj ) )
      {
        iobject = createInstance(genericRegisterName,
                                 handle,
                                 0,
                                 reason); // force multiStudy instance here !
      }
      else
      {
        iobject = Engines::EngineComponent::_narrow( obj ) ;
        Engines_Component_i *servant =
          dynamic_cast<Engines_Component_i*>
          (_poa->reference_to_servant(iobject));
        ASSERT(servant)
          int studyId = servant->getStudyId();
        ASSERT (studyId >= 0);
        if (studyId == 0) // multiStudy instance, OK
        {
          // No ReBind !
          MESSAGE(component_registerBase.c_str()<<" already bound");
        }
        else // monoStudy instance: NOK
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
}

//=============================================================================
//! Indicate if container is a python one
/*! 
*  Retrieves only with container naming convention if it is a python container
*/
//=============================================================================
bool Engines_Container_i::isPythonContainer(const char* ContainerName)
{
  bool ret=false;
  int len=strlen(ContainerName);
  if(len>=2)
    if(strcmp(ContainerName+len-2,"Py")==0)
      ret=true;
  return ret;
}

//=============================================================================
//! Kill the container
/*!
*  CORBA method: Kill the container process with exit(0).
*  To remove :  never returns !
*/
//=============================================================================
bool Engines_Container_i::Kill_impl()
{
  MESSAGE("Engines_Container_i::Kill() pid "<< getpid() << " containerName "
    << _containerName.c_str() << " machineName "
    << Kernel_Utils::GetHostname().c_str());
  INFOS("===============================================================");
  INFOS("= REMOVE calls to Kill_impl in C++ container                  =");
  INFOS("===============================================================");
  //_exit( 0 ) ;
  ASSERT(0);
  return false;
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
  sigemptyset(&SigIntAct.sa_mask);
  SigIntAct.sa_flags = SA_SIGINFO ;
#endif

  // DEBUG 03.02.2005 : the first parameter of sigaction is not a mask of signals
  // (SIGINT | SIGUSR1) :
  // it must be only one signal ===> one call for SIGINT 
  // and an other one for SIGUSR1

#ifndef WIN32
  if ( sigaction( SIGINT , &SigIntAct, NULL ) ) 
  {
    perror("SALOME_Container main ") ;
    exit(0) ;
  }
  if ( sigaction( SIGUSR1 , &SigIntAct, NULL ) )
  {
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
// legacy code required to supervisor. Commented in order to avoid problems on Windows 
//  signal( SIGUSR1, SigIntHandler );
#endif

}

void SetCpuUsed() ;
void CallCancelThread() ;

#ifndef WIN32
void SigIntHandler(int what ,
                   siginfo_t * siginfo ,
                   void * toto ) 
{
  //PAL9042 JR : during the execution of a Signal Handler (and of methods called through Signal Handlers)
  //             use of streams (and so on) should never be used because :
  //             streams of C++ are naturally thread-safe and use pthread_mutex_lock ===>
  //             A stream operation may be interrupted by a signal and if the Handler use stream we
  //             may have a "Dead-Lock" ===HangUp
  //==MESSAGE is commented
  //  MESSAGE(pthread_self() << "SigIntHandler what     " << what << std::endl
  //          << "              si_signo " << siginfo->si_signo << std::endl
  //          << "              si_code  " << siginfo->si_code << std::endl
  //          << "              si_pid   " << siginfo->si_pid) ;

  if ( _Sleeping )
  {
    _Sleeping = false ;
    //     MESSAGE("SigIntHandler END sleeping.") ;
    return ;
  }
  else
  {
    ActSigIntHandler() ;
    if ( siginfo->si_signo == SIGUSR1 )
    {
      SetCpuUsed() ;
    }
    else if ( siginfo->si_signo == SIGUSR2 )
    {
      CallCancelThread() ;
    }
    else 
    {
      _Sleeping = true ;
      //      MESSAGE("SigIntHandler BEGIN sleeping.") ;
      int count = 0 ;
      while( _Sleeping )
      {
        sleep( 1 ) ;
        count += 1 ;
      }
      //      MESSAGE("SigIntHandler LEAVE sleeping after " << count << " s.") ;
    }
    return ;
  }
}
#else // Case WIN32
void SigIntHandler( int what )
{
#ifndef WIN32
  MESSAGE( pthread_self() << "SigIntHandler what     " << what << std::endl );
#else
  MESSAGE( "SigIntHandler what     " << what << std::endl );
#endif
  if ( _Sleeping )
  {
    _Sleeping = false ;
    MESSAGE("SigIntHandler END sleeping.") ;
    return ;
  }
  else
  {
    ActSigIntHandler() ;
    if ( what == SIGUSR1 )
    {
      SetCpuUsed() ;
    }
    else
    {
      _Sleeping = true ;
      MESSAGE("SigIntHandler BEGIN sleeping.") ;
      int count = 0 ;
      while( _Sleeping ) 
      {
        Sleep( 1000 ) ;
        count += 1 ;
      }
      MESSAGE("SigIntHandler LEAVE sleeping after " << count << " s.") ;
    }
    return ;
  }
}
#endif

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
Engines_Container_i::createFileRef(const char* origFileName)
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
//! Get a fileTransfer reference
/*!
*  CORBA method:
*  \return a reference to the fileTransfer object
*/
//=============================================================================
Engines::fileTransfer_ptr
Engines_Container_i::getFileTransfer()
{
  Engines::fileTransfer_var aFileTransfer
    = Engines::fileTransfer::_duplicate(_fileTransfer);
  return aFileTransfer._retn();
}

//=============================================================================
//! Create a Salome file
//=============================================================================
Engines::Salome_file_ptr
Engines_Container_i::createSalome_file(const char* origFileName)
{
  std::string origName(origFileName);
  if (CORBA::is_nil(_Salome_file_map[origName]))
  {
    Salome_file_i* aSalome_file = new Salome_file_i();
    aSalome_file->setContainer(Engines::Container::_duplicate(this->_this()));
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
/*! \brief copy a file from a remote host (container) to the local host
 * \param container the remote container
 * \param remoteFile the file to copy locally from the remote host into localFile
 * \param localFile the local file
 */
//=============================================================================
void Engines_Container_i::copyFile(Engines::Container_ptr container, const char* remoteFile, const char* localFile)
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
          //SCRUTE(ctr);
          aBlock = fileTransfer->getBlock(fileId);
          toFollow = aBlock->length();
          //SCRUTE(toFollow);
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

//=============================================================================
/*! \brief create a PyNode object to execute remote python code
 * \param nodeName the name of the node
 * \param code the python code to load
 * \return the PyNode
 */
//=============================================================================
Engines::PyNode_ptr Engines_Container_i::createPyNode(const char* nodeName, const char* code)
{
    Engines::PyNode_var node= Engines::PyNode::_nil();

    PyGILState_STATE gstate = PyGILState_Ensure();
    PyObject *res = PyObject_CallMethod(_pyCont,
      (char*)"create_pynode",
      (char*)"ss",
      nodeName,
      code);
    if(res==NULL)
      {
        //internal error
        PyErr_Print();
        PyGILState_Release(gstate);
        SALOME::ExceptionStruct es;
        es.type = SALOME::INTERNAL_ERROR;
        es.text = "can not create a python node";
        throw SALOME::SALOME_Exception(es);
      }
    long ierr=PyInt_AsLong(PyTuple_GetItem(res,0));
    PyObject* result=PyTuple_GetItem(res,1);
    std::string astr=PyString_AsString(result);
    Py_DECREF(res);
    PyGILState_Release(gstate);
    if(ierr==0)
      {
        Utils_Locker lck(&_mutexForDftPy);
        CORBA::Object_var obj=_orb->string_to_object(astr.c_str());
        node=Engines::PyNode::_narrow(obj);
        std::map<std::string,Engines::PyNode_var>::iterator it(_dftPyNode.find(nodeName));
        if(it==_dftPyNode.end())
          {
            _dftPyNode[nodeName]=node;
          }
        else
          {
            Engines::PyNode_var oldNode((*it).second);
            if(!CORBA::is_nil(oldNode))
              oldNode->UnRegister();
            (*it).second=node;
          }
        if(!CORBA::is_nil(node))
          node->Register();
        return node._retn();
      }
    else
      {
        SALOME::ExceptionStruct es;
        es.type = SALOME::INTERNAL_ERROR;
        es.text = astr.c_str();
        throw SALOME::SALOME_Exception(es);
      }
}

//=============================================================================
/*! \brief Retrieves the last created PyNode instance with createPyNode.
 *
 */
//=============================================================================
Engines::PyNode_ptr  Engines_Container_i::getDefaultPyNode(const char *nodeName)
{
  Utils_Locker lck(&_mutexForDftPy);
  std::map<std::string,Engines::PyNode_var>::iterator it(_dftPyNode.find(nodeName));
  if(it==_dftPyNode.end())
    return Engines::PyNode::_nil();
  else
    {
      Engines::PyNode_var tmpVar((*it).second);
      if(!CORBA::is_nil(tmpVar))
        return Engines::PyNode::_duplicate(tmpVar);
      else
        return Engines::PyNode::_nil();
    }
}

//=============================================================================
/*! \brief create a PyScriptNode object to execute remote python code
 * \param nodeName the name of the node
 * \param code the python code to load
 * \return the PyScriptNode
 */
//=============================================================================
Engines::PyScriptNode_ptr Engines_Container_i::createPyScriptNode(const char* nodeName, const char* code)
{
    Engines::PyScriptNode_var node= Engines::PyScriptNode::_nil();

    PyGILState_STATE gstate = PyGILState_Ensure();
    PyObject *res = PyObject_CallMethod(_pyCont,
      (char*)"create_pyscriptnode",
      (char*)"ss",
      nodeName,
      code);
    if(res==NULL)
      {
        //internal error
        PyErr_Print();
        PyGILState_Release(gstate);
        SALOME::ExceptionStruct es;
        es.type = SALOME::INTERNAL_ERROR;
        es.text = "can not create a python node";
        throw SALOME::SALOME_Exception(es);
      }
    long ierr=PyInt_AsLong(PyTuple_GetItem(res,0));
    PyObject* result=PyTuple_GetItem(res,1);
    std::string astr=PyString_AsString(result);
    Py_DECREF(res);
    PyGILState_Release(gstate);

    if(ierr==0)
      {
        Utils_Locker lck(&_mutexForDftPy);
        CORBA::Object_var obj=_orb->string_to_object(astr.c_str());
        node=Engines::PyScriptNode::_narrow(obj);
        std::map<std::string,Engines::PyScriptNode_var>::iterator it(_dftPyScriptNode.find(nodeName));
        if(it==_dftPyScriptNode.end())
          {
            _dftPyScriptNode[nodeName]=node;
          }
        else
          {
            Engines::PyScriptNode_var oldNode((*it).second);
            if(!CORBA::is_nil(oldNode))
              oldNode->UnRegister();
            (*it).second=node;
          }
        if(!CORBA::is_nil(node))
          node->Register();
        return node._retn();
      }
    else
      {
        SALOME::ExceptionStruct es;
        es.type = SALOME::INTERNAL_ERROR;
        es.text = astr.c_str();
        throw SALOME::SALOME_Exception(es);
      }
}

//=============================================================================
/*! \brief Retrieves the last created PyScriptNode instance with createPyScriptNode.
 *
 */
//=============================================================================
Engines::PyScriptNode_ptr Engines_Container_i::getDefaultPyScriptNode(const char *nodeName)
{
  Utils_Locker lck(&_mutexForDftPy);
  std::map<std::string,Engines::PyScriptNode_var>::iterator it(_dftPyScriptNode.find(nodeName));
  if(it==_dftPyScriptNode.end())
    return Engines::PyScriptNode::_nil();
  else
    {
      Engines::PyScriptNode_var tmpVar((*it).second);
      if(!CORBA::is_nil(tmpVar))
        return Engines::PyScriptNode::_duplicate(tmpVar);
      else
        return Engines::PyScriptNode::_nil();
    }
}

//=============================================================================
/* int checkifexecutable(const char *filename)
*
* Return non-zero if the name is an executable file, and
* zero if it is not executable, or if it does not exist.
*/
//=============================================================================
int checkifexecutable(const std::string& filename)
{
  int result;
  struct stat statinfo;

  result = stat(filename.c_str(), &statinfo);
  if (result < 0) return 0;
  if (!S_ISREG(statinfo.st_mode)) return 0;

#ifdef WIN32
  return 1;
#else
  if (statinfo.st_uid == geteuid()) return statinfo.st_mode & S_IXUSR;
  if (statinfo.st_gid == getegid()) return statinfo.st_mode & S_IXGRP;
  return statinfo.st_mode & S_IXOTH;
#endif
}


//=============================================================================
/*! \brief Find a file by searching in a path
 *  \param filename file name to search
 *  \param path path to search in
 *  \param pth the complete file path if found
 *  \return 1 if found 0 if not 
*/
//=============================================================================
int findpathof(const std::string& path, std::string& pth, const std::string& filename)
{
  if ( path.size() == 0 ) return 0;

  std::string::size_type offset = 0;
  std::string::size_type pos = 0;
  int found = 0;
  struct stat statinfo;

  while(!found)
  {
    pos = path.find( SEP, offset );
    pth = path.substr( offset, pos - offset );
    if ( pth.size() > 0 )
    {
      if( pth[pth.size()-1] != SLASH ) pth += SLASH;
      pth += filename;
      int result=stat(pth.c_str(), &statinfo);
      if(result == 0) found=1;
    }
    if (pos == std::string::npos) break;
    offset = pos+1;
  }
  return found;
}

void Engines_Container_i::registerTemporaryFile( const std::string& fileName )
{
  _tmp_files.remove( fileName );
  _tmp_files.push_back( fileName );
}

void Engines_Container_i::unregisterTemporaryFile( const std::string& fileName )
{
  _tmp_files.remove( fileName );
}

void Engines_Container_i::clearTemporaryFiles()
{
  std::list<std::string>::const_iterator it;
  for ( it = _tmp_files.begin(); it != _tmp_files.end(); ++it ) {
#ifdef WIN32
    std::string command = "del /F /P";
#else
    std::string command = "rm -rf ";
#endif
    command += *it;
    system( command.c_str() );
  }
  _tmp_files.clear();
}
