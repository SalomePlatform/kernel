//  SALOME_LoggerClient : CORBA distributed log
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOME_Loggerclient.hxx
//  Author : Vasily Rusyaev, Paul RASCLE, EDF
//  Module : KERNEL
//  $Header$

#include <string>
#include <iostream>

using namespace std;

#include "SALOME_Log.hxx"

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(Logger)

// class SALOME_LoggerClient : public SALOME_Log
// {
// protected:
//   SALOME_Logger::Logger_var m_pInterfaceLogger; // object reference on Logger server
// };

SALOME_Logger::Logger_ptr m_pInterfaceLogger; // object reference on Logger server

SALOME_Log::SALOME_Log()
{
  cout << "SALOME_LoggerClient: constructor" << endl;
  //get reference on object reference from NS
  //and initialize m_pInterfaceLogger 

  int argc = 1;
  char* argv[1] = {"application"};
  try
    {
      //NB. You can't use SALOME_NamingService class because it uses MESSAGE macro
      //Otherwise, you will get segmentation fault.   

      //Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init(argc,argv);
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
      for (int i = 1; i<=NumberOfTries; i++)
	{
	  if (i!=1) 
	    a=nanosleep(&ts_req,&ts_rem);
	  try
	    { 
	      if(!CORBA::is_nil(orb)) 
		theObj = orb->resolve_initial_references("NameService");
	      if (!CORBA::is_nil(theObj))
		inc = CosNaming::NamingContext::_narrow(theObj);
	    }  
	  catch( CORBA::COMM_FAILURE& )
	    {
	      cout<<"SALOME_LoggerClient: CORBA::COMM_FAILURE: Unable to contact the Naming Service"<<endl;
	    }
          catch(...)
	    {
	      cout<<"SALOME_LoggerClient: Unknown exception dealing with Naming Service"<<endl;
	    }
	  
	  if(!CORBA::is_nil(inc))
	    {
	      obj = inc->resolve(name);
	      m_pInterfaceLogger = SALOME_Logger::Logger::_narrow(obj);
	      if (!CORBA::is_nil(m_pInterfaceLogger))
		cout<<"SALOME_LoggerClient: Logger Server was found"<<endl;
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

SALOME_Log::~SALOME_Log()
{
}

SALOME_Log& SALOME_Log::Instance()
{
  static SALOME_Log instance;
  return instance;
}

void SALOME_Log::putMessage(std::ostream& msg)
{
  //write resulting string into Logger CORBA server

  char* adt = str();
  CORBA::String_var LogMsg = CORBA::string_dup(adt);
  rdbuf()->freeze(false);
  seekp(0);

  if (CORBA::is_nil(m_pInterfaceLogger))
    cout << LogMsg << std::flush;
  else
    m_pInterfaceLogger-> putMessage (LogMsg) ;
 }

