using namespace std;
//=============================================================================
// File      : SALOMEGUI_Swig.cxx
// Created   : ven oct 12 15:44:16 CEST 2001
// Author    : Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2001

// Modified  : Mon Jul 29 21:38:07 2002
// Author    : Nicolas REJNERI
// Copyright : OPEN CASCADE 2002
// $Header$
//=============================================================================

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
SALOMEGUI_Swig::SALOMEGUI_Swig(): _studyId(0)
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
  Call when updated object browser.
*/
void SALOMEGUI_Swig::updateObjBrowser( bool updateSelection )
{
  QAD_Study* myActiveStudy = QAD_Application::getDesktop()->getActiveStudy();
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
  _studyId = QAD_Application::getDesktop()->getActiveStudy()->getStudyId();
  SCRUTE(_studyId);
  //    }
  return _studyId;
}

/*!
  Returns active Study Name.
*/
const char *SALOMEGUI_Swig::getActiveStudyName()
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  _name = myStudy->getTitle();
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
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );
  return Sel->IObjectCount();
}

/*!
  Returns the selected object at index i.
*/
const char* SALOMEGUI_Swig::getSelected(int i)
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
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
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );

  if ( !SALOMEGUI_Swig::IsInCurrentView( Entry ) )
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
  QAD_Study* myStudy
    = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );

  if ( !SALOMEGUI_Swig::IsInCurrentView( Entry ) )
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
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );
  Sel->ClearIObjects();
}

/*!
  Display
*/		
void SALOMEGUI_Swig::Display(const char *Entry)
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();

  if ( !SALOMEGUI_Swig::IsInCurrentView( Entry ) )
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
  if ( !SALOMEGUI_Swig::IsInCurrentView( Entry ) )
    return;

  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();

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
  if ( !SALOMEGUI_Swig::IsInCurrentView( Entry ) )
    return;

  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();

  Handle(SALOME_InteractiveObject) IO = 
    myStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->FindIObject( Entry );

  myStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->Erase(IO);
}

/*!
  Display all
*/
void SALOMEGUI_Swig::DisplayAll()
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  myStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->DisplayAll();
}

/*!
  Erse only
*/
void SALOMEGUI_Swig::EraseAll()
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel 
    = SALOME_Selection::Selection( myStudy->getSelection() );
  
  Sel->ClearIObjects();
  myStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame()->EraseAll();
}

/*!
  Check
*/
bool SALOMEGUI_Swig::IsInCurrentView(const char *Entry)
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  return myStudy->isInViewer( Entry, myStudy->getActiveStudyFrame()->entry() );
}

vtkRenderer *SALOMEGUI_Swig::getRenderer(int viewId)
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
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
