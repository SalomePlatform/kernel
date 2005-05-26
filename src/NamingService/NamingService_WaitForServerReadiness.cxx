//  Copyright (C) 2004  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  File   : NamingService_WaitForServerReadiness.cxx
//  Author : Paul RASCLE (EDF)
//  Module : KERNEL
//  $Header$

#include "NamingService_WaitForServerReadiness.hxx"
#include "utilities.h"
#include <iostream>
#include <ctime>

using namespace std;

// ============================================================================
/*!
 * Wait until a server is registered in naming service.
 * \param serverName name of the server to find.
 * \param NS SALOME_NamingService object
 * Wait until the given server is ready i.e. is name is found in namingService.
 * Try 40 times, with 500 ms sleep between each try.
 * If Logger is used for traces, it must be ready before this call, because
 * SALOME_NamingService client uses SALOME traces. So, Logger readiness must be
 * checked in Launch script before execution of WaitForServerReadiness.
 */
// ============================================================================


void NamingService_WaitForServerReadiness(SALOME_NamingService* NS,
					  string serverName)
{
  long TIMESleep = 500000000; // 500 ms.
  int NumberOfTries = 40;     // total wait = 20 s.
  int found = 0;

  timespec ts_req;
  ts_req.tv_nsec=TIMESleep;
  ts_req.tv_sec=0;
  timespec ts_rem;
  ts_rem.tv_nsec=0;
  ts_rem.tv_sec=0;

  ASSERT(NS);
  for (int itry=0; itry < NumberOfTries; itry++)
    {
      try
	{
	  if (serverName.length() == 0)
	    {
	      string dummyadr = NS->getIORaddr(); // to wait for naming service
	      found = 1;
	      break; // naming service found
	    }
	  else
	    {
	      CORBA::Object_ptr obj = NS->Resolve(serverName.c_str());
	      if (! CORBA::is_nil(obj))
		{
		  found =1;
		  break; // server found, no more try to do
		}
	      MESSAGE("Server "<< serverName <<" not yet ready, waiting...");
	      int a = nanosleep(&ts_req,&ts_rem); // wait before retry
	    }
	}
      catch( ServiceUnreachable& )
	{
	  MESSAGE("CORBA::COMM_FAILURE: Naming Service not yet ready, waiting...");
	  int a = nanosleep(&ts_req,&ts_rem); // wait before retry
	}
    }
  if (!found)
    {
    INFOS("Server "<< serverName <<" not found, abort...");
    exit(EXIT_FAILURE);
    }
}
