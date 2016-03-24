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

#include <SALOMEconfig.h>
#include <stdexcept>
#include <omniORB4/CORBA.h>

#include "utilities.h"

// CCRT porting
// #include "CASCatch_SignalsHandler.h" // CAREFUL ! position of this file is critic : see Lucien PIGNOLONI / OCC

extern "C" void HandleServerSideSignals(CORBA::ORB_ptr theORB)
{
 // CCRT porting
 //  CASCatch_SignalsHandler aSignalsHandler;
  try {
    theORB->run();
    // CCRT porting
    //  }catch(CORBA::SystemException&){
    //    Handle(Standard_Failure) aFail = Standard_Failure::Caught();
    //    throw std::runtime_error(aFail->GetMessageString());
  }catch(CORBA::SystemException&){
    INFOS("Caught CORBA::SystemException.");
  }catch(PortableServer::POA::WrongPolicy&){
    INFOS("Caught CORBA::WrongPolicyException.");
  }catch(PortableServer::POA::ServantAlreadyActive&){
    INFOS("Caught CORBA::ServantAlreadyActiveException");
  }catch(CORBA::Exception&){
    INFOS("Caught CORBA::Exception.");
  }catch(std::exception& exc){
    INFOS("Caught std::exception - "<<exc.what()); 
  }catch(...){
    INFOS("Caught unknown exception.");
  }
}
