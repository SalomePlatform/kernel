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
//  File   : QAD_ViewFrame.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "QAD.h"
#include "QAD_ViewFrame.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_FileDlg.h"
#include "QAD_MessageBox.h"
#include "QAD_Tools.h"
#include <qapplication.h>
#include <qimage.h>
#include "utilities.h"

using namespace std;

/*!
    Constructor
*/
QAD_ViewFrame::QAD_ViewFrame(QWidget* parent, const QString& title) 
  : QMainWindow (parent, title, 0)
{
}

/*!
    Constructor
*/
QAD_ViewFrame::QAD_ViewFrame( QWidget* parent ) 
  : QMainWindow (parent, "",0)
{
}

/*!
    Destructor
*/
QAD_ViewFrame::~QAD_ViewFrame()
{
  cleanup();
}

/*!
    Cleanup viewframe 
*/
void QAD_ViewFrame::cleanup()
{
} 

/*
   Dumps 3d-Viewer contents into image file
   File format is defined by file's extension; supported formats : PNG, BMP, GIF, JPG
*/
void QAD_ViewFrame::onViewDump()
{
  if (!getViewWidget())
    return;
  QApplication::setOverrideCursor( Qt::waitCursor );
  QPixmap px = QPixmap::grabWindow(getViewWidget()->winId());
  QApplication::restoreOverrideCursor();
  
  QString fileName = QAD_FileDlg::getFileName(QAD_Application::getDesktop(),
					      QString::null,
					      tr("OCC_IMAGE_FILES"),
					      tr("INF_APP_DUMP_VIEW"),
					      false);
  if (!fileName.isNull()) {
    QApplication::setOverrideCursor( Qt::waitCursor );
    QString fmt = QAD_Tools::getFileExtensionFromPath(fileName).upper();
    if (fmt.isEmpty())
      fmt = QString("BMP"); // default format
    if (fmt == "JPG")
      fmt = "JPEG";
    bool bOk = px.save(fileName, fmt.latin1());
    QApplication::restoreOverrideCursor();
    if (!bOk) {
      QAD_MessageBox::error1(QAD_Application::getDesktop(),
			     tr("ERR_ERROR"),
			     tr("ERR_DOC_CANT_SAVE_FILE"), 
			     tr("BUT_OK"));
    }
  }
}

#define DUMP_EVENT QEvent::User + 123
/*!
   This method is used to dump the viewer contents to the image file
   from the context popup menu (uses event mechanizm to assure redrawing
   the viewer contents before dumping by sending custom event)
*/
void QAD_ViewFrame::onProcessViewDump() 
{
  qApp->postEvent( this, new QPaintEvent( QRect( 0, 0, width(), height() ), TRUE ) );
  qApp->postEvent( this, new QCustomEvent( DUMP_EVENT ) );
}

/*!
  Processes the custom event sent by onProcessViewDump() method to
  call onViewDump() slot: dumping the view contents to the image file
  (see desription for onProcessViewDump() method above)
*/
bool QAD_ViewFrame::event ( QEvent* e )
{
  if ( e->type() == DUMP_EVENT ) {
    onViewDump();
    return TRUE;
  }
  return QMainWindow::event( e );
}
