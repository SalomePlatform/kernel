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

namespace SALOME{
  QAD_ViewFrame* GetViewFrame(QAD_Study* theStudy){
    return theStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame();
  }

  Handle(SALOME_InteractiveObject) FindIObject(QAD_Study* theStudy, const char *theEntry)
  {
    return GetViewFrame(theStudy)->FindIObject(theEntry);
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
  Gets active study or 0 if no study opened
*/
QAD_Study* SALOMEGUI_Swig::getActiveStudy()
{
  if(QAD_Application* app = QAD_Application::getDesktop()->getActiveApp()) 
    return app->getActiveStudy();
  return 0;
}

/*!
  Call when updated object browser.
*/
void SALOMEGUI_Swig::updateObjBrowser( bool updateSelection )
{
  if(QAD_Study* myActiveStudy = getActiveStudy())
    ProcessVoidEvent(new TVoidMemFun1ArgEvent<QAD_Study,bool>
		     (myActiveStudy,&QAD_Study::updateObjBrowser,updateSelection));
}

/*!
  Returns active Study Id.
*/
int SALOMEGUI_Swig::getActiveStudyId()
{
  if(QAD_Study* myActiveStudy = getActiveStudy())
    return myActiveStudy->getStudyId();
  return 0;
}

/*!
  Returns active Study Name.
*/
const char *SALOMEGUI_Swig::getActiveStudyName()
{
  if(QAD_Study* myActiveStudy = getActiveStudy())
    return myActiveStudy->getTitle().latin1();
  return QString::null;
}

/*!
  Returns the name of component.
*/
const char* SALOMEGUI_Swig::getComponentName( const char* ComponentUserName )
{
  return QAD_Application::getDesktop()->getComponentName( ComponentUserName );
}
/*!
  Returns the user name of component.
*/
const char* SALOMEGUI_Swig::getComponentUserName( const char* ComponentName )
{
  return QAD_Application::getDesktop()->getComponentUserName( ComponentName );
}

/*!
  Returns the number of selected objects.
*/
int SALOMEGUI_Swig::SelectedCount()
{
  if(QAD_Study* myStudy = getActiveStudy()){
    SALOME_Selection* Sel = SALOME_Selection::Selection(myStudy->getSelection());
    return Sel->IObjectCount();
  }
  return 0;
}

/*!
  Returns the selected object at index i.
*/
const char* SALOMEGUI_Swig::getSelected(int i)
{
  if(QAD_Study* myStudy = getActiveStudy()){
    SALOME_Selection* Sel = SALOME_Selection::Selection( myStudy->getSelection() );
    SALOME_ListIteratorOfListIO It( Sel->StoredIObjects() );
    int index = 0;
    for(;It.More();It.Next()){
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
void SALOMEGUI_Swig::AddIObject(const char *theEntry)
{
  if(QAD_Study* myStudy = getActiveStudy()){
    SALOME_Selection* aSel = SALOME_Selection::Selection( myStudy->getSelection() );
    if(IsInCurrentView(theEntry)){
      Handle(SALOME_InteractiveObject) anIO = SALOME::FindIObject(myStudy,theEntry);
      if(anIO.IsNull())	return;
      ProcessEvent(new TMemFun2ArgEvent<SALOME_Selection,int,
		   const Handle(SALOME_InteractiveObject)&,bool,
		   Handle(SALOME_InteractiveObject)>
      		   (aSel,&SALOME_Selection::AddIObject,anIO,true));
    }
  }
}


/*!
  Removes object with Entry into selection.
*/
void SALOMEGUI_Swig::RemoveIObject(const char *theEntry)
{
  if(QAD_Study* myStudy = getActiveStudy()){
    SALOME_Selection* aSel = SALOME_Selection::Selection( myStudy->getSelection() );
    if(IsInCurrentView(theEntry)){
      Handle(SALOME_InteractiveObject) anIO = SALOME::FindIObject(myStudy,theEntry);
      if(anIO.IsNull())	return;
      ProcessEvent(new TMemFun2ArgEvent<SALOME_Selection,int,
		   const Handle(SALOME_InteractiveObject)&,bool,
		   Handle(SALOME_InteractiveObject)>
      		   (aSel,&SALOME_Selection::RemoveIObject,anIO,true));
    }
  }
}


/*!
  Removes all objects into selection.
*/
void SALOMEGUI_Swig::ClearIObjects()
{
  if(QAD_Study* myStudy = getActiveStudy()){
    SALOME_Selection* aSel = SALOME_Selection::Selection( myStudy->getSelection() );
    ProcessVoidEvent(new TVoidMemFunEvent<SALOME_Selection>
		     (aSel,&SALOME_Selection::ClearIObjects));
  }
}

/*!
  Display
*/		
void SALOMEGUI_Swig::Display(const char *theEntry)
{
  if(QAD_Study* myStudy = getActiveStudy()){
    if(IsInCurrentView(theEntry)){
      Handle(SALOME_InteractiveObject) anIO = SALOME::FindIObject(myStudy,theEntry);
      if(anIO.IsNull())	return;
      QAD_ViewFrame* aViewFrame = SALOME::GetViewFrame(myStudy);
      ProcessVoidEvent(new TVoidMemFun2ArgEvent<QAD_ViewFrame,
		       const Handle(SALOME_InteractiveObject)&,bool,
		       Handle(SALOME_InteractiveObject)>
		       (aViewFrame,&QAD_ViewFrame::Display,anIO,true));
    }
  }
}

/*!
  Display only
*/
void SALOMEGUI_Swig::DisplayOnly(const char *theEntry)
{
  if(QAD_Study* myStudy = getActiveStudy()){
    if(IsInCurrentView(theEntry)){
      Handle(SALOME_InteractiveObject) anIO = SALOME::FindIObject(myStudy,theEntry);
      if(anIO.IsNull())	return;
      QAD_ViewFrame* aViewFrame = SALOME::GetViewFrame(myStudy);
      ProcessVoidEvent(new TVoidMemFun1ArgEvent<QAD_ViewFrame,
		       const Handle(SALOME_InteractiveObject)&,
		       Handle(SALOME_InteractiveObject)>
		       (aViewFrame,&QAD_ViewFrame::DisplayOnly,anIO));
    }
  }
}

/*!
  Erase
*/
void SALOMEGUI_Swig::Erase(const char *theEntry)
{
  if(QAD_Study* myStudy = getActiveStudy()){
    if(IsInCurrentView(theEntry)){
      Handle(SALOME_InteractiveObject) anIO = SALOME::FindIObject(myStudy,theEntry);
      if(anIO.IsNull())	return;
      QAD_ViewFrame* aViewFrame = SALOME::GetViewFrame(myStudy);
      ProcessVoidEvent(new TVoidMemFun2ArgEvent<QAD_ViewFrame,
		       const Handle(SALOME_InteractiveObject)&,bool,
		       Handle(SALOME_InteractiveObject)>
		       (aViewFrame,&QAD_ViewFrame::Erase,anIO,true));
    }
  }
}

/*!
  Display all
*/
void SALOMEGUI_Swig::DisplayAll()
{
  if(QAD_Study* myStudy = getActiveStudy()){
    QAD_ViewFrame* aViewFrame = SALOME::GetViewFrame(myStudy);
    ProcessVoidEvent(new TVoidMemFunEvent<QAD_ViewFrame>
		     (aViewFrame,&QAD_ViewFrame::DisplayAll));
  }
}

/*!
  Erase only
*/
void SALOMEGUI_Swig::EraseAll()
{
  ClearIObjects();
  if(QAD_Study* myStudy = getActiveStudy()){
    QAD_ViewFrame* aViewFrame = SALOME::GetViewFrame(myStudy);
    ProcessVoidEvent(new TVoidMemFunEvent<QAD_ViewFrame>
		     (aViewFrame,&QAD_ViewFrame::EraseAll));
  }
}

/*!
  Checks if object is displayed in current viewer
*/
bool SALOMEGUI_Swig::IsInCurrentView(const char *theEntry)
{
  if(QAD_Study* myStudy = getActiveStudy())
    return myStudy->isInViewer( theEntry, myStudy->getActiveStudyFrame()->entry() );
  return false;
}
