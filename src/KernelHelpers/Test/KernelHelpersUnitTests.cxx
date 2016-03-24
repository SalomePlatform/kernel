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

#include "KernelHelpersUnitTests.hxx"

#include "SALOME_KernelServices.hxx"
#include "Basics_Utils.hxx"

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_TestComponent)

// ============================================================================
/*!
 * Set up the environment
 */
// ============================================================================

void KernelHelpersUnitTests::setUp()
{
}

// ============================================================================
/*!
 *  - delete
 */
// ============================================================================

void KernelHelpersUnitTests::tearDown()
{
}

// ============================================================================
/*!
 * Check SalomeApp functionality
 */
// ============================================================================
void KernelHelpersUnitTests::TEST_corba()
{
  CORBA::ORB_var orb = KERNEL::getORB();
  CPPUNIT_ASSERT(!CORBA::is_nil(orb));


  SALOME_NamingService *  ns  = KERNEL::getNamingService();
  CPPUNIT_ASSERT(ns!=NULL);

  SALOME_LifeCycleCORBA * lcc = KERNEL::getLifeCycleCORBA();
  CPPUNIT_ASSERT(lcc!=NULL);
}

void KernelHelpersUnitTests::TEST_getLifeCycleCORBA() {
  Engines::EngineComponent_var component =
    KERNEL::getLifeCycleCORBA()->FindOrLoad_Component( "FactoryServer","SalomeTestComponent" );

  Engines::TestComponent_var engine = Engines::TestComponent::_narrow(component);
  char * coucou_res = engine->Coucou(123.);
  const char * coucou_ref = "TestComponent_i : L = 123";
  LOG(coucou_res);
  CPPUNIT_ASSERT( strcmp(coucou_res, coucou_ref) == 0 );
}

void KernelHelpersUnitTests::TEST_getStudyManager() {
  SALOMEDS::StudyManager_var studyManager = KERNEL::getStudyManager();

  #ifndef ALLOW_MULTI_STUDIES
  SALOMEDS::ListOfOpenStudies_var _list_open_studies =  studyManager->GetOpenStudies();
  for (unsigned int ind = 0; ind < _list_open_studies->length();ind++)
    {
      LOG("Close study : " << _list_open_studies[ind]);
      SALOMEDS::Study_var study = studyManager->GetStudyByName(_list_open_studies[0]);
      if(study)
        studyManager->Close(study);
    }
  #endif

  SALOMEDS::Study_ptr myTestStudy = studyManager->NewStudy("kerneltest");
  CPPUNIT_ASSERT(!CORBA::is_nil(myTestStudy));

  // One can use the study to store some general properties
  myTestStudy->SetString("material","wood");
  myTestStudy->SetReal("volume",3.23);

  // The study is characterized by an ID
  int myTestStudyId = myTestStudy->StudyId();
  LOG("TestComponentImpl::testkernel: study id = "<<myTestStudyId);
}

void KernelHelpersUnitTests::TEST_getSalomeLauncher() {
  Engines::SalomeLauncher_var salomeLauncher = KERNEL::getSalomeLauncher();
  try {
    int pid = salomeLauncher->getPID();
    LOG("SALOME launcher PID = " << pid);
  }
  catch (const SALOME::SALOME_Exception & ex) {
    LOG("SALOME Exception in createJob !" <<ex.details.text.in());
    CPPUNIT_ASSERT(false);
  }
  catch (const CORBA::SystemException& ex) {
    LOG("Receive SALOME System Exception: "<<ex);
    LOG("Check SALOME servers...");
    CPPUNIT_ASSERT(false);
  }
  catch (const std::exception& ex) {
    LOG("Receive undefined exception : "<<ex.what());
    CPPUNIT_ASSERT(false);
  }
}
