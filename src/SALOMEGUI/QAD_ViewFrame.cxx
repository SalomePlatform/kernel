using namespace std;
//  File      : QAD_ViewFrame.cxx
//  Created   : UI team, 05.09.00
//  Descrip   : Frame window for viewport in QAD-based application

//  Modified  : Mon Dec 03 15:41:43 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
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

vtkRenderer* QAD_ViewFrame::getRenderer()
{
  MESSAGE ( "Only redefined  VTKViewer")
  return NULL;
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
