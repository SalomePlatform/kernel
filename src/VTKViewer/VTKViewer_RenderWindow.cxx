//  SALOME VTKViewer : build VTK viewer into Salome desktop
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
//  File   : VTKViewer_RenderWindow.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

using namespace std;
#include "VTKViewer_RenderWindow.h"
#include "utilities.h"
#include "QAD_Settings.h"
#include "QAD_Config.h"
#include "QAD_Desktop.h"
#include "QAD_Study.h"
#include "QAD_Tools.h"
#include "SALOME_Selection.h"

#include <qcolordialog.h>

#include <stdlib.h>
#include <math.h>
#include <iostream.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkRendererCollection.h>
#include <vtkXOpenGLRenderWindow.h>
#include <vtkCamera.h>

//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <qgl.h>

#if QT_VERSION > 300
#include <qcursor.h>
#endif

VTKViewer_RenderWindow::VTKViewer_RenderWindow(QWidget *parent, const char *name) :
  QWidget(parent, name, 
	  Qt::WStyle_NoBorder | Qt::WDestructiveClose | 
	  Qt::WResizeNoErase | Qt::WRepaintNoErase)
{
  myRW = vtkRenderWindow::New();
  MESSAGE(QString("RenderWindow is ") + QString(myRW->GetClassName()));
  myRW->SetDisplayId((void*)x11Display());
  myRW->SetWindowId((void*)winId());
  myRW->DoubleBufferOn();
  setMouseTracking(true);
}

VTKViewer_RenderWindow::~VTKViewer_RenderWindow() {
  myRW->Delete(); //BUG SAL2954, Commented by SRN, to avoid problems when using rlogin
  // Uncommented because of bug SAL3913
}

void VTKViewer_RenderWindow::paintEvent(QPaintEvent* theEvent) {
  myRW->Render();
}

void VTKViewer_RenderWindow::resizeEvent(QResizeEvent* theEvent) {
  int aWidth = myRW->GetSize()[0], aHeight = myRW->GetSize()[1];
  if(vtkRenderWindowInteractor* aRWI = myRW->GetInteractor())
    aRWI->UpdateSize(width(), height());
  if(aWidth != width() || aHeight != height()){
    vtkRendererCollection * aRenderers = myRW->GetRenderers();
    aRenderers->InitTraversal();
    double aCoeff = 1.0;
    if(vtkRenderer *aRenderer = aRenderers->GetNextItem()){
      vtkCamera *aCamera = aRenderer->GetActiveCamera();
      double aScale = aCamera->GetParallelScale();
      if((aWidth - width())*(aHeight - height()) > 0)
	aCoeff = sqrt(double(aWidth)/double(width())*double(height())/double(aHeight));
      else
	aCoeff = double(aWidth)/double(width());
      aCamera->SetParallelScale(aScale*aCoeff);
    }
  }
}


void VTKViewer_RenderWindow::mouseMoveEvent( QMouseEvent *event ) {
  emit MouseMove(event) ;
}

void VTKViewer_RenderWindow::mousePressEvent( QMouseEvent *event ) {
  emit ButtonPressed(event) ;
  switch(event->button()) {
  case LeftButton:
    emit LeftButtonPressed(event) ;
    break ;
  case MidButton:
    emit MiddleButtonPressed(event) ;
    break ;
  case RightButton:
    if ( event->state() == Qt::ControlButton ) {
      emit RightButtonPressed(event) ;
    } else {
      QPopupMenu* popup = createPopup();
      if ( popup ) {
	QAD_Tools::checkPopup( popup );
	if ( popup->count()>0 ) {
	  popup->exec( QCursor::pos() );
	}
	destroyPopup();
      }
    }
    break;
  default:
    break ;
  }
}


void VTKViewer_RenderWindow::mouseReleaseEvent( QMouseEvent *event ) {
  emit ButtonReleased(event) ;
  switch(event->button()) {
  case LeftButton:
    emit LeftButtonReleased(event) ;
    break ;
  case MidButton:
    emit MiddleButtonReleased(event) ;
    break ;
  case RightButton:
    emit RightButtonReleased(event) ;
    break;
  default:
    break ;
  }
}

void VTKViewer_RenderWindow::keyPressEvent (QKeyEvent * event) {
  emit KeyPressed(event) ;
}

/*!
    Creates the popup 
*/
void VTKViewer_RenderWindow::onCreatePopup() 
{
  if ( myPopup ) {	
    QAD_Desktop*     Desktop = (QAD_Desktop*) QAD_Application::getDesktop();
    QAD_Study*   myActiveStudy = Desktop->getActiveStudy();
    SALOME_Selection*      Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
    
    QString theContext;
    QString theParent("Viewer");
    QString theObject;
    
    Desktop->definePopup( theContext, theParent, theObject );
    Desktop->createPopup( myPopup, theContext, theParent, theObject);
    Desktop->customPopup( myPopup, theContext, theParent, theObject );

//    if (Sel->IObjectCount() == 0 && myPopup->count()<1) {
    if ( myPopup->count() > 0 )
      myIDs.append ( myPopup->insertSeparator() );	
    int id;
    myIDs.append ( id = myPopup->insertItem (tr ("MEN_VP3D_CHANGEBGR")) );	
    QAD_ASSERT ( myPopup->connectItem ( id, this, SLOT(onChangeBackgroundColor())) );
//    }
  }
}


void VTKViewer_RenderWindow::onChangeBackgroundColor()
{
  float red, green, blue;
  float backint[3];

  vtkRendererCollection * theRenderers = myRW->GetRenderers();
  theRenderers->InitTraversal();
  vtkRenderer * theRenderer = theRenderers->GetNextItem();
  theRenderer->GetBackground(backint);

  QColor selColor = QColorDialog::getColor ( QColor(int(backint[0]*255), int(backint[1]*255), int(backint[2]*255)), NULL );	
  if ( selColor.isValid() ) {
    theRenderer->SetBackground( selColor.red()/255., selColor.green()/255., selColor.blue()/255. ); 
    QAD_CONFIG->addSetting( "VTKViewer:BackgroundColorRed",   selColor.red() );
    QAD_CONFIG->addSetting( "VTKViewer:BackgroundColorGreen", selColor.green() );
    QAD_CONFIG->addSetting( "VTKViewer:BackgroundColorBlue",  selColor.blue() );
  }
}
