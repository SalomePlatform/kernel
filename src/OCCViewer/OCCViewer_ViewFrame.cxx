//  SALOME OCCViewer : build OCC Viewer into Salome desktop
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
//  File   : OCCViewer_ViewFrame.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "OCCViewer_ViewFrame.h"
#include "OCCViewer_Viewer3d.h"
#include "OCCViewer_ViewPort.h"
#include "OCCViewer_ViewPort3d.h"
#include "OCCViewer_Prs.h"

#include "QAD.h"
#include "QAD_Tools.h"
#include "QAD_Desktop.h"
#include "QAD_ViewFrame.h"
#include "QAD_MessageBox.h"
#include "QAD_Application.h"
#include "utilities.h"

#include "QAD_Config.h"
#include "QAD_Settings.h"
#include "SALOME_Selection.h"
#include "SALOME_AISShape.hxx"
#include "SALOMEGUI.h"
#include "SALOMEDS_Tool.hxx"
#include "ToolsGUI.h"

// QT Include
#include <qapplication.h>

// Open CASCADE Include
#include <V3d_View.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <Visual3d_View.hxx>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

using namespace std;

//=======================================================================
// name    : getMapOfEntry
// Purpose : Convert list of interactive objects in map <entry <--> interactive object>
//=======================================================================
static void getMapOfEntry( const AIS_ListOfInteractive&                    theList,
                           QMap< QString, Handle(AIS_InteractiveObject) >& theMap )
{
  AIS_ListIteratorOfListOfInteractive anIter( theList );
  for ( ; anIter.More(); anIter.Next() )
  {
    Handle(SALOME_InteractiveObject) anIO =
      Handle(SALOME_InteractiveObject)::DownCast( anIter.Value() );
    if ( !anIO.IsNull() )
      theMap[ anIO->getEntry() ] = anIter.Value();
  }
}


/*
  Class       : OCCViewer_ViewFrame
  Description : View frame for Open CASCADE view
*/


//=======================================================================
// name    : OCCViewer_ViewFrame
// Purpose : Constructor
//=======================================================================
OCCViewer_ViewFrame::OCCViewer_ViewFrame( QWidget* parent, const QString& title ) 
  : QAD_ViewFrame ( parent, "" )
{
  initialize();
}

//=======================================================================
// name    : OCCViewer_ViewFrame
// Purpose : Constructor
//=======================================================================
OCCViewer_ViewFrame::OCCViewer_ViewFrame( QWidget* parent ) 
  : QAD_ViewFrame (parent, "")
{
  initialize();
}

//=======================================================================
// name    : ~OCCViewer_ViewFrame
// Purpose : Destructor
//=======================================================================
OCCViewer_ViewFrame::~OCCViewer_ViewFrame()
{
  cleanup();
}

//=======================================================================
// name    : initialize
// Purpose : Initialize view frame (called from constructors )
//=======================================================================
void OCCViewer_ViewFrame::initialize()
{
  myViewPort = NULL;
  myViewer   = new OCCViewer_Viewer3d( this );
  
  /* enable/disable selection */
  myViewer->enableSelection( true );
  myViewer->enableMultipleSelection( true );
}

//=======================================================================
// name    : cleanup
// Purpose : Cleanup viewframe 
//=======================================================================
void OCCViewer_ViewFrame::cleanup()
{
} 

//=======================================================================
// name    : setViewPort
// Purpose : Sets the viewport for this frame
//=======================================================================
void OCCViewer_ViewFrame::setViewPort( OCCViewer_ViewPort* view )
{
  myViewPort = view;
  initViewPort();		
  setCentralWidget ( myViewPort );
}

//=======================================================================
// name    : initViewPort
// Purpose : Inits the viewport for this frame
//=======================================================================
void OCCViewer_ViewFrame::initViewPort()
{
  /* Active Key Event */
  
  setFocus();

  /* Initial attributes */
  
  myViewPort->setCursor( cursor() );
  myViewPort->setBackgroundColor( backgroundColor() );
  
  QAD_ASSERT( QObject::connect(myViewPort, SIGNAL( vpKeyPress ( QKeyEvent* ) ), 
              this, SIGNAL(vfKeyPress(QKeyEvent*))) );

  /*  Listen to my viewport */
  
  QAD_ASSERT( connect( myViewPort, SIGNAL( vpTransformationStarted( OCCViewer_ViewPort::OperationType ) ), 
                       this, SIGNAL( vfTransformationStarted( OCCViewer_ViewPort::OperationType ) ) ) );
  QAD_ASSERT( connect( myViewPort, SIGNAL( vpTransformationFinished ( OCCViewer_ViewPort::OperationType ) ),
                       this, SIGNAL( vfTransformationFinished( OCCViewer_ViewPort::OperationType ) ) ) );

  QAD_ASSERT( connect( myViewPort, SIGNAL( vpDrawExternal( QPainter* ) ), 
                       this, SIGNAL( vfDrawExternal( QPainter* ) ) ) );	
  QAD_ASSERT( connect( myViewPort, SIGNAL( vpMousePress( QMouseEvent* ) ), 
                       this, SIGNAL( vfMousePress( QMouseEvent* ) ) ) );
  QAD_ASSERT( connect( myViewPort, SIGNAL( vpMouseRelease( QMouseEvent* ) ),
                       this, SIGNAL( vfMouseRelease( QMouseEvent* ) ) ) );
  QAD_ASSERT( connect( myViewPort, SIGNAL( vpMouseMove( QMouseEvent* ) ), 
                       this, SIGNAL( vfMouseMove( QMouseEvent* ) ) ) );
  QAD_ASSERT( connect( myViewPort, SIGNAL( vpMouseDoubleClick( QMouseEvent* ) ), 
                       this, SIGNAL( vfMouseDoubleClick( QMouseEvent* ) ) ) );

  QAD_ASSERT( connect( myViewPort, SIGNAL( vpKeyPress( QKeyEvent* ) ), 
                       this, SIGNAL( vfKeyPress( QKeyEvent* ) ) ) );
  QAD_ASSERT( connect( myViewPort, SIGNAL( vpKeyRelease( QKeyEvent* ) ),
                       this, SIGNAL( vfKeyRelease( QKeyEvent* ) ) ) );
}

//=======================================================================
// name    : getViewWidget
// Purpose : Returns widget containing 3D-Viewer
//=======================================================================
QWidget* OCCViewer_ViewFrame::getViewWidget() 
{
  return (QWidget*)getViewPort();
}

//=======================================================================
// name    : getViewPort
// Purpose : Returns the viewport of this frame	
//=======================================================================
OCCViewer_ViewPort* OCCViewer_ViewFrame::getViewPort() const
{
  return myViewPort;
}

//=======================================================================
// name    : Sets the cursor for the viewframe's viewport
// Purpose : setCursor
//=======================================================================
void OCCViewer_ViewFrame::setCursor( const QCursor& cursor)
{
  if ( myViewPort ) 
    myViewPort->QWidget::setCursor( cursor );
}


//=======================================================================
// name    : cursor
// Purpose : Returns the current cursor 
//=======================================================================
QCursor OCCViewer_ViewFrame::cursor() const
{
  if ( myViewPort ) 
    return myViewPort->cursor();
  return QMainWindow::cursor();
}

//=======================================================================
// name    : setBackgroundColor
// Purpose : Set background of the viewport
//=======================================================================
void OCCViewer_ViewFrame::setBackgroundColor( const QColor& color)
{
  if ( myViewPort )
    myViewPort->setBackgroundColor( color );
}

//=======================================================================
// name    : backgroundColor
// Purpose : Returns background of the viewport
//=======================================================================
QColor OCCViewer_ViewFrame::backgroundColor() const
{
  if ( myViewPort )
    return myViewPort->backgroundColor();
  return QMainWindow::backgroundColor();
}

//=======================================================================
// name    : setViewer
// Purpose : Sets the parent viewer for the window
//=======================================================================
void OCCViewer_ViewFrame::setViewer( OCCViewer_Viewer3d* viewer )
{
  myViewer = viewer;
}

//=======================================================================
// name    : getViewer
// Purpose : Returns the parent viewer for the window
//=======================================================================
OCCViewer_Viewer3d* OCCViewer_ViewFrame::getViewer() const
{
  return myViewer;
}

//=======================================================================
// name    : setVisible
// Purpose : Show/hide view
//=======================================================================
void OCCViewer_ViewFrame::setVisible( const bool visible )
{
  if ( visible == QWidget::isVisible() )
    return;
  
  if ( visible )
    show();
  else
    hide();
}

//=======================================================================
// name    : closeEvent
// Purpose : Called when viewframe is about to close
//=======================================================================
void OCCViewer_ViewFrame::closeEvent( QCloseEvent* e )
{
  emit vfViewClosing( e );  /* notify our viewer */
}

//=======================================================================
// name    : resizeEvent
// Purpose : Called when viewframe is resized 
//=======================================================================
void OCCViewer_ViewFrame::resizeEvent( QResizeEvent* e )
{
  emit vfResize( e );
}

//=======================================================================
// name    : keyPressEvent
// Purpose : Called when key is pressed
//=======================================================================
void OCCViewer_ViewFrame::keyPressEvent( QKeyEvent *k )
{
  emit vfKeyPress(k);
}

//=======================================================================
// name    : onViewFitAll
// Purpose : Fits all objects in the active view
//=======================================================================
void OCCViewer_ViewFrame::onViewFitAll()
{
  myViewPort->fitAll();
}

//=======================================================================
// name    : onViewFitArea
// Purpose : Fits all obejcts within a rectangular area of the active view
//=======================================================================
void OCCViewer_ViewFrame::onViewFitArea()
{
  myViewPort->activateWindowFit();
  QAD_Application::getDesktop()->putInfo( tr( "PRP_VW3D_SKETCHAREA" ) );
}

//=======================================================================
// name    : onViewPan
// Purpose : Moves the active view
//=======================================================================
void OCCViewer_ViewFrame::onViewPan()
{
  myViewPort->activatePanning();
}

//=======================================================================
// name    : onViewZoom
// Purpose : Zooms the active view
//=======================================================================
void OCCViewer_ViewFrame::onViewZoom()
{
  myViewPort->activateZoom();
}

//=======================================================================
// name    : onViewGlobalPan
// Purpose : Sets a new center of the active view
//=======================================================================
void OCCViewer_ViewFrame::onViewGlobalPan()
{
  myViewPort->activateGlobalPanning();
  QAD_Application::getDesktop()->putInfo( tr("PRP_VW3D_POINTCENTER") );
}

//=======================================================================
// name    : onViewRotate
// Purpose : Rotates the active view
//=======================================================================
void OCCViewer_ViewFrame::onViewRotate()
{
  myViewPort->activateRotation();
}

//=======================================================================
// name    : onViewReset
// Purpose : Reset the active view
//=======================================================================
void OCCViewer_ViewFrame::onViewReset()
{
  myViewPort->reset();
}

//=======================================================================
// name    : onViewFront
// Purpose : Provides front projection of the active view
//=======================================================================
void OCCViewer_ViewFrame::onViewFront()
{
  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)myViewPort)->getView();
  if ( !view3d.IsNull() ) view3d->SetProj (V3d_Xpos);
  onViewFitAll();
}

//=======================================================================
// name    : onViewBack
// Purpose : Provides back projection of the active view
//=======================================================================
void OCCViewer_ViewFrame::onViewBack()
{
  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)myViewPort)->getView();
  if ( !view3d.IsNull() ) view3d->SetProj (V3d_Xneg);
  onViewFitAll();
}

//=======================================================================
// name    : onViewRight
// Purpose : Provides right projection of the active view
//=======================================================================
void OCCViewer_ViewFrame::onViewRight()
{
  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)myViewPort)->getView();
  if ( !view3d.IsNull() )
    view3d->SetProj( V3d_Ypos );
  onViewFitAll();
}

//=======================================================================
// name    : onViewLeft
// Purpose : Provides left projection of the active view
//=======================================================================
void OCCViewer_ViewFrame::onViewLeft()
{
  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)myViewPort)->getView();
  if ( !view3d.IsNull() )
    view3d->SetProj( V3d_Yneg );
  onViewFitAll();
}

//=======================================================================
// name    : onViewBottom
// Purpose : Provides bottom projection of the active view
//=======================================================================
void OCCViewer_ViewFrame::onViewBottom()
{
  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)myViewPort)->getView();
  if ( !view3d.IsNull() ) view3d->SetProj(V3d_Zneg);
  onViewFitAll();
}

//=======================================================================
// name    : onViewTop
// Purpose : Provides top projection of the active view
//=======================================================================
void OCCViewer_ViewFrame::onViewTop()
{
  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)myViewPort)->getView();
  if ( !view3d.IsNull() ) view3d->SetProj( V3d_Zpos );
  onViewFitAll();
}

//=======================================================================
// name    : onViewTrihedron
// Purpose : Display/hide Trihedron
//=======================================================================
void OCCViewer_ViewFrame::onViewTrihedron()
{
  if (!myViewer->getAISContext()->IsDisplayed( myViewer->getTrihedron() ))
    myViewer->getAISContext()->Display( myViewer->getTrihedron() );
  else
    myViewer->getAISContext()->Erase( myViewer->getTrihedron() );
  onAdjustTrihedron();
}

//=======================================================================
// name    : rename
// Purpose : Rename entry object
//=======================================================================
void OCCViewer_ViewFrame::rename( const Handle(SALOME_InteractiveObject)& IObject,
                                  const QString newName )
{
  myViewer->rename(IObject, newName);
}

//=======================================================================
// name    : unHighlightAll
// Purpose : Unhighlight all objects
//=======================================================================
void OCCViewer_ViewFrame::unHighlightAll() 
{
  myViewer->unHighlightAll();
}

//=======================================================================
// name    : highlight
// Purpose : Highlight object
//=======================================================================
void OCCViewer_ViewFrame::highlight( const Handle(SALOME_InteractiveObject)& IObject,
                                     const bool highlight,
                                     const bool update ) 
{
  myViewer->highlight( IObject, highlight, update );
}

//=======================================================================
// name    : isInViewer
// Purpose : Magic function
//=======================================================================
bool OCCViewer_ViewFrame::isInViewer( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  return myViewer->isInViewer( IObject );
}

//=======================================================================
// name    : isVisible
// Purpose : Verify whether object is visible
//=======================================================================
bool OCCViewer_ViewFrame::isVisible( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  return myViewer->isVisible( IObject );
}

//=======================================================================
// name    : setPopupServer
// Purpose : Set popup server
//=======================================================================
void OCCViewer_ViewFrame::setPopupServer( QAD_Application* App )
{
  myViewer->setPopupServer( App );
}

//=======================================================================
// name    : redisplayAll
// Purpose : Redisplay all objects of active component in accordance with
//           their display flags
//=======================================================================
void OCCViewer_ViewFrame::redisplayAll( QAD_Study* theQADStudy, const bool theToUpdate ) 
{
  SALOMEDS::Study_var      aStudy     = theQADStudy->getStudyDocument();
  QAD_Desktop*             aDesktop   = QAD_Application::getDesktop();
  SALOMEGUI*               aGUI       = aDesktop->getActiveGUI();
  const QString&           aCompName  = aDesktop->getComponentDataType();
  SALOMEDS::SObject_var    aComponent =
    SALOMEDS::SObject::_narrow( aStudy->FindComponent ( aCompName.latin1() ) );

  if ( aComponent->_is_nil() )
    return;

  Handle(AIS_InteractiveContext) anIC = myViewer->getAISContext();
  bool isTrhDisplayed = anIC->IsDisplayed( myViewer->getTrihedron() );
  
  anIC->DisplayAll( true, false );
  anIC->EraseAll( false, false );

  if ( isTrhDisplayed )
    anIC->Display( myViewer->getTrihedron(), false );

  std::list<SALOMEDS::SObject_var> aList;
  SALOMEDS_Tool::GetAllChildren( aStudy, aComponent, aList );

  std::list<SALOMEDS::SObject_var>::iterator anIter = aList.begin();
  for ( ; anIter != aList.end(); ++anIter )
  {
    SALOMEDS::SObject_var anObj = (*anIter);
    if ( ToolsGUI::GetVisibility( aStudy, anObj, this ) )
    {
      Handle(SALOME_InteractiveObject) anIObj = new SALOME_InteractiveObject();
      anIObj->setEntry( anObj->GetID() );
      aGUI->BuildPresentation( anIObj, this );
    }
  }

  if ( theToUpdate )
    Repaint();
}

//=======================================================================
// name    : undo
// Purpose : Redisplay all objects of active component in accordance with
//           their display flags. Called when undo operation is complited
//=======================================================================
void OCCViewer_ViewFrame::undo( QAD_Study* theQADStudy, const char* )
{
  redisplayAll( theQADStudy );
}

//=======================================================================
// name    : redo
// Purpose : Redisplay all objects of active component in accordance with
//           their display flags. Called when undo operation is complited
//=======================================================================
void OCCViewer_ViewFrame::redo( QAD_Study* theQADStudy, const char* )
{
  redisplayAll( theQADStudy );
}

//=======================================================================
// name    : FindIObject
// Purpose : Find in context SALOME_InteractiveObject by entry
//=======================================================================
Handle(SALOME_InteractiveObject) OCCViewer_ViewFrame::FindIObject( const char* Entry )
{
  Handle (AIS_InteractiveContext) ic = myViewer->getAISContext();
      
  AIS_ListOfInteractive List;
  ic->DisplayedObjects( List );
  AIS_ListOfInteractive List1;
  ic->ObjectsInCollector( List1 );
  List.Append( List1 );

  AIS_ListIteratorOfListOfInteractive ite( List );
  for ( ; ite.More(); ite.Next() )
  {
    Handle(SALOME_InteractiveObject) anObj =
      Handle(SALOME_InteractiveObject)::DownCast( ite.Value()->GetOwner() );

    if ( !anObj.IsNull() && anObj->hasEntry() && strcmp( anObj->getEntry(), Entry ) == 0 )
    {
      MESSAGE ( "IO found")
      return anObj;
    }
  }
  MESSAGE ( "IO not found")
  return Handle(SALOME_InteractiveObject)();
}

//=======================================================================
// name    : Display
// Purpose : Display object
//=======================================================================
void OCCViewer_ViewFrame::Display( const Handle(SALOME_InteractiveObject)& theIObject,
                                   bool toUpdate )
{
  if ( theIObject.IsNull() )
    return;

  QAD_Study*          aQADStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection*   aSel      = SALOME_Selection::Selection( aQADStudy->getSelection() );
  SALOMEDS::Study_var aStudy    = aQADStudy->getStudyDocument();

  Handle(AIS_InteractiveContext) anIC = myViewer->getAISContext();

  AIS_ListOfInteractive aList;
  anIC->ObjectsInCollector( aList );
  AIS_ListIteratorOfListOfInteractive anIter( aList );
  for ( ; anIter.More(); anIter.Next() )
  {
    Handle(SALOME_InteractiveObject) anObj =
      Handle(SALOME_InteractiveObject)::DownCast( anIter.Value()->GetOwner() );

    if ( !anObj.IsNull() && anObj->hasEntry() && anObj->isSame( theIObject ) )
    {
      anIC->Display( anIter.Value(), false );
      aSel->AddIObject( anObj, false );
      ToolsGUI::SetVisibility( aStudy, anObj->getEntry(), true, this );
      break;
    }
  }
  
  if ( toUpdate )
    Repaint();
}

//=======================================================================
// name    : DisplayOnly
// Purpose : Display object and erase all other ones
//=======================================================================
void OCCViewer_ViewFrame::DisplayOnly( const Handle(SALOME_InteractiveObject)& theIO )
{
  EraseAll();
  Display( theIO );
}

//=======================================================================
// name    : Erase
// Purpose : Erase object
//=======================================================================
void OCCViewer_ViewFrame::Erase( const Handle(SALOME_InteractiveObject)& theIObject,
                                 bool                                    toUpdate )
{
  QAD_Study* aStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* aSel = SALOME_Selection::Selection( aStudy->getSelection() );

  Handle(AIS_InteractiveContext) anIC = myViewer->getAISContext();
  
  AIS_ListOfInteractive aList;
  anIC->DisplayedObjects( aList );
  
  AIS_ListIteratorOfListOfInteractive ite( aList );
  for ( ; ite.More(); ite.Next() )
  {
    Handle(SALOME_InteractiveObject) anObj =
      Handle(SALOME_InteractiveObject)::DownCast( ite.Value()->GetOwner() );

    if ( !anObj.IsNull() && anObj->hasEntry() )
    {
      if ( anObj->isSame( theIObject ) )
      {
        anIC->Erase( ite.Value(), false );
        aSel->RemoveIObject( anObj, false );
        ToolsGUI::SetVisibility( aStudy->getStudyDocument(), anObj->getEntry(), false, this );
        break;
      }
    }
  }

  if ( toUpdate )
    Repaint();
}

//=======================================================================
// name    : DisplayAll
// Purpose : Display all objects of active component
//=======================================================================
void OCCViewer_ViewFrame::DisplayAll()
{
  SALOMEDS::Study_var      aStudy     = QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();
  QAD_Desktop*             aDesktop   = QAD_Application::getDesktop();
  SALOMEGUI*               aGUI       = aDesktop->getActiveGUI();
  const QString&           aCompName  = aDesktop->getComponentDataType();
  SALOMEDS::SObject_var    aComponent =
    SALOMEDS::SObject::_narrow( aStudy->FindComponent ( aCompName.latin1() ) );

  if ( aComponent->_is_nil() )
    return;

  Handle(AIS_InteractiveContext) anIC = myViewer->getAISContext();

  AIS_ListOfInteractive aDispList;
  AIS_ListOfInteractive aCollList;
  anIC->DisplayedObjects( aDispList );
  anIC->ObjectsInCollector( aCollList );

  QMap< QString, Handle(AIS_InteractiveObject) > aDispMap;
  QMap< QString, Handle(AIS_InteractiveObject) > aCollMap;
  getMapOfEntry( aDispList, aDispMap );
  getMapOfEntry( aCollList, aCollMap );

  std::list<SALOMEDS::SObject_var> aList;
  SALOMEDS_Tool::GetAllChildren( aStudy, aComponent, aList );

  std::list<SALOMEDS::SObject_var>::iterator anIter = aList.begin();
  for ( ; anIter != aList.end(); ++anIter )
  {
    SALOMEDS::SObject_var anObj = (*anIter);
    if ( anObj->_is_nil() )
      continue;

    const char* aEntry = anObj->GetID();
    if ( aCollMap.contains( aEntry ) )
    {
      anIC->DisplayFromCollector( aCollMap[ aEntry ], false );
      ToolsGUI::SetVisibility( aStudy, aEntry, true, this );
    }
    else if ( !aDispMap.contains( aEntry ) )
    {
      Handle(SALOME_InteractiveObject) anIObj = new SALOME_InteractiveObject();
      anIObj->setEntry( anObj->GetID() );
      aGUI->BuildPresentation( anIObj, this );
    }
  }

  Repaint();
}

//=======================================================================
// name    : EraseAll
// Purpose : Erase all objects
//=======================================================================
void OCCViewer_ViewFrame::EraseAll()
{
  SALOMEDS::Study_var aStudy =
    QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();

  Standard_Boolean isTrihedronDisplayed =
    myViewer->getAISContext()->IsDisplayed( myViewer->getTrihedron() );

  Handle(AIS_InteractiveContext) anIC = myViewer->getAISContext();

  AIS_ListOfInteractive aList;
  anIC->DisplayedObjects( aList );
  AIS_ListIteratorOfListOfInteractive anIter( aList );
  for ( ; anIter.More(); anIter.Next() )
  {
    if ( anIC->IsDisplayed( myViewer->getTrihedron() ) &&
         anIter.Value()->DynamicType() == STANDARD_TYPE( AIS_Trihedron ) )
      continue;

    Handle(AIS_InteractiveObject) anIO = anIter.Value();
    anIC->Erase( anIO, false, true );

    Handle(SALOME_InteractiveObject) anObj =
      Handle(SALOME_InteractiveObject)::DownCast( anIO->GetOwner() );

    if ( !anObj.IsNull() && anObj->hasEntry() )
      ToolsGUI::SetVisibility( aStudy, anObj->getEntry(), false, this );
  }

  if ( isTrihedronDisplayed )
    myViewer->getAISContext()->Display( myViewer->getTrihedron() );
  else
    Repaint();
}

//=======================================================================
// name    : Repaint
// Purpose : Uodate view
//=======================================================================
void OCCViewer_ViewFrame::Repaint()
{
  onAdjustTrihedron();
  myViewer->getViewer3d()->Update();
}

//=======================================================================
// name    : getTrihedronSize
// Purpose : Get new and current trihedron size corresponding to the
//           current model size
//=======================================================================
bool OCCViewer_ViewFrame::getTrihedronSize( double& theNewSize, double& theSize )
{
  theNewSize = 100;
  theSize = 100;

  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)myViewPort)->getView();

  if ( view3d.IsNull() )
    return false;

  double Xmin = 0, Ymin = 0, Zmin = 0, Xmax = 0, Ymax = 0, Zmax = 0;
  double aMaxSide;

  view3d->View()->MinMaxValues( Xmin, Ymin, Zmin, Xmax, Ymax, Zmax );

  if ( Xmin == RealFirst() || Ymin == RealFirst() || Zmin == RealFirst() ||
       Xmax == RealLast()  || Ymax == RealLast()  || Zmax == RealLast() )
    return false;

  aMaxSide = Xmax - Xmin;
  if ( aMaxSide < Ymax -Ymin ) aMaxSide = Ymax -Ymin;
  if ( aMaxSide < Zmax -Zmin ) aMaxSide = Zmax -Zmin;

  static float aSizeInPercents = 105;
  QString aSetting = QAD_CONFIG->getSetting("Viewer:TrihedronSize");
  if (!aSetting.isEmpty())
    aSizeInPercents = aSetting.toFloat();

  static float EPS = 5.0E-3;
  theSize = myViewer->getTrihedron()->Size();
  theNewSize = aMaxSide*aSizeInPercents / 100.0;

  return fabs( theNewSize - theSize ) > theSize * EPS ||
         fabs( theNewSize - theSize) > theNewSize * EPS;
}

//=======================================================================
// name    : AdjustTrihedrons
// Purpose : Adjust trihedron size in accordance with size of model
//=======================================================================
void OCCViewer_ViewFrame::AdjustTrihedrons( const bool forced )
{
  Handle (AIS_InteractiveContext) ic = myViewer->getAISContext();

  if ( !myViewer->isTrihedronDisplayed() )
    return;
  else
  {
    AIS_ListOfInteractive List;
    ic->DisplayedObjects(List);
    if ( List.First() == List.Last() && List.First() == myViewer->getTrihedron() )
    {
      myViewer->setTrihedronSize( 100 );
      return;
    }
  }

  double aNewSize = 100, aSize = 100;
  if ( getTrihedronSize( aNewSize, aSize ) || forced )
    myViewer->setTrihedronSize( aNewSize );
}

//=======================================================================
// name    : onAdjustTrihedron
// Purpose : Slot. Called when trihedrons must be resized
//=======================================================================
void OCCViewer_ViewFrame::onAdjustTrihedron()
{
  AdjustTrihedrons( false );
}

//=======================================================================
// name    : Display
// Purpose : Display presentation
//=======================================================================
void OCCViewer_ViewFrame::Display( const SALOME_OCCPrs* prs )
{
  // try do downcast object
  const OCCViewer_Prs* anOCCPrs = dynamic_cast<const OCCViewer_Prs*>( prs );
  if ( !anOCCPrs || anOCCPrs->IsNull() )
    return;

  // get context
  Handle (AIS_InteractiveContext) ic = myViewer->getAISContext();
  // get all displayed objects
  AIS_ListOfInteractive List;
  ic->DisplayedObjects( List );
  // get objects in the collector
  AIS_ListOfInteractive ListCollector;
  ic->ObjectsInCollector( ListCollector );

  // get objects to be displayed
  AIS_ListOfInteractive anAISObjects;
  anOCCPrs->GetObjects( anAISObjects );

  AIS_ListIteratorOfListOfInteractive aIter( anAISObjects );
  for ( ; aIter.More(); aIter.Next() )
  {
    Handle(AIS_InteractiveObject) anAIS = aIter.Value();
    if ( !anAIS.IsNull() )
    {
      // try to find presentation in the viewer
      bool bDisplayed = false;
      AIS_ListIteratorOfListOfInteractive ite( List );
      for ( ; ite.More(); ite.Next() )
      {
        // compare presentations by handles
        // if the object is already displayed - nothing to do more
        if ( ite.Value() == anAIS )
        {
          // Deactivate object if necessary
          if ( !anOCCPrs->ToActivate() )
            ic->Deactivate( anAIS );
          bDisplayed = true;
          break;
        }
      }

      if ( bDisplayed )
        continue;

      // then try to find presentation in the collector
      bDisplayed = false;
      ite.Initialize( ListCollector );
      for ( ; ite.More(); ite.Next() )
      {
        // compare presentations by handles
        // if the object is in collector - display it
        if ( ite.Value() == anAIS )
        {
          ic->DisplayFromCollector( anAIS, false );

          // Deactivate object if necessary
          if ( !anOCCPrs->ToActivate() )
            ic->Deactivate( anAIS );
          bDisplayed = true;

          // Set visibility flag
          Handle(SALOME_InteractiveObject) anObj =
            Handle(SALOME_InteractiveObject)::DownCast( anAIS->GetOwner() );
          if ( !anObj.IsNull() && anObj->hasEntry() )
          {
            SALOMEDS::Study_var aStudy =
              QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();
            ToolsGUI::SetVisibility( aStudy, anObj->getEntry(), true, this );
          }

          break;
        }
      }
      if ( bDisplayed )
        continue;

      // if object is not displayed and not found in the collector - display it
      if ( anAIS->IsKind( STANDARD_TYPE(AIS_Trihedron) ) )
      {
        Handle(AIS_Trihedron) aTrh = Handle(AIS_Trihedron)::DownCast( anAIS );
        double aNewSize = 100, aSize = 100;
        getTrihedronSize( aNewSize, aSize );
        aTrh->SetSize( aTrh == myViewer->getTrihedron() ? aNewSize : 0.5 * aNewSize );
      }

      ic->Display( anAIS, false );

      // Set visibility flag
      Handle(SALOME_InteractiveObject) anObj =
        Handle(SALOME_InteractiveObject)::DownCast( anAIS->GetOwner() );
      if ( !anObj.IsNull() && anObj->hasEntry() )
      {
        SALOMEDS::Study_var aStudy =
          QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();
        ToolsGUI::SetVisibility( aStudy, anObj->getEntry(), true, this );
      }

      // Deactivate object if necessary
      if ( !anOCCPrs->ToActivate() )
        ic->Deactivate( anAIS );
    }
  }
}

//=======================================================================
// name    : Erase
// Purpose : Erase presentation
//=======================================================================
void OCCViewer_ViewFrame::Erase( const SALOME_OCCPrs* prs, const bool forced )
{
  // try do downcast object
  const OCCViewer_Prs* anOCCPrs = dynamic_cast<const OCCViewer_Prs*>( prs );
  if ( !anOCCPrs || anOCCPrs->IsNull() )
    return;

  // get context
  Handle(AIS_InteractiveContext) ic = myViewer->getAISContext();

  // get objects to be erased
  AIS_ListOfInteractive anAISObjects;
  anOCCPrs->GetObjects( anAISObjects );

  AIS_ListIteratorOfListOfInteractive aIter( anAISObjects );
  for ( ; aIter.More(); aIter.Next() ) {
    Handle(AIS_InteractiveObject) anAIS = aIter.Value();
    if ( !anAIS.IsNull() ) {
      // erase the object from context : move it to collector
      ic->Erase( anAIS, false, forced ? false : true );

      // Set visibility flag if necessary
      if ( !forced )
      {
        Handle(SALOME_InteractiveObject) anObj =
          Handle(SALOME_InteractiveObject)::DownCast( anAIS->GetOwner() );
        if ( !anObj.IsNull() && anObj->hasEntry() )
        {
          SALOMEDS::Study_var aStudy =
            QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();
          ToolsGUI::SetVisibility( aStudy, anObj->getEntry(), true, this );
        }
      }
    }
  }
}

//=======================================================================
// name    : CreatePrs
// Purpose : Create presentation corresponding to the entry
//=======================================================================
SALOME_Prs* OCCViewer_ViewFrame::CreatePrs( const char* entry )
{
  OCCViewer_Prs* prs = new OCCViewer_Prs();
  if ( entry )
  {
    // get context
    Handle(AIS_InteractiveContext) ic = myViewer->getAISContext();

    // get displayed objects
    AIS_ListOfInteractive List;
    ic->DisplayedObjects( List );
    // get objects in the collector
    AIS_ListOfInteractive ListCollector;
    ic->ObjectsInCollector( ListCollector );
    List.Append( ListCollector );

    AIS_ListIteratorOfListOfInteractive ite( List );
    for ( ; ite.More(); ite.Next() )
    {
      Handle(SALOME_InteractiveObject) anObj =
        Handle(SALOME_InteractiveObject)::DownCast( ite.Value()->GetOwner() );

      if ( !anObj.IsNull() && anObj->hasEntry() && strcmp( anObj->getEntry(), entry ) == 0 )
        prs->AddObject( ite.Value() );
    }
  }
  return prs;
}

//=======================================================================
// name    : LocalSelection
// Purpose : Activates selection of sub shapes
//=======================================================================
void OCCViewer_ViewFrame::LocalSelection( const SALOME_OCCPrs* thePrs, const int theMode )
{
  Handle(AIS_InteractiveContext) anIC = myViewer->getAISContext();
  
  const OCCViewer_Prs* anOCCPrs = dynamic_cast<const OCCViewer_Prs*>( thePrs );
  if ( anIC.IsNull() )
    return;
  
  // Open local context if there is no one
  bool allObjects = thePrs == 0 || thePrs->IsNull();
  if ( !anIC->HasOpenedContext() ) {
    anIC->ClearCurrents( false );
    anIC->OpenLocalContext( allObjects, true, true );
  }

  AIS_ListOfInteractive anObjs;
  // Get objects to be activated
  if ( allObjects ) 
    anIC->DisplayedObjects( anObjs );
  else
    anOCCPrs->GetObjects( anObjs );

  // Activate selection of objects from prs
  AIS_ListIteratorOfListOfInteractive aIter( anObjs );
  for ( ; aIter.More(); aIter.Next() ) {
    Handle(AIS_InteractiveObject) anAIS = aIter.Value();
    if ( !anAIS.IsNull() )
    {
      if ( anAIS->IsKind( STANDARD_TYPE( AIS_Shape ) ) )
      {
        anIC->Load( anAIS, -1, false );
        anIC->Activate( anAIS, AIS_Shape::SelectionMode( (TopAbs_ShapeEnum)theMode ) );
      }
      else if ( anAIS->DynamicType() != STANDARD_TYPE(AIS_Trihedron) )
      {
        anIC->Load( anAIS, -1, false );
        anIC->Activate( anAIS, theMode );
      }
    }
  }
}

//=======================================================================
// name    : GlobalSelection
// Purpose : Deactivates selection of sub shapes
//=======================================================================
void OCCViewer_ViewFrame::GlobalSelection( const bool update ) const
{
  Handle(AIS_InteractiveContext) anIC = myViewer->getAISContext();
  if ( !anIC.IsNull() )
    anIC->CloseAllContexts( false );
  if ( update )
    anIC->CurrentViewer()->Redraw();
}

//=======================================================================
// name    : BeforeDisplay
// Purpose : Axiluary method called before displaying of objects
//=======================================================================
void  OCCViewer_ViewFrame::BeforeDisplay( SALOME_Displayer* d )
{
  d->BeforeDisplay( this, SALOME_OCCViewType() );
}

//=======================================================================
// name    : AfterDisplay
// Purpose : Axiluary method called after displaying of objects
//=======================================================================
void OCCViewer_ViewFrame::AfterDisplay( SALOME_Displayer* d )
{
  d->AfterDisplay( this, SALOME_OCCViewType() );
}

