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

// QT Include
#include <qapplication.h>

// Open CASCADE Include
#include <V3d_View.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <Visual3d_View.hxx>
using namespace std;

/*!
    Constructor
*/
OCCViewer_ViewFrame::OCCViewer_ViewFrame(QWidget* parent, const QString& title) 
  : QAD_ViewFrame (parent, "")
{
  initialize();
}

/*!
    Constructor
*/
OCCViewer_ViewFrame::OCCViewer_ViewFrame( QWidget* parent ) 
  : QAD_ViewFrame (parent, "")
{
  initialize();
}

/*!
    Destructor
*/
OCCViewer_ViewFrame::~OCCViewer_ViewFrame()
{
  cleanup();
}

/*!
    Init viewframe
*/
void OCCViewer_ViewFrame::initialize()
{
  myViewPort = NULL;
  myViewer   = new OCCViewer_Viewer3d( this );
  
  /* enable/disable selection */
  myViewer->enableSelection( true );
  myViewer->enableMultipleSelection( true );
}

/*!
    Cleanup viewframe 
*/
void OCCViewer_ViewFrame::cleanup()
{
  
} 

/*!
    Sets the viewport for this frame
*/
void OCCViewer_ViewFrame::setViewPort( OCCViewer_ViewPort* view )
{
  myViewPort = view;
  initViewPort();		
  setCentralWidget ( myViewPort );
}

/*!
    Inits the viewport for this frame
*/
void OCCViewer_ViewFrame::initViewPort()
{
  /* Active Key Event */
  setFocus();

  /* Initial attributes */
  myViewPort->setCursor( cursor() );
  myViewPort->setBackgroundColor( backgroundColor() );
  
  QAD_ASSERT ( QObject::connect(myViewPort, SIGNAL(vpKeyPress (QKeyEvent*)), 
				this, SIGNAL(vfKeyPress(QKeyEvent*))) );

  /*  Listen to my viewport */	
  QAD_ASSERT ( QObject::connect(myViewPort, 
				SIGNAL(vpTransformationStarted (OCCViewer_ViewPort::OperationType)), 
				this, 
				SIGNAL(vfTransformationStarted(OCCViewer_ViewPort::OperationType))) );
  QAD_ASSERT ( QObject::connect( myViewPort, 
				 SIGNAL(vpTransformationFinished (OCCViewer_ViewPort::OperationType)),
				 this, 
				 SIGNAL(vfTransformationFinished(OCCViewer_ViewPort::OperationType))) );

  QAD_ASSERT ( QObject::connect(myViewPort, SIGNAL(vpDrawExternal (QPainter*)), 
				this, SIGNAL(vfDrawExternal(QPainter*))) );	
  QAD_ASSERT ( QObject::connect(myViewPort, SIGNAL(vpMousePress (QMouseEvent*)), 
				this, SIGNAL(vfMousePress(QMouseEvent*))) );
  QAD_ASSERT ( QObject::connect(myViewPort, SIGNAL(vpMouseRelease (QMouseEvent*)), 
				this, SIGNAL(vfMouseRelease(QMouseEvent*))) );
  QAD_ASSERT ( QObject::connect(myViewPort, SIGNAL(vpMouseMove (QMouseEvent*)), 
				this, SIGNAL(vfMouseMove(QMouseEvent*))) );
  QAD_ASSERT ( QObject::connect(myViewPort, SIGNAL(vpMouseDoubleClick (QMouseEvent*)), 
				this, SIGNAL(vfMouseDoubleClick(QMouseEvent*))) );

  QAD_ASSERT ( QObject::connect(myViewPort, SIGNAL(vpKeyPress (QKeyEvent*)), 
				this, SIGNAL(vfKeyPress(QKeyEvent*))) );
  QAD_ASSERT ( QObject::connect(myViewPort, SIGNAL(vpKeyRelease (QKeyEvent*)), 
				this, SIGNAL(vfKeyRelease(QKeyEvent*))) );
}

/*!
  Returns widget containing 3D-Viewer
*/
QWidget* OCCViewer_ViewFrame::getViewWidget() 
{
  return (QWidget*)getViewPort();
}

/*!
    Returns the viewport of this frame	
*/
OCCViewer_ViewPort* OCCViewer_ViewFrame::getViewPort() const
{
  return myViewPort;
}

/*!
    Sets the cursor for the viewframe's viewport
*/
void OCCViewer_ViewFrame::setCursor( const QCursor& cursor)
{
  if ( myViewPort ) 
    myViewPort->QWidget::setCursor(cursor);
}

/*!
    Returns the current cursor 
*/
QCursor OCCViewer_ViewFrame::cursor() const
{
  if ( myViewPort ) 
    return myViewPort->cursor();
  return QMainWindow::cursor();
}

/*!
    Set background of the viewport
*/
void OCCViewer_ViewFrame::setBackgroundColor( const QColor& color)
{
  if ( myViewPort )
    myViewPort->setBackgroundColor(color);
}

/*!
    Returns background of the viewport
*/
QColor OCCViewer_ViewFrame::backgroundColor() const
{
  if ( myViewPort )
    return myViewPort->backgroundColor();
  return QMainWindow::backgroundColor();
}

/*!
    Sets the parent viewer for the window
*/
void OCCViewer_ViewFrame::setViewer(OCCViewer_Viewer3d* viewer)
{
  myViewer = viewer;
}

/*!
  Returns the parent viewer for the window
*/
OCCViewer_Viewer3d* OCCViewer_ViewFrame::getViewer() const
{
  return myViewer;
}

/*!
    Returns 'true' if viewframe is visible
*/
void OCCViewer_ViewFrame::setVisible( bool visible )
{
  if ( visible == QWidget::isVisible() )
    return;
  
  if ( visible ) show();
  else hide();
}

/*!
   Called when viewframe is about to close
*/
void OCCViewer_ViewFrame::closeEvent(QCloseEvent* e)
{
  emit vfViewClosing(e);  /* notify our viewer */
}

/*!
   Called when viewframe is resized 
*/
void OCCViewer_ViewFrame::resizeEvent(QResizeEvent* e)
{
  emit vfResize(e);	
}

void OCCViewer_ViewFrame::keyPressEvent( QKeyEvent *k )
{
  emit vfKeyPress(k);
}

/*!
    Fits all objects in the active view
*/
void OCCViewer_ViewFrame::onViewFitAll()
{
  myViewPort->fitAll();
}

/*!
    Fits all obejcts within a rectangular area of the active view
*/
void OCCViewer_ViewFrame::onViewFitArea()
{
  myViewPort->activateWindowFit();
  QAD_Application::getDesktop()->putInfo( tr("PRP_VW3D_SKETCHAREA") );
}

/*!
    Moves the active view
*/
void OCCViewer_ViewFrame::onViewPan()
{
  myViewPort->activatePanning();
}

/*!
    Zooms the active view
*/
void OCCViewer_ViewFrame::onViewZoom()
{
  myViewPort->activateZoom();
}

/*!
    Sets a new center of the active view
*/
void OCCViewer_ViewFrame::onViewGlobalPan()
{
  myViewPort->activateGlobalPanning();
  QAD_Application::getDesktop()->putInfo( tr("PRP_VW3D_POINTCENTER") );
}

/*!
    Rotates the active view
*/
void OCCViewer_ViewFrame::onViewRotate()
{
  myViewPort->activateRotation();
}

/*!
    Reset the active view
*/
void OCCViewer_ViewFrame::onViewReset()
{
  myViewPort->reset();
}

/*!
    Provides front projection of the active view
*/
void OCCViewer_ViewFrame::onViewFront()
{
  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)myViewPort)->getView();
  if ( !view3d.IsNull() ) view3d->SetProj (V3d_Xpos);
  onViewFitAll();
}

/*!
    Provides back projection of the active view
*/
void OCCViewer_ViewFrame::onViewBack()
{
  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)myViewPort)->getView();
  if ( !view3d.IsNull() ) view3d->SetProj (V3d_Xneg);
  onViewFitAll();
}

/*!
    Provides right projection of the active view
*/
void OCCViewer_ViewFrame::onViewRight()
{
  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)myViewPort)->getView();
  if ( !view3d.IsNull() ) view3d->SetProj (V3d_Yneg);
  onViewFitAll();
}

/*!
    Provides left projection of the active view
*/
void OCCViewer_ViewFrame::onViewLeft()
{
  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)myViewPort)->getView();
  if ( !view3d.IsNull() ) view3d->SetProj (V3d_Ypos);
  onViewFitAll();
}

/*!
    Provides bottom projection of the active view
*/
void OCCViewer_ViewFrame::onViewBottom()
{
  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)myViewPort)->getView();
  if ( !view3d.IsNull() ) view3d->SetProj(V3d_Zneg);
  onViewFitAll();
}

/*!
    Provides top projection of the active view
*/
void OCCViewer_ViewFrame::onViewTop()
{
  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)myViewPort)->getView();
  if ( !view3d.IsNull() ) view3d->SetProj( V3d_Zpos );
  onViewFitAll();
}

/*!
  Display/hide Trihedron
*/
void OCCViewer_ViewFrame::onViewTrihedron()
{
  if (!myViewer->getAISContext()->IsDisplayed( myViewer->getTrihedron() ))
    myViewer->getAISContext()->Display( myViewer->getTrihedron() );
  else
    myViewer->getAISContext()->Erase( myViewer->getTrihedron() );
  onAdjustTrihedron();
}

void OCCViewer_ViewFrame::rename( const Handle(SALOME_InteractiveObject)& IObject, QString newName )
{
  myViewer->rename(IObject, newName);
}

void OCCViewer_ViewFrame::unHighlightAll() 
{
  myViewer->unHighlightAll();
}

void OCCViewer_ViewFrame::highlight( const Handle(SALOME_InteractiveObject)& IObject, bool highlight, bool update ) 
{
  myViewer->highlight(IObject, highlight, update);
}

bool OCCViewer_ViewFrame::isInViewer( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  return myViewer->isInViewer( IObject );
}

bool OCCViewer_ViewFrame::isVisible( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  return myViewer->isVisible( IObject );
}

void OCCViewer_ViewFrame::setPopupServer( QAD_Application* App )
{
  myViewer->setPopupServer( App );
}

void OCCViewer_ViewFrame::undo(SALOMEDS::Study_var aStudy,
				  const char* StudyFrameEntry)
{
  AIS_ListOfInteractive List1;
  myViewer->getAISContext()->ObjectsInCollector(List1);
  AIS_ListIteratorOfListOfInteractive ite1(List1);
  while (ite1.More()) {
    if (ite1.Value()->IsKind(STANDARD_TYPE(SALOME_AISShape))) {
      Handle(SALOME_AISShape) aSh = Handle(SALOME_AISShape)::DownCast(ite1.Value());
      if ( aSh->hasIO() ) {
	Handle(SALOME_InteractiveObject) IO = aSh->getIO();
	if ( IO->hasEntry() ) { 
	  if (!QAD_ViewFrame::isInViewer(aStudy, IO->getEntry(), StudyFrameEntry))
	    myViewer->getAISContext()->Display(aSh);
	}
      }
    }
    ite1.Next();
  }
  
  AIS_ListOfInteractive List;
  myViewer->getAISContext()->DisplayedObjects(List);
  AIS_ListIteratorOfListOfInteractive ite(List);
  while (ite.More()) {
    if (ite.Value()->IsKind(STANDARD_TYPE(SALOME_AISShape))) {
      Handle(SALOME_AISShape) aSh = Handle(SALOME_AISShape)::DownCast(ite.Value());
      if ( aSh->hasIO() ) {
	Handle(SALOME_InteractiveObject) IO = aSh->getIO();
	if ( IO->hasEntry() ) { 
	  if (!QAD_ViewFrame::isInViewer(aStudy, IO->getEntry(), StudyFrameEntry ))
	    myViewer->getAISContext()->Erase(aSh,true,true);
	}
      }
    }
    ite.Next();
  }
}

void OCCViewer_ViewFrame::redo(SALOMEDS::Study_var aStudy,
				  const char* StudyFrameEntry)
{
  SALOMEDS::SObject_var RefSO;
  SALOMEDS::SObject_var SO = aStudy->FindObjectID( StudyFrameEntry );
  SALOMEDS::ChildIterator_var it = aStudy->NewChildIterator(SO);
  for (; it->More();it->Next()){
    SALOMEDS::SObject_var CSO= it->Value();
    if (CSO->ReferencedObject(RefSO)) {
      
      AIS_ListOfInteractive List;
      myViewer->getAISContext()->ObjectsInCollector(List);
      AIS_ListIteratorOfListOfInteractive ite(List);
      while (ite.More()) {
	if (ite.Value()->IsKind(STANDARD_TYPE(SALOME_AISShape))) {
	  Handle(SALOME_AISShape) aSh = Handle(SALOME_AISShape)::DownCast(ite.Value());
	  if ( aSh->hasIO() ) {
	    Handle(SALOME_InteractiveObject) IO = aSh->getIO();
	    if ( IO->hasEntry() ) { 
	      if ( strcmp(IO->getEntry(),RefSO->GetID()) == 0 )
		myViewer->getAISContext()->Display(aSh, false);
	    }
	  }
	}
	ite.Next();
      }
      
    }
  }
  
  AIS_ListOfInteractive List1;
  myViewer->getAISContext()->DisplayedObjects(List1);
  AIS_ListIteratorOfListOfInteractive ite1(List1);
  while (ite1.More()) {
    if (ite1.Value()->IsKind(STANDARD_TYPE(SALOME_AISShape))) {
      Handle(SALOME_AISShape) aSh = Handle(SALOME_AISShape)::DownCast(ite1.Value());
      if ( aSh->hasIO() ) {
	Handle(SALOME_InteractiveObject) IO = aSh->getIO();
	if ( IO->hasEntry() ) { 
	  if (!QAD_ViewFrame::isInViewer(aStudy, IO->getEntry(), StudyFrameEntry ))
	    myViewer->getAISContext()->Erase(aSh,false,true);
	}
      }
    }
    ite1.Next();
  }
  Repaint();
}

/* selection */
Handle(SALOME_InteractiveObject) OCCViewer_ViewFrame::FindIObject(const char* Entry)
{
  Handle(SALOME_InteractiveObject) IO;
  Handle (AIS_InteractiveContext) ic = myViewer->getAISContext();
      
  AIS_ListOfInteractive List;
  ic->DisplayedObjects(List);
  AIS_ListOfInteractive List1;
  ic->ObjectsInCollector(List1);
  List.Append(List1);

  AIS_ListIteratorOfListOfInteractive ite(List);
  while (ite.More()) {
    if (ite.Value()->IsKind(STANDARD_TYPE(SALOME_AISShape))) {
      Handle(SALOME_AISShape) aSh
	= Handle(SALOME_AISShape)::DownCast(ite.Value());
      if ( aSh->hasIO() ) {
	IO = aSh->getIO();
	if ( IO->hasEntry() ) {
	  if ( strcmp( IO->getEntry(), Entry ) == 0 ) {
	    MESSAGE ( "IO found")
	    return IO;
	  }
	}
      }
    }
    ite.Next();
  }
  MESSAGE ( "IO not found")
  return IO;
}

/* display */		
void OCCViewer_ViewFrame::Display(const Handle(SALOME_InteractiveObject)& IObject, bool update)
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );

  Handle (AIS_InteractiveContext) ic = myViewer->getAISContext();

  AIS_ListOfInteractive List;
  ic->DisplayedObjects(List);
  AIS_ListOfInteractive List1;
  ic->ObjectsInCollector(List1);
  List.Append(List1);
  
  AIS_ListIteratorOfListOfInteractive ite(List);
  while (ite.More()) {
    if (ite.Value()->IsKind(STANDARD_TYPE(SALOME_AISShape))) {
      Handle(SALOME_AISShape) aSh
	= Handle(SALOME_AISShape)::DownCast(ite.Value());
      if ( aSh->hasIO() ) {
	Handle(SALOME_InteractiveObject) IO = aSh->getIO();
	if ( IO->isSame( IObject ) ) {
	  ic->Display(aSh, false);
	  //	  ic->AddOrRemoveCurrentObject(aSh, false); repeated in Viewer3d after next statement
	  Sel->AddIObject(IO, false);
	  break;
	}
      }
    }
    ite.Next();
  }
  if (update)
    Repaint();
}

void OCCViewer_ViewFrame::DisplayOnly(const Handle(SALOME_InteractiveObject)& IObject)
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );

  Handle (AIS_InteractiveContext) ic = myViewer->getAISContext();
  
  AIS_ListOfInteractive List;
  ic->DisplayedObjects(List);
  AIS_ListOfInteractive List1;
  ic->ObjectsInCollector(List1);
  List.Append(List1);
  
  AIS_ListIteratorOfListOfInteractive ite(List);
  while (ite.More()) {
    if (ite.Value()->IsKind(STANDARD_TYPE(SALOME_AISShape))) {
      Handle(SALOME_AISShape) aSh
	= Handle(SALOME_AISShape)::DownCast(ite.Value());
      if ( aSh->hasIO() ) {
	Handle(SALOME_InteractiveObject) IO = aSh->getIO();
	if ( !IO->isSame( IObject ) ) {
	  ic->Erase(aSh, false);
	  Sel->RemoveIObject(IO, false);
	} else {
	  ic->Display(aSh, false);
	  Sel->AddIObject(IO, false);
	}
      }
    }
    ite.Next();
  }
  Repaint();
}
void OCCViewer_ViewFrame::Erase(const Handle(SALOME_InteractiveObject)& IObject, bool update)
{
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOME_Selection* Sel
    = SALOME_Selection::Selection( myStudy->getSelection() );

  Handle (AIS_InteractiveContext) ic = myViewer->getAISContext();
  
  AIS_ListOfInteractive List;
  ic->DisplayedObjects(List);
  AIS_ListIteratorOfListOfInteractive ite(List);
  while (ite.More()) 
    {
      if (ite.Value()->IsKind(STANDARD_TYPE(SALOME_AISShape)))
	{
	  Handle(SALOME_AISShape) aSh
	    = Handle(SALOME_AISShape)::DownCast(ite.Value());
	  if ( aSh->hasIO() )
	    {
	      Handle(SALOME_InteractiveObject) IO = aSh->getIO();
	      if ( IO->isSame( IObject ) )
		{
		  ic->Erase(aSh, false);
		  Sel->RemoveIObject(IO, false);
		  break;
		}
	    }
	}
      ite.Next();
    }
  if (update)
    Repaint();
}

void OCCViewer_ViewFrame::DisplayAll()
{
  myViewer->getAISContext()->DisplayAll(Standard_False,Standard_True);
}
void OCCViewer_ViewFrame::EraseAll()
{
  Standard_Boolean isTrihedronDisplayed = 
    myViewer->getAISContext()->IsDisplayed( myViewer->getTrihedron() );
  myViewer->getAISContext()->EraseAll();
  if (isTrihedronDisplayed)
    myViewer->getAISContext()->Display( myViewer->getTrihedron() );
  else
    Repaint();
}



void OCCViewer_ViewFrame::Repaint()
{
  onAdjustTrihedron();
  myViewer->getViewer3d()->Update();
}

void OCCViewer_ViewFrame::onAdjustTrihedron()
{
  Handle (AIS_InteractiveContext) ic = myViewer->getAISContext();
  
  if (!ic->IsDisplayed(myViewer->getTrihedron())) 
    return;
  else 
    {
      AIS_ListOfInteractive List;
      ic->Erase( myViewer->getTrihedron() );
      ic->DisplayedObjects(List);
      ic->Display( myViewer->getTrihedron() ); 
      if (List.IsEmpty())
	{
	  myViewer->setTrihedronSize(100);
	  return;
	}
    }
  
  Handle( V3d_View) view3d = ((OCCViewer_ViewPort3d*)myViewPort)->getView();
  
  if (!view3d.IsNull())
    {
      double Xmin=0, Ymin=0, Zmin=0, Xmax=0, Ymax=0, Zmax=0;
      double aMaxSide; 
      double aPercents;
     
      view3d->View()->MinMaxValues(Xmin, Ymin, Zmin, Xmax, Ymax, Zmax);

      if (Xmin==RealFirst() || Ymin==RealFirst() || Zmin==RealFirst() ||
	  Xmax==RealLast()  || Ymax==RealLast()  || Zmax==RealLast())
	return;
           
      aMaxSide = Xmax - Xmin;
      if (aMaxSide < Ymax -Ymin) aMaxSide = Ymax -Ymin;
      if (aMaxSide < Zmax -Zmin) aMaxSide = Zmax -Zmin;
      
      static float aSizeInPercents = 105;
      QString aSetting = QAD_CONFIG->getSetting("Viewer:TrihedronSize");
      if (!aSetting.isEmpty())	aSizeInPercents = aSetting.toFloat();

      static float EPS = 5.0E-3;
      float aSize = myViewer->getTrihedron()->Size();
      float aNewSize = aMaxSide*aSizeInPercents/100.0;
      // if the new trihedron size have sufficient difference, then apply the value
      if(fabs(aNewSize-aSize) > aSize*EPS || fabs(aNewSize-aSize) > aNewSize*EPS)
	myViewer->setTrihedronSize(aNewSize);
    }
}
