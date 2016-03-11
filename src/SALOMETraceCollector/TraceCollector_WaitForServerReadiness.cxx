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

//  File   : TraceCollector_WaitForServerReadiness.cxx
//  Author : Paul RASCLE (EDF)
//  Module : KERNEL
//  $Header$
//
#include "TraceCollector_WaitForServerReadiness.hxx"
#include <iostream>
#include <ctime>

#ifdef WIN32
#include <omnithread/pthread_nt.h>
#endif

// ============================================================================
/*!
 *  Wait until a server is registered in naming service.
 *  \param serverName name of the server to find.
 *  When SALOME_NamingService is available,
 *  use NamingService_WaitForServerReadiness instead.
 *  This function is needed when macro MESSAGE used by SALOME_NamingService
 *  is not available (inside LocalTrace methods, for instance !).
 *  Direct access to CORBA Name Service. Look for serverName at Name service
 *  Root without extensions.
 */
// ============================================================================

CORBA::Object_ptr TraceCollector_WaitForServerReadiness(CORBA::ORB_ptr orb,
                                                        std::string serverName)
{
  long TIMESleep = 500000000;
  int NumberOfTries = 40;

  timespec ts_req;
  ts_req.tv_nsec=TIMESleep;
  ts_req.tv_sec=0;
  timespec ts_rem;
  ts_rem.tv_nsec=0;
  ts_rem.tv_sec=0;

  CORBA::Object_var obj;

  try
    {
      // NB. You can't use SALOME_NamingService class because
      // it uses MESSAGE macro
      // Otherwise, you will get segmentation fault.   

      CosNaming::NamingContext_var inc;
      CosNaming::Name name;
      name.length(1);
      name[0].id = CORBA::string_dup(serverName.c_str());
      CORBA::Object_var theObj=CORBA::Object::_nil();

      for (int itry=0; itry < NumberOfTries; itry++)
        {
          try
            { 
              if(!CORBA::is_nil(orb)) 
                theObj = orb->resolve_initial_references("NameService");
              if (!CORBA::is_nil(theObj))
                inc = CosNaming::NamingContext::_narrow(theObj);
            }  
          catch( CORBA::SystemException& )
            {
              std::cout << "TraceCollector_WaitForServerReadiness: "
                   << "CORBA::SystemException: "
                   << "Unable to contact the Naming Service" << std::endl;
            }
          catch(...)
            {
              std::cout << "TraceCollector_WaitForServerReadiness: "
                   << "Unknown exception dealing with Naming Service" << std::endl;
            }
          
          obj=CORBA::Object::_nil();
          if(!CORBA::is_nil(inc))
            {
              try
                {
                  obj = inc->resolve(name);
                  if (!CORBA::is_nil(obj))
                    {
                      //cout << "TraceCollector_WaitForServerReadiness: "
                      //           << serverName << " found in CORBA Name Service" << endl;
                      break;
                    }
                }
              catch (const CosNaming::NamingContext::NotFound&)
                {
                  std::cout << "Caught exception: Naming Service can't found Logger";
                }
            }
#ifndef WIN32
          nanosleep(&ts_req,&ts_rem);
#else
          Sleep(TIMESleep / 1000000);
#endif
		  std::cout << "TraceCollector_WaitForServerReadiness: retry look for"
               << serverName << std::endl;
        }          
    }
  catch (const CosNaming::NamingContext::NotFound&)
    {
      std::cout << "Caught exception: Naming Service can't found Logger";
    }
  catch (CORBA::COMM_FAILURE&)
    {
      std::cout << "Caught CORBA::SystemException CommFailure.";
    }
  catch (CORBA::SystemException&)
    {
      std::cout << "Caught CORBA::SystemException.";
    }
  catch (CORBA::Exception&)
    {
      std::cout << "Caught CORBA::Exception.";
    }
  catch (...)
    {
      std::cout << "Caught unknown exception.";
    }
  return obj._retn();
}

