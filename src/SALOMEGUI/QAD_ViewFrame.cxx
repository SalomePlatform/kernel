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

bool QAD_ViewFrame::isInViewer(SALOMEDS::Study_var aStudy,
			       const char* anEntry, const char* StudyFrameEntry)
{
  SALOMEDS::SObject_var RefSO;
  SALOMEDS::SObject_var SO = aStudy->FindObjectID(StudyFrameEntry);
  SALOMEDS::ChildIterator_var it = aStudy->NewChildIterator(SO);
  for (; it->More();it->Next()){
    SALOMEDS::SObject_var CSO= it->Value();
    if (CSO->ReferencedObject(RefSO))
      if ( strcmp( anEntry, RefSO->GetID() ) == 0 )
	return 1;
  }
  return 0;
}

//san:T3.13 - it's up to VTKViewer_ViewFrame to implement (or not implement) getRenderer() method. Be careful!
/*
vtkRenderer* QAD_ViewFrame::getRenderer()
{
  MESSAGE ( "Only redefined  VTKViewer")
  return NULL;
}
*/

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
