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
      //Get initial naming context
      CORBA::Object_var theObj = orb->resolve_initial_references("NameService");
      //Narrow to NamingContext
      CosNaming::NamingContext_var inc = CosNaming::NamingContext::_narrow(theObj);

      CosNaming::Name name;
      name.length(1);
      name[0].id = CORBA::string_dup("Logger");
      
      CORBA::Object_var obj;
      obj = inc->resolve(name);
      
      m_pInterfaceLogger = SALOME_Logger::Logger::_narrow(obj) ;

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

