using namespace std;
//=============================================================================
// File      : SALOME_Logger_Server.cxx
// Created   : nov 18 10:28:17 2002
// Author    : Vasily Rusyaev
// Project   : SALOME/PRO
//=============================================================================

// SALOME_Logger_Server.cxx: implementation of the SALOME_Logger_Server class.
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include "SALOME_Logger_Server.hxx"

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
      CORBA::ORB_var orb = CORBA::ORB_init(argc,argv) ;

      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA") ;
      PortableServer::POA_var poa = PortableServer::POA::_narrow(obj) ;

      // NB. You can activate the POA before or after
      // activating objects in that POA.
      PortableServer::POAManager_var pman = poa->the_POAManager();
      pman->activate();

      Logger* myLogger;
      if (argc == 1)
	myLogger = new Logger();
      else
	myLogger = new Logger(argv[1]);


      // This activates the object in the root POA (by default), and
      // returns a reference to it.
      SALOME_Logger::Logger_var myLoggerRef = myLogger->_this();

       //NB. You can't use SALOME_NamingService class because it uses MESSAGE macro
      //Otherwise, you will get segmentation fault.   

      //Get initial naming context
      CORBA::Object_var theObj = orb->resolve_initial_references("NameService");
      //Narrow to NamingContext
      CosNaming::NamingContext_var inc = CosNaming::NamingContext::_narrow(theObj);

      CosNaming::Name name;
      name.length(1);
      name[0].id = CORBA::string_dup("Logger");
      
      inc->bind(name,myLoggerRef);

      myLogger->_remove_ref();

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
