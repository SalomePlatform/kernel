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
//  File   : SALOMEGUI_Application.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

using namespace std;
#include "SALOMEGUI_Application.h"
#include "SALOMEGUI_Desktop.h"
#include "SALOMEGUI_ImportOperation.h"
#include "SALOME_Selection.h"
#include "SALOME_ListIO.hxx"
#include "SALOME_ListIteratorOfListIO.hxx"
#include "SALOME_InteractiveObject.hxx"
#include "QAD.h"
#include "QAD_Desktop.h"
#include "QAD_RightFrame.h"
#include "QAD_LeftFrame.h"
#include "QAD_ObjectBrowser.h"
#include "QAD_Resource.h"
#include "QAD_Tools.h"
#include "QAD_WaitCursor.h"

// QT Includes
#include <qapplication.h>
#include <qpopupmenu.h>
#include <qmessagebox.h>

// Open CASCADE Include
#include <Standard_Failure.hxx>
#include <TCollection_AsciiString.hxx>

/*!
    Constructor
*/
SALOMEGUI_Application::SALOMEGUI_Application( const QString& format,
					      const QString& description,
					      const QString& filters) :
QAD_Application( format, description, filters )
{
    /* load resources */
  QString message;
    QAD_ResourceMgr* resMgr = QAD_Desktop::getResourceManager();
    if ( !resMgr ) resMgr = QAD_Desktop::createResourceManager();
    if ( resMgr ) resMgr->loadResources( "SALOMEGUI", message );

    QPalette* palette = QAD_Desktop::getPalette();
    if ( !palette) palette = QAD_Desktop::createPalette();
    //	  if ( !palette) palette->loadPalette();

  //myViewActions.setAutoDelete( true );

    /*	We need to know that the desktop is created to have
	some additional internal initialization */
    QAD_ASSERT( connect( this, SIGNAL(desktopCreated()), SLOT(onDesktopCreated()) ));
}

/*!
    Destructor
*/
SALOMEGUI_Application::~SALOMEGUI_Application()
{
}

/*!
    Creates the actions provided by this application( internal )
*/
void SALOMEGUI_Application::createActions()
{
#ifdef DEBUG
    /* this function must be called only once */
    static bool created = false;
    QAD_ASSERT_DEBUG_ONLY( !created );
    created = true;
#endif

    /* Create 'View' actions
    */
    QToolBar* tbView = createToolBar( ViewToolBarId, tr("MEN_DESK_VIEWERTOOLBAR") );

    QAD_ResourceMgr* rmgr = QAD_Desktop::getResourceManager();
#define CREATE_ACTION(ID,NAME) \
    if(!myViewActions.at(ID)){ \
      QActionP* action = new QActionP(tr("TOT_APP_VIEW_" #NAME), \
      			    rmgr->loadPixmap("SALOMEGUI", tr("ICON_APP_VIEW_" #NAME)), \
      			    tr("MEN_APP_VIEW_" #NAME), 0, QAD_Application::getDesktop()); \
      action->setStatusTip(tr("PRP_APP_VIEW_" #NAME)); \
      myViewActions.insert(ID,action); \
    }

    CREATE_ACTION(ViewDumpId,DUMP);
    CREATE_ACTION(ViewTrihedronId,TRIHEDRON);
    CREATE_ACTION(ViewFitAllId,FITALL);
    CREATE_ACTION(ViewFitAreaId,FITAREA);
    CREATE_ACTION(ViewZoomId,ZOOM);
    CREATE_ACTION(ViewPanId,PAN);
    CREATE_ACTION(ViewRotateId,ROTATE);
    CREATE_ACTION(ViewGlobalPanId,GLOBALPAN);
    CREATE_ACTION(ViewFrontId,FRONT);
    CREATE_ACTION(ViewBackId,BACK);
    CREATE_ACTION(ViewTopId,TOP);
    CREATE_ACTION(ViewBottomId,BOTTOM);
    CREATE_ACTION(ViewLeftId,LEFT);
    CREATE_ACTION(ViewRightId,RIGHT);
    CREATE_ACTION(ViewResetId,RESET);

#undef CREATE_ACTION
}

/*!
    Enables/disables the actions provided by this
    application( internal )
*/
void SALOMEGUI_Application::updateActions()
{
}

/*!
    Creates CAF application
*/
bool SALOMEGUI_Application::initApp(SALOME_NamingService* name_service)
{
  if ( myStudyMgr->_is_nil() ) {
    try {
      CORBA::Object_ptr obj = name_service->Resolve("/myStudyManager");
      myStudyMgr = SALOMEDS::StudyManager::_narrow(obj);
    }
    catch ( Standard_Failure ) {
      return false;
    }
  }

  return true;
}

/*!
    Creates the main desktop
*/
bool SALOMEGUI_Application::createDesktop( SALOME_NamingService* name_service )
{
  QAD_ASSERT_DEBUG_ONLY( !desktop );
  desktop = new SALOMEGUI_Desktop(name_service);
  return ( desktop != NULL );
}

/*!
    Called when the main desktop is created ( internal )
*/
void SALOMEGUI_Application::onDesktopCreated()
{
  createActions();
}

/*!
    Called for customization when study is created
*/
void SALOMEGUI_Application::onStudyCreated( QAD_Study* study )
{
}

/*!
    Called for customization when study is opened
*/
void SALOMEGUI_Application::onStudyOpened( QAD_Study* study )
{
}

/*!
    Called for customization when study is activated (shown)
*/
void SALOMEGUI_Application::onStudyShown( QAD_Study* study )
{
  SALOMEDS::Study_var aStudy = study->getStudyDocument();

//srn: Forbid appending Interface Applicative and its children to UseCase
  aStudy->EnableUseCaseAutoFilling(false); 

  /* mpv: study may be locked */
  int aLocked = aStudy->GetProperties()->IsLocked();
  if (aLocked) aStudy->GetProperties()->SetLocked(false);

  /* update Object Browser */
  SALOMEDS::StudyBuilder_var B = aStudy->NewBuilder();
  SALOMEDS::SComponentIterator_var itcomp = aStudy->NewComponentIterator();
  for (; itcomp->More(); itcomp->Next()) {
    SALOMEDS::SComponent_var SC = itcomp->Value();
    Standard_CString dataType = SC->ComponentDataType();
    if ( QString(dataType).compare("Interface Applicative") == 0 ) {
      SALOMEDS::ChildIterator_var it = aStudy->NewChildIterator(SC);
      for (; it->More();it->Next()) {
	SALOMEDS::SObject_var CSO = it->Value();
	B->RemoveObject(CSO);
      }
    }
  }

  QAD_StudyFrame* sf = myActiveStudy->getActiveStudyFrame();
  Standard_CString name = strdup(sf->title().latin1());
  
  SALOMEDS::SComponent_var father = aStudy->FindComponent("Interface Applicative");
  SALOMEDS::SObject_var newObj = B->NewObject(father);
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeName_var    aName;
  SALOMEDS::AttributeComment_var aComment;
  SALOMEDS::AttributeSelectable_var aSelAttr;

  anAttr = B->FindOrCreateAttribute(newObj, "AttributeName");
  aName = SALOMEDS::AttributeName::_narrow(anAttr);
  aName->SetValue(name);

  anAttr = B->FindOrCreateAttribute(newObj, "AttributeSelectable");
  aSelAttr = SALOMEDS::AttributeSelectable::_narrow(anAttr);
  aSelAttr->SetSelectable(false);

  if (sf->getTypeView() == VIEW_OCC) {
    anAttr = B->FindOrCreateAttribute(newObj, "AttributeComment");
    aComment = SALOMEDS::AttributeComment::_narrow(anAttr);
    aComment->SetValue("OCC");
  }
  else if (sf->getTypeView() == VIEW_VTK) {
    anAttr = B->FindOrCreateAttribute(newObj, "AttributeComment");
    aComment = SALOMEDS::AttributeComment::_narrow(anAttr);
    aComment->SetValue("VTK");
  } else {
    anAttr = B->FindOrCreateAttribute(newObj, "AttributeComment");
    aComment = SALOMEDS::AttributeComment::_narrow(anAttr);
    aComment->SetValue("GRAPH");
  }   
  sf->setEntry(newObj->GetID());
  
  if (aLocked) aStudy->GetProperties()->SetLocked(true);

  study->updateObjBrowser( false );

  aStudy->EnableUseCaseAutoFilling(true); 
}

/*!
    Called for customization when the active study is changed
*/
void SALOMEGUI_Application::onActiveStudyChanged( QAD_Study* oldActiveStudy,
						  QAD_Study* newActiveStudy )
{
  //  MESSAGE ("SALOMEGUI_Application::onActiveStudyChanged init.");
  /* superclass MUST be called */
  QAD_Application::onActiveStudyChanged( oldActiveStudy, newActiveStudy );

  if ( oldActiveStudy ) {
    int count = oldActiveStudy->getStudyFramesCount();
    for ( int i = 0; i < count; i++ ) {
      disconnectFromViewer( oldActiveStudy->getStudyFrame(i)->getRightFrame()->getViewFrame() );
    }
  }
  
  if ( newActiveStudy && newActiveStudy->getActiveStudyFrame() ) {
    connectToViewer( newActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame() );
  }
}

void SALOMEGUI_Application::connectToViewer( QAD_ViewFrame* vf )
{
#define CONNECT_ACTION(NAME) \
  QAD_ASSERT(connect(myViewActions.at(View ##NAME ##Id), SIGNAL(activated()), vf, SLOT(onView ##NAME())))

  if ( vf ) {
    QToolBar* tbView = getToolBar( ViewToolBarId );
    for ( int cmd = ViewDumpId; cmd <= ViewResetId; cmd++ )
      if ( myViewActions.at( cmd ) )
	myViewActions.at( cmd )->removeFrom(tbView);
    if( vf->getTypeView() == VIEW_GRAPHSUPERV ) {
      //myViewActions.at(ViewDumpId)->addTo(tbView);
      //myViewActions.at(ViewFitAllId)->addTo(tbView);
      //myViewActions.at(ViewFitAreaId)->addTo(tbView);
      //myViewActions.at(ViewZoomId)->addTo(tbView);
      myViewActions.at(ViewPanId)->addTo(tbView);
      //myViewActions.at(ViewGlobalPanId)->addTo(tbView);
      myViewActions.at(ViewResetId)->addTo(tbView);
    }
    else if( vf->getTypeView() == VIEW_PLOT2D ) {
      myViewActions.at(ViewDumpId)->addTo(tbView);
      myViewActions.at(ViewFitAllId)->addTo(tbView);
      myViewActions.at(ViewFitAreaId)->addTo(tbView);
      myViewActions.at(ViewZoomId)->addTo(tbView);
      myViewActions.at(ViewPanId)->addTo(tbView);
      //myViewActions.at(ViewGlobalPanId)->addTo(tbView);
      //myViewActions.at(ViewResetId)->addTo(tbView);
    }
    else {
      for ( int cmd = ViewDumpId; cmd <= ViewResetId; cmd++ )
	if ( myViewActions.at( cmd ) )
	  myViewActions.at( cmd )->addTo(tbView);
    }
    //CONNECT_ACTION(Mu4Id)
    CONNECT_ACTION(Dump);
    CONNECT_ACTION(Trihedron);
    CONNECT_ACTION(FitAll);
    CONNECT_ACTION(FitArea);
    CONNECT_ACTION(Zoom);
    CONNECT_ACTION(Pan);
    CONNECT_ACTION(Rotate);
    CONNECT_ACTION(GlobalPan);
    CONNECT_ACTION(Front);
    CONNECT_ACTION(Back);
    CONNECT_ACTION(Top);
    CONNECT_ACTION(Bottom);
    CONNECT_ACTION(Left);
    CONNECT_ACTION(Right);
    CONNECT_ACTION(Reset);
  }

#undef CONNECT_ACTION
}

void SALOMEGUI_Application::disconnectFromViewer( QAD_ViewFrame* vf )
{
#define DISCONNECT_ACTION(NAME) \
  QAD_ASSERT(disconnect(myViewActions.at(View ##NAME ##Id), SIGNAL(activated()), vf, SLOT(onView ##NAME())));

  if ( !vf || myViewActions.isEmpty() )
    return;
  //DISCONNECT_ACTION(Mu4Id)
  DISCONNECT_ACTION(Dump);
  DISCONNECT_ACTION(Trihedron);
  DISCONNECT_ACTION(FitAll);
  DISCONNECT_ACTION(FitArea);
  DISCONNECT_ACTION(Zoom);
  DISCONNECT_ACTION(Pan);
  DISCONNECT_ACTION(Rotate);
  DISCONNECT_ACTION(GlobalPan);
  DISCONNECT_ACTION(Front);
  DISCONNECT_ACTION(Back);
  DISCONNECT_ACTION(Top);
  DISCONNECT_ACTION(Bottom);
  DISCONNECT_ACTION(Left);
  DISCONNECT_ACTION(Right);
  DISCONNECT_ACTION(Reset);

#undef DISCONNECT_ACTION
}

/*!
    Customizes 'View' menu of the desktop
*/
int SALOMEGUI_Application::onUpdateViewActions ( QPopupMenu* popup, bool add, int index )
{
  int nItemsAdded = QAD_Application::onUpdateViewActions( popup, add, index );
  // NRI
  //	if ( add ) {
  //	  index += nItemsAdded;
  //	  QAD_ASSERT_DEBUG_ONLY( !myViewActions.isEmpty() );
  //	  popup->insertSeparator( index++ );
  //	  myViewActions.at( ViewTrihedronId )->addTo( popup );
  //	  index++;
  //	  popup->insertSeparator( index++ );
  //	  for ( int i = (int)ViewMu4Id; i <= (int)ViewRightId; i++ ) {
  //	    myViewActions.at( i )->addTo( popup );
  //	    index++;
  //	  }
  //	  popup->insertSeparator( index++ );
  //	  myViewActions.at( ViewResetId )->addTo( popup );
  //	  index++;
  //	} else {
  //	  for ( int i = (int)ViewTrihedronId; i <= (int)ViewResetId; i++ ) {
  //	    popup->removeItemAt(index);
  //	  }

  //	  /* remove separators */
  //	  popup->removeItemAt( index );
  //	  popup->removeItemAt( index );
  //	  popup->removeItemAt( index );
  //	}

  return index;
}

/*!
    Adds the context popup items
*/
void SALOMEGUI_Application::onCreatePopup()
{
  // Here common menu items should be added (Display, Erase, etc.).
  // They should be obtained through the standard XML-based mechanism,
  // i.e. corresponding XML file should be loaded somewhere in advance 
  // and desktop->getOperatorsMenu() should return the default popup menu.
  // These items are always added; if necessary any of them can be hidden
  // during popup customization by means of the corresponding unique ID defined in QAD_Resource.h.
  SALOME_Selection*      Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );

  if ( Sel->IObjectCount() > 0 ) {
    myPopup->insertItem (tr ("MEN_APP_DISPLAY"),      this, SLOT(onDisplay(int)), 0, QAD_Display_Popup_ID);
    myPopup->insertItem (tr ("MEN_APP_DISPLAY_ONLY"), this, SLOT(onDisplay(int)), 0, QAD_DisplayOnly_Popup_ID);
    myPopup->insertItem (tr ("MEN_APP_ERASE"),        this, SLOT(onErase()),      0, QAD_Erase_Popup_ID);
  }

}


void SALOMEGUI_Application::onDisplay(int id)
{
  QAD_WaitCursor waitCursor;

  // Obtain the component to deal with (according to selected object's parent component)
  SALOME_Selection* Sel = SALOME_Selection::Selection(myActiveStudy->getSelection() );
  
  QString parentComp = ((SALOMEGUI_Desktop*)desktop)->getComponentFromSelection();
  if (parentComp.isNull()) { // objects from several components are selected -> do nothing
    return;
  }

  // Obtain the component's GUI library  
  // Library cashing will be implemented soon in QAD_Desktop to increase performance
  OSD_Function osdF, osdViewTypeFunc;
  OSD_SharedLibrary foreignGUI;
  void (*builder)(const Handle(SALOME_InteractiveObject)&); 
  bool isForeignGUIUsed = false;
  bool isViewTypeOK     = true;
  int  viewTypes[VIEW_TYPE_MAX];

  for (int i = 0; i < VIEW_TYPE_MAX; i++)
    viewTypes[i] = -1;

  if (parentComp.compare(desktop->getActiveComponent()) == 0) { // use active GUI library
    const OSD_SharedLibrary& compGUI = desktop->getHandle();
    osdF = compGUI.DlSymb("buildPresentation");
    if ( osdF == NULL ) {
      MESSAGE("BuildPresentation method not found in component's GUI")
      return;
    }
    osdViewTypeFunc = compGUI.DlSymb("supportedViewType");
    if ( osdViewTypeFunc == NULL ) {
      MESSAGE("supportedViewType method not found in component's GUI")
    }
    MESSAGE("onDisplay(): using active GUI to build presentations")
  } else { // use native GUI library
    QString ComponentLib;
    QCString dir;
    QFileInfo fileInfo ;
    bool found = false;
    if ( getenv( QAD_Application::getDesktop()->getComponentName(parentComp) + "_ROOT_DIR")  ) {
      dir.fill('\0');
      dir.sprintf("%s", getenv( QAD_Application::getDesktop()->getComponentName(parentComp) + "_ROOT_DIR"));
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
      dir = dir + "lib" + QAD_Application::getDesktop()->getComponentName(parentComp).latin1() + "GUI.dll" ;
#else
      dir = dir + "lib" + QAD_Application::getDesktop()->getComponentName(parentComp).latin1() + "GUI.so" ;
#endif
      MESSAGE ( " GUI library = " << dir )
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ComponentLib = fileInfo.fileName() ;
	found = true;
	MESSAGE ( " found " )
      } else {
	MESSAGE ( " Not found " )
      }
    }
    
    if (ComponentLib.isEmpty()) {
      waitCursor.stop();
      QMessageBox::critical( desktop,
			    tr("ERR_ERROR"),
			    "Empty name of component "+ parentComp + " library");
      return;
    }

    foreignGUI.SetName(TCollection_AsciiString((char*)ComponentLib.latin1()).ToCString());
   
    bool ok = foreignGUI.DlOpen(OSD_RTLD_LAZY);
    if (!ok) {
      waitCursor.stop();
      QMessageBox::critical( desktop,
			   tr("ERR_ERROR"),
			   tr( foreignGUI.DlError() ) );
      return;
    }
    
    osdF = foreignGUI.DlSymb("buildPresentation");
    if ( osdF == NULL ) {
      MESSAGE("BuildPresentation method not found in component's GUI")
      foreignGUI.DlClose();
      return;
    } 
    osdViewTypeFunc = foreignGUI.DlSymb("supportedViewType");
    if ( osdViewTypeFunc == NULL ) {
      MESSAGE("supportedViewType method not found in component's GUI")
    }
    isForeignGUIUsed = true;
    MESSAGE("onDisplay(): using parent component's GUI to build presentations")
  }

  // Check if another view type is required (if viewToActivate < 0 then any type of view is acceptable)
  if (osdViewTypeFunc) {
    void (*viewTypeChecker)(int*, int) = (void (*)(int*, int)) osdViewTypeFunc; 
    (*viewTypeChecker)(viewTypes, VIEW_TYPE_MAX);
    if (viewTypes[0] >= 0) { // not all the view types are supported
      for (int i = 0; i < VIEW_TYPE_MAX; i++) {
	if (viewTypes[i] < 0) // no more types supported
	  break;
	isViewTypeOK = ((int)myActiveStudy->getActiveStudyFrame()->getTypeView() == viewTypes[i]);
	if (isViewTypeOK) // one of supported views is already active
	  break;
      }
    }
  }

  // Try to activate a view of one of supported types
  if (!isViewTypeOK) {
    MESSAGE("onDisplay(): view type is not acceptable")
    unsigned frameCount = myActiveStudy->getStudyFramesCount();
    for (unsigned i = 0; i < frameCount; i++) {
      QAD_StudyFrame* frame = myActiveStudy->getStudyFrame(i);
      for (int j = 0; j < VIEW_TYPE_MAX; j++) {
	if (frame->getTypeView() == viewTypes[j]) {
	  MESSAGE("onDisplay(): activating other frame")
	    isViewTypeOK = true;
	  frame->setFocus();
	  break;
	}
      }
    }
    if (!isViewTypeOK) {
      waitCursor.stop();
      QMessageBox::warning( desktop,
			   tr("ERR_ERROR"),
			   tr("ERR_APP_WRONG_VIEW_TYPE"));
      return;
    }
  }

  QAD_ViewFrame* viewFrame = myActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame();

  // Build the graphic presentation (it is stored in the corresponding viewer)
  builder = (void (*) (const Handle(SALOME_InteractiveObject)&)) osdF;

  // Copy the selection
  SALOME_ListIteratorOfListIO itInit( Sel->StoredIObjects() );
  SALOME_ListIO selList;
  for (; itInit.More(); itInit.Next()) {
    selList.Append(itInit.Value());
  }
  
  SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();

  bool needRepaint = false;
    
  if (id == QAD_DisplayOnly_Popup_ID) 
    viewFrame->EraseAll();

  SALOME_ListIteratorOfListIO It( selList );
  for(;It.More();It.Next()) {
    Handle(SALOME_InteractiveObject) IObject    = It.Value();

    // First check whether the object is a component or a child object
    bool isComponent = false;

    if (!IObject->hasEntry()) continue; 

    SALOMEDS::SObject_var obj = aStudy->FindObjectID(IObject->getEntry());
    if (!obj->_is_nil()) {
      SALOMEDS::SComponent_var comp = obj->GetFatherComponent();
      isComponent = (strcmp(comp->GetID(), obj->GetID()) == 0);
    }

    // For component -> display all children
    if (isComponent) {
      SALOMEDS::ChildIterator_ptr it = aStudy->NewChildIterator(obj);
      it->InitEx(true);
      for ( ; it->More(); it->Next()) {
	SALOMEDS::SObject_ptr child = it->Value();
	Handle(SALOME_InteractiveObject) childIObject = new SALOME_InteractiveObject();
	childIObject->setEntry(child->GetID());

	// Ensure that proper 3D presentation exists for IObject
	(*builder)(childIObject);
	viewFrame->Display(childIObject, false);
	needRepaint = true;
      }
    } else { // for child object -> simply display it (no children are displayed)
      // Ensure that proper 3D presentation exists for IObject
      (*builder)(IObject);
      viewFrame->Display(IObject, false);
      needRepaint = true;
    }
    
  }

  if (needRepaint)
    viewFrame->Repaint();

  if (isForeignGUIUsed)
    foreignGUI.DlClose();
  myActiveStudy->updateObjBrowser(true);
}



void SALOMEGUI_Application::onErase()
{
  QAD_WaitCursor waitCursor;

  SALOME_Selection* Sel = SALOME_Selection::Selection(myActiveStudy->getSelection() );
  SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();
  QAD_ViewFrame*   viewFrame = myActiveStudy->getActiveStudyFrame()->getRightFrame()->getViewFrame();
  bool needRepaint = false;

  // Copy the selection
  SALOME_ListIteratorOfListIO itInit( Sel->StoredIObjects() );
  SALOME_ListIO selList;
  for (; itInit.More(); itInit.Next()) {
    selList.Append(itInit.Value());
  }

  SALOME_ListIteratorOfListIO It( selList );
  for(;It.More();It.Next()) {
    Handle(SALOME_InteractiveObject) IObject = It.Value();

    // First check whether the object is a component or a child object
    bool isComponent = false;
    SALOMEDS::SObject_var obj;

    if (IObject->hasEntry()) {
      obj = aStudy->FindObjectID(IObject->getEntry());
      if (!obj->_is_nil()) {
	SALOMEDS::SComponent_var comp = obj->GetFatherComponent();
	isComponent = (strcmp(comp->GetID(), obj->GetID()) == 0);
      }
    }

    // For component -> erase all children
    if (isComponent) {
      SALOMEDS::ChildIterator_ptr it = aStudy->NewChildIterator(obj);
      it->InitEx(true);
      for ( ; it->More(); it->Next()) {
	SALOMEDS::SObject_ptr child = it->Value();
	Handle(SALOME_InteractiveObject) childIObject = new SALOME_InteractiveObject();
	childIObject->setEntry(child->GetID());
	viewFrame->Erase(childIObject, false);
      }
    } else { // for child object -> simply erase it (no children are erased)
      viewFrame->Erase(IObject, false);
    }
    needRepaint = true;
  }

  if (needRepaint)
    viewFrame->Repaint();

}
