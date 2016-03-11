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
//  File   : SALOME_Logger_Server.cxx
//  Author : Vasily Rusyaev
//  Module : SALOME
//
#include <iostream>
#include "SALOME_Logger_Server.hxx"
#include <SALOMEconfig.h>
#include <sys/types.h>
#ifndef WIN32
# include <unistd.h>
#endif

#ifdef WIN32
#include <omnithread/pthread_nt.h>
#endif

omni_mutex Logger::myLock;

/////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Logger::Logger()
{
  m_putIntoFile = false;
}

Logger::Logger(const char *filename)
{
  //  m_outputFile.open( filename, ios::out | ios::trunc , filebuf::openprot);
  m_outputFile.open( filename, std::ios::out | std::ios::trunc);
  if (m_outputFile.is_open())
    m_putIntoFile = true;
  else
    m_putIntoFile = false;
}

Logger::~Logger()
{
  if (m_putIntoFile)
    m_outputFile.close();
}

void Logger::putMessage(const char* message)
{
  myLock.lock();
  if (m_putIntoFile)

        m_outputFile << message << std::flush;
  else
    std::cout << message;
  myLock.unlock();
}

void Logger::ping()
{
  //cout<<" Logger::ping() pid "<< getpid()<<endl;
}
