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

//  File   : SALOME_FileRef_i.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$
//
#include "SALOME_FileRef_i.hxx"
#include "utilities.h"
#include "Basics_Utils.hxx"
#include <string>

//=============================================================================
/*! 
 *  Default constructor, not for use
 */
//=============================================================================

fileRef_i::fileRef_i()
{
  ASSERT(0);
}
//=============================================================================
/*! 
 *  Constructor to use
 */
//=============================================================================

fileRef_i::fileRef_i(Engines::Container_ptr container,
                     const char* origFileName)
{
  MESSAGE("fileRef_i::fileRef_i "<< origFileName);
  _container =  Engines::Container::_duplicate(container);
  _origFileName = origFileName;
  _machine = Kernel_Utils::GetHostname();
#if defined(_DEBUG_) || defined(_DEBUG)
  int OK = addRef(_machine.c_str(), _origFileName.c_str());
  SCRUTE(OK);
#else
  addRef(_machine.c_str(), _origFileName.c_str());
#endif
}


//=============================================================================
/*! 
 *  Destructor
 */
//=============================================================================

fileRef_i::~fileRef_i()
{
  MESSAGE("fileRef_i::~fileRef_i");
}

//=============================================================================
/*! 
 *  CORBA method: 
 *  \return the file name (absolute path) on the computer which runs the
 *          container server
 */
//=============================================================================

char* fileRef_i::origFileName()
{
  MESSAGE("fileRef_i::origFileName " << _origFileName);
  return CORBA::string_dup(_origFileName.c_str());
}

//=============================================================================
/*! 
 *  CORBA method: 
 *  \return the hostname of the computer which runs the container server
 */
//=============================================================================

char* fileRef_i::refMachine()
{
  MESSAGE("fileRef_i::refMachine " << _machine);
  return CORBA::string_dup(_machine.c_str());
}

//=============================================================================
/*! 
 *  CORBA method: 
 *  \return the container reference
 */
//=============================================================================

Engines::Container_ptr fileRef_i::getContainer()
{
  MESSAGE("fileRef_i::getContainer");
 Engines::Container_var theContainer = 
   Engines::Container::_duplicate(_container);
 return theContainer._retn();
}

//=============================================================================
/*! 
 *  CORBA method: after a file transfer on a client computer, registers 
 *  hostname of client and file path of the copied file.
 *  \param machine client hostname
 *  \param fileName asolute path of the transfered file on client computer
 */
//=============================================================================

CORBA::Boolean fileRef_i::addRef(const char* machine,
                                 const char* fileName)
{
  MESSAGE("fileRef_i::addRef " << machine << " " << fileName);
  std::string theMachine = machine;
  std::string theFileName = fileName;

  if (theFileName[0] != '/')
    {
      INFOS("destination file path must be absolute, begining with '/'");
      return 0;
    }

  if (theMachine.empty())
    {
      INFOS("provide a hostname for the copy destination");
      return 0;
    }

  if (! _copies[theMachine].empty())
    {
      INFOS("there is already a copy on " << theMachine << " under the path "
            <<  _copies[theMachine] << " new ref not added! ");
      return 0;
    }

  _copies[theMachine] = theFileName;
  return 1;
}

//=============================================================================
/*! 
 *  CORBA method: check if a copy of the file referenced by fileRef is
 *  available on the client computer.
 *  \param  machine hostname of the client computer
 *  \return path of the copy on the client computer, if the copy exists,
 *          else empty string
 */
//=============================================================================

char* fileRef_i::getRef(const char* machine)
{
  MESSAGE("fileRef_i::getRef "<< machine);
  std::string theMachine = machine;
  std::string theFileName = _copies[theMachine];
  if (_copies[theMachine].empty())
    {
      MESSAGE("no copy of " << _machine << _origFileName << " available on "
              << theMachine);
    }
  else
    {
      MESSAGE("a copy of " << _machine << _origFileName << "is available on "
              << theMachine << _copies[theMachine]);
    }
  return CORBA::string_dup(_copies[theMachine].c_str());
}

