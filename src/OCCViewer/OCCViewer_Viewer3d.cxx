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
//  File   : OCCViewer_Viewer3d.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

using namespace std;
/*!
  \class OCCViewer_Viewer3d OCCViewer_Viewer3d.h
  \brief Open CASCADE Viewer 3D with viewport 3D and selection.
*/

#include "OCCViewer_Viewer3d.h"
#include "QAD.h"
#include "QAD_Config.h"
#include "QAD_Desktop.h"
#include "QAD_Settings.h"
#include "QAD_LeftFrame.h"
#include "OCCViewer_ViewPort3d.h"
#include "QAD_RightFrame.h"
#include "QAD_MessageBox.h"
#include "QAD_Application.h"

#include "SALOME_Selection.h"
#include "SALOME_InteractiveObject.hxx"
#include "SALOME_ListIteratorOfListIO.hxx"

#include "SALOME_AISShape.hxx"
#include "SALOME_AISObject.hxx"

#include "utilities.h"

// Open CASCADE Includes
#include <Geom_Axis2Placement.hxx>
#include <AIS_ListOfInteractive.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <SelectMgr_EntityOwner.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <TColStd_IndexedMapOfInteger.hxx>
#include <NCollection_DefineBaseCollection.hxx>
#include <NCollection_DefineDataMap.hxx>

DEFINE_BASECOLLECTION( OCCViewer_CollectionOfIndexedMapOfShapes, TopTools_IndexedMapOfShape )
DEFINE_DATAMAP( OCCViewer_MapOfIOIndexedMapOfShape, OCCViewer_CollectionOfIndexedMapOfShapes, Handle_SALOME_InteractiveObject, TopTools_IndexedMapOfShape)

DEFINE_BASECOLLECTION( OCCViewer_CollectionOfMapOfInteger, TColStd_MapOfInteger )
DEFINE_DATAMAP( OCCViewer_MapOfIOMapOfInteger, OCCViewer_CollectionOfMapOfInteger, Handle_SALOME_InteractiveObject, TColStd_MapOfInteger )

/*!
    Constructor
*/
OCCViewer_Viewer3d::OCCViewer_Viewer3d(OCCViewer_ViewFrame* vf) :
  myAISContext( NULL ),
  myAISSelector( NULL )
{
  if ( myV3dViewer.IsNull() )
    createViewer3d();	/* create viewer */

  QString BackgroundColorRed   = QAD_CONFIG->getSetting("OCCViewer:BackgroundColorRed");
  QString BackgroundColorGreen = QAD_CONFIG->getSetting("OCCViewer:BackgroundColorGreen");
  QString BackgroundColorBlue  = QAD_CONFIG->getSetting("OCCViewer:BackgroundColorBlue");

  if( !BackgroundColorRed.isEmpty() && !BackgroundColorGreen.isEmpty() && !BackgroundColorBlue.isEmpty() )
    myBackColor = QColor(BackgroundColorRed.toInt(),
			 BackgroundColorGreen.toInt(),
			 BackgroundColorBlue.toInt());
  else
    myBackColor = QColor(35, 136, 145);

  myViewFrame = vf;
  myViewFrame->setViewPort ( new OCCViewer_ViewPort3d( myViewFrame, myV3dViewer ) );
  myViewFrame->setViewer(this);

  /* properties settings */
  //    if ( !myIcon.isNull() )
  //      myStudyFrame->setIcon( myIcon );

  myViewFrame->setCursor( myCursor );
  myViewFrame->setBackgroundColor( myBackColor );

  if ( myAISContext.IsNull() ) {
    /* create interactive manager */
    myAISContext = new AIS_InteractiveContext ( myV3dViewer, myV3dCollector );

    myAISContext->IsoOnPlane(true) ;

    /* create trihedron */
    Handle(Geom_Axis2Placement) anAxis=new Geom_Axis2Placement(gp::XOY());
    myTrihedron=new AIS_Trihedron(anAxis);
    myTrihedron->SetInfiniteState( Standard_True );

    Quantity_Color Col(193/255., 205/255., 193/255., Quantity_TOC_RGB);
    myTrihedron->SetColor( Col );
    myTrihedron->SetArrowColor( Col.Name() );

    float dim = 100;
    QString Size = QAD_CONFIG->getSetting("Viewer:TrihedronSize");
    if (!Size. isEmpty() ) dim = Size.toFloat(); //get size from preferences
   
    myTrihedron->SetSize(dim);

    myAISContext->Display(myTrihedron);
    myAISContext->Deactivate(myTrihedron);

    /* create selector */
    myAISSelector = new OCCViewer_AISSelector ( myAISContext );
    QAD_ASSERT ( connect (myAISSelector, SIGNAL(selSelectionDone(bool)),
			  this, SLOT(onSelectionDone(bool))) );
    QAD_ASSERT ( connect (myAISSelector, SIGNAL(selSelectionCancel(bool)),
			  this, SLOT(onSelectionCancel(bool))) );
    myEnableSelection = true;
  }

  QAD_ASSERT ( connect(myViewFrame,
		       SIGNAL(vfKeyPress(QKeyEvent*)),
		       this, SLOT(onKeyPress(QKeyEvent*))) );

  /* mouse events of the view */
  QAD_ASSERT ( QObject::connect(myViewFrame,
				SIGNAL(vfMousePress(QMouseEvent*)),
				this, SLOT(onMousePress(QMouseEvent*))) );
  QAD_ASSERT ( QObject::connect(myViewFrame,
				SIGNAL(vfMouseRelease(QMouseEvent*)),
				this, SLOT(onMouseRelease (QMouseEvent*))) );
  QAD_ASSERT ( QObject::connect(myViewFrame,
				SIGNAL(vfMouseMove(QMouseEvent*)),
				this, SLOT(onMouseMove (QMouseEvent*))) );
  QAD_ASSERT ( QObject::connect(myViewFrame,
				SIGNAL(vfMouseDoubleClick(QMouseEvent*)),
				this, SLOT(onMouseDoubleClick (QMouseEvent*))) );

  /* transformation events */
  QAD_ASSERT ( QObject::connect(myViewFrame,
				SIGNAL(vfTransformationStarted(OCCViewer_ViewPort::OperationType)),
				this, SLOT(onTransformationStarted (OCCViewer_ViewPort::OperationType))) );
  QAD_ASSERT ( QObject::connect(myViewFrame,
				SIGNAL(vfTransformationFinished(OCCViewer_ViewPort::OperationType)),
				this, SLOT(onTransformationFinished (OCCViewer_ViewPort::OperationType))) );

  /* set popup manager for the viewport */
  myViewFrame->getViewPort()->setPopupServer ( this );

}


/*!
    Destructor
*/
OCCViewer_Viewer3d::~OCCViewer_Viewer3d()
{
  delete myAISSelector;
}


/* trihedron */
Handle(AIS_Trihedron) OCCViewer_Viewer3d::getTrihedron() const
{
  return myTrihedron;
}

void OCCViewer_Viewer3d::setTrihedronSize( float size )
{
  AIS_ListOfInteractive aList;
  myAISContext->DisplayedObjects( aList );
  myAISContext->ObjectsInCollector( aList );

  AIS_ListIteratorOfListOfInteractive anIter( aList );
  for ( ; anIter.More(); anIter.Next() )
  {
    Handle(AIS_Trihedron) aTrh = Handle(AIS_Trihedron)::DownCast( anIter.Value() );
    if ( !aTrh.IsNull() )
      aTrh->SetSize( aTrh == myTrihedron ? size : 0.5 *size );
  }

  myAISContext->UpdateCurrentViewer();
}

bool OCCViewer_Viewer3d::isTrihedronDisplayed()

{
  return myAISContext->IsDisplayed(myTrihedron);
}

void OCCViewer_Viewer3d::rename( const Handle(SALOME_InteractiveObject)& IObject, QString newName )
{
  AIS_ListOfInteractive List;
  myAISContext->DisplayedObjects(List);
  
  AIS_ListIteratorOfListOfInteractive ite(List);
  while (ite.More())
  {
    if (ite.Value()->IsKind(STANDARD_TYPE(SALOME_AISShape)))
    {
      Handle(SALOME_AISShape) aSh = Handle(SALOME_AISShape)::DownCast(ite.Value());
      
      if ( aSh->hasIO() )
      {
        Handle(SALOME_InteractiveObject) IO = aSh->getIO();
        if ( IO->isSame( IObject ) )
        {
          char* aCopyName = CORBA::string_dup(newName.latin1());
          aSh->setName( aCopyName );
          delete( aCopyName );
          break;
        }
      }
    }
    else if ( ite.Value()->IsKind( STANDARD_TYPE( SALOME_AISObject ) ) )
    {
      Handle(SALOME_AISObject) aSh = Handle(SALOME_AISObject)::DownCast( ite.Value() );

      // Add code here, if someone create a MODULE_AISObject.
    }
    ite.Next();
  }
}

void OCCViewer_Viewer3d::SetColor(const Handle(SALOME_InteractiveObject)& IObject,
				  QColor thecolor)
{
  AIS_ListOfInteractive List;
  myAISContext->DisplayedObjects(List);
  
  AIS_ListIteratorOfListOfInteractive ite(List);
  for ( ; ite.More(); ite.Next() )
  {
    Handle(SALOME_InteractiveObject) anObj =
        Handle(SALOME_InteractiveObject)::DownCast( ite.Value()->GetOwner() );

    if ( !anObj.IsNull() && anObj->hasEntry() && anObj->isSame( IObject ) )
    {
      Quantity_Color CSFColor = Quantity_Color ( thecolor.red() / 255.,
                                                 thecolor.green() / 255.,
                                                 thecolor.blue() / 255.,
                                                 Quantity_TOC_RGB );
      ite.Value()->SetColor( CSFColor );
      break;
    }
  }

  myV3dViewer->Update();
}

void OCCViewer_Viewer3d::SwitchRepresentation( const Handle(SALOME_InteractiveObject)& IObject,
                                               int mode )
{  
  AIS_ListOfInteractive List;
  myAISContext->DisplayedObjects(List);
  
  AIS_ListIteratorOfListOfInteractive ite(List);
  for ( ; ite.More(); ite.Next() )
  {
    Handle(SALOME_InteractiveObject) anObj =
        Handle(SALOME_InteractiveObject)::DownCast( ite.Value()->GetOwner() );

    if ( !anObj.IsNull() && anObj->hasEntry() && anObj->isSame( IObject ) )
      myAISContext->SetDisplayMode( ite.Value(), (Standard_Integer)mode ,true );
  }

  myV3dViewer->Update();
}

void OCCViewer_Viewer3d::SetTransparency( const Handle(SALOME_InteractiveObject)& IObject,
                                          float transp )
{
  AIS_ListOfInteractive List;
  myAISContext->DisplayedObjects( List );
  
  AIS_ListIteratorOfListOfInteractive ite( List );
  for ( ; ite.More(); ite.Next() )
  {
    Handle(SALOME_InteractiveObject) anObj =
        Handle(SALOME_InteractiveObject)::DownCast( ite.Value()->GetOwner() );

    if ( !anObj.IsNull() && anObj->hasEntry() && anObj->isSame( IObject ) )
    {
      myAISContext->SetTransparency( ite.Value(), transp, false );
      myAISContext->Redisplay( ite.Value(), Standard_False, Standard_True );
    }
  }
  
  myV3dViewer->Update();  
}

/*!
    Highlights 'obj' in viewer, returns 'true' when selected successfully
*/
bool OCCViewer_Viewer3d::highlight( const Handle(SALOME_InteractiveObject)& IObject, bool highlight, bool update )
{
  bool isInLocal = myAISContext->HasOpenedContext();
  QAD_Study* ActiveStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel = SALOME_Selection::Selection( ActiveStudy->getSelection() );

  AIS_ListOfInteractive List;
  myAISContext->DisplayedObjects(List);
  
  AIS_ListIteratorOfListOfInteractive ite(List);
  for ( ; ite.More(); ite.Next() )
  {
    Handle(SALOME_InteractiveObject) anObj =
      Handle(SALOME_InteractiveObject)::DownCast( ite.Value()->GetOwner() );

    if ( !anObj.IsNull() && anObj->hasEntry() && anObj->isSame( IObject ) )
    {
      if ( !isInLocal )
      {
        if ( highlight && !myAISContext->IsSelected( ite.Value() ) )
          myAISContext->AddOrRemoveCurrentObject( ite.Value(), false );
        else if ( !highlight && myAISContext->IsSelected( ite.Value() ) )
          myAISContext->AddOrRemoveCurrentObject( ite.Value(), false );
      }
      // highlight subshapes only when local selection is active
      else
      {
        if ( ite.Value()->IsKind( STANDARD_TYPE( SALOME_AISShape ) ) )
        {
          Handle(SALOME_AISShape) aSh = Handle(SALOME_AISShape)::DownCast( ite.Value() );
          TColStd_IndexedMapOfInteger MapIndex;
          Sel->GetIndex( IObject, MapIndex );
          aSh->highlightSubShapes( MapIndex, highlight );
        }
      }
      break;
    }
  }
    
  if ( update )
    myV3dViewer->Redraw();
    
  return false;
}

/*!
    Unhighlight all selected objects
*/
bool OCCViewer_Viewer3d::unHighlightAll()
{
  if ( myAISContext->HasOpenedContext() )
    myAISContext->ClearSelected();
  else
    myAISContext->ClearCurrents();
  return false;
}

/*!
  Returns true if the IObject has a Graphical Object in this viewer. Returns FALSE otherwise.
*/
bool OCCViewer_Viewer3d::isInViewer( const Handle(SALOME_InteractiveObject)& IObject, bool onlyInViewer)
{
  AIS_ListOfInteractive List;
  myAISContext->DisplayedObjects(List);

  if ( onlyInViewer ) {
    AIS_ListOfInteractive List1;
    myAISContext->ObjectsInCollector(List1);
    List.Append(List1);
  }

  AIS_ListIteratorOfListOfInteractive ite(List);
  for ( ; ite.More(); ite.Next() )
  {
    Handle(SALOME_InteractiveObject) anObj =
        Handle(SALOME_InteractiveObject)::DownCast( ite.Value()->GetOwner() );

    if ( !anObj.IsNull() && anObj->hasEntry() && anObj->isSame( IObject ) )
      return true;
  }
  return false;
}

bool OCCViewer_Viewer3d::isVisible( const Handle(SALOME_InteractiveObject)& IObject )
{
  AIS_ListOfInteractive List;
  myAISContext->DisplayedObjects( List );

  AIS_ListIteratorOfListOfInteractive ite( List );
  for ( ; ite.More(); ite.Next() )
  {
    Handle(SALOME_InteractiveObject) anObj =
      Handle(SALOME_InteractiveObject)::DownCast( ite.Value()->GetOwner() );

    if ( !anObj.IsNull() && anObj->hasEntry() && anObj->isSame( IObject ) )
      return myAISContext->IsDisplayed( ite.Value() );
  }
  
  return false;
}

/*!
    Returns the interactive context
*/
Handle (AIS_InteractiveContext) OCCViewer_Viewer3d::getAISContext() const
{
  return myAISContext;
}

OCCViewer_AISSelector* OCCViewer_Viewer3d::getAISSelector() const
{
  return myAISSelector;
}

/*!
    Returns 'true' if selection is enabled in this viewer,
    'false' otherwise.
*/
bool OCCViewer_Viewer3d::enabledSelection() const
{
  return myEnableSelection;
}

/*!
    Enables/disables selection in the viewer
*/
void OCCViewer_Viewer3d::enableSelection( bool enable )
{
  myEnableSelection = enable;
  myAISSelector->enableSelection( enable );

  /* send notification */
  emit vw3dSelectionEnabled( enable );
}

/*!
    Enables/disables ordinary and multiple selection
    ( by 'Shift' key or with a rectangle ) in this viewer.
*/
void OCCViewer_Viewer3d::enableMultipleSelection( bool enable )
{
  /*  Enable/disable rectangle skectching which
      is used for multiple selection */
  myAISSelector->enableMultipleSelection( enable );

  myViewFrame->getViewPort()->enableDrawMode( enable );

  /* Enable non-multiple selection as well */
  if ( !myEnableSelection && enable )
    enableSelection( enable );
}

/*!
    Key handler
*/
void OCCViewer_Viewer3d::onKeyPress( QKeyEvent* pe )
{
  if (!QAD_Application::getDesktop()->getActiveComponent().isEmpty()) {
    QAD_Application::getDesktop()->onKeyPress( pe );
  }
}

/*!
    Mouse handler
*/
void OCCViewer_Viewer3d::onMousePress( QMouseEvent* pe )
{
  if ( pe->button() == Qt::LeftButton ) {
    if ( !QAD_Application::getDesktop()->onMousePress( pe ) ) {

      QAD_ASSERT_DEBUG_ONLY ( myAISSelector );
      if ( pe->state() & Qt::ShiftButton )
	myAISSelector->shiftSelect();  /* append selection */
      else
	myAISSelector->select();       /* new selection */
    }
  }
}

/*!
    Mouse handler
*/
void OCCViewer_Viewer3d::onMouseMove( QMouseEvent* pe )
{
  /* activate hilighting only if no MB pressed */
  if ( ! ( pe->state() & ( Qt::LeftButton | Qt::MidButton | Qt::RightButton ) ) ) {
    QAD_Application::getDesktop()->onMouseMove( pe );
    OCCViewer_ViewPort* vp = myViewFrame->getViewPort();
    myAISSelector->moveTo ( pe->x(), pe->y(), ((OCCViewer_ViewPort3d*)vp)->getView() );
  }
}

/*!
    Mouse handler
*/
void OCCViewer_Viewer3d::onMouseRelease( QMouseEvent* pe )
{
  /* Selection with a rectangle ( multiple ) */
  OCCViewer_ViewPort* vp = myViewFrame->getViewPort();
  QAD_ASSERT_DEBUG_ONLY ( vp );
  QRect selRect = vp->getSelectionRect ();
  if ( selRect.isValid() ) {
    QAD_ASSERT_DEBUG_ONLY ( myAISSelector );
    QAD_ASSERT_DEBUG_ONLY ( vp->inherits("OCCViewer_ViewPort3d") );
    if ( pe->state() & Qt::ShiftButton )
      myAISSelector->shiftSelect ( selRect.left(), selRect.top(),
				   selRect.right(), selRect.bottom(),
				   ((OCCViewer_ViewPort3d*)vp)->getView() );
    else
      myAISSelector->select ( selRect.left(), selRect.top(),
			      selRect.right(), selRect.bottom(),
			      ((OCCViewer_ViewPort3d*)vp)->getView() );
  }
}

/*!
    Mouse handler
*/
void OCCViewer_Viewer3d::onMouseDoubleClick( QMouseEvent* pe )
{
}

/*!
    Called when a transformation in viewport is started
*/
void OCCViewer_Viewer3d::onTransformationStarted( OCCViewer_ViewPort::OperationType type )
{
  QAD_ASSERT_DEBUG_ONLY ( myAISSelector );
  if ( myEnableSelection )
    myAISSelector->enableSelection ( false );	// lock until the end of transform

  if ( type == OCCViewer_ViewPort::PANGLOBAL ||
       type == OCCViewer_ViewPort::WINDOWFIT )
    {
      /*  Start watching 'global panning' and 'window fit'.
	  These operations require additional user's actions
	  in viewport and we have to reset the operations if
	  instead of these actions a user does something else.
      */
      qApp->installEventFilter( this );
    }
}

/*!
    Called when a transformation in viewport is finished
*/
void OCCViewer_Viewer3d::onTransformationFinished( OCCViewer_ViewPort::OperationType type )
{
  QAD_ASSERT_DEBUG_ONLY ( myAISSelector );
  if ( myEnableSelection )
    myAISSelector->enableSelection ( true );	// unlock

  if ( type == OCCViewer_ViewPort::PANGLOBAL ||
       type == OCCViewer_ViewPort::WINDOWFIT )
    {
      /* stop watching 'global panning' and 'window fit'*/
      qApp->removeEventFilter( this );
    }
}

/*!
    Called when an object is selected
*/
void OCCViewer_Viewer3d::onSelectionDone( bool bAdded )
{
  emit vw3dSelectionDone( bAdded );

//  QAD_Study*	  myActiveStudy	 = QAD_Application::getDesktop()->getActiveStudy();
//  QAD_StudyFrame* myActiveSFrame = myActiveStudy->getActiveStudyFrame();
 // SALOME_Selection*	    Sel	 = SALOME_Selection::Selection( myActiveStudy->getSelection() );
//  MESSAGE ( "OCCViewer_Viewer3d - NB SELECTED INTERACTIVE OBJECT : " << Sel->IObjectCount() )

  QString ActiveComp = QAD_Application::getDesktop()->getActiveComponent();

  if ( ActiveComp.isEmpty() )
  {
    unHighlightAll();
    return;
  }

  if ( myAISContext->IndexOfCurrentLocal() <= 0 )
    globalSelectionDone( bAdded );
  else
    localSelectionDone( bAdded );
  
  myV3dViewer->Update();
}

/*!
  Called when an object is selected and there is no opened local context
*/
void OCCViewer_Viewer3d::globalSelectionDone( const bool bAdded )
{
  SALOME_Selection* Sel = SALOME_Selection::Selection(
    QAD_Application::getDesktop()->getActiveStudy()->getSelection() );

  MESSAGE ( "OCCViewer_Viewer3d - NB SELECTED INTERACTIVE OBJECT : " << Sel->IObjectCount() )
  
  SALOME_ListIO DeltaPos;
  DeltaPos.Clear();
  SALOME_ListIO DeltaNeg;
  DeltaNeg.Clear();

  if ( !bAdded )
  { 
    for ( myAISContext->InitCurrent(); myAISContext->MoreCurrent(); myAISContext->NextCurrent() )
    {
      Handle(SALOME_InteractiveObject) anObj =
        Handle(SALOME_InteractiveObject)::DownCast( myAISContext->Current()->GetOwner() );

      if ( !anObj.IsNull() )
      {
        bool itemAllreadySelected = false;
        int nbSel = Sel->IObjectCount();
        if ( nbSel == 0 )
          DeltaPos.Append( anObj );
        else
        {
          SALOME_ListIteratorOfListIO It( Sel->StoredIObjects() );
          for( ; It.More(); It.Next() )
          {
            Handle( SALOME_InteractiveObject) IO1 = It.Value();
            if ( anObj->isSame( IO1 ) )
            {
              itemAllreadySelected = true;
              break;
            }
          }
          if ( !itemAllreadySelected )
            DeltaPos.Append( anObj );
        }
      }
      else if ( myAISContext->Current()->IsKind( STANDARD_TYPE( SALOME_AISObject ) ) )
      {
        //Handle(SALOME_AISObject) aSh =
        //  Handle(SALOME_AISObject)::DownCast( myAISContext->Current() );
        // Add code here, if someone create a MODULE_AISObject.
      }
    }

    if ( DeltaPos.Extent() > 0 )
      Sel->ClearIObjects();

  }
  else
  { /* shift select */
    SALOME_ListIteratorOfListIO It( Sel->StoredIObjects() );
    for ( ;It.More(); It.Next() )
    {
      Handle( SALOME_InteractiveObject) IO1 = It.Value();

      bool itemAllreadySelected = false;
      for ( myAISContext->InitCurrent(); myAISContext->MoreCurrent(); myAISContext->NextCurrent() )
      {
        Handle(SALOME_InteractiveObject) anObj =
          Handle(SALOME_InteractiveObject)::DownCast( myAISContext->Current()->GetOwner() );

        if ( !anObj.IsNull() && anObj->isSame( IO1 ) )
        {
          itemAllreadySelected = true;
          break;
        }
        else if ( myAISContext->Current()->IsKind( STANDARD_TYPE( SALOME_AISObject ) ) )
        {
          //Handle(SALOME_AISObject) aSh =
          //  Handle(SALOME_AISObject)::DownCast(myAISContext->Current());
          // Add code here, if someone create a MODULE_AISObject.
        }
      }

      if ( !itemAllreadySelected )
        DeltaNeg.Append( IO1 );
    }

    myAISContext->InitCurrent();
    for ( myAISContext->InitCurrent(); myAISContext->MoreCurrent(); myAISContext->NextCurrent() )
    {
      Handle(SALOME_InteractiveObject) anObj =
        Handle(SALOME_InteractiveObject)::DownCast( myAISContext->Current()->GetOwner() );

      if ( !anObj.IsNull() )
      {
        bool itemAllreadySelected = false;
        SALOME_ListIteratorOfListIO It( Sel->StoredIObjects() );
        for ( ;It.More(); It.Next() )
        {
          Handle( SALOME_InteractiveObject) IO1 = It.Value();

          if ( anObj->isSame( IO1 ) )
          {
            itemAllreadySelected = true;
            break;
          }
        }

        if ( !itemAllreadySelected )
          DeltaPos.Append( anObj );
      }
      else if ( myAISContext->Current()->IsKind(STANDARD_TYPE(SALOME_AISObject)))
      {
        //Handle(SALOME_AISObject) aSh =
        //  Handle(SALOME_AISObject)::DownCast(myAISContext->Current());
        // Add code here, if someone create a MODULE_AISObject.
      }
    }
  }

  //  MESSAGE ( "VIEWER onSelectionDone DeltaNeg.count() == " << DeltaNeg.count() )
  SALOME_ListIteratorOfListIO ItNeg( DeltaNeg );
  for( ;ItNeg.More();ItNeg.Next() )
  {
    Sel->RemoveIObject( ItNeg.Value(), false );
  }

  //  MESSAGE ( "VIEWER onSelectionDone DeltaPos.count() == " << DeltaPos.Extent() )
  SALOME_ListIteratorOfListIO ItPos( DeltaPos );
  for ( ;ItPos.More();ItPos.Next() )
  {
    Sel->AddIObject( ItPos.Value(), false );
  }
}

/*!
  Called when an object is selected and there is opened local context
*/
void OCCViewer_Viewer3d::localSelectionDone( const bool /*bAdded*/ )
{
  SALOME_Selection* aSelection = SALOME_Selection::Selection(
    QAD_Application::getDesktop()->getActiveStudy()->getSelection() );

  aSelection->BlockSignals( true );

  OCCViewer_MapOfIOIndexedMapOfShape aMapsOfShapes;   // SALOME_InteractiveObject <--> TopTools_IndexedMapOfShape
  OCCViewer_MapOfIOMapOfInteger      aMapsOfIndexes; // SALOME_InteractiveObject <--> TColStd_MapOfInteger


  // Iterate through selected objects and add them to selection
  for( myAISContext->InitSelected(); myAISContext->MoreSelected(); myAISContext->NextSelected() )
  {
    // Retrive selected shape and subshape
    Handle(SelectMgr_EntityOwner) anOwner = myAISContext->SelectedOwner();
    if ( anOwner.IsNull() )
      continue;
      
    Handle(AIS_InteractiveObject) anIO =
      Handle(AIS_InteractiveObject)::DownCast( myAISContext->SelectedInteractive() );
    if ( anIO.IsNull() )
      continue;

    Handle(SALOME_InteractiveObject) anObj =
      Handle(SALOME_InteractiveObject)::DownCast( anIO->GetOwner() );
    if ( anObj.IsNull() )
      continue;
    
    if ( anIO->IsKind( STANDARD_TYPE( AIS_Shape ) ) )
    {
      Handle(AIS_Shape) anAISShape = Handle(AIS_Shape)::DownCast( anIO );
      TopoDS_Shape aShape = anAISShape->Shape();
      TopoDS_Shape aSubShape = anOwner->Shape();
      if ( aShape.IsNull() || aSubShape.IsNull() )
        continue;

      // Get index of selected shape
      if ( aMapsOfShapes.IsBound( anObj ) )
      {
        const TopTools_IndexedMapOfShape& aShapes = aMapsOfShapes( anObj );
        int anIndex = aShapes.FindIndex( aSubShape );

        aMapsOfIndexes( anObj ).Add( anIndex );
      }
      else
      {
        TopTools_IndexedMapOfShape aShapes;
        TopExp::MapShapes( aShape, aShapes );
        int anIndex = aShapes.FindIndex( aSubShape );

        TColStd_MapOfInteger anIndexes;
        anIndexes.Add( anIndex );

        aMapsOfShapes.Bind( anObj, aShapes );
        aMapsOfIndexes.Bind( anObj, anIndexes );
      }
    }
    else
    {
      aMapsOfShapes.Bind( anObj, TopTools_IndexedMapOfShape() );
      aMapsOfIndexes.Bind( anObj, TColStd_MapOfInteger() );
    }
  }

  // Clear selection
  aSelection->ClearIObjects();

  // Add object in selection
  OCCViewer_MapOfIOMapOfInteger::Iterator anIter( aMapsOfIndexes );
  for ( ; anIter.More(); anIter.Next() )
  {
    if ( anIter.Value().IsEmpty() )
      aSelection->AddIObject( anIter.Key(), false );
    else
    {
      aSelection->AddIObject( anIter.Key(), false );
      aSelection->AddOrRemoveIndex( anIter.Key(), anIter.Value(), false, false );
    }
  }

  aSelection->BlockSignals( false );
  aSelection->SelectionChanged();
}

/*!
    Called when an object is unselected
*/
void OCCViewer_Viewer3d::onSelectionCancel( bool bAdded )
{
  emit vw3dSelectionCancel();

  QAD_Study*  myActiveStudy	 = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection*  Sel	 = SALOME_Selection::Selection( myActiveStudy->getSelection() );

  SALOME_ListIO DeltaPos;
  DeltaPos.Clear();
  SALOME_ListIO DeltaNeg;
  DeltaNeg.Clear();
  
  if ( !bAdded )
  { /* select */
    Sel->ClearIObjects();
  }
  else
  { /* shiftselect */
    SALOME_ListIteratorOfListIO It( Sel->StoredIObjects() );
    for ( ; It.More(); It.Next() )
    {
      Handle( SALOME_InteractiveObject) IO1 = It.Value();

      bool itemAllreadySelected = false;
      myAISContext->InitCurrent();
      for ( myAISContext->InitCurrent(); myAISContext->MoreCurrent(); myAISContext->NextCurrent() )
      {
        Handle(SALOME_InteractiveObject) anObj =
          Handle(SALOME_InteractiveObject)::DownCast( myAISContext->Current()->GetOwner() );
          
        if ( !anObj.IsNull() && anObj->isSame( IO1 ) )
        {
          itemAllreadySelected = true;
          break;
        }
        else if ( myAISContext->Current()->IsKind( STANDARD_TYPE( SALOME_AISObject ) ) )
        {
          Handle(SALOME_AISObject) aSh =
            Handle(SALOME_AISObject)::DownCast(myAISContext->Current());
          // Add code here, if someone create a MODULE_AISObject.
        }
      }
      
      // only if isknown
      if ( !itemAllreadySelected )
        DeltaNeg.Append( IO1 );
    }
  }

  //  MESSAGE ( "VIEWER onSelectionCancel DeltaNeg.count() == " << DeltaNeg.Extent() )
  SALOME_ListIteratorOfListIO ItNeg( DeltaNeg );
  for ( ; ItNeg.More(); ItNeg.Next() ) 
    Sel->RemoveIObject( ItNeg.Value(), false);

  myV3dViewer->Update();
}

/*!
    Installed while 'fit area' and 'global panning'
    operations are active
*/
bool OCCViewer_Viewer3d::eventFilter( QObject * po, QEvent * pe)
{
  if ( ( pe->type() == QEvent::MouseButtonPress &&
	 po != myViewFrame->getViewPort() ) ||
       pe->type() == QEvent::KeyPress ) {
    /* user press any key or a MB outside the viewport */
    myViewFrame->getViewPort()->resetState();
  }
  return QObject::eventFilter( po, pe );
}

/*!
  Creates CasCade viewer 3D
*/
void OCCViewer_Viewer3d::createViewer3d()
{
  myV3dViewer = OCCViewer_VService::Viewer ( "", (short*) "Viewer3d", "", 1000.,
					     V3d_XposYnegZpos, true, true);
  myV3dViewer->Init();

  myV3dCollector = OCCViewer_VService::Viewer ( "", (short*) "Collector3d", "", 1000.,
						V3d_XposYnegZpos, true, true);
  myV3dCollector->Init();
}

/*!
  Returns CasCade viewer 3D
*/
Handle(V3d_Viewer) OCCViewer_Viewer3d::getViewer3d() const
{
  return myV3dViewer;
}

/*!
    Sets a new cursor on all its studyframes
*/
void OCCViewer_Viewer3d::setCursor( const QCursor& cursor)
{
  myCursor = cursor;
}

/*!
    Returns the current cursor
*/
inline const QCursor& OCCViewer_Viewer3d::cursor() const
{
  return myCursor;
}

/*!
    Sets default background color on all its studyframes
*/
void OCCViewer_Viewer3d::setBackgroundColor( const QColor& aColor )
{
  myBackColor = aColor;
}

/*!
    Returns the current background
*/
inline const QColor& OCCViewer_Viewer3d::backgroundColor() const
{
  return myBackColor;
}

/*!
    Sets a new icon to all its studyframes
*/
void OCCViewer_Viewer3d::setIcon( const QPixmap& icon )
{
  myIcon = icon;
}

/*!
  Returns current icon
*/
inline const QPixmap& OCCViewer_Viewer3d::icon() const
{
  return myIcon;
}

/*!
    Builds popup menu
*/
void OCCViewer_Viewer3d::onCreatePopup()
{
}

