//  SALOME Logger : CORBA server managing trace output
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOME_Logger_Server.cxx
//  Author : Vasily Rusyaev
//  Module : SALOME

#include <iostream>
#include "SALOME_Logger_Server.hxx"
#include <SALOMEconfig.h>
#include <sys/types.h>
#ifndef __WIN32__
# include <unistd.h>
#endif

#ifdef WNT
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
