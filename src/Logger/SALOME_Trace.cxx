//  SALOME Logger : CORBA server managing trace output
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOME_Logger.cxx
//  Author : Vasily Rusyaev
//  Module : SALOME

#include "SALOME_Trace.hxx"
#include <memory.h>
#include <string>
//#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SALOME_Trace::SALOME_Trace()
{
  isInitialized = 0;
}

SALOME_Trace::~SALOME_Trace()
{
}

SALOME_Trace& SALOME_Trace::Instance()
{
	static SALOME_Trace instance;
	return instance;
}

int SALOME_Trace::Initialize(CORBA::ORB_ptr theOrb) {
  //get reference on object reference from NS
  //and initialize m_pInterfaceLogger 
  if (isInitialized && !CORBA::is_nil(m_pInterfaceLogger))
    return 1;

  const long TIMESleep = 250000000;
  const int NumberOfTries = 40;
  int i;
  timespec ts_req = {0, TIMESleep};
  timespec ts_rem = {0, 0};

  CosNaming::NamingContext_var inc;
  CORBA::Object_var theObj;
  CORBA::Object_var obj;

  // searchin for naming service for 0.25*40=10 seconds
  for (i = 1; i <= NumberOfTries; i++) {
    if (i != 1) nanosleep(&ts_req,&ts_rem);
    try{ 
      if(CORBA::is_nil(obj))
	obj = theOrb->resolve_initial_references("RootPOA");
      if(CORBA::is_nil(theObj))
	theObj = theOrb->resolve_initial_references("NameService"); 
      if (!CORBA::is_nil(theObj))
	inc = CosNaming::NamingContext::_narrow(theObj);
      if (!CORBA::is_nil(inc)) break;
    } catch( CORBA::SystemException& ) {
    } catch (...) {
    }
  }
  
  if (CORBA::is_nil(inc)) {
    cout<<"SALOME_Trace can not find NameService"<<endl;
    return 0;
  }
  
  //cout<<"SALOME_Trace : NameService was found"<<endl;
  
  const char * Env = getenv("USE_LOGGER");
  int EnvL = (Env != NULL && strlen(Env))?1:0;
  
  // the try to get Logger server if it is necessary
  if(EnvL) {
    CosNaming::Name name;
    name.length(1);
    name[0].id=CORBA::string_dup("Logger");    
    
    for(i = 1; i <= NumberOfTries; i++){
      if (i != 1) nanosleep(&ts_req, &ts_rem);
      try {
	obj = inc->resolve(name);
	if (!CORBA::is_nil(obj)) m_pInterfaceLogger = SALOME_Logger::Logger::_narrow(obj);
      } catch(CosNaming::NamingContext::NotFound) {
      } catch(...) {
      }
      if (!CORBA::is_nil(m_pInterfaceLogger)) {
	//cout<<"SALOME_Trace : Logger Server was found"<<endl;
	m_pInterfaceLogger->ping();
	break;
      }
    }
    if (CORBA::is_nil(m_pInterfaceLogger)) {
      cout<<"SALOME_Trace can not find Logger"<<endl;
      return 0;
    }
  }
  isInitialized = 1;
  return 1;
}

void SALOME_Trace::putMessage(ostream& msg)
{
  //if (!isInitialized) cout<<"!!! SALOME_Trace is used without initialising !!!"<<endl;
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

