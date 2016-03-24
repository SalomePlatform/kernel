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

//  SALOME NamingService : wrapping NamingService services
//  File   : SALOME_NamingService.cxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$
//
#include "SALOME_NamingService.hxx"
#include "ServiceUnreachable.hxx"

#include "utilities.h"

#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>

#ifdef WIN32
#define strdup _strdup
#endif

/*! \class SALOME_NamingService
    \brief A class to manage the SALOME naming service

*/

// ============================================================================
/*! \brief Default Constructor without ORB reference.
 *
 *  After Default Constructor, one needs to initialize ORB.
 *  \sa init_orb(CORBA::ORB_ptr orb), SALOME_NamingService(CORBA::ORB_ptr orb)
 */ 
// ============================================================================

SALOME_NamingService::SALOME_NamingService()
{
  MESSAGE("SALOME_NamingService default constructor");
  _orb = CORBA::ORB::_nil();
  _root_context = CosNaming::NamingContext::_nil();
}

// ============================================================================
/*! \brief Standard Constructor, with ORB reference. 
 * 
 * Initializes the naming service root context
 * \param orb CORBA::ORB_ptr arguments
 */ 
// ============================================================================

SALOME_NamingService::SALOME_NamingService(CORBA::ORB_ptr orb)
{
  MESSAGE("SALOME_NamingService creation");
  _orb = CORBA::ORB::_duplicate(orb);
  _initialize_root_context();
}

// ============================================================================
/*! \brief Standard destructor.
 *
 *  The standard destructor does nothing special.
 */ 
// ============================================================================

SALOME_NamingService::~SALOME_NamingService()
{
  // Problem MESSAGE with singleton: late destruction,
  // after trace system destruction ?
  //MESSAGE("SALOME_NamingService destruction");
}

// ============================================================================
/*! \brief initializes ORB reference and naming service root context.
 * 
 *  Initializes ORB reference and naming service root context.
 *  For use after default constructor.
 *  If param orb is null, the orb is initialized
 *  \param orb CORBA::ORB_ptr arguments
 */ 
// ============================================================================

void SALOME_NamingService::init_orb(CORBA::ORB_ptr orb)
{
  MESSAGE("SALOME_NamingService initialisation");

  Utils_Locker lock (&_myMutex);
  if(orb)
    _orb = CORBA::ORB::_duplicate(orb);
  else
    {
      int argc=0;
      _orb = CORBA::ORB_init(argc, 0); // Here we make the assumption that the orb has already been initialized
    }

  _initialize_root_context();
}

// ============================================================================
/*! \brief Registers a CORBA object reference under a path.
 *
 * Registers a CORBA object reference under a path. If the path ends with '/',
 * only a directory is created.
 * If the NamingService is out, the exception ServiceUnreachable is thrown.
 * \param ObjRef CORBA object reference to associate to the path. To create
 *               only a directory, give nil pointer.
 * \param Path   A relative or absolute pathname to store the object reference.
 *               If the pathname begins with a '/', pathname is taken
 *               as an absolute pathname. Else, pathname is taken as a relative
 *               path, to current context. Prefer absolute pathname, relative
 *               pathname are not safe, when SALOME_NamingService object is
 *               shared or use in multithreaded context. 
 *               If the path ends with '/', only a directory is created.
 * \sa           Change_Directory(const char* Path),
 *               Create_Directory(const char* Path)
 *               CORBA::Object_ptr Resolve(const char* Path)
 */ 
// ============================================================================

void SALOME_NamingService::Register(CORBA::Object_ptr ObjRef,
                                    const char* Path)
  throw(ServiceUnreachable)
{
  MESSAGE("BEGIN OF Register: " << Path);

  Utils_Locker lock (&_myMutex);

  // --- _current_context is replaced to the _root_context
  //     if the Path begins whith '/'

  if (Path[0] == '/'){
    _current_context = _root_context;
  }

  // --- the resolution of the directory path has to be done
  //      to place the current_context to the correct node

  CosNaming::Name context_name;
  std::vector<std::string> splitPath;
  int dimension_resultat = _createContextNameDir(Path,
                                                 context_name,
                                                 splitPath,
                                                 true);

  CORBA::Boolean not_exist = false;

  if (dimension_resultat > 0){
    // A directory is treated (not only an object name)
    // test if the directory where ObjRef should be recorded already exists
    // If not, create the new context
    
    try{
      CORBA::Object_var obj = _current_context->resolve(context_name);
      _current_context = CosNaming::NamingContext::_narrow(obj);
    }

    catch (CosNaming::NamingContext::NotFound &){
      // --- failed to resolve, therefore assume cold start
      not_exist = true;
    }

    catch (CosNaming::NamingContext::InvalidName &){
      INFOS("Register() : CosNaming::NamingContext::InvalidName");
    }

    catch (CosNaming::NamingContext::CannotProceed &){
      INFOS("Register() : CosNaming::NamingContext::CannotProceed");
    }

    catch (CORBA::SystemException&){
      INFOS("Register() : CORBA::SystemException: "
            << "unable to contact the naming service");
      throw ServiceUnreachable();
    }

    if (not_exist){
      try{
        context_name.length(1);
        for (int i = 0 ; i < dimension_resultat ;i++){
          context_name[0].id = CORBA::string_dup(splitPath[i].c_str());
          context_name[0].kind = CORBA::string_dup("dir");
          // SCRUTE(_context_name[0].id);
          // --- check if the path is created
          try{
            // --- if the context is already created, nothing to do
            CORBA::Object_var obj = _current_context->resolve(context_name);
            _current_context = CosNaming::NamingContext::_narrow(obj);
          }

          catch (CosNaming::NamingContext::NotFound &){
            try{
              // --- the context must be created
              CosNaming::NamingContext_var temp_context =
                _current_context->bind_new_context(context_name);
              _current_context = temp_context;
            }
            catch (CosNaming::NamingContext::AlreadyBound&){
              CORBA::Object_var obj = _current_context->resolve(context_name);
              _current_context = CosNaming::NamingContext::_narrow(obj);
            }
          }
        }
      }

      catch (CosNaming::NamingContext::AlreadyBound&){
        INFOS("Register() : CosNaming::NamingContext::AlreadyBound");
      }

      catch (CosNaming::NamingContext::NotFound& ex){
        CosNaming::Name n = ex.rest_of_name;

        if (ex.why == CosNaming::NamingContext::missing_node)
          INFOS("Register() : " << (char *) n[0].id
                << " (" << (char *) n[0].kind << ") not found");

        if (ex.why == CosNaming::NamingContext::not_context)
          INFOS("Register() : " << (char *) n[0].id
                << " (" << (char *) n[0].kind
                << ") is not a context");

        if (ex.why == CosNaming::NamingContext::not_object)
          INFOS("Register() : " << (char *) n[0].id
                << " (" << (char *) n[0].kind
                << ") is not an object");
      }

      catch (CosNaming::NamingContext::CannotProceed&){
        INFOS("Register(): CosNaming::NamingContext::CannotProceed");
      }

      catch (CosNaming::NamingContext::InvalidName&){
        INFOS("Register(): CosNaming::NamingContext::InvalidName");
      }

      catch (CORBA::SystemException&){
        INFOS("Register():CORBA::SystemException: "
              << "unable to contact the naming service");
        throw ServiceUnreachable();
      }
    }
  }

  // --- The current directory is now the directory where the object should
  //     be recorded

  int sizePath = splitPath.size();
  if (sizePath > dimension_resultat){
    ASSERT(sizePath == dimension_resultat+1);
    context_name.length(1);

    try{
      // --- the last element is an object and not a directory

      context_name[0].id = CORBA::string_dup(splitPath[dimension_resultat].c_str());
      context_name[0].kind = CORBA::string_dup("object");
      //SCRUTE(context_name[0].id);

      _current_context->bind(context_name, ObjRef);
    }

    catch (CosNaming::NamingContext::NotFound& ex){
      CosNaming::Name n = ex.rest_of_name;

      if (ex.why == CosNaming::NamingContext::missing_node)
        INFOS("Register() : " << (char *) n[0].id
              << " (" << (char *) n[0].kind << ") not found");

      if (ex.why == CosNaming::NamingContext::not_context)
        INFOS("Register() : " << (char *) n[0].id
              << " (" << (char *) n[0].kind
              << ") is not a context");

      if (ex.why == CosNaming::NamingContext::not_object)
        INFOS("Register() : " << (char *) n[0].id
              << " (" << (char *) n[0].kind
              << ") is not an object");
    }

    catch (CosNaming::NamingContext::CannotProceed&){
      INFOS("Register(): CosNaming::NamingContext::CannotProceed");
    }

    catch (CosNaming::NamingContext::InvalidName&){
      INFOS("Register(): CosNaming::NamingContext::InvalidName");
    }

    catch (CosNaming::NamingContext::AlreadyBound&){
      INFOS("Register(): CosNaming::NamingContext::AlreadyBound, "
            << "object will be rebind");
      _current_context->rebind(context_name, ObjRef);
    }

    catch (CORBA::SystemException&){
      INFOS("!!!Register(): CORBA::SystemException: "
            << "unable to contact the naming service");
      throw ServiceUnreachable();
    }
  }
}

// ============================================================================
/*! \brief get the CORBA object reference associated to a name.
 * 
 *  get the CORBA object reference associated to a complete name with a path.
 *  If the NamingService is out, the exception ServiceUnreachable is thrown 
 * \param Path pathname. If the pathname begins with a '/', pathname is taken
 *             as an absolute pathname. Else, pathname is taken as a relative
 *             path, to current context. Prefer absolute pathname, relative
 *             pathname are not safe, when SALOME_NamingService object is
 *             shared or use in multithreaded context.
 * \return the object reference if it exists under the pathname,
 *             or nil reference in other cases.
 * \sa         Register(CORBA::Object_ptr ObjRef, const char* Path),
 *             Change_Directory(const char* Path)
 */ 
// ============================================================================

CORBA::Object_ptr SALOME_NamingService::Resolve(const char* Path)
  throw(ServiceUnreachable)
{
//   MESSAGE("BEGIN OF Resolve: " << Path);

  Utils_Locker lock (&_myMutex);

  // --- _current_context is replaced to the _root_context
  //     if the Path begins whith '/'

  if (Path[0] == '/')
    {
      _current_context = _root_context;
    }

  // --- the resolution of the directory path has to be done
  //     to place the current_context to the correct node

  CosNaming::Name context_name;
  std::vector<std::string> splitPath;
  _createContextNameDir(Path,
                                                 context_name,
                                                 splitPath,
                                                 false);

  ASSERT(!CORBA::is_nil(_current_context));

  CORBA::Object_var obj =  CORBA::Object::_nil();

  try
    {
      obj = _current_context->resolve(context_name);
    }

  catch (CosNaming::NamingContext::NotFound& ex)
    {
      CosNaming::Name n = ex.rest_of_name;

      if (ex.why == CosNaming::NamingContext::missing_node)
        MESSAGE("Resolve() : " << (char *) n[0].id
              << " (" << (char *) n[0].kind << ") not found");

      if (ex.why == CosNaming::NamingContext::not_context)
        INFOS("Resolve() : "
              << (char *) n[0].id << " (" << (char *) n[0].kind
              << ") is not a context");

      if (ex.why == CosNaming::NamingContext::not_object)
        INFOS("Resolve() : " << (char *) n[0].id
              << " (" << (char *) n[0].kind
              << ") is not an object");
    }

  catch (CosNaming::NamingContext::CannotProceed&)
    {
      INFOS("Resolve(): CosNaming::NamingContext::CannotProceed");
    }

  catch (CosNaming::NamingContext::InvalidName&)
    {
      INFOS("Resolve(): CosNaming::NamingContext::InvalidName");
    }

  catch (CORBA::SystemException&)
    {
      INFOS("Resolve():CORBA::SystemException : unable to contact"
            << "the naming service");
      throw ServiceUnreachable();
    }

  return obj._retn();
}

// ============================================================================
/*! \brief get the CORBA object reference associated to an uncomplete name.
 *
 *  get the CORBA object reference associated to an uncomplete name with a
 *  path. Look for the first occurence of name*.
 *  If the NamingService is out, the exception ServiceUnreachable is thrown 
 * \param Path pathname under the form "/path/name" (Absolute reference !)
 *             search the fist reference like "/path(.dir)/name*(.kind)"
 * \return     the object reference if found, or nil reference.
 * \sa         Resolve(const char* Path)
 */
// ============================================================================

CORBA::Object_ptr SALOME_NamingService::ResolveFirst(const char* Path)
  throw(ServiceUnreachable)
{
//   MESSAGE("ResolveFirst");

  Utils_Locker lock (&_myMutex);
//   SCRUTE(Path);

  std::string thePath = Path;
  std::string basePath = "";
  std::string name = thePath;

  std::string::size_type idx = thePath.rfind('/');

  if (idx != std::string::npos) // at least one '/' found
    {
      basePath = thePath.substr(0, idx);
      name = thePath.substr(idx + 1);
//       SCRUTE(basePath);
    }

//   SCRUTE(name);
  CORBA::Object_var obj = CORBA::Object::_nil();

  bool isOk = false;
  if (basePath.empty())
    isOk =true;
  else
    isOk = Change_Directory(basePath.c_str());
  
  if (isOk)
    {
      std::vector<std::string> listElem = list_directory();
      std::vector<std::string>::iterator its = listElem.begin();
      
      while (its != listElem.end())
        {
          MESSAGE(*its);
          
          if ((*its).find(name) == 0)
            {
              return Resolve((*its).c_str());
            }
          
          its++;
        }
    }

  return obj._retn();
}

// ============================================================================
/*! \brief find a component instance from hostname, containername,
 *         componentName and number of processors.
 *
 *  find a component instance from hostname, containername, componentName and
 *  number of processors.
 *  If the NamingService is out, the exception ServiceUnreachable is thrown.
 * \param hostname      name of the machine on which the component is searched.
 * \param containerName name of the container in which the component is
                        instanciated.
 * \param componentName name of the component we are looking for an existing 
                        instance.
 * \param nbproc        in case of multi processor machine, container name is
 *                      suffixed with _nbproc.
 * \return the object reference
 */ 
// ============================================================================

CORBA::Object_ptr
SALOME_NamingService::ResolveComponent(const char* hostname,
                                       const char* containerName,
                                       const char* componentName,
                                       const int nbproc)
  throw(ServiceUnreachable)
{
//   MESSAGE("ResolveComponent");

  Utils_Locker lock (&_myMutex);

  std::string name = "/Containers/";

  name += hostname;

  if ( strlen(containerName) != 0 )
    {
      name += "/";

      if ( nbproc >= 1 )
        {
          char *newContainerName = new char[strlen(containerName) + 8];
          sprintf(newContainerName, "%s_%d", containerName, nbproc);
          name += newContainerName;
          delete [] newContainerName;
        }

      else
        name += containerName;

      name += "/";

      name += componentName;

      return ResolveFirst(name.c_str());
    }

  else
    {
      SCRUTE(name);
      std::string basename = name;
      if (Change_Directory(basename.c_str()))
        {
          std::vector<std::string> contList = list_subdirs();

          for (unsigned int ind = 0; ind < contList.size(); ind++)
            {
              name = contList[ind].c_str();

              if ( nbproc >= 1 )
                {
                  char *str_nbproc = new char[8];
                  sprintf(str_nbproc, "_%d", nbproc);
                  if( strstr(name.c_str(),str_nbproc) == NULL)
                    continue; // check only containers with _%d in name
                  delete [] str_nbproc;
                }

              name += "/";
              name += componentName;
              SCRUTE(name);
              CORBA::Object_ptr obj = ResolveFirst(name.c_str());

              if ( !CORBA::is_nil(obj) )
                return obj;
              else
                Change_Directory(basename.c_str());
            }
        }

      return CORBA::Object::_nil();
    }
}

// ============================================================================
/*! \brief provide a default container name if empty.
 *
 *  the given container name is returned unchanged, unless it is empty.
 * \param  containerName
 * \return container name, where empty input is replaced by "FactoryServer",
 *         without the path.
 * \sa BuildContainerNameForNS(const char *containerName, const char *hostname)
 */
// ============================================================================

std::string SALOME_NamingService::ContainerName(const char *containerName)
{
  std::string ret,containerNameCpp(containerName);

  if (containerNameCpp.empty())
    ret = "FactoryServer";
  else
    ret = containerName;

  return ret;
}

// ============================================================================
/*! \brief build a container name, given a ContainerParameters struct.
 *
 *  Build a container name with a ContainerParameters struct. In case of multi
 *  processor machine, container name is suffixed with number of processors.
 * \param params struct from which we get container name (may be empty) and
 *               number of processors.
 * \return a container name without the path.
 * \sa BuildContainerNameForNS(const Engines::ContainerParameters& params,
 *                             const char *hostname)
 */
// ============================================================================

std::string SALOME_NamingService::ContainerName(const Engines::ContainerParameters& params)
{
  int nbproc;

  if ( !params.isMPI )
    nbproc = 0;
  else if ( params.nb_proc <= 0 )
    nbproc = 1;
  else
    nbproc = params.nb_proc;

  std::string ret(ContainerName(params.container_name));

  if ( nbproc >= 1 )
    {
	  std::ostringstream suffix;
	  suffix << "_" << nbproc;
      ret += suffix.str();
    }

  return ret;
}

// ============================================================================
/*! \brief build a string representing a container in Naming Service.
 *
 *  Build a string representing the absolute pathname of a container in
 *  SALOME_NamingService. This form gives a suffixed containerName in case of
 *  multi processor machine.
 * \param containerName name of the container in which the component is
                        instanciated.
 * \param hostname name of the host of the container, without domain names.
 * \return the path under the form /Containers/hostname/containerName
 * \sa ContainerName(const Engines::MachineParameters& params)
 */
// ============================================================================

std::string SALOME_NamingService::BuildContainerNameForNS(const char *containerName, const char *hostname)
{
  std::string ret("/Containers/");
  ret += hostname;
  ret += "/";
  ret += ContainerName(containerName);

  return ret;
}

// ============================================================================
/*! \brief build a string representing a container in Naming Service.
 *
 *  Build a string representing the absolute pathname of a container in
 *  SALOME_NamingService.
 * \param params used as it is, or replaced by FactoryServer if empty.
 * \param hostname name of the host of the container, without domain names.
 * \return the path under the form /Containers/hostname/containerName
 * \sa ContainerName(const char *containerName)
 */
// ============================================================================

std::string SALOME_NamingService::BuildContainerNameForNS(const Engines::ContainerParameters& params, const char *hostname)
{
  std::string ret("/Containers/");
  ret += hostname;
  ret += "/";
  ret += ContainerName(params);

  return ret;
}

// ============================================================================
/*! \brief search a name in current directory.
 *
 *  Search a name in the current directory. after call, the current directory
 *  is changed to the directory containing the last occurence of name found.
 *  If no occurence found (see return value), current directory remains
 *  unchanged.
 *
 * \param  name the name to search.
 * \return number of occurences found.
 * \sa Change_Directory(const char* Path)
 */ 
// ============================================================================

int SALOME_NamingService::Find(const char* name)
throw(ServiceUnreachable)
{
  MESSAGE("BEGIN OF Find " << name);

  Utils_Locker lock (&_myMutex);

  CORBA::Long occurence_number = 0 ;

  try
    {
      _Find(name, occurence_number);
    }

  catch (CORBA::SystemException&)
    {
      INFOS("!!!Find() : CORBA::SystemException : unable to contact"
            << " the naming service");
      throw ServiceUnreachable();
    }

  return occurence_number;
}

// ============================================================================
/*! \brief Creates a directory (context_name)
 *
 *  Creates a directory (context_name) relative to the current directory 
 *  (current context) or relative to the root directory (root context), if
 *  the path given begins with a '/'.
 *  If the NamingService is out, the exception ServiceUnreachable is thrown.
 * \param Path   A relative or absolute pathname to store the object reference.
 *               If the pathname begins with a '/', pathname is taken
 *               as an absolute pathname. Else, pathname is taken as a relative
 *               path, to current context. Prefer absolute pathname, relative
 *               pathname are not safe, when SALOME_NamingService object is
 *               shared or use in multithreaded context.   
 *  \return true if successfull
 *          (creation not strictly garanteed if true, because Register may
 *           catch some specific unlikely exception without throw anything
 *           --- to be corrected ---)
 *  \sa RegisterCORBA::Object_ptr ObjRef, const char* Path)
 */ 
// ============================================================================

bool SALOME_NamingService::Create_Directory(const char* Path) throw(ServiceUnreachable)
{
  MESSAGE("BEGIN OF Create_Directory");

  Utils_Locker lock (&_myMutex);

  std::string path(Path);

  // --- if path empty, nothing to create, no context change

  if (path.empty())
    return false;

  // --- if path ='/', nothing to create, only change to root_context

  if (path == "/")
    {
      MESSAGE("Create Directory '/', just change to root_context");
      _current_context = _root_context;
      return true;
    }

  // --- path must end with '/'
  
  if (path[path.length()-1] != '/') path += '/';

  Register(CORBA::Object::_nil(), path.c_str());
  return true;
}

// ============================================================================
/*! \brief change current directory to the given path
 *
 *  change the current directory to the given path in parameter.
 *  Warning: avoid use when the SALOME_NamingService instance is shared by
 *  several threads (current context may be modified by another thread).
 *  If the path is empty, nothing done return OK.
 *  If Path ="/", the current directory changes to the root directory.
 *  If the NamingService is out, the exception ServiceUnreachable is thrown.
 * \param  Path the new current directory
 * \return true if the change succeeded
 */ 
// ============================================================================

bool SALOME_NamingService::Change_Directory(const char* Path) throw(ServiceUnreachable)
{
//   MESSAGE("BEGIN OF Change_Directory " << Path);
  Utils_Locker lock (&_myMutex);

  std::string path(Path);

  // --- if path empty, nothing to do

  if (path.empty())
    return true;

  // --- if path ='/', nothing to resolve, only change to root_context

  if (path == "/")
    {
//       MESSAGE("Change_Directory is called to go to the root_context");
      _current_context = _root_context;
      return true;
    }

  CosNaming::NamingContext_var current_context = _current_context;
  bool changeOK = false;

  // --- replace _current_context with _root_context if Path begins whith '/'

  if (path[0] == '/')
    current_context = _root_context;

  // --- need to resolve directory path

  ASSERT(!CORBA::is_nil(current_context));
  
  if (path[path.length()-1] != '/') path += '/';
//   SCRUTE(path);
  CosNaming::Name context_name;
  std::vector<std::string> splitPath;
  _createContextNameDir(path.c_str(),
                                                 context_name,
                                                 splitPath,
                                                 true);
  
  // --- Context creation
  
  try
    {
      CORBA::Object_var obj = current_context->resolve(context_name);
      current_context = CosNaming::NamingContext::_narrow(obj);
      ASSERT(!CORBA::is_nil(current_context));
      _current_context = current_context;
      changeOK = true;
    }
  
  catch (CosNaming::NamingContext::NotFound& ex)
    {
      CosNaming::Name n = ex.rest_of_name;
      
      if (ex.why == CosNaming::NamingContext::missing_node)
        MESSAGE( "Change_Directory() : " << (char *) n[0].id
               << " (" << (char *) n[0].kind << ") not found");
      if (ex.why == CosNaming::NamingContext::not_context)
        INFOS("Change_Directory() : " << (char *) n[0].id
              << " (" << (char *) n[0].kind
                  << ") is not a context" );
      if (ex.why == CosNaming::NamingContext::not_object)
        INFOS( "Change_Directory() : " << (char *) n[0].id
               << " (" << (char *) n[0].kind
               << ") is not an object" );
    }
  
  catch (CosNaming::NamingContext::CannotProceed&)
    {
      INFOS("Change_Directory(): CosNaming::NamingContext::CannotProceed");
    }
  
  catch (CosNaming::NamingContext::InvalidName&)
    {
      INFOS("Change_Directory(): CosNaming::NamingContext::InvalidName");
    }
  
  catch (CORBA::SystemException&)
    {
      INFOS("Change_Directory():CORBA::SystemException : unable to contact"
            << "the naming service");
      throw ServiceUnreachable();
    }

  return changeOK;
}

// ============================================================================
/*! \brief get the current directory path
 *
 *  Get the current directory path.
 *  If the NamingService is out, the exception ServiceUnreachable is thrown.
 * \return the path of the current_context
 * \sa  _current_directory
 */ 
// ============================================================================

char *SALOME_NamingService::Current_Directory() throw(ServiceUnreachable)
{
  MESSAGE("BEGIN OF Current_Directory");

  Utils_Locker lock (&_myMutex);

  CosNaming::NamingContext_var ref_context = _current_context;

  std::vector<std::string> splitPath;
  splitPath.resize(0);
  int lengthPath = 0;
  bool notFound = true ;

  // --- start search from root context

  _current_context = _root_context ;

  try
    {
      _current_directory(splitPath, lengthPath, ref_context, notFound );
    }

  catch (CORBA::SystemException&)
    {
      INFOS("Current_Directory(): CORBA::SystemException: unable to contact"
            << " the naming service" )
      throw ServiceUnreachable();
    }

  std::string path;
  lengthPath = splitPath.size();
  for (int k = 0 ; k < lengthPath ;k++)
    {
      path += "/";
      path += splitPath[k];
    }

  SCRUTE(path)
  _current_context = ref_context ;

  return strdup(path.c_str());
}

// ============================================================================
/*! \brief list recursively all objects in the current context
 *
 *  List and print via trace all directories and objects in the current
 *  context. Trace must be activated: compile option _DEBUG_
 *  If the NamingService is out, the exception ServiceUnreachable is thrown
 */ 
// ============================================================================

void SALOME_NamingService::list() throw(ServiceUnreachable)
{
  MESSAGE("Begin of list");

  Utils_Locker lock (&_myMutex)

    ;
  CosNaming::BindingList_var binding_list;
  CosNaming::BindingIterator_var binding_iterator;
  CosNaming::Binding_var binding ;

  unsigned long nb = 0 ; // --- only for the BindingIterator use,
                         //     to access the bindings

  CosNaming::NamingContext_var ref_context = _current_context;

  _current_context->list(nb, binding_list, binding_iterator) ;

  if (! CORBA::is_nil(binding_iterator))
    {
      while (binding_iterator->next_one(binding))
        {
          CosNaming::Name bindingName = binding->binding_name;

          if (binding->binding_type == CosNaming::ncontext)
            {
              MESSAGE( "Context : " << bindingName[0].id );

              try
                {
                  Change_Directory(bindingName[0].id);
                }

              catch (ServiceUnreachable&)
                {
                  INFOS( "list(): ServiceUnreachable" )
                    throw ServiceUnreachable();
                }

              list();
              _current_context = ref_context ;
            }

          else if (binding->binding_type == CosNaming::nobject)
            {
              MESSAGE( "Object : " << bindingName[0].id );
            }
        }

      binding_iterator->destroy();
    }
}

// ============================================================================
/*! \brief list all the objects in the current directory.
 *
 *  get a list of all the objects in the current directory, without recursion
 *  on the subdirectories. Only the objects are listed, not the directories.
 *  If the NamingService is out, the exception ServiceUnreachable is thrown.
 * \return list of strings with objects found.
 * \sa vector<string> list_directory_recurs()
 */ 
// ============================================================================

std::vector<std::string> SALOME_NamingService::list_directory() throw(ServiceUnreachable)
{
//   MESSAGE("list_directory");
  Utils_Locker lock (&_myMutex);
  std::vector<std::string> dirList ;
  dirList.resize(0);

  CosNaming::BindingList_var binding_list;
  CosNaming::BindingIterator_var binding_iterator;
  CosNaming::Binding_var binding ;

  unsigned long nb = 0 ; // --- only for the BindingIterator use,
                         //     to access the bindings

  CosNaming::NamingContext_var ref_context = _current_context;

  _current_context->list(nb, binding_list, binding_iterator);

  if (binding_iterator->_is_nil())
    return dirList;

  while (binding_iterator->next_one(binding))
    {
      CosNaming::Name bindingName = binding->binding_name;

      if (binding->binding_type == CosNaming::nobject)
        {
          // remove memory leak
          // dirList.push_back(CORBA::string_dup(bindingName[0].id));
          dirList.push_back(std::string(bindingName[0].id));
        }
    }

//   for (unsigned int ind = 0; ind < dirList.size(); ind++)
//     MESSAGE("list_directory : Object : " << dirList[ind]);

  binding_iterator->destroy();

  return dirList;
}


// ============================================================================
/*! \brief list all the subdirectories in the current directory.
 *
 *  get a list of all the subdirectories in the current directory,
 *  without recursion on the subdirectories.
 *  Only the subdirectories are listed, not the objects.
 *  If the NamingService is out, the exception ServiceUnreachable is thrown.
 * \return list of strings with directories found.
 * \sa vector<string> list_directory()
 */ 
// ============================================================================

std::vector<std::string> SALOME_NamingService::list_subdirs() throw(ServiceUnreachable)
{
  MESSAGE("list_subdirs");
  Utils_Locker lock (&_myMutex);
  std::vector<std::string> dirList ;
  dirList.resize(0);

  CosNaming::BindingList_var binding_list;
  CosNaming::BindingIterator_var binding_iterator;
  CosNaming::Binding_var binding ;

  unsigned long nb = 0 ; // --- only for the BindingIterator use,
                         //     to access the bindings

  CosNaming::NamingContext_var ref_context = _current_context;

  _current_context->list(nb, binding_list, binding_iterator) ;

  if (binding_iterator->_is_nil())
    return dirList;

  while (binding_iterator->next_one(binding))
    {
      CosNaming::Name bindingName = binding->binding_name;

      if (binding->binding_type == CosNaming::ncontext)
        {
          dirList.push_back(bindingName[0].id.in());
        }
    }

  for (unsigned int ind = 0; ind < dirList.size(); ind++)
    MESSAGE("list_directory : Object : " << dirList[ind]);

  binding_iterator->destroy();

  return dirList;
}

// ============================================================================
/*! \brief  list all the objects in the current directory and subdirectories.
 *
 *  get a list of all the objects in the current directory, with recursion
 *  on the subdirectories. Only the objects are listed, not the directories.
 *  If the NamingService is out, the exception ServiceUnreachable is thrown.
 * \return list of strings with objects found.
 * \sa vector<string> list_directory()
 */ 
// ============================================================================

std::vector<std::string> SALOME_NamingService::list_directory_recurs()
throw(ServiceUnreachable)
{
  MESSAGE("list_directory_recurs");

  Utils_Locker lock (&_myMutex);

  std::vector<std::string> dirList ;

  char* currentDir = Current_Directory();

  _list_directory_recurs(dirList, "", currentDir);

  free(currentDir);

  return dirList;
}

// ============================================================================
/*! \brief destroy an entry in naming service.
 *
 *  Destroy an association Path - Object Reference.
 *  If the NamingService is out, the exception ServiceUnreachable is thrown 
 * \param Path object path
 */ 
// ============================================================================

void SALOME_NamingService::Destroy_Name(const char* Path)
throw(ServiceUnreachable)
{
  MESSAGE("BEGIN OF Destroy_Name " << Path);

  Utils_Locker lock (&_myMutex);

  std::string path(Path);

  // --- if path empty, nothing to do

  if (path.empty())
    return;

  // --- if path = '/' not applicable, nothing to do

  if (path == "/")
    return;

  // --- if path begins with '/', set current directory to root context

  if (path[0] == '/')
    _current_context = _root_context;

  // --- context of the directory containing the object

  CosNaming::Name context_name;
  std::vector<std::string> splitPath;
  int dimension_resultat = _createContextNameDir(path.c_str(),
                                                 context_name,
                                                 splitPath,
                                                 true);

  bool exist = false;

  if (dimension_resultat > 0)
    {
      // --- path contains a directory, not only an object name
      //     switch to the new directory (or return if directory not found)

      try
        {
          CORBA::Object_var obj = _current_context->resolve(context_name);
          _current_context = CosNaming::NamingContext::_narrow(obj);
          exist = true;
        }

      catch (CosNaming::NamingContext::NotFound &ex)
        {
          // --- failed to resolve
          exist = false;

          CosNaming::Name n = ex.rest_of_name;

          if (ex.why == CosNaming::NamingContext::missing_node)
            INFOS( "Destroy_Name(): " << (char *) n[0].id
                   << " (" << (char *) n[0].kind << ") not found" );
          if (ex.why == CosNaming::NamingContext::not_context)
            INFOS( "Destroy_Name() : " << (char *) n[0].id
                   << " (" << (char *) n[0].kind
                   << ") is not a context" );
          if (ex.why == CosNaming::NamingContext::not_object)
            INFOS( "Destroy_Name() : " << (char *) n[0].id
                   << " (" << (char *) n[0].kind
                   << ") is not an object" );
        }

      catch (CosNaming::NamingContext::InvalidName &)
        {
          INFOS("Destroy_Name: CosNaming::NamingContext::InvalidName");
        }

      catch (CosNaming::NamingContext::CannotProceed &)
        {
          INFOS("Destroy_Name: CosNaming::NamingContext::CannotProceed");
        }

      catch (CORBA::SystemException&)
        {
          INFOS("Destroy_Name : CORBA::SystemException: "
                << "unable to contact the naming service");
          throw ServiceUnreachable();
        }

      if (! exist) return;
    }

  ASSERT(!CORBA::is_nil(_current_context));

  // --- The current directory is now the directory where the object should
  //     be destroyed

  int sizePath = splitPath.size();
  if (sizePath > dimension_resultat)
    {
      ASSERT(sizePath == dimension_resultat+1);
      context_name.length(1);

      try
        {
          // --- the last element is an object and not a directory

          context_name[0].id =
            CORBA::string_dup(splitPath[dimension_resultat].c_str());
          context_name[0].kind = CORBA::string_dup("object");
          SCRUTE(context_name[0].id);
 
          _current_context->unbind(context_name);
          MESSAGE("The object " << context_name[0].id << " has been deleted");
        }

      catch (CosNaming::NamingContext::NotFound& ex)
        {
          CosNaming::Name n = ex.rest_of_name;

          if (ex.why == CosNaming::NamingContext::missing_node)
            INFOS( "Destroy_Name() : " << (char *) n[0].id
                   << " (" << (char *) n[0].kind << ") not found" );
          if (ex.why == CosNaming::NamingContext::not_context)
            INFOS( "Destroy_Name() : " << (char *) n[0].id
                   << " (" << (char *) n[0].kind
                   << ") is not a context" );
          if (ex.why == CosNaming::NamingContext::not_object)
            INFOS( "Destroy_Name() : " << (char *) n[0].id
                   << " (" << (char *) n[0].kind
                   << ") is not an object" );
          }

      catch (CosNaming::NamingContext::CannotProceed&)
        {
          INFOS( "Destroy_Name(): CosNaming::NamingContext::CannotProceed");
        }

      catch (CosNaming::NamingContext::InvalidName&)
        {
          INFOS( "Destroy_Name(): CosNaming::NamingContext::InvalidName");
        }

      catch (CORBA::SystemException&)
        {
          INFOS( "Destroy_Name(): CORBA::SystemException: unable to contact"
                 << " the naming service");
          throw ServiceUnreachable();
        }
    }
}

// ============================================================================
/*! \brief Destroy an empty directory
 *
 *  Destroy an empty directory in Naming Service.
 *  If the NamingService is out, the exception ServiceUnreachable is thrown.
 * \param Path directory path
 */ 
// ============================================================================

void SALOME_NamingService::Destroy_Directory(const char* Path) throw(ServiceUnreachable)
{
  MESSAGE("BEGIN OF Destroy_Directory " << Path);

  Utils_Locker lock (&_myMutex);

  std::string path(Path);

  // --- if path empty, nothing to do

  if (path.empty())
    return;

  // --- if path begins with '/', set current directory to root context

  if (path[0] == '/')
    _current_context = _root_context;

  CosNaming::NamingContext_var ref_context = _current_context;

  // --- path must ends with '/' for a directory

  if (path[path.size() -1] != '/')
    path += '/';

  // --- context of the directory

  CosNaming::Name context_name;
  std::vector<std::string> splitPath;
  int dimension_resultat = _createContextNameDir(path.c_str(),
                                                 context_name,
                                                 splitPath,
                                                 true);
  bool exist = false;

  if (dimension_resultat > 0)
    {
      // --- path contains a directory, not only an object name
      //     switch to the new directory (or return if directory not found)

      try
        {
          CORBA::Object_var obj = _current_context->resolve(context_name);
          _current_context = CosNaming::NamingContext::_narrow(obj);
          exist = true;
        }

      catch (CosNaming::NamingContext::NotFound &ex)
        {
          // --- failed to resolve
          exist = false;

          CosNaming::Name n = ex.rest_of_name;

          if (ex.why == CosNaming::NamingContext::missing_node)
            INFOS( "Destroy_Directory(): " << (char *) n[0].id
                   << " (" << (char *) n[0].kind << ") not found" );
          if (ex.why == CosNaming::NamingContext::not_context)
            INFOS( "Destroy_Directory() : " << (char *) n[0].id
                   << " (" << (char *) n[0].kind
                   << ") is not a context" );
          if (ex.why == CosNaming::NamingContext::not_object)
            INFOS( "Destroy_Directory() : " << (char *) n[0].id
                   << " (" << (char *) n[0].kind
                   << ") is not an object" );
        }

      catch (CosNaming::NamingContext::InvalidName &)
        {
          INFOS("Destroy_Directory: CosNaming::NamingContext::InvalidName");
        }

      catch (CosNaming::NamingContext::CannotProceed &)
        {
          INFOS("Destroy_Directory: CosNaming::NamingContext::CannotProceed");
        }

      catch (CORBA::SystemException&)
        {
          INFOS("Destroy_Directory : CORBA::SystemException: "
                << "unable to contact the naming service");
          throw ServiceUnreachable();
        }

      if (! exist) return;
    }

  ASSERT(!CORBA::is_nil(_current_context));

  // --- Context Destruction

  bool isContextDestroyed = false;
  try
    {
      _current_context->destroy();
      MESSAGE( "The context " << path << " has been deleted" );
      isContextDestroyed = true;
    }

  catch (CosNaming::NamingContext::NotEmpty&)
    {
      INFOS( "Destroy_Directory(): CosNaming::NamingContext::NoEmpty "
             << path << " is not empty" );
    }

  catch (CORBA::SystemException&)
    {
      INFOS( "Destroy_Directory():CORBA::SystemException : "
             << "unable to contact the naming service");
      throw ServiceUnreachable();
    }

  // --- go to the reference directory

  _current_context = ref_context ;

  ASSERT(!CORBA::is_nil(_current_context));

  if (isContextDestroyed)
    {
      try
        {
          _current_context->unbind(context_name);
          MESSAGE( "The bind to the context "
                   << context_name[0].id
                   << " has been deleted" );
        }

      catch (CosNaming::NamingContext::NotFound& ex)
        {
          CosNaming::Name n = ex.rest_of_name;

          if (ex.why == CosNaming::NamingContext::missing_node)
            INFOS( "Destroy_Directory() : " << (char *) n[0].id
                   << " (" << (char *) n[0].kind << ") not found" );
          if (ex.why == CosNaming::NamingContext::not_context)
            INFOS( "Destroy_Directory() : " << (char *) n[0].id
                   << " (" << (char *) n[0].kind
                   << ") is not a context" );
          if (ex.why == CosNaming::NamingContext::not_object)
            INFOS( "Destroy_Directory() : " << (char *) n[0].id
                   << " (" << (char *) n[0].kind
                   << ") is not an object" );
        }

      catch (CosNaming::NamingContext::CannotProceed&)
        {
          INFOS("Destroy_Directory: CosNaming::NamingContext::CannotProceed");
        }

      catch (CosNaming::NamingContext::InvalidName&)
        {
          INFOS("Destroy_Directory: CosNaming::NamingContext::InvalidName");
            }

      catch (CORBA::SystemException&)
        {
          INFOS("Destroy_Directory:CORBA::SystemException : unable to contact"
                 << " the naming service");
          throw ServiceUnreachable();
        }
    }
}

// ============================================================================
/*! \brief Destroy a directory with its contents.
 *
 *  Destroy the objects associations in a directory, and the directory itself,
 *  if there is no subdirectories. 
 *  If the NamingService is out, the exception ServiceUnreachable is thrown.
 * \param Path the directory path.
 */ 
// ============================================================================

void SALOME_NamingService::Destroy_FullDirectory(const char* Path) throw(ServiceUnreachable)
{
  MESSAGE("begin of Destroy_FullDirectory " << Path);
  //no need to lock here because method calls are threadsafe.
  if( Change_Directory(Path) )
    {
      std::vector<std::string> contList = list_directory();

      for (unsigned int ind = 0; ind < contList.size(); ind++)
        Destroy_Name(contList[ind].c_str());
      
      Destroy_Directory(Path);
    }
}

// ============================================================================
/*! \brief  initialize root context (root directory)
 *
 * the root context initialisation must be done when the SALOME_NamingService
 * instance is created and before any othe call. See constructors.
 */ 
// ============================================================================

void SALOME_NamingService::_initialize_root_context()
{
  //MESSAGE("Get the root context");
  //no lock here because initialization is expected to be done once.
  try
    {
      CORBA::Object_var obj = _orb->resolve_initial_references("NameService");
      _root_context = CosNaming::NamingContext::_narrow(obj);
      _current_context = _root_context ;
      ASSERT(!CORBA::is_nil(_root_context));
    }

  catch (CORBA::SystemException&)
    {
      INFOS("CORBA::SystemException: unable to contact the naming service");
      throw ServiceUnreachable();
    }

  catch (...)
    {
      INFOS("Unknown Exception: unable to contact the naming service");
      throw ServiceUnreachable();
    }
}

// ============================================================================
/*! \brief transform a string path in CosNaming structure.
 *
 *  Transform a path given as a string in a CosNaming structure.
 *  \param path         a relative or absolute path, with or without an object.
 *                      An absolute path begins with '/'.
 *                      A path without an object ends with '/'.
 *  \param context_name CosNaming structure to put the path.
 *  \param splitPath    a vector of string with subdirectories and final
 *                      object, if any.
 *  \param onlyDir      if true, final object (if any) is ommited
 *                      in context_name.
 *                      if false, final object (if any) is included in
 *                      context_name.
 *  \return             dimension of context_name
 */ 
// ============================================================================

int
SALOME_NamingService::_createContextNameDir(std::string path,
                                            CosNaming::Name& context_name,
                                            std::vector<std::string>& splitPath,
                                            bool onlyDir)
{
  if (path.empty())
    return 0;

  std::string::size_type begIdx, endIdx;
  const std::string delims("/");
  splitPath.resize(0);
  bool endWithDelim = false;

  begIdx = path.find_first_not_of(delims);
  while (begIdx != std::string::npos)
    {
      endIdx = path.find_first_of(delims, begIdx);
      if (endIdx == path.length()-1)
        endWithDelim = true;
      if (endIdx == std::string::npos)
        endIdx = path.length();
      int lsub = endIdx - begIdx;
      if (lsub >= 1)
        splitPath.push_back(path.substr(begIdx, lsub));
      begIdx = path.find_first_not_of(delims, endIdx);
    }

  int dim;
  if (onlyDir)                  // only directory part
    {
      dim = splitPath.size()-1; // omit final object
      if (endWithDelim)         // unless the path ends with a delimiter 
        dim++;
      endWithDelim = true;
    }
  else
    dim = splitPath.size();     // directories and final object

  context_name.length(dim);
  for (int i=0; i<dim; i++)
    {
//       SCRUTE(splitPath[i]);
      context_name[i].id = CORBA::string_dup(splitPath[i].c_str());
      if (!endWithDelim && (i == dim-1)) // here, the last string is an object
        {
          context_name[i].kind = CORBA::string_dup("object");
//        MESSAGE("--- " <<splitPath[i] <<".object");
        }
      else
        {
          context_name[i].kind = CORBA::string_dup("dir");
//        MESSAGE("--- " <<splitPath[i] <<".dir");
        }
    }
  return dim;
}

// ============================================================================
/*! \brief search a name in current directory.
 *
 *  Search a name in the current directory. after call, the current directory
 *  is changed to the directory containing the last occurence of name found.
 *  If no occurence found (see return value), current directory remains
 *  unchanged. The call is recursive.
 *
 * \param  name the name to search.
 * \param  occurence_number number of occurence already found (incremented)
 */ 
// ============================================================================

void SALOME_NamingService::_Find(const char* name,
                                 CORBA::Long& occurence_number)
{
  MESSAGE("BEGIN OF _Find "<<  occurence_number << " " << name);

  CosNaming::BindingList_var binding_list;
  CosNaming::BindingIterator_var binding_iterator;
  CosNaming::Binding_var binding;

  unsigned long nb = 0 ; // --- only for the use of the BindingIterator,
                         //     to access the bindings

  CosNaming::NamingContext_var ref_context = _current_context;
  CosNaming::NamingContext_var found_context = _current_context;

  _current_context->list(nb, binding_list, binding_iterator) ;

  if (! CORBA::is_nil(binding_iterator))
    {
      while (binding_iterator->next_one(binding))
        {
          CosNaming::Name bindingName = binding->binding_name;
          
          if (binding->binding_type == CosNaming::ncontext)
            {
              // --- We work on a directory,
              //     the search should be done in this directory
              
              Change_Directory(bindingName[0].id);
              _Find(name, occurence_number);
              
              // --- We'll go back to the initial context
              
              _current_context = ref_context ;
            }
          
          else if (binding->binding_type == CosNaming::nobject)
            {
              // --- We work on an object...
              
              if (!strcmp( bindingName[0].id, name))
                {
                  //MESSAGE("One occurence was found");
                  occurence_number++;
                  
                  // --- We keep in memory the directory where
                  //     one occurence was found
                  
                  found_context = _current_context ;
                }
            }
        }
      
      binding_iterator->destroy();
    }
  // --- We go to the last directory where an occurence was found

  _current_context = found_context;

  SCRUTE(occurence_number);
}

// ============================================================================
/*! \brief find the current directory path.
 * 
 *  Parse the naming service tree to find the current context and give the
 *  associated directory path (relative to root context).
 * \param splitPath 
 * \param lengthResult
 * \param contextToFind
 * \param notFound
 */ 
// ============================================================================

void
SALOME_NamingService::
_current_directory(std::vector<std::string>& splitPath,
                   int& lengthResult,
                   CosNaming::NamingContext_var contextToFind,
                   bool& notFound)
{
  MESSAGE("BEGIN OF _current_Directory");

  CosNaming::BindingList_var binding_list;
  CosNaming::BindingIterator_var binding_iterator;
  CosNaming::Binding_var binding;

  unsigned long nb = 0 ; // --- only for the BindingIterator use,
                         //     to access the bindings

  CosNaming::NamingContext_var ref_context = _current_context;
  CosNaming::NamingContext_var temp_context = _current_context;

  _current_context->list(nb, binding_list, binding_iterator);

  if ( !binding_iterator->_is_nil() )
    {
      while ((binding_iterator->next_one(binding)) && notFound)
        {
          CosNaming::Name bindingName = binding->binding_name;

          if (binding->binding_type == CosNaming::ncontext)
            {
              // --- directory, search in it

              const char* bindingNameid=bindingName[0].id;
              splitPath.push_back(bindingNameid);
              lengthResult++;

              CORBA::Object_var obj = _current_context->resolve(bindingName);
              temp_context = CosNaming::NamingContext::_narrow(obj);

              if (temp_context->_is_equivalent(contextToFind))
                {
                  MESSAGE("The context is found, we stop the search");
                  notFound = false;
                  SCRUTE(notFound);
                }

              if (notFound)
                {
                  SCRUTE(bindingName[0].id);
                  Change_Directory(bindingName[0].id);
                  _current_directory(splitPath,
                                     lengthResult,
                                     contextToFind,
                                     notFound);

                  if (notFound)
                    {
                      // --- go back to the initial context

                      _current_context = ref_context;

                      MESSAGE("Just before the delete of "
                              << splitPath[lengthResult-1]);
                      splitPath.pop_back();
                      lengthResult--;
                    }
                }
            }
        }

      binding_iterator->destroy();
    }

  // --- return to the last directory where an occurence was found

  _current_context = ref_context ;
}


// ============================================================================
/*! \brief list recursively all objects in the given directory and subdirs.
 *
 *  get a list of all the objects in the current directory, with recursion
 *  on the subdirectories. Only the objects are listed, not the directories.
 *  If the NamingService is out, the exception ServiceUnreachable is thrown.
 *  _current_context must refer to absCurDirectory.
 *
 *  \param myList          The list that will be filled.
 *  \param relativeSubDir  The directory relative to absCurDirectory in which
 *                         the objects are found.
 *  \param absCurDirectory The current directory, absolute path
 */ 
// ============================================================================

void SALOME_NamingService::_list_directory_recurs(std::vector<std::string>& myList,
                                                  std::string relativeSubDir,
                                                  std::string absCurDirectory)
{
  CosNaming::BindingList_var binding_list;
  CosNaming::BindingIterator_var binding_iterator;
  CosNaming::Binding_var binding ;

  unsigned long nb = 0 ; // --- only for thethe use of BindingIterator
                         //     to access the bindings

  std::string absDir;

  CosNaming::NamingContext_var ref_context = _current_context;

  if (! relativeSubDir.empty())
    {
      Change_Directory(relativeSubDir.c_str());
      absDir = absCurDirectory + "/" + relativeSubDir;
    }

  else
    absDir = absCurDirectory;

  SCRUTE(absDir);
  _current_context->list(nb, binding_list, binding_iterator) ;

  if (! CORBA::is_nil(binding_iterator))
    {
      while (binding_iterator->next_one(binding))
        {
          CosNaming::Name bindingName = binding->binding_name;

          if (binding->binding_type == CosNaming::ncontext)
            {
              std::string relativeSdir(bindingName[0].id);
              _list_directory_recurs(myList, relativeSdir, absDir);
            }

          else if (binding->binding_type == CosNaming::nobject)
            {
              std::string objName(bindingName[0].id);
              std::string elt = absDir + "/" + objName;
              SCRUTE(elt);
              myList.push_back(elt);
            }
        }

      binding_iterator->destroy();
    }
  if (! relativeSubDir.empty())
    {
      _current_context = ref_context;
    }
}

// ============================================================================
/*! \brief return a stringified reference of root context
 *
 * \return a stringified reference of root context
 */
// ============================================================================

char * SALOME_NamingService::getIORaddr()
{
  return _orb->object_to_string(_root_context);
}

/*! \brief get the orb used by the naming service
 *
 *  \return the orb
 */
CORBA::ORB_ptr SALOME_NamingService::orb()
{
  return _orb;
}

