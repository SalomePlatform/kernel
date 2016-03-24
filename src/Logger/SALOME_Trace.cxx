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
//  File   : SALOME_Logger.cxx
//  Author : Vasily Rusyaev
//  Module : SALOME
//
#include "SALOME_Trace.hxx"
#include <memory.h>
#include <string>
//#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#ifdef WIN32
#include <omnithread/pthread_nt.h>
#endif

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
#ifndef WIN32
    if (i != 1) nanosleep(&ts_req,&ts_rem);
#else
        if (i != 1) Sleep(TIMESleep / 1000000);
#endif
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
    std::cout<<"SALOME_Trace can not find NameService"<<std::endl;
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
#ifndef WIN32
      if (i != 1) nanosleep(&ts_req, &ts_rem);
#else
          if (i != 1) Sleep(TIMESleep / 1000000);
#endif
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
      std::cout<<"SALOME_Trace can not find Logger"<<std::endl;
      return 0;
    }
  }
  isInitialized = 1;
  return 1;
}

void SALOME_Trace::putMessage(std::ostream& msg)
{
  //if (!isInitialized) std::cout<<"!!! SALOME_Trace is used without initialising !!!"<<std::endl;
  //write resulting string into Logger CORBA server
  //concatenate string from passing parameters for transfering into Logger CORBA server

  //std::cerr << "-+- " << msg << " ";

  //   CORBA::String_var LogMsg = CORBA::string_dup( str() );
  //Allow automatic deletion of ostrstream content 
  const char* adt = str().c_str();
  CORBA::String_var LogMsg = CORBA::string_dup( adt );
  //rdbuf()->freeze(false);
  //rdbuf()->sync(); // problem with gcc3.2
  seekp(0);

  if (CORBA::is_nil(m_pInterfaceLogger))
    std::cout << LogMsg;
  else
    m_pInterfaceLogger-> putMessage (LogMsg) ;
}

