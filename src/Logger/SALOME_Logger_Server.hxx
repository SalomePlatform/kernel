//=============================================================================
// File      : SALOME_Logger_Server.hxx
// Created   : nov 18 10:28:17 2002
// Author    : Vasily Rusyaev
// Project   : SALOME/PRO
//=============================================================================

// SALOME_Logger_Server.hxx: interface for the SALOME_Logger_Server class.
//
//////////////////////////////////////////////////////////////////////

#if !defined SALOME_Logger_Server_include
#define SALOME_Logger_Server_include

#include <fstream.h>
#include <omnithread.h>
#include "Logger.hh"

class Logger :
  public POA_SALOME_Logger::Logger,
  public PortableServer::RefCountServantBase 
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
private:
	//if m_putIntoFile is true all messages will be put into special 
	//otherwise all messages will be put into terminal via cout 
        bool m_putIntoFile;
	//ofstream class specialized for disk file output
        ofstream m_outputFile; 
	//synchronisation object
	static omni_mutex myLock;
};

#endif // !defined(SALOME_Logger_Server_include)
