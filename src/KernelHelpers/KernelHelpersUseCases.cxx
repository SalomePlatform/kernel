// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
// Author: Guillaume Boulant (EDF/R&D)

#include "SALOME_KernelServices.hxx"
#include "Basics_Utils.hxx"

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_TestComponent)

bool TEST_corba() {
  CORBA::ORB_var orb = KERNEL::getORB();
  if ( CORBA::is_nil(orb) ) {
    LOG("TEST_Corba: orb ERROR");
    return false;
  }
  SALOME_NamingService *  ns  = KERNEL::getNamingService();
  if ( ns == NULL ) {
    LOG("TEST_Corba: ns ERROR");
    return false;

  }
  SALOME_LifeCycleCORBA * lcc = KERNEL::getLifeCycleCORBA();
  if ( lcc == NULL ) {
    LOG("TEST_Corba: lcc ERROR");
    return false;
  }
  LOG("TEST_Corba: OK");
  return true;
}

#include <string.h>
bool TEST_getLifeCycleCORBA() {
  Engines::EngineComponent_var component =
    KERNEL::getLifeCycleCORBA()->FindOrLoad_Component( "FactoryServer","SalomeTestComponent" );

  Engines::TestComponent_var engine = Engines::TestComponent::_narrow(component);
  char * coucou_res = engine->Coucou(123.);
  const char * coucou_ref = "L = 123";
  LOG(coucou_res);
  if ( strcmp(coucou_res, coucou_ref) == 0 ) {
    return false;
  }
  return true;
}

bool TEST_getStudyManager() {
  SALOMEDS::Study_ptr myTestStudy = KERNEL::getStudyManager()->NewStudy("kerneltest");
  if ( CORBA::is_nil(myTestStudy) ) {
    return false;
  }

  // One can use the study to store some general properties
  myTestStudy->SetString("material","wood");
  myTestStudy->SetReal("volume",3.23);

  // The study is characterized by an ID
  int myTestStudyId = myTestStudy->StudyId();
  LOG("TestComponentImpl::testkernel: study id = "<<myTestStudyId);
  return true;
}


bool TEST_getSalomeLauncher() {
  Engines::SalomeLauncher_var salomeLauncher = KERNEL::getSalomeLauncher();
  for (int i=0; i<10; i++) {
    try {
      int pid = salomeLauncher->getPID();
      LOG("["<<i<<"] SALOME launcher PID = " << pid);
    }
    catch (const SALOME::SALOME_Exception & ex) {
      LOG("SALOME Exception in createJob !" <<ex.details.text.in());
      return false;
    }
    catch (const CORBA::SystemException& ex) {
      LOG("Receive SALOME System Exception: "<<ex);
      LOG("Check SALOME servers...");
      return false;
    }
    catch (const std::exception& ex) {
      LOG("Receive undefined exception : "<<ex.what());
    }
#ifndef WIN32
      sleep(2);
#else
      Sleep(2000);
#endif
  }
  return true;
}

// TODO:
// - complete the coverture of the KernelService interface
// - provide use case for the StudyEditor



int main (int argc, char * argv[]) {
  TEST_corba();
  TEST_getLifeCycleCORBA();
  TEST_getStudyManager();
  TEST_getSalomeLauncher();
  return 0;
}
