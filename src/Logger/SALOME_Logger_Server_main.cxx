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
//  File   : SALOME_Logger_Server.cxx
//  Author : Vasily Rusyaev
//  Module : SALOME
//
#include <iostream>
#include "SALOME_Logger_Server.hxx"
#include <SALOMEconfig.h>
#include <sys/types.h>
#include <stdlib.h>

#ifndef WIN32
# include <unistd.h>
#else
# include "utilities.h" // for compilation type "timespec"
#endif

int main(int argc, char **argv)
{
  if (argc > 2)
    {
      std::cout << "usage: SALOME_Logger_Server [output_file]" << std::endl;
      exit(1);
    }
  try
    {
      //Initialize the ORB
      const long TIMESleep = 250000000;
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

      for (i = 1; i <= NumberOfTries; i++)
          {
#ifndef WIN32
                  if (i != 1) nanosleep(&ts_req, &ts_rem);
#else
                  if (i != 1) Sleep(TIMESleep / 1000000);
#endif
                  try 
                  {
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
                  } catch(...) {
                          //cout<<"Logger Server: Unknown exception dealed with Naming Service" <<endl;
                  }
                  
                  if (!CORBA::is_nil(inc)) {
                          //      cout<<"Logger Server: Naming Service was found"<<endl; 
                          break;
                  }
      }
      if (argc == 1)
                  myLogger = new Logger();
      else
                  myLogger = new Logger(argv[1]);

      myLogger->SetOrb(orb);
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
  catch(CORBA::COMM_FAILURE&)
        {
      std::cerr << "Caught system exception COMM_FAILURE -- unable to contact the "
           << "object." << std::endl;
    }
  catch(CORBA::SystemException&) 
    {
      std::cerr << "Caught CORBA::SystemException." << std::endl;
    }
  catch(CORBA::Exception&) 
    {
      std::cerr << "Caught CORBA::Exception." << std::endl;
    }
  catch(omniORB::fatalException& fe) 
    {
      std::cerr << "Caught omniORB::fatalException:" << std::endl;
      std::cerr << "  file: " << fe.file() << std::endl;
      std::cerr << "  line: " << fe.line() << std::endl;
      std::cerr << "  mesg: " << fe.errmsg() << std::endl;
    }
  catch(...) 
    {
      std::cerr << "Caught unknown exception." << std::endl;
    }
  return 0;
}
