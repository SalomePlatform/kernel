//  SALOME Logger : CORBA server managing trace output
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOME_Logger.cxx
//  Author : Vasily Rusyaev
//  Module : SALOME

using namespace std;
#include "SALOME_Trace.hxx"
#include <memory.h>
#include <string>
//#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SALOME_Trace::SALOME_Trace()
{
  //get reference on object reference from NS
  //and initialize m_pInterfaceLogger 

  int argc = 1;
  char* argv[1] = {"application"};
  try
    {
      //NB. You can't use SALOME_NamingService class because it uses MESSAGE macro
      //Otherwise, you will get segmentation fault.   

      //Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init(argc,argv) ;
      long TIMESleep = 250000000;
      int NumberOfTries = 40;
      int a;
      timespec ts_req;
      ts_req.tv_nsec=TIMESleep;
      ts_req.tv_sec=0;
      timespec ts_rem;
      ts_rem.tv_nsec=0;
      ts_rem.tv_sec=0;
      CosNaming::NamingContext_var inc;
      CORBA::Object_var theObj;
      CORBA::Object_var obj;
      CosNaming::Name name;
      name.length(1);
      name[0].id = CORBA::string_dup("Logger");
      for (int i = 1; i<=NumberOfTries; i++){
	if (i!=1) 
	    a=nanosleep(&ts_req,&ts_rem);
	  try{ 
	    if(!CORBA::is_nil(orb)) 
	      theObj = orb->resolve_initial_references("NameService");
	    if (!CORBA::is_nil(theObj))
	      inc = CosNaming::NamingContext::_narrow(theObj);
	  }  
	  catch( CORBA::COMM_FAILURE& )
	    {
	      cout<<"SALOME_TRACE: CORBA::COMM_FAILURE: Unable to contact the Naming Service" <<endl;
	    }
          catch(...){ cout<< "SALOME_TRACE: Unknown exception dealed with Naming Service" <<endl; }
	  
	  if(!CORBA::is_nil(inc)) {
	    obj = inc->resolve(name);
	    m_pInterfaceLogger = SALOME_Logger::Logger::_narrow(obj);
	    if (!CORBA::is_nil(m_pInterfaceLogger))
	      cout<<"SALOME_TRACE: Logger Server was found"<<endl;
	    break;
	    
	  }
      }
	   
    }
      catch (const CosNaming::NamingContext::NotFound&)
	{
	  //       cout << "Caught exception: Naming Service can't found Logger";
	}
      catch (CORBA::COMM_FAILURE&)
	{
	  //       cout << "Caught CORBA::SystemException CommFailure.";
    }
  catch (CORBA::SystemException&)
    {
//       cout << "Caught CORBA::SystemException.";
    }
  catch (CORBA::Exception&)
    {
//       cout << "Caught CORBA::Exception.";
    }
  catch (...)
    {
//       cout << "Caught unknown exception.";
    }
  //cerr << "-----SALOME_Trace::SALOME_Trace----"<<endl;
}

SALOME_Trace::~SALOME_Trace()
{
}

SALOME_Trace& SALOME_Trace::Instance()
{
	static SALOME_Trace instance;
	return instance;
}

void SALOME_Trace::putMessage(ostream& msg)
{
  //write resulting string into Logger CORBA server
  //concatenate string from passing parameters for transfering into Logger CORBA server

  //cerr << "-+- " << msg << " ";

  //   CORBA::String_var LogMsg = CORBA::string_dup( str() );
  //Allow automatic deletion of ostrstream content 
  char* adt = str();
  CORBA::String_var LogMsg = CORBA::string_dup( adt );
  rdbuf()->freeze(false);
  //rdbuf()->sync(); // problem with gcc3.2
  seekp(0);

  if (CORBA::is_nil(m_pInterfaceLogger))
    cout << LogMsg;
  else
    m_pInterfaceLogger-> putMessage (LogMsg) ;
}

