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
//  File   : SALOMEGUI_Swig.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "SALOMEGUI_Swig.hxx"

#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_Study.h"
#include "QAD_RightFrame.h"
#include "QAD_StudyFrame.h"

#include "SALOME_Event.hxx"

#include "SALOME_Selection.h"
#include "SALOME_InteractiveObject.hxx"
#include "SALOME_ListIteratorOfListIO.hxx"

#include "utilities.h"

using namespace std;

// asv : 3.12.04 : added checking for NULL GUI objects in almost all methods.
// In the scope of fixing bug PAL6869.
// (PR : modify comments)
// Instance of this class is created every time "import salome" line is typed 
// - in IAPP embedded Python interpretor  (SALOME_Session_Server executable),
// - in inline Python nodes in Supervisor (in SALOME_Container executable),
// - in stand-alone Python console outside any executable.
// SALOME GUI(desktop and other objects) is only available in SALOME_Session_Server

namespace SALOME{
  QAD_ViewFrame* GetViewFrame( QAD_Study* theStudy ) {
    if ( theStudy )
      if ( QAD_StudyFrame* aSF = theStudy->getActiveStudyFrame() )
	if ( QAD_RightFrame* aRF = aSF->getRightFrame() )
	  return aRF->getViewFrame();
    return 0;
  }

  Handle(SALOME_InteractiveObject) FindIObject( QAD_Study* theStudy, const char *theEntry )
  {
    if ( QAD_ViewFrame* aVF = GetViewFrame( theStudy ) )
      return aVF->FindIObject( theEntry );
    return Handle(SALOME_InteractiveObject)(); // NULL Handle 
  }
}


/*!
  Constructor
*/
SALOMEGUI_Swig::SALOMEGUI_Swig()
{
  //  MESSAGE("Constructeur");
}

/*!
  Destructor
*/
SALOMEGUI_Swig::~SALOMEGUI_Swig()
{
  //  MESSAGE("Destructeur");
}

/*!
  Check if GUI is available (if we are linked to IAPP)
*/
bool SALOMEGUI_Swig::hasDesktop()
{
  if ( QAD_Desktop* aDT = QAD_Application::getDesktop() )
    return true;
  else
    return false;
}

/*!
  Gets active study or 0 if no study opened
*/
QAD_Study* SALOMEGUI_Swig::getActiveStudy()
{
  if ( QAD_Desktop* aDT = QAD_Application::getDesktop() )       
    if ( QAD_Application* anApp = aDT->getActiveApp() )
      return anApp->getActiveStudy();
  return 0;
}

/*!
  Call when updated object browser.
*/
void SALOMEGUI_Swig::updateObjBrowser( bool updateSelection )
{
  if ( QAD_Study* myActiveStudy = getActiveStudy() )
    ProcessVoidEvent( new TVoidMemFun1ArgEvent<QAD_Study,bool>
		     ( myActiveStudy, &QAD_Study::updateObjBrowser,updateSelection) );
}

/*!
  Returns active Study Id.
*/
int SALOMEGUI_Swig::getActiveStudyId()
{
  if ( QAD_Study* myActiveStudy = getActiveStudy() )
    return myActiveStudy->getStudyId();
  return 0;
}

/*!
  Returns active Study Name.
*/
const char *SALOMEGUI_Swig::getActiveStudyName()
{
  if ( QAD_Study* myActiveStudy = getActiveStudy() )
    return myActiveStudy->getTitle().latin1();
  return NULL;
}

/*!
  Returns the name of component.
*/
const char* SALOMEGUI_Swig::getComponentName( const char* ComponentUserName )
{
  if ( QAD_Desktop* aDT = QAD_Application::getDesktop() )
    return aDT->getComponentName( ComponentUserName );
  return NULL;
}
/*!
  Returns the user name of component.
*/
const char* SALOMEGUI_Swig::getComponentUserName( const char* ComponentName )
{
  if ( QAD_Desktop* aDT = QAD_Application::getDesktop() )
    return aDT->getComponentUserName( ComponentName );
  return NULL;
}

/*!
  Returns the number of selected objects.
*/
int SALOMEGUI_Swig::SelectedCount()
{
  if ( QAD_Study* myActiveStudy = getActiveStudy() ) {
    SALOME_Selection* Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
    return Sel->IObjectCount();
  }
  return 0;
}

/*!
  Returns the selected object at index i.
*/
const char* SALOMEGUI_Swig::getSelected( int i )
{
  if ( QAD_Study* myActiveStudy = getActiveStudy() ) {
    SALOME_Selection* Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
    SALOME_ListIteratorOfListIO It( Sel->StoredIObjects() );
    int index = 0;
    for( ;It.More();It.Next() ) {
	Handle(SALOME_InteractiveObject) IObject = It.Value();
	if( i == index++ ){
	  if ( IObject->hasEntry() )
	    return IObject->getEntry();
	}
    }
  }
  return "";
}

/*!
  Add object with Entry into selection.
*/
void SALOMEGUI_Swig::AddIObject( const char *theEntry )
{
  if ( QAD_Study* myActiveStudy = getActiveStudy() ) {
    SALOME_Selection* aSel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
    if ( IsInCurrentView( theEntry ) ) {
      Handle(SALOME_InteractiveObject) anIO = SALOME::FindIObject( myActiveStudy,theEntry );
      if ( anIO.IsNull() )  return;
      ProcessEvent( new TMemFun2ArgEvent<SALOME_Selection,int,
	            const Handle(SALOME_InteractiveObject)&,bool,
		    Handle(SALOME_InteractiveObject)>
      		    ( aSel, &SALOME_Selection::AddIObject, anIO, true ) );
    }
  }
}


/*!
  Removes object with Entry into selection.
*/
void SALOMEGUI_Swig::RemoveIObject( const char *theEntry )
{
  if ( QAD_Study* myActiveStudy = getActiveStudy() ) {
    SALOME_Selection* aSel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
    if ( IsInCurrentView( theEntry ) ) {
      Handle(SALOME_InteractiveObject) anIO = SALOME::FindIObject( myActiveStudy,theEntry );
      if ( anIO.IsNull() ) return;
      ProcessEvent( new TMemFun2ArgEvent<SALOME_Selection,int,
		    const Handle(SALOME_InteractiveObject)&, bool,
		    Handle(SALOME_InteractiveObject)>
      		    ( aSel, &SALOME_Selection::RemoveIObject, anIO, true ) );
    }
  }
}


/*!
  Removes all objects into selection.
*/
void SALOMEGUI_Swig::ClearIObjects()
{
  if ( QAD_Study* myActiveStudy = getActiveStudy() ) {
    SALOME_Selection* aSel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
    ProcessVoidEvent( new TVoidMemFunEvent<SALOME_Selection>
		     ( aSel, &SALOME_Selection::ClearIObjects ) );
  }
}

/*!
  Display
*/		
void SALOMEGUI_Swig::Display( const char *theEntry )
{
  if ( QAD_Study* myActiveStudy = getActiveStudy() ) {
    if ( IsInCurrentView( theEntry ) ) {
      Handle(SALOME_InteractiveObject) anIO = SALOME::FindIObject( myActiveStudy, theEntry );
      if ( anIO.IsNull() )  return;
      QAD_ViewFrame* aViewFrame = SALOME::GetViewFrame( myActiveStudy );
      ProcessVoidEvent( new TVoidMemFun2ArgEvent<QAD_ViewFrame,
		        const Handle(SALOME_InteractiveObject)&, bool,
		        Handle(SALOME_InteractiveObject)>
		        ( aViewFrame, &QAD_ViewFrame::Display, anIO, true ) );
    }
  }
}

/*!
  Display only
*/
void SALOMEGUI_Swig::DisplayOnly( const char *theEntry )
{
  if ( QAD_Study* myActiveStudy = getActiveStudy() ) {
    if ( IsInCurrentView( theEntry ) ) {
      Handle(SALOME_InteractiveObject) anIO = SALOME::FindIObject( myActiveStudy, theEntry );
      if ( anIO.IsNull() )  return;
      QAD_ViewFrame* aViewFrame = SALOME::GetViewFrame( myActiveStudy );
      ProcessVoidEvent( new TVoidMemFun1ArgEvent<QAD_ViewFrame,
		        const Handle(SALOME_InteractiveObject)&,
		        Handle(SALOME_InteractiveObject)>
		        ( aViewFrame, &QAD_ViewFrame::DisplayOnly, anIO ) );
    }
  }
}

/*!
  Erase
*/
void SALOMEGUI_Swig::Erase( const char *theEntry )
{
  if ( QAD_Study* myActiveStudy = getActiveStudy() ) {
    if ( IsInCurrentView( theEntry ) ) {
      Handle(SALOME_InteractiveObject) anIO = SALOME::FindIObject( myActiveStudy, theEntry );
      if ( anIO.IsNull() )  return;
      QAD_ViewFrame* aViewFrame = SALOME::GetViewFrame( myActiveStudy );
      ProcessVoidEvent( new TVoidMemFun2ArgEvent<QAD_ViewFrame,
		        const Handle(SALOME_InteractiveObject)&, bool,
		        Handle(SALOME_InteractiveObject)>
		        ( aViewFrame, &QAD_ViewFrame::Erase, anIO, true ) );
    }
  }
}

/*!
  Display all
*/
void SALOMEGUI_Swig::DisplayAll()
{
  if ( QAD_Study* myActiveStudy = getActiveStudy() ) {
    QAD_ViewFrame* aViewFrame = SALOME::GetViewFrame( myActiveStudy );
    ProcessVoidEvent( new TVoidMemFunEvent<QAD_ViewFrame>
		      ( aViewFrame, &QAD_ViewFrame::DisplayAll ) );
  }
}

/*!
  Erase only
*/
void SALOMEGUI_Swig::EraseAll()
{
  ClearIObjects();
  if ( QAD_Study* myActiveStudy = getActiveStudy() ) {
    QAD_ViewFrame* aViewFrame = SALOME::GetViewFrame( myActiveStudy );
    ProcessVoidEvent ( new TVoidMemFunEvent<QAD_ViewFrame>
		      ( aViewFrame, &QAD_ViewFrame::EraseAll ) );
  }
}

/*!
  Checks if object is displayed in current viewer
*/
bool SALOMEGUI_Swig::IsInCurrentView( const char *theEntry )
{
  if ( QAD_Study* myActiveStudy = getActiveStudy() )
    return myActiveStudy->isInViewer( theEntry, myActiveStudy->getActiveStudyFrame()->entry() );
  return false;
}
