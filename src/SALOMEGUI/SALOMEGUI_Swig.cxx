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

using namespace std;
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_Study.h"
#include "QAD_RightFrame.h"
#include "QAD_StudyFrame.h"
#include "SALOMEGUI_Swig.hxx"
#include "utilities.h"

#include "SALOME_Selection.h"
#include "SALOME_InteractiveObject.hxx"
#include "SALOME_ListIteratorOfListIO.hxx"


/*!
  Constructor
*/
SALOMEGUI_Swig::SALOMEGUI_Swig()
// VSR 06-05-03 : _studyId(0)
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
  QAD_Application* app = QAD_Application::getDesktop()->getActiveApp();
  if ( app ) return app->getActiveStudy();
  return 0;
}

/*!
  Call when updated object browser.
*/
void SALOMEGUI_Swig::updateObjBrowser( bool updateSelection )
{
  // VSR 06-05-03 ============================================================
  //QAD_Study* myActiveStudy = QAD_Application::getDesktop()->getActiveStudy();
  QAD_Study* myActiveStudy = getActiveStudy();
  if ( !myActiveStudy) return;
  // end of VSR 06-05-03 =====================================================
  myActiveStudy->updateObjBrowser( updateSelection );
}

/*!
  Returns active Study Id.
*/
int SALOMEGUI_Swig::getActiveStudyId()
{
  // NRI 24-02-03 :
  //  if (_studyId ==0)
  //    {
  //  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  //  _name = myStudy->getTitle();

  // VSR 06-05-03 ============================================================
  // _studyId = QAD_Application::getDesktop()->getActiveStudy()->getStudyId();
  int _studyId = 0;
  QAD_Study* myActiveStudy = getActiveStudy();
  if ( myActiveStudy) _studyId = myActiveStudy->getStudyId();
  // SCRUTE(_studyId);
  // end of VSR 06-05-03 =====================================================
  //    }
  return _studyId;
}

/*!
  Returns active Study Name.
*/
const char *SALOMEGUI_Swig::getActiveStudyName()
{
  // VSR 06-05-03 ============================================================
  // QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  // _name = myStudy->getTitle();
  QString _name = QString::null;
  QAD_Study* myActiveStudy = getActiveStudy();
  if ( myActiveStudy) _name = myActiveStudy->getTitle();
  // end of VSR 06-05-03 =====================================================

  // NRI 24-02-03 :
  //if (_studyId ==0) 
  //  getActiveStudyId();
  return _name.latin1();
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
  // VSR 06-05-03 ============================================================
  //QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  QAD_Study* myStudy = getActiveStudy();
  if ( !myStudy) return 0;
  // end of VSR 06-05-03 =====================================================
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );
  return Sel->IObjectCount();
}

/*!
  Returns the selected object at index i.
*/
const char* SALOMEGUI_Swig::getSelected(int i)
{
  // VSR 06-05-03 ============================================================
  //QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  QAD_Study* myStudy = getActiveStudy();
  if ( !myStudy) return "";
  // end of VSR 06-05-03 =====================================================
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );
  SALOME_ListIteratorOfListIO It( Sel->StoredIObjects() );
  int index = 0;
  for(;It.More();It.Next())
    {
      Handle(SALOME_InteractiveObject) IObject = It.Value();
      if ( i == index++ )
	{
	  if ( IObject->hasEntry() )
	    return IObject->getEntry();
	}
    }
  return "";
}

/*!
  Add object with Entry into selection.
*/
void SALOMEGUI_Swig::AddIObject(const char *Entry)
{
  // VSR 06-05-03 ============================================================
  //QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  QAD_Study* myStudy = getActiveStudy();
  if ( !myStudy) return;
  // end of VSR 06-05-03 =====================================================
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );

  if ( !IsInCurrentView( Entry ) )
    return;
  
  Handle(SALOME_InteractiveObject) IO = 
    myStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->FindIObject( Entry );
  Sel->AddIObject( IO );
}


/*!
  Removes object with Entry into selection.
*/
void SALOMEGUI_Swig::RemoveIObject(const char *Entry)
{
  // VSR 06-05-03 ============================================================
  //QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  QAD_Study* myStudy = getActiveStudy();
  if ( !myStudy) return;
  // end of VSR 06-05-03 =====================================================
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );

  if ( !IsInCurrentView( Entry ) )
    return;

  Handle(SALOME_InteractiveObject) IO = 
    myStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->FindIObject( Entry );
  Sel->RemoveIObject( IO );
}


/*!
  Removes all objects into selection.
*/
void SALOMEGUI_Swig::ClearIObjects()
{
  // VSR 06-05-03 ============================================================
  //QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  QAD_Study* myStudy = getActiveStudy();
  if ( !myStudy) return;
  // end of VSR 06-05-03 =====================================================
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );
  Sel->ClearIObjects();
}

/*!
  Display
*/		
void SALOMEGUI_Swig::Display(const char *Entry)
{
  // VSR 06-05-03 ============================================================
  //QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  QAD_Study* myStudy = getActiveStudy();
  if ( !myStudy) return;
  // end of VSR 06-05-03 =====================================================

  if ( !IsInCurrentView( Entry ) )
    return;

  Handle(SALOME_InteractiveObject) IO = 
    myStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->FindIObject( Entry );

  myStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->Display(IO);
}

/*!
  Display only
*/
void SALOMEGUI_Swig::DisplayOnly(const char *Entry)
{
  // VSR 06-05-03 ============================================================
  //QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  QAD_Study* myStudy = getActiveStudy();
  if ( !myStudy) return;
  // end of VSR 06-05-03 =====================================================

  if ( !IsInCurrentView( Entry ) )
    return;

  Handle(SALOME_InteractiveObject) IO = 
    myStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->FindIObject( Entry );

  if ( !IO.IsNull() ) {
    myStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->DisplayOnly(IO);
  }
}

/*!
  Erase
*/
void SALOMEGUI_Swig::Erase(const char *Entry)
{
  // VSR 06-05-03 ============================================================
  //QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  QAD_Study* myStudy = getActiveStudy();
  if ( !myStudy) return;
  // end of VSR 06-05-03 =====================================================

  if ( !IsInCurrentView( Entry ) )
    return;

  Handle(SALOME_InteractiveObject) IO = 
    myStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->FindIObject( Entry );

  myStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->Erase(IO);
}

/*!
  Display all
*/
void SALOMEGUI_Swig::DisplayAll()
{
  // VSR 06-05-03 ============================================================
  //QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  QAD_Study* myStudy = getActiveStudy();
  if ( !myStudy) return;
  // end of VSR 06-05-03 =====================================================
  myStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->DisplayAll();
}

/*!
  Erase only
*/
void SALOMEGUI_Swig::EraseAll()
{
  // VSR 06-05-03 ============================================================
  //QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  QAD_Study* myStudy = getActiveStudy();
  if ( !myStudy) return;
  // end of VSR 06-05-03 =====================================================
  SALOME_Selection* Sel 
    = SALOME_Selection::Selection( myStudy->getSelection() );
  
  Sel->ClearIObjects();
  myStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->EraseAll();
}

/*!
  Checks if object is displayed in current viewer
*/
bool SALOMEGUI_Swig::IsInCurrentView(const char *Entry)
{
  // VSR 06-05-03 ============================================================
  //QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  QAD_Study* myStudy = getActiveStudy();
  if ( !myStudy) return false;
  // end of VSR 06-05-03 =====================================================
  return myStudy->isInViewer( Entry, myStudy->getActiveStudyFrame()->entry() );
}

/*!
  Gets VTK renderer if available
*/
//san:T3.13 - move getRenderer() implementation from here to SalomePy.cxx
/*vtkRenderer *SALOMEGUI_Swig::getRenderer(int viewId)
{
  // VSR 06-05-03 ============================================================
  //QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  QAD_Application* app = QAD_Application::getDesktop()->getActiveApp();
  if ( !app ) return NULL;
  QAD_Study* myStudy = app->getActiveStudy();
  if ( !myStudy) return NULL;
  // end of VSR 06-05-03 =====================================================
  int nbStudyFrames = myStudy->getStudyFramesCount();
  vtkRenderer *myRenderer = NULL;
  if (viewId == -1) // find the first frame with VTK viewer & get renderer
    {
      int i=0;
      for(i=0; i<nbStudyFrames; i++)
	{
	  if ( myStudy->getStudyFrame(i)->getTypeView() == VIEW_VTK )
	    {
	      myRenderer = myStudy->getStudyFrame(i)->getRightFrame()->getViewFrame()->getRenderer();
	      break;
	    }
	}
    }
  else     // get the VTK renderer of a given frame
    {
      SCRUTE(viewId);
      if ((viewId >=0) && (viewId <nbStudyFrames))
	myRenderer = myStudy->getStudyFrame(viewId)->getRightFrame()->getViewFrame()->getRenderer();
    }
  if (myRenderer == NULL) INFOS("No VTK Renderer available !");
  return myRenderer;
}
*/

