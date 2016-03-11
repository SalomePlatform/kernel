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

#ifndef _SALOMEDSTEST_HXX_
#define _SALOMEDSTEST_HXX_

#include <cppunit/extensions/HelperMacros.h>

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

#ifdef WIN32
# if defined SALOMEDSTEST_EXPORTS || defined SALOMEDSTest_EXPORTS
#  define SALOMEDSTEST_EXPORT __declspec( dllexport )
# else
#  define SALOMEDSTEST_EXPORT __declspec( dllimport )
# endif
#else
# define SALOMEDSTEST_EXPORT
#endif

class SALOMEDSTEST_EXPORT SALOMEDSTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( SALOMEDSTest );

  //Attributes' tests
  
  CPPUNIT_TEST( testAttributeComment );
  CPPUNIT_TEST( testAttributeDrawable );
  CPPUNIT_TEST( testAttributeExpandable );
  CPPUNIT_TEST( testAttributeExternalFileDef );
  CPPUNIT_TEST( testAttributeFileType );
  CPPUNIT_TEST( testAttributeFlags );
  CPPUNIT_TEST( testAttributeGraphic );
  CPPUNIT_TEST( testAttributeIOR );
  CPPUNIT_TEST( testAttributeInteger );
  CPPUNIT_TEST( testAttributeLocalID );
  CPPUNIT_TEST( testAttributeName );
  CPPUNIT_TEST( testAttributeOpened );
  CPPUNIT_TEST( testAttributeParameter );  
  CPPUNIT_TEST( testAttributePersistentRef );
  CPPUNIT_TEST( testAttributePixMap );
  CPPUNIT_TEST( testAttributePythonObject );
  CPPUNIT_TEST( testAttributeReal );
  CPPUNIT_TEST( testAttributeSelectable );
  CPPUNIT_TEST( testAttributeSequenceOfInteger );
  CPPUNIT_TEST( testAttributeSequenceOfReal );
  CPPUNIT_TEST( testAttributeStudyProperties );
  CPPUNIT_TEST( testAttributeTableOfInteger );  
  CPPUNIT_TEST( testAttributeTableOfReal );  
  CPPUNIT_TEST( testAttributeTableOfString );
  CPPUNIT_TEST( testAttributeTarget );
  CPPUNIT_TEST( testAttributeTextColor );
  CPPUNIT_TEST( testAttributeTextHighlightColor );
  CPPUNIT_TEST( testAttributeTreeNode );
  CPPUNIT_TEST( testAttributeUserID );
  CPPUNIT_TEST( testChildIterator );
  CPPUNIT_TEST( testSComponent );
  CPPUNIT_TEST( testSComponentIterator );
  CPPUNIT_TEST( testSObject );
  CPPUNIT_TEST( testStudy );  
  CPPUNIT_TEST( testStudyBuilder );
  CPPUNIT_TEST( testStudyManager );
  CPPUNIT_TEST( testUseCase );


  CPPUNIT_TEST_SUITE_END();

public:

  virtual void setUp();
  virtual void tearDown();

  //Attributes
  void testAttributeComment();
  void testAttributeDrawable();
  void testAttributeExpandable();
  void testAttributeExternalFileDef();
  void testAttributeFileType();
  void testAttributeFlags();
  void testAttributeGraphic();
  void testAttributeIOR();
  void testAttributeInteger();
  void testAttributeLocalID();
  void testAttributeName();
  void testAttributeOpened();
  void testAttributeParameter();
  void testAttributePersistentRef();
  void testAttributePixMap();
  void testAttributePythonObject();
  void testAttributeReal();
  void testAttributeSelectable();
  void testAttributeSequenceOfInteger();
  void testAttributeSequenceOfReal();
  void testAttributeStudyProperties();
  void testAttributeTableOfInteger();
  void testAttributeTableOfReal();
  void testAttributeTableOfString();
  void testAttributeTarget();
  void testAttributeTextColor();
  void testAttributeTextHighlightColor();
  void testAttributeTreeNode();
  void testAttributeUserID(); 

  //Objects
  void testChildIterator();
  void testSComponent();
  void testSComponentIterator();
  void testSObject();
  void testStudy();
  void testStudyBuilder();
  void testStudyManager();
  void testUseCase();

protected:

SALOMEDS::StudyManager_var _sm;
CORBA::ORB_var             _orb;
                
};



class SALOMEDSTEST_EXPORT SALOMEDSTest_Embedded : public SALOMEDSTest
{
  CPPUNIT_TEST_SUITE( SALOMEDSTest_Embedded );

  //Attributes' tests
  
  CPPUNIT_TEST( testAttributeComment );
  CPPUNIT_TEST( testAttributeDrawable );
  CPPUNIT_TEST( testAttributeExpandable );
  CPPUNIT_TEST( testAttributeExternalFileDef );
  CPPUNIT_TEST( testAttributeFileType );
  CPPUNIT_TEST( testAttributeFlags );
  CPPUNIT_TEST( testAttributeGraphic );
  CPPUNIT_TEST( testAttributeIOR );
  CPPUNIT_TEST( testAttributeInteger );
  CPPUNIT_TEST( testAttributeLocalID );
  CPPUNIT_TEST( testAttributeName );
  CPPUNIT_TEST( testAttributeOpened );
  CPPUNIT_TEST( testAttributeParameter );
  CPPUNIT_TEST( testAttributePersistentRef );
  CPPUNIT_TEST( testAttributePixMap );
  CPPUNIT_TEST( testAttributePythonObject );  
  CPPUNIT_TEST( testAttributeReal );
  CPPUNIT_TEST( testAttributeSelectable );
  CPPUNIT_TEST( testAttributeSequenceOfInteger );
  CPPUNIT_TEST( testAttributeSequenceOfReal );
  CPPUNIT_TEST( testAttributeStudyProperties );
  CPPUNIT_TEST( testAttributeTableOfInteger );  
  CPPUNIT_TEST( testAttributeTableOfReal );
  CPPUNIT_TEST( testAttributeTableOfString );
  CPPUNIT_TEST( testAttributeTarget );
  CPPUNIT_TEST( testAttributeTextColor );
  CPPUNIT_TEST( testAttributeTextHighlightColor );
  CPPUNIT_TEST( testAttributeTreeNode );
  CPPUNIT_TEST( testAttributeUserID );
  CPPUNIT_TEST( testChildIterator );
  CPPUNIT_TEST( testSComponent );  
  CPPUNIT_TEST( testSComponentIterator );
  CPPUNIT_TEST( testSObject );
  CPPUNIT_TEST( testStudy );
  CPPUNIT_TEST( testStudyBuilder ); 
  CPPUNIT_TEST( testChildIterator );
  CPPUNIT_TEST( testStudyManager );
  CPPUNIT_TEST( testUseCase );
  
  CPPUNIT_TEST_SUITE_END();

public:

  virtual void setUp();
};

#endif
