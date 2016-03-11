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

#include "SALOMEDSTest.hxx"


#include "SALOMEDS_StudyManager_i.hxx"
#include "utilities.h"
#include "Utils_SINGLETON.hxx"
#include "Utils_ORB_INIT.hxx"
#include "SALOME_NamingService.hxx"
#include "SALOME_LifeCycleCORBA.hxx"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "SALOMEDSClient.hxx"
#include "SALOMEDS_StudyManager_i.hxx"
#include "SALOMEDS_StudyManager.hxx"
#include "SALOMEDS_SObject.hxx"



// ============================================================================
/*!
 * Set up the environment
 */
// ============================================================================
 
void SALOMEDSTest::setUp()
{  
  int argc = 1;
  char* argv[] = {(char*)""};

  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting());
  _orb = init(argc , argv ) ;
  SALOME_NamingService NS(_orb);
  CORBA::Object_var obj = NS.Resolve( "/myStudyManager" );
  _sm = SALOMEDS::StudyManager::_narrow( obj );

  CPPUNIT_ASSERT( !CORBA::is_nil(_sm) ); 
}

// ============================================================================
/*!
 *  - delete trace classes
 */
// ============================================================================

void SALOMEDSTest::tearDown()
{
  _PTR(StudyManager) sm ( new SALOMEDS_StudyManager(_sm) );
  std::vector<std::string> v = sm->GetOpenStudies();
  for(int i = 0; i<v.size(); i++) {
    _PTR(Study) study = sm->GetStudyByName(v[i]);
    if(study)
      sm->Close(study);
  }
}

#include "SALOMEDSTest_AttributeComment.cxx"
#include "SALOMEDSTest_AttributeDrawable.cxx"
#include "SALOMEDSTest_AttributeExpandable.cxx"
#include "SALOMEDSTest_AttributeExternalFileDef.cxx"
#include "SALOMEDSTest_AttributeFileType.cxx"
#include "SALOMEDSTest_AttributeFlags.cxx"
#include "SALOMEDSTest_AttributeGraphic.cxx"
#include "SALOMEDSTest_AttributeIOR.cxx"
#include "SALOMEDSTest_AttributeInteger.cxx"
#include "SALOMEDSTest_AttributeLocalID.cxx"
#include "SALOMEDSTest_AttributeName.cxx"
#include "SALOMEDSTest_AttributeOpened.cxx"
#include "SALOMEDSTest_AttributeParameter.cxx"
#include "SALOMEDSTest_AttributePersistentRef.cxx"
#include "SALOMEDSTest_AttributePixMap.cxx"
#include "SALOMEDSTest_AttributePythonObject.cxx"
#include "SALOMEDSTest_AttributeReal.cxx"
#include "SALOMEDSTest_AttributeSelectable.cxx"
#include "SALOMEDSTest_AttributeSequenceOfInteger.cxx"
#include "SALOMEDSTest_AttributeSequenceOfReal.cxx"
#include "SALOMEDSTest_AttributeStudyProperties.cxx"
#include "SALOMEDSTest_AttributeTableOfInteger.cxx"
#include "SALOMEDSTest_AttributeTableOfReal.cxx"
#include "SALOMEDSTest_AttributeTableOfString.cxx"
#include "SALOMEDSTest_AttributeTarget.cxx"
#include "SALOMEDSTest_AttributeTextColor.cxx"
#include "SALOMEDSTest_AttributeTextHighlightColor.cxx"
#include "SALOMEDSTest_AttributeTreeNode.cxx"
#include "SALOMEDSTest_AttributeUserID.cxx"
#include "SALOMEDSTest_ChildIterator.cxx"
#include "SALOMEDSTest_SComponent.cxx"
#include "SALOMEDSTest_SComponentIterator.cxx"
#include "SALOMEDSTest_SObject.cxx"
#include "SALOMEDSTest_Study.cxx"
#include "SALOMEDSTest_StudyBuilder.cxx"
#include "SALOMEDSTest_StudyManager.cxx"
#include "SALOMEDSTest_UseCase.cxx"


// ============================================================================
/*!
 * Set up the environment
 */
// ============================================================================
 
void SALOMEDSTest_Embedded::setUp()
{  
  int argc = 1;
  char* argv[] = {(char*)""};

  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting());
  _orb = init(argc , argv ) ;
  SALOME_NamingService NS(_orb);
  CORBA::Object_var obj = NS.Resolve( "/myStudyManager_embedded" );
  _sm = SALOMEDS::StudyManager::_narrow( obj );

  CPPUNIT_ASSERT( !CORBA::is_nil(_sm) ); 
}
