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
#include <unistd.h>
using namespace std;

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
  m_outputFile.open( filename, ios::out | ios::trunc);
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
    m_outputFile << message << flush;
  else
    cout << message;
  myLock.unlock();
}

void Logger::ping()
{
  //cout<<" Logger::ping() pid "<< getpid()<<endl;
}

int main(int argc, char **argv)
{
  if (argc > 2)
    {
      cout << "usage: SALOME_Logger_Server [output_file]" << endl;
      exit(1);
    }
  try
    {
      //Initialize the ORB
      const long TIMESleep = 500000000;
      const int NumberOfTries = 40;
      int i;
      timespec ts_req = {0, TIMESleep};
      timespec ts_rem = {0, 0};
      CosNaming::NamingContext_var inc;
      SALOME_Logger::Logger_var myLoggerRef;
      CORBA::Object_var theObj;
      Logger* myLogger;
      CORBA::Object_var obj;
      PortableServer::POA_var poa;
      PortableServer::POAManager_var pman;   

      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv) ;

      for (int i = 1; i <= NumberOfTries; i++){        
	if (i != 1) nanosleep(&ts_req, &ts_rem);
	try {
	  obj = orb->resolve_initial_references("RootPOA") ;
	  if(!CORBA::is_nil(obj))
	    poa = PortableServer::POA::_narrow(obj) ;
	  pman = poa->the_POAManager();
	  // NB. You can activate the POA before or after
	  // activating objects in that POA.
	  
	  // This activates the object in the root POA (by default), and
	  // returns a reference to it.
	  //NB. You can't use SALOME_NamingService class because it uses MESSAGE macro
	  //Otherwise, you will get segmentation fault.   
	  //Get initial naming context
	  if(!CORBA::is_nil(orb)) 
	    theObj = orb->resolve_initial_references("NameService");
	  //Narrow to NamingContext
	  if (!CORBA::is_nil(theObj))
	    inc = CosNaming::NamingContext::_narrow(theObj);
	} catch(CORBA::COMM_FAILURE&) {
	  //cout<<"Logger Server: CORBA::COMM_FAILURE: Unable to contact the Naming Service"<<endl;
	}
	catch(...) {
	  //cout<<"Logger Server: Unknown exception dealed with Naming Service" <<endl;
	}
	if (!CORBA::is_nil(inc)) {
//	  cout<<"Logger Server: Naming Service was found"<<endl; 
	  break;
	}
      }
      if (argc == 1)
	myLogger = new Logger();
      else
	myLogger = new Logger(argv[1]);


      myLoggerRef = myLogger->_this();
      CosNaming::Name name;
      name.length(1);
      name[0].id = CORBA::string_dup("Logger");
      inc->bind(name,myLoggerRef);
      myLogger->_remove_ref();
      pman->activate();   
      orb->run() ;
      orb->destroy() ;
    }
  catch(CORBA::COMM_FAILURE& ex) 
    {
      cerr << "Caught system exception COMM_FAILURE -- unable to contact the "
	   << "object." << endl;
    }
  catch(CORBA::SystemException&) 
    {
      cerr << "Caught CORBA::SystemException." << endl;
    }
  catch(CORBA::Exception&) 
    {
      cerr << "Caught CORBA::Exception." << endl;
    }
  catch(omniORB::fatalException& fe) 
    {
      cerr << "Caught omniORB::fatalException:" << endl;
      cerr << "  file: " << fe.file() << endl;
      cerr << "  line: " << fe.line() << endl;
      cerr << "  mesg: " << fe.errmsg() << endl;
    }
  catch(...) 
    {
      cerr << "Caught unknown exception." << endl;
    }
}
