using namespace std;
//  File      : QAD_RightFrame.cxx
//  Created   : Thu Jun 14 14:51:52 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#include "QAD_RightFrame.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_StudyFrame.h"
#include "QAD_Tools.h"
#include <qvaluelist.h>

// QT Include
#include <qmessagebox.h>

// Open CASCADE Include
#include <OSD_SharedLibrary.hxx>

/*!
  \class QAD_RightFrame QAD_RightFrame.h
  \brief Frame window which contains QAD_ViewFrame, QAD_PyInterp and QAD_Message.
*/

typedef QAD_ViewFrame* View(QAD_RightFrame*);


/*!
    Constructor
*/
QAD_RightFrame::QAD_RightFrame(QWidget *parent, const char *name,
			       QAD_PyInterp* interp, ViewType vt)
  : QAD_Splitter( Qt::Vertical, parent, name )
{
  this->setCompressEnabled( true );
  myViewType  = vt;

  QAD_Desktop* Desktop = QAD_Application::getDesktop();
  int DesktopHeight = Desktop->getMainFrame()->width();
  int DesktopWidth  = Desktop->getMainFrame()->height();

  _interp = interp;

  OSD_SharedLibrary SharedLib = OSD_SharedLibrary();
  QString ComponentLib;
  QCString dir;
  QFileInfo fileInfo ;
  bool found = false;
  if (myViewType == VIEW_OCC) {
    if ( dir = getenv("SALOME_SITE_DIR")) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
      dir = dir + "libOCCViewer.dll" ;
#else
      dir = dir + "libOCCViewer.so" ;
#endif
      MESSAGE ( " GUI library = " << dir )
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ComponentLib = fileInfo.fileName();
	found = true;
	MESSAGE (" Found ")
      } else
	MESSAGE (" Not found ")
    }
    
    if ( (dir = getenv("SALOME_ROOT_DIR")) && !found ) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
      dir = dir + "libOCCViewer.dll" ;
#else
      dir = dir + "libOCCViewer.so" ;
#endif
      MESSAGE ( " GUI library = " << dir )
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ComponentLib = fileInfo.fileName();
	found = true;
	MESSAGE (" Found ")
      } else
	MESSAGE (" Not found ")
    }
    
    if ( (dir = getenv("KERNEL_ROOT_DIR")) && !found ) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
      dir = dir + "libOCCViewer.dll" ;
#else
      dir = dir + "libOCCViewer.so" ;
#endif
      MESSAGE ( " GUI library = " << dir )
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ComponentLib = fileInfo.fileName();
	found = true;
	MESSAGE (" Found ")
      } else
	MESSAGE (" Not found ")
    }
  } else if (myViewType == VIEW_VTK) {
    
    if ( dir = getenv("SALOME_SITE_DIR")) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
      dir = dir + "libVTKViewer.dll" ;
#else
      dir = dir + "libVTKViewer.so" ;
#endif
      MESSAGE ( " GUI library = " << dir );
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ComponentLib = fileInfo.fileName();
	found = true;
	MESSAGE (" Found ");
      } else
	MESSAGE (" Not found ");
    }
        
    if ( (dir = getenv("SALOME_ROOT_DIR")) && !found ) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
      dir = dir + "libVTKViewer.dll" ;
#else
      dir = dir + "libVTKViewer.so" ;
#endif
      MESSAGE ( " GUI library = " << dir );
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ComponentLib = fileInfo.fileName();
	found = true;
	MESSAGE (" Found ");
      } else
	MESSAGE (" Not found ");
    }
        
    if ( (dir = getenv("KERNEL_ROOT_DIR")) && !found ) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
      dir = dir + "libVTKViewer.dll" ;
#else
      dir = dir + "libVTKViewer.so" ;
#endif
      MESSAGE ( " GUI library = " << dir );
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ComponentLib = fileInfo.fileName();
	found = true;
	MESSAGE (" Found ");
      } else
	MESSAGE (" Not found ");
    }
  } else if (myViewType == VIEW_GRAPHSUPERV) {
    if ( dir = getenv("SALOME_SITE_DIR")){
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
      dir = dir + "libSUPERVGraph.dll" ;
#else
      dir = dir + "libSUPERVGraph.so" ;
#endif
      MESSAGE ( " GUI library = " << dir )
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ComponentLib = fileInfo.fileName() ;
	found = true;
	MESSAGE (" Found ")
      } else
	MESSAGE (" Not found ")
    }
    
    if ( (dir = getenv("SALOME_ROOT_DIR")) && !found ) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
      dir = dir + "libSUPERVGraph.dll" ;
#else
      dir = dir + "libSUPERVGraph.so" ;
#endif
      MESSAGE ( " GUI library = " << dir )
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ComponentLib = fileInfo.fileName() ;
	found = true;
	MESSAGE (" Found ")
      } else
	MESSAGE (" Not found ")
    }
    
    if ( (dir = getenv("KERNEL_ROOT_DIR")) && !found ) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
      dir = dir + "libSUPERVGraph.dll" ;
#else
      dir = dir + "libSUPERVGraph.so" ;
#endif
      MESSAGE ( " GUI library = " << dir )
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ComponentLib = fileInfo.fileName() ;
	found = true;
	MESSAGE (" Found ")
      } else
	MESSAGE (" Not found ")
    }
  } else if (myViewType == VIEW_PLOT2D) {
    if ( dir = getenv("SALOME_SITE_DIR")){
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
      dir = dir + "libPlot2d.dll" ;
#else
      dir = dir + "libPlot2d.so" ;
#endif
      MESSAGE ( " GUI library = " << dir )
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ComponentLib = fileInfo.fileName() ;
	found = true;
	MESSAGE (" Found ")
      } else
	MESSAGE (" Not found ")
    }
    
    if ( (dir = getenv("SALOME_ROOT_DIR")) && !found ) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
      dir = dir + "libPlot2d.dll" ;
#else
      dir = dir + "libPlot2d.so" ;
#endif
      MESSAGE ( " GUI library = " << dir )
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ComponentLib = fileInfo.fileName() ;
	found = true;
	MESSAGE (" Found ")
      } else
	MESSAGE (" Not found ")
    }
    
    if ( (dir = getenv("KERNEL_ROOT_DIR")) && !found ) {
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "lib" ;
      dir = QAD_Tools::addSlash(dir) ;
      dir = dir + "salome" ;
      dir = QAD_Tools::addSlash(dir) ;
#ifdef WNT
      dir = dir + "libPlot2d.dll" ;
#else
      dir = dir + "libPlot2d.so" ;
#endif
      MESSAGE ( " GUI library = " << dir )
      fileInfo.setFile(dir) ;
      if (fileInfo.exists()) {
	ComponentLib = fileInfo.fileName() ;
	found = true;
	MESSAGE (" Found ")
      } else
	MESSAGE (" Not found ")
    }
  } else {
    QMessageBox::critical( this,
			   tr("ERR_ERROR"),
			   tr("undefined view type") );
    return;
  }

  SharedLib.SetName( strdup(ComponentLib) );
  
  bool ok = SharedLib.DlOpen(OSD_RTLD_LAZY);
  if (!ok) {
    QMessageBox::critical( this,
			   tr("ERR_ERROR"),
			   tr( SharedLib.DlError() ) );
    return;
  } else {
    OSD_Function osdF = SharedLib.DlSymb("createView");
    if ( osdF != NULL ) {
      View (*f1) = (QAD_ViewFrame* (*) (QAD_RightFrame*)) osdF;
      myViewFrame = (*f1)(this);
    } else {
      QMessageBox::critical( this,
			     tr("ERR_ERROR"),
			     tr("createView undefined") );
      return;
    }
  }

  QValueList<int> sizes;

  myViewFrame->setMinimumSize( 1, 1 );
  vsplitter = new QAD_Splitter( Qt::Horizontal, this );
  vsplitter->setMinimumSize( 1, 1 );
  vsplitter->setCompressEnabled( true );
  myPyEditor = new QAD_PyEditor(_interp, vsplitter ,"Python Interpreter");
  myPyEditor->setMinimumSize( 1, 1 );
  myMessage  = new QAD_Message( vsplitter ,"Message");  
  myMessage->setMinimumSize( 1, 1 );

  sizes.append( 0.48 * DesktopHeight );
  sizes.append( 0.16 * DesktopHeight );
  setSizes( sizes );
  sizes.clear();
  sizes.append( 0.25 * DesktopWidth );
  sizes.append( 0.25 * DesktopWidth );
  vsplitter->setSizes( sizes );
}

/*!
    Destructor
*/
QAD_RightFrame::~QAD_RightFrame()
{  
}

QAD_ViewFrame* QAD_RightFrame::getViewFrame() const
{
  return myViewFrame;
}

/*!
    Returns reference to the message window [ static ]
*/
QAD_Message* QAD_RightFrame::getMessage() const
{
  return myMessage;
}

/*!
  Returns the Python interpreter that belongs to this study
*/
QAD_PyInterp* QAD_RightFrame::get_PyInterp(void)
{
  return _interp;
}

/*!
    Returns reference to the Python window [ static ]
*/
QAD_PyEditor* QAD_RightFrame::getPyEditor() const
{
  return myPyEditor;
}

/*!
    Compress views
*/
void QAD_RightFrame::compressUp()
{
  QSplitterPHandle* h = getHandleAfter(myViewFrame);
  if (h)
    h->compressBefore();
}

/*!
    Uncompress views
*/
void QAD_RightFrame::unCompressUp()
{
  QSplitterPHandle* h = getHandleAfter(myViewFrame);
  if (h)
    h->unCompressBefore();
}

/*!
    Compress Python Interpreter and Messages windows
*/
void QAD_RightFrame::compressBottom()
{
  QSplitterPHandle* h = getHandleAfter(myViewFrame);
  if (h)
    h->compressAfter();
}

/*!
    Uncompress Python Interpreter and Messages windows
*/
void QAD_RightFrame::unCompressBottom()
{
  QSplitterPHandle* h = getHandleAfter(myViewFrame);
  if (h)
    h->unCompressAfter();
}

