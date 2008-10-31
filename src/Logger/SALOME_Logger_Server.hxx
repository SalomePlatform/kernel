//  SALOME Logger : CORBA server managing trace output
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOME_Logger_Server.hxx
//  Author : Vasily Rusyaev
//  Module : SALOME

#if !defined SALOME_Logger_Server_include
#define SALOME_Logger_Server_include

#include <SALOMEconfig.h>
#include <fstream>

#ifdef WNT
#include <iosfwd>
#endif
#include <omnithread.h>
#include "Logger.hh"

#ifdef WIN32
# if defined LOGGER_EXPORTS
#  define LOGGER_EXPORT __declspec( dllexport )
# else
#  define LOGGER_EXPORT __declspec( dllimport )
# endif
#else
# define LOGGER_EXPORT
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
private:
	//if m_putIntoFile is true all messages will be put into special 
	//otherwise all messages will be put into terminal via cout 
        bool m_putIntoFile;
	//ofstream class specialized for disk file output
	std::ofstream m_outputFile; 

	//synchronisation object
	static omni_mutex myLock;
};

#endif // !defined(SALOME_Logger_Server_include)
