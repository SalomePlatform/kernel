// Copyright (C) 2007-2024  CEA, EDF, OPEN CASCADE
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

//  SALOME Utils : general SALOME's definitions and tools
//  File   : Utils_ORB_INIT.cxx
//  Author : Antoine YESSAYAN, EDF
//  Module : SALOME
//  $Header$
//
# include "Utils_ORB_INIT.hxx" 
# include "Utils_SALOME_Exception.hxx"
# include "utilities.h" 
# include "ArgvKeeper.hxx"

# include "SALOMEconfig.h"

# include <string>
# include <vector>
#include <mutex>

ORB_INIT::ORB_INIT( void ): _orb( CORBA::ORB::_nil() )
{
}

namespace
{
  std::recursive_mutex mutex; //!< mutex to protect access to static data
}

ORB_INIT::~ORB_INIT()
{
  if ( ! CORBA::is_nil( _orb ) )
  {
    MESSAGE("WARNING: orb destroy is no more called at exit. Use explicit call.");
    //std::cerr << "appel _orb->destroy()" << std::endl;
    /*
    try {
      _orb->destroy() ;
    }
    catch(...) {
      MESSAGE("Caught CORBA::Exception.");
    }
    */
    //std::cerr << "retour _orb->destroy()" << std::endl;
  }
}

void ORB_INIT::explicit_destroy()
{
  if ( ! CORBA::is_nil( _orb ) )
    {
    try {
          _orb->destroy() ;
          _orb = CORBA::ORB::_nil();
        }
        catch(...) {
          MESSAGE("Caught CORBA::Exception.");
        }
    }
}

CORBA::ORB_var &ORB_INIT::operator() ()
{
  try {
    std::lock_guard<std::recursive_mutex> g(mutex);  
    if ( CORBA::is_nil( _orb ) )
      {
        try
          {
            if (!ArgcArgvInitialized())
            {
              MESSAGE("WARNING: ORB_INIT(): argc and argv are not initialized");
            }
            std::vector<std::string> args = GetArgcArgv();
            int argc = args.size();
            char** argv = argc > 0 ? new char*[argc] : nullptr;
            for (int i = 0; i < argc; ++i) {
              argv[i] = new char[args.at(i).size()+1];
              strcpy(argv[i], args.at(i).c_str());
            }
            _orb = CORBA::ORB_init( argc, argv, "omniORB4" ) ;
            for (int i = 0; i < argc; ++i)
              delete[] argv[i];
            if (argc>0)
              delete[] argv;
          }
        catch( const CORBA::Exception & )
          {
            throw CommException( "Unable to create an ORB connexion" ) ;
          }
      }
    return _orb ;
  } catch ( CommException& e) {throw e;}
  catch (...) { throw CommException( "ORB_INIT::operator() : Unknown exception was caught" ) ;}
}
