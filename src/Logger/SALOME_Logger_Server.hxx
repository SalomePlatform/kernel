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

//  SALOME Logger : CORBA server managing trace output
//  File   : SALOME_Logger_Server.hxx
//  Author : Vasily Rusyaev
//  Module : SALOME
//
#if !defined SALOME_Logger_Server_include
#define SALOME_Logger_Server_include

#include <SALOMEconfig.h>
#include <fstream>

#ifdef WIN32
#include <iosfwd>
#endif
#include <omnithread.h>
#include "Logger.hh"

#ifdef WIN32
# if defined LOGGER_EXPORTS || defined SalomeLoggerServer_EXPORTS
#  define LOGGER_EXPORT __declspec( dllexport )
# else
#  define LOGGER_EXPORT __declspec( dllimport )
# endif
#else
# define LOGGER_EXPORT
#endif

#ifdef WIN32
#pragma warning(disable:4275) // Disable warning interface non dll
#endif

class LOGGER_EXPORT Logger :
  public POA_SALOME_Logger::Logger
{
public:
        //constructor w/o parameters
        //all messages will be put into terminal via cout
        Logger();
        //constructor with parameter, filename is output file
        //all messages will be put into special file passed as parameter
        Logger(const char *filename);
        virtual ~Logger();
        //put message into one special place for all servers
        void putMessage(const char* message);
        void ping();
        void SetOrb( CORBA::ORB_ptr orb ) { _orb = CORBA::ORB::_duplicate(orb); return; }
        void shutdown() { if(!CORBA::is_nil(_orb)) _orb->shutdown(0); };  
private:
        //if m_putIntoFile is true all messages will be put into special 
        //otherwise all messages will be put into terminal via cout 
        bool m_putIntoFile;
        //ofstream class specialized for disk file output
        std::ofstream m_outputFile; 

        //synchronisation object
        static omni_mutex myLock;

        CORBA::ORB_ptr _orb;
};

#endif // !defined(SALOME_Logger_Server_include)
