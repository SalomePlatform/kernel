//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
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
//  File   : SALOMEGUI_Desktop.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "SALOMEGUI_Desktop.h"
#include "SALOME_Selection.h"
#include "SALOME_ListIteratorOfListIO.hxx"

#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
using namespace std;

/*!
    Constructor 
*/
SALOMEGUI_Desktop::SALOMEGUI_Desktop(SALOME_NamingService* name_service) :
  QAD_Desktop( name_service )
{
  QAD_ASSERT_DEBUG_ONLY( !myStdActions.isEmpty() );
  //myStdActions.at( FileOpenId )->setEnabled( true );
  //myStdActions.at( FileSaveId )->setEnabled( true );
  //myStdActions.at( FileSaveAsId )->setEnabled( true );
}

/*!
    Disable file actions for DEMO 
*/
void SALOMEGUI_Desktop::updateDesktop( UpdateCommand  cmd )
{
  QAD_Desktop::updateDesktop( cmd  );
  
  QAD_ASSERT_DEBUG_ONLY( !myStdActions.isEmpty() );
  //myStdActions.at( FileOpenId )->setEnabled( true );
  //myStdActions.at( FileSaveId )->setEnabled( true );
  //myStdActions.at( FileSaveAsId )->setEnabled( true );
}

QString SALOMEGUI_Desktop::getComponentFromSelection()
{
  SALOME_Selection* Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
  /* copy the list */
  SALOME_ListIO List;
  SALOME_ListIteratorOfListIO Itinit( Sel->StoredIObjects() );
  for (; Itinit.More(); Itinit.Next()) {
    List.Append(Itinit.Value());
  }
  
  SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();

  SALOME_ListIteratorOfListIO It( List );
  QString parentComp;

  for (; It.More(); It.Next()) {
    Handle(SALOME_InteractiveObject) IObject = It.Value();

    if (!IObject->hasEntry()) continue;

    SALOMEDS::SObject_var  obj = aStudy->FindObjectID( IObject->getEntry() );
    
    if ( !obj->_is_nil() ) {
      SALOMEDS::SComponent_var comp = obj->GetFatherComponent();
      if ( !comp->_is_nil() ) {
	SALOMEDS::GenericAttribute_var anAttr;
	SALOMEDS::AttributeName_var    aName;
	if (comp->FindAttribute(anAttr, "AttributeName")) {
	  aName = SALOMEDS::AttributeName::_narrow(anAttr);
	  QString compName(aName->Value());
	  if (parentComp.isNull())
	    parentComp = compName;
	  else if (parentComp.compare(compName) != 0) { // objects belonging to different components are selected
	    parentComp = QString::null;
	    break;
	  }
	}
      }
    }
  }

  return parentComp;
}
