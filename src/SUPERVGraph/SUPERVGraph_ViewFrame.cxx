//  SALOME SUPERVGraph : build Supervisor viewer into desktop
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
//  File   : SUPERVGraph_ViewFrame.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "SUPERVGraph_ViewFrame.h"

#include "QAD_Settings.h"
#include "QAD_Config.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "SALOME_Selection.h"

#include "utilities.h"

//QT Include
#include <qlayout.h>
#include <qcolordialog.h>
using namespace std;

SUPERVGraph_View::SUPERVGraph_View(QWidget* theParent): QWidget(theParent){
  init(theParent);
}
SUPERVGraph_View::SUPERVGraph_View(SUPERVGraph_View* theParent): QWidget(theParent){
  setPopupServer(theParent->getPopupServer());
  init(theParent);
}
void SUPERVGraph_View::init(QWidget* theParent){
  if ( theParent->inherits( "QMainWindow" ) ) {
    ( ( QMainWindow* )theParent )->setCentralWidget( this );
  }
  else {
    QBoxLayout* layout = new QVBoxLayout( theParent );
    layout->addWidget( this );
  }

}
void SUPERVGraph_View::onCreatePopup(){
  /*
  if ( myPopup ) {	
    QAD_Desktop*     Desktop = (QAD_Desktop*) QAD_Application::getDesktop();
    QAD_Study*   ActiveStudy = Desktop->getActiveStudy();
    SALOME_Selection*      Sel = SALOME_Selection::Selection( ActiveStudy->getSelection() );
    
    QString theContext;
    QString theParent("Viewer");
    QString theObject;
    
    Desktop->definePopup( theContext, theParent, theObject );
    Desktop->createPopup( myPopup, theContext, theParent, theObject);
    Desktop->customPopup( myPopup, theContext, theParent, theObject );
    if (Sel->IObjectCount() == 0 && myPopup->count()<1) {
      int id;
      myIDs.append ( id = myPopup->insertItem (tr ("MEN_VP3D_CHANGEBGR")) );	
      QAD_ASSERT ( myPopup->connectItem ( id, this, SLOT(onChangeBackgroundColor())) );
    }
  }
  */
 }
/*!
    Constructor
*/
SUPERVGraph_ViewFrame::SUPERVGraph_ViewFrame(QWidget* parent, const char* name) 
  : QAD_ViewFrame(parent, name)
{
  myView = 0;
  //myView = new SUPERVGraph_View(this); 
  // Set BackgroundColor
  /*
  int R = QAD_CONFIG->getSetting("SUPERVGraph:BackgroundColorRed").toInt();
  int G = QAD_CONFIG->getSetting("SUPERVGraph:BackgroundColorGreen").toInt();
  int B = QAD_CONFIG->getSetting("SUPERVGraph:BackgroundColorBlue").toInt();
  setBackgroundColor(QColor(R,G,B));*/
}
SUPERVGraph_ViewFrame::~SUPERVGraph_ViewFrame(){}

/*!
  Returns widget containing 3D-Viewer
*/
SUPERVGraph_View* SUPERVGraph_ViewFrame::getViewWidget(){
  return myView;
}


void SUPERVGraph_ViewFrame::setViewWidget(SUPERVGraph_View* theView) {
  myView = theView;
  if (myApp) {
    myView->setPopupServer(myApp);
  }
}


/*!
  Display/hide Trihedron
*/
void SUPERVGraph_ViewFrame::onViewTrihedron()
{
  MESSAGE ( "SUPERVGraph_ViewFrame::onViewTrihedron" )
}

/*!
  Provides top projection of the active view
*/
void SUPERVGraph_ViewFrame::onViewTop()
{
  MESSAGE ( "SUPERVGraph_ViewFrame::onViewTop" )
}

/*!
  Provides bottom projection of the active view
*/
void SUPERVGraph_ViewFrame::onViewBottom()
{
  MESSAGE ( "SUPERVGraph_ViewFrame::onViewBottom" )
}

/*!
  Provides left projection of the active view
*/
void SUPERVGraph_ViewFrame::onViewLeft()    
{
  MESSAGE ( "SUPERVGraph_ViewFrame::onViewLeft" )
}

/*!
  Provides right projection of the active view
*/
void SUPERVGraph_ViewFrame::onViewRight()
{
  MESSAGE ( "SUPERVGraph_ViewFrame::onViewRight" )
}

/*!
  Provides back projection of the active view
*/
void SUPERVGraph_ViewFrame::onViewBack()
{
  MESSAGE ( "SUPERVGraph_ViewFrame::onViewBack" )
}

/*!
  Provides front projection of the active view
*/
void SUPERVGraph_ViewFrame::onViewFront()
{
  MESSAGE ( "SUPERVGraph_ViewFrame::onViewFront" )
}

/*!
  Reset the active view
*/
void SUPERVGraph_ViewFrame::onViewReset()    
{
  MESSAGE ( "SUPERVGraph_ViewFrame::onViewReset" )
    if (myView)
      myView->ResetView();
}

/*!
  Rotates the active view
*/
void SUPERVGraph_ViewFrame::onViewRotate()
{
   MESSAGE ( "SUPERVGraph_ViewFrame::onViewRotate" )
}

/*!
  Sets a new center of the active view
*/
void SUPERVGraph_ViewFrame::onViewGlobalPan()
{
   MESSAGE ( "SUPERVGraph_ViewFrame::onViewGlobalPan" )
}

/*!
  Zooms the active view
*/
void SUPERVGraph_ViewFrame::onViewZoom()
{
  MESSAGE ( "SUPERVGraph_ViewFrame::onViewZoom" )
}

/*!
  Moves the active view
*/
void SUPERVGraph_ViewFrame::onViewPan()
{
  MESSAGE ( "SUPERVGraph_ViewFrame::onViewPan" )
    if (myView != NULL)
      myView->ActivatePanning();
}

/*!
  Fits all obejcts within a rectangular area of the active view
*/
void SUPERVGraph_ViewFrame::onViewFitArea()
{
  MESSAGE ( "SUPERVGraph_ViewFrame::onViewFitArea" )
}

/*!
  Fits all objects in the active view
*/
void SUPERVGraph_ViewFrame::onViewFitAll()
{
  MESSAGE ( "SUPERVGraph_ViewFrame::onViewFitAll" )
}

/*!
    Set background of the viewport
*/
void SUPERVGraph_ViewFrame::setBackgroundColor( const QColor& color)
{
    if (myView)
      myView->setPaletteBackgroundColor(color);
}

/*!
    Returns background of the viewport
*/
QColor SUPERVGraph_ViewFrame::backgroundColor() const
{
  if (myView)
    return myView->paletteBackgroundColor();
  return QMainWindow::backgroundColor();
}


void SUPERVGraph_ViewFrame::SetSelectionMode( Selection_Mode mode )
{
  MESSAGE ( "SUPERVGraph_ViewFrame::SetSelectionMode" )
}

void SUPERVGraph_ViewFrame::onAdjustTrihedron(  )
{
  MESSAGE ( "SUPERVGraph_ViewFrame::onAdjustTrihedron" )  
}

void SUPERVGraph_ViewFrame::rename( const Handle(SALOME_InteractiveObject)& IObject, 
				    QString newName )
{
  MESSAGE ( "SUPERVGraph_ViewFrame::rename" )
}

void SUPERVGraph_ViewFrame::unHighlightAll() 
{
  MESSAGE ( "SUPERVGraph_ViewFrame::unHighlightAll" )
}

void SUPERVGraph_ViewFrame::highlight( const Handle(SALOME_InteractiveObject)& IObject, 
				       bool highlight, bool immediatly ) 
{
  MESSAGE ( "SUPERVGraph_ViewFrame::highlight" )
}

bool SUPERVGraph_ViewFrame::isInViewer( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  MESSAGE ( "SUPERVGraph_ViewFrame::isInViewer" )
  return false;
}

bool SUPERVGraph_ViewFrame::isVisible( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  MESSAGE ( "SUPERVGraph_ViewFrame::isVisible" )
  return false;
}

void SUPERVGraph_ViewFrame::setPopupServer( QAD_Application* App )
{
  myApp = App;
  if (myView) {
    myView->setPopupServer(myApp);
  }
}

void SUPERVGraph_ViewFrame::undo(SALOMEDS::Study_var aStudy,
				 const char* StudyFrameEntry)
{
  MESSAGE ( "SUPERVGraph_ViewFrame::undo" )
}

void SUPERVGraph_ViewFrame::redo(SALOMEDS::Study_var aStudy,
				 const char* StudyFrameEntry)
{
  MESSAGE ( "SUPERVGraph_ViewFrame::redo" )
}

