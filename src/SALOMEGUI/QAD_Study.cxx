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
//  File   : QAD_Study.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

/*!
  \class QAD_Study QAD_Study.h
  \brief Study for QAD-based application.
*/

#include "QAD_PyInterp.h" // this include must be first (see PyInterp_base.h)!
#include "QAD.h"
#include "QAD_Tools.h"
#include "QAD_Desktop.h"
#include "QAD_Study.h"
#include "QAD_RightFrame.h"
#include "QAD_LeftFrame.h"
#include "QAD_MessageBox.h"
#include "QAD_Application.h"
#include "QAD_ObjectBrowser.h"
#include "QAD_Config.h"
 
#include "utilities.h"

#include "SALOME_Selection.h"
#include "SALOME_TypeFilter.hxx"
#include "SALOME_InteractiveObject.hxx"
#include "SALOME_ListIteratorOfListIO.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

// OPEN CASCADE Include
#include <Standard_Failure.hxx>

// QT Include
#include <qapplication.h>
#include <qthread.h>
#include <qmutex.h>

using namespace std;


#ifdef _DEBUG_
static int MYDEBUG = 1;
#else
static int MYDEBUG = 0;
#endif


class TInitStudyThread : public QThread{
  TInitStudyThread();
  TInitStudyThread(const TInitStudyThread&);

public:
  TInitStudyThread(QAD_PyInterp*& theInterp, QMutex* theMutex): 
    myInterp(theInterp),
    myStudyLock(new ThreadLock(theMutex,"TInitStudyThread::TInitStudyThread"))
  {}
  virtual ~TInitStudyThread() {
    if(myStudyLock)
    delete myStudyLock;
  }

protected:
  virtual void run(){
    {
      ThreadLock aPyLock = GetPyThreadLock("TInitStudyThread::aPyLock");
      if(MYDEBUG) MESSAGE("TInitStudyThread::run()");
      myInterp = new QAD_PyInterp();
      myInterp->initialize();
    }
    delete myStudyLock;
    myStudyLock = NULL;
  }
  
private:
  QAD_PyInterp*& myInterp;
  ThreadLock* myStudyLock;
};


/*!
    Constructor
*/
QAD_Study::QAD_Study(QAD_Application* theApp,
		     SALOMEDS::Study_var theStudy,
		     const QString& thePath):
  myStudy(theStudy),
  myOperationState(Undef),
  myApp(theApp),
  myActiveStudyFrame(0),
  myStudyFrameCount(0),
  myPath(thePath),
  myTitle(QAD_Tools::getFileNameFromPath(thePath,true)),
  myIsActive(false),
  myIsSaved(false),
  myIsModified(false),
  myIsReadOnly(false),
  myResult(true),
  myInterp(0),
  myInitStudyThread(0),
  myMutex(new QMutex())
{
  myStudyFrames.setAutoDelete( true );
  myOperations.setAutoDelete( true );
  myChildWidgets.setAutoDelete( true );
  
  /* create default selection */
  //NRI    Selection( "Salome" );
  Selection( QAD_Application::getDesktop()->getComponentUserName( "KERNEL" ) );
  
  /* create python interpreter */
  QAD_PyInterp::init_python(); // initialize Python out of qthread
  myInitStudyThread = new TInitStudyThread(myInterp,myMutex);
  myInitStudyThread->start();

  /* create study frame */
  createStudyFrame( getNextStudyFrameName() );
  
  /* set default Undo/Redo limit */
  QAD_ASSERT_DEBUG_ONLY( !myStudy->_is_nil() );
  SALOMEDS::StudyBuilder_var SB = myStudy->NewBuilder();
  
  int aLocked = myStudy->GetProperties()->IsLocked();
  if (aLocked) myStudy->GetProperties()->SetLocked(false);
  SB->UndoLimit(QAD_Desktop::getUndoLevel());
  if (aLocked) myStudy->GetProperties()->SetLocked(true);
}


void QAD_Study::Init()
{
}


/*!
    Destructor
*/
QAD_Study::~QAD_Study ()
{
  close();
  //SRN: added - clear selection in case the study will be loaded again so the title will coincide
  SALOME_Selection::RemoveSelection( QString(myTitle + "_" + mySelection) );
  {
    {
      ThreadLock aLock(myMutex,"QAD_Study::~QAD_Study()");
      delete myInitStudyThread;
    }
    delete myMutex;
    delete myInterp;
  }
}

/*!
  Returns TRUE if Study is created correctly. Returns FALSE otherwise.
*/
bool QAD_Study::getResult()
{
  return myResult;
}


//=======================================================================//
//			    StudyFrames management			 //
//=======================================================================//
/*!
    Adds study frame
*/
void QAD_Study::addStudyFrame(QAD_StudyFrame* sf )
{
  myStudyFrames.append( sf );
  
  sf->getRightFrame()->getViewFrame()->setPopupServer( myApp );
  // Provide the same common popup menu commands for both the Object Browser and 3D viewer
  sf->getLeftFrame()->getObjectBrowser()->setPopupServer( myApp );

  /* icon of Study Frame */
  const QPixmap& icon = myApp->getApplicationIcon();
  if ( !icon.isNull() )
    sf->setIcon ( icon );

  /* activation */
  QAD_ASSERT ( connect( sf, SIGNAL(sfStudyFrameActivated(QAD_StudyFrame*) ),
			SLOT(onStudyFrameActivated(QAD_StudyFrame*))) );

  /* closing */
  QAD_ASSERT ( connect( sf,
			SIGNAL(sfStudyFrameClosing(QAD_StudyFrame*) ),
			this,
			SLOT(onLastStudyFrameClosing(QAD_StudyFrame*))) );

  /* set active sf */
  myActiveStudyFrame = sf;
}

/*!
    Removes studyframe from the list
*/
void QAD_Study::removeStudyFrame( QAD_StudyFrame* sf )
{
  if (sf) {
    if (!myStudy->IsEmpty()) {
      SALOMEDS::SObject_var fatherSF = myStudy->FindObjectID(sf->entry());
      if (!fatherSF->_is_nil()) {
	SALOMEDS::StudyBuilder_var aStudyBuilder = myStudy->NewBuilder();
	int aLocked = myStudy->GetProperties()->IsLocked();
	if (aLocked) myStudy->GetProperties()->SetLocked(false);
	aStudyBuilder->RemoveObject(fatherSF);
	if (aLocked) myStudy->GetProperties()->SetLocked(true);
      }

      updateObjBrowser( true );
    }
    
    QAD_RightFrame* rightFrame = sf->getRightFrame();
    if ( rightFrame ) {
      QAD_ViewFrame* viewFrame = rightFrame->getViewFrame();
      if ( viewFrame )
	emit supervStudyFrameClosing( viewFrame );
    }

    myStudyFrames.removeRef( sf );
  }
}

/*!
  Returns number of study frames
*/
int QAD_Study::getStudyFramesCount() const
{
  return myStudyFrames.count();
}

/*!
  Returns study frame by its index in list
*/
QAD_StudyFrame* QAD_Study::getStudyFrame( unsigned i )
{
  return myStudyFrames.at(i);
}

/*!
  Returns study frame by its name or null if not found
*/
QAD_StudyFrame* QAD_Study::getStudyFrameByName ( const QString& name )
{
  for ( QAD_StudyFrame* sf = myStudyFrames.first(); sf; sf = myStudyFrames.next() )
    {
      if ( sf->title().compare(name) == 0 ) {
	return sf;
      }
    }
  return NULL;
}

/*!
  Returns first study frame in the list
*/
QAD_StudyFrame* QAD_Study::getFirstStudyFrame()
{
  return myStudyFrames.first();
}

/*!
  Returns last study frame in the list
*/
QAD_StudyFrame* QAD_Study::getLastStudyFrame()
{
  return myStudyFrames.last();
}

/*!
  Returns the vector of studyframes
*/
const QList<QAD_StudyFrame>& QAD_Study::getStudyFrames() const
{
  return myStudyFrames;
}

/*!
  Returns the active study frame
*/
QAD_StudyFrame* QAD_Study::getActiveStudyFrame() const
{
  return myActiveStudyFrame;
}

//=======================================================================//
//		    Study properties					 //
//=======================================================================//
/*!
  Returns reference to supporting application
*/
QAD_Application* QAD_Study::getApp() const
{
  return myApp;
}

/*!
  Returns the name of document ( filename without path and extension )
*/
const QString& QAD_Study::getTitle() const
{
  return myTitle;
}

/*!
    Returns the absolute file path of this document
*/
const QString& QAD_Study::getPath() const
{
  return myPath;
}

/*!
    Changes the name of document
*/
void QAD_Study::setTitle( const QString& path )
{
  myPath = path;

  QString title = QAD_Tools::getFileNameFromPath( path, true );
  QAD_ASSERT_DEBUG_ONLY ( !title.isNull() );

  for ( QAD_StudyFrame* sf = myStudyFrames.first(); sf ; sf = myStudyFrames.next() )
    {
      QString oldName = sf->title();
      int k = oldName.find( myTitle, 0, false );
      QString restName = ( oldName.length() > myTitle.length() + k) ?
	oldName.right(oldName.length() - (myTitle.length() + k) + 1) :
	QString( "" );

      if ( k != -1 )
	{
	  QString newName;
	  if ( k == 0 ) {
	    int l = restName.find( "#", 0, false );
	    int ll =  restName.length();
	    newName = title + restName.mid( l, ll-l );
	  } else {
	    newName = oldName.mid(0, k);
	    newName = newName + title + restName;
	  }
	  sf->setTitle( newName );
	}
    }
  myTitle = title;

  Selection( QAD_Application::getDesktop()->getActiveComponent() );
}

/*!
    Returns whether the document is active
*/
bool QAD_Study::isActive() const
{
  return myIsActive;
}

/*!
    Returns whether the document is read only
*/
bool QAD_Study::isReadOnly() const 
{
  return myIsReadOnly;
}

/*!
    Sets read-only property
*/
void QAD_Study::setReadOnly(bool state)
{
  myIsReadOnly = state;
}

bool QAD_Study::isLocked()
{
  bool state = false;
  if ( !CORBA::is_nil( myStudy ) )
    state = myStudy->GetProperties()->IsLocked();
  return state;
}

//=======================================================================//
//			Study operations				 //
//=======================================================================//
/*!
    Performs specific study frame related actions when it is activated.
    By default resumes all suspended operations.
*/
void QAD_Study::onStudyFrameActivated( QAD_StudyFrame* activeStudyFrame )
{
//  static int IS_FIRST_STUDY = 1;  
//  if(IS_FIRST_STUDY){ //for normally initialize "salome.py and ..."
//    _interp->run("");  IS_FIRST_STUDY = 0;
//  }
//  bool found = false;
  for ( QPtrListIterator<QAD_StudyFrame> it( myStudyFrames ); it.current(); ++it ) {
    if ( it.current() == activeStudyFrame) {		/* one of my study frames */
//      found = true;
//      if ( !myActiveStudyFrame || myActiveStudyFrame != activeStudyFrame ) {
	myActiveStudyFrame =  activeStudyFrame;
	//NRI	 if ( !myIsActive ) {
	myIsActive = true;
	resumeAllOperations();
	/* activate application */
	myApp->onStudyActivated( this );
	//NRI	   }
//      }
//      return;
    }
  }
//  if (!found)
//    myActiveStudyFrame = 0;
}


/*!
    Performs specific study related actions when it is deactivated.
    By default suspends all performing operations.
*/
void QAD_Study::onStudyDeactivated()
{
  //  MESSAGE ("QAD_Study::onStudyDeactivated init. "); 
  myIsActive = false;
  suspendAllOperations();
  //  MESSAGE ("QAD_Study::onStudyDeactivated done. "); 
}

/*!
    Closes all study frames of the study and performs other specific study
    related actions needed for proper closing of the study
*/
void QAD_Study::close()
{
  emit closed();
  if ( !myStudy->_is_nil() )
    abortAllOperations();
    /* clear each study frame */
    for ( QAD_StudyFrame* sf = myStudyFrames.first(); sf; sf = myStudyFrames.first() ) {
      sf->disconnect();
      sf->close();
      // SAL2200
      QApplication::sendPostedEvents();
      removeStudyFrame(sf);
    }

  /* delete all studyframes */
  myStudyFrames.clear();
  myOperations.clear();
  myChildWidgets.clear();

//  QWorkspace* ws = QAD_Application::getDesktop()->getMainFrame();
//  if (/* !ws->activeWindow() && */ws->windowList().count() > 0 )
//    ws->activateWindow(ws->windowList().last());
}

//=======================================================================//
//			Operations management				 //
//=======================================================================//
/*!
    Returns number of completed operations
*/
int QAD_Study::getOperationsCount() const
{
    return myOperations.count();
}

/*!
    Returns the state of the last operation
*/
OperationState QAD_Study::getOperationState() const
{
    return myOperationState;
}

/*!
    Returns operation by its index in list
*/
QAD_Operation* QAD_Study::getOperation( unsigned i)
{
    return ( myOperations.isEmpty() ? 0 :  myOperations.at(i) );
}

/*!
    Returns performing operation launched first
*/
QAD_Operation* QAD_Study::getFirstOperation()
{
    return ( myOperations.isEmpty() ? 0 :  myOperations.first() );
}

/*!
    Returns performing operation launched last
*/
QAD_Operation* QAD_Study::getLastOperation()
{
    return ( myOperations.isEmpty() ? 0 :  myOperations.last() );
}

/*!
    Aborts all performing operations
*/
void QAD_Study::abortAllOperations()
{
  for ( QAD_Operation* op = myOperations.last(); op;
	op = myOperations.prev() )
    op->abort();
}

/*!
    Resumes all performing operations
*/
void QAD_Study::resumeAllOperations()
{
  for ( QAD_Operation* op = myOperations.first(); op;
	op = myOperations.next() )
    op->resume();
}

/*!
    Suspendes all performing operations
*/
void QAD_Study::suspendAllOperations()
{
  //  MESSAGE ( " QAD_Study::suspendAllOperations init. " )
  for ( QAD_Operation* op = myOperations.last(); op;
	op = myOperations.prev() )
    op->suspend();
}

/*!
    Initializes the operation and checks its compatibility
    with another operation in process if any.
    Returns 'true' if init'ed OK, 'false' otherwise.
*/
bool QAD_Study::initOperation( QAD_Operation* op )
{
    if ( myIsReadOnly )
    {	/* can't start a non-const operation */
	if ( op->changesData() )
	{
	    QAD_MessageBox::error1( QAD_Application::getDesktop(), tr("ERR_ERROR"),
				    tr("ERR_DOC_READONLY"), tr("BUT_OK") );
	    return false;
	}
    }

    /* Add the new operation
    */
    if ( myOperations.isEmpty() )
	myOperations.append( op );
    else
    {
	/*  Check compatibility of new operation with the existing operations.
	    Since each operations is checked in that way, it's enough to check
	    the last operation only */
	if ( !getLastOperation()->compatibleWith( op ) )
	{
	    if ( QAD_MessageBox::warn2( QAD_Application::getDesktop(), tr("WRN_WARNING"),
					tr("QUE_ABORTCURRENTOPERATION"), tr("BUT_YES"),
					tr("BUT_NO"), QAD_YES, QAD_NO, QAD_NO )
		   == QAD_NO )
		return false;
	    getLastOperation()->abort();
	}
	myOperations.append( op );
    }
    return true;
}

/*!
    Called when operation was finished.
    Removes appointed operation from the list and aborted all operations
    launched after it.
*/
void QAD_Study::clearOperation( QAD_Operation* op)
{
  if ( myOperations.contains( op ) )
    {
      /* abort all the operations launched after 'op' */
      for ( QAD_Operation* cur = myOperations.last(); cur;
	    cur = myOperations.prev() )
	{
	  if ( cur == op ) break;
	  cur->abort();
	}
      myOperations.remove( op );
    }
}

/*!
    Creates study frame
*/
QAD_StudyFrame* QAD_Study::createStudyFrame( const QString& title, ViewType theViewType)
{
  QAD_Desktop* parent = QAD_Application::getDesktop();
//srn: Forbid appending Interface Applicative and its children to UseCase
  myStudy->EnableUseCaseAutoFilling(false);   
  
  SALOMEDS::SComponent_var father = myStudy->FindComponent("Interface Applicative");
  SALOMEDS::StudyBuilder_var aStudyBuilder = myStudy->NewBuilder();
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeName_var    aName;
  SALOMEDS::AttributeComment_var aComment;
  SALOMEDS::AttributeSelectable_var aSelAttr;

// mpv : where is "NewCommand" call?
//  aStudyBuilder->CommitCommand();

// mpv : is study is locked, then next code will raise exception. So, temporary unlock study
  int aLocked = myStudy->GetProperties()->IsLocked();
  if (aLocked) myStudy->GetProperties()->SetLocked(false);

  if ( father->_is_nil() ) {
    father = aStudyBuilder->NewComponent("Interface Applicative");
    anAttr = aStudyBuilder->FindOrCreateAttribute(father, "AttributeName");
    aName = SALOMEDS::AttributeName::_narrow(anAttr);
    aName->SetValue("Interface Applicative");
    
    anAttr = aStudyBuilder->FindOrCreateAttribute(father, "AttributeSelectable");
    aSelAttr = SALOMEDS::AttributeSelectable::_narrow(anAttr);
    aSelAttr->SetSelectable(false);
  }
  
  //VRV: T2.5 - add default viewer
  if(theViewType == VIEW_TYPE_MAX) {
    QString viewerValue = QAD_CONFIG->getSetting( "Viewer:DefaultViewer" );
    bool ok;
    theViewType = (ViewType)viewerValue.toInt( &ok, 10 ); 
    if (!ok || theViewType < VIEW_OCC || theViewType >= VIEW_TYPE_MAX)
      theViewType = VIEW_VTK;
  }
  //QApplication::restoreOverrideCursor();
  QAD_StudyFrame* sf = NULL;
  SALOMEDS::SObject_var newObj = aStudyBuilder->NewObject(father);
  if ( theViewType == VIEW_OCC) {
    //      MESSAGE ("Create Study Frame for OCC viewer");
    sf = new QAD_StudyFrame ( this, parent->getMainFrame(),
			      title, VIEW_OCC,
			      myInterp, myMutex );
    
    Standard_CString name = CORBA::string_dup(sf->title().latin1());
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeName");
    aName = SALOMEDS::AttributeName::_narrow(anAttr);
    aName->SetValue(name);
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeComment");
    aComment = SALOMEDS::AttributeComment::_narrow(anAttr);
    aComment->SetValue("OCC");
    
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeSelectable");
    aSelAttr = SALOMEDS::AttributeSelectable::_narrow(anAttr);
    aSelAttr->SetSelectable(false);
  } 
  else if ( theViewType == VIEW_VTK) {
    //      MESSAGE ("Create Study Frame for VTK viewer");
    sf = new QAD_StudyFrame ( this, parent->getMainFrame(),
			      title, VIEW_VTK,
			      myInterp, myMutex );
    Standard_CString name = strdup(sf->title().latin1());
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeName");
    aName = SALOMEDS::AttributeName::_narrow(anAttr);
    aName->SetValue(name);
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeComment");
    aComment = SALOMEDS::AttributeComment::_narrow(anAttr);
    aComment->SetValue("VTK");
    
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeSelectable");
    aSelAttr = SALOMEDS::AttributeSelectable::_narrow(anAttr);
    aSelAttr->SetSelectable(false);
  } 
  else if ( theViewType == VIEW_GRAPHSUPERV) { 
    //MESSAGE ("Create Study Frame for SUPER`VISOR Graph");
    sf = new QAD_StudyFrame ( this, parent->getMainFrame(),
			      title, VIEW_GRAPHSUPERV,
			      myInterp, myMutex );
    Standard_CString name = strdup(sf->title().latin1());
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeName");
    aName = SALOMEDS::AttributeName::_narrow(anAttr);
    aName->SetValue(name);
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeComment");
    aComment = SALOMEDS::AttributeComment::_narrow(anAttr);
    aComment->SetValue("GRAPH");
    
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeSelectable");
    aSelAttr = SALOMEDS::AttributeSelectable::_narrow(anAttr);
    aSelAttr->SetSelectable(false);
  }
  else if ( theViewType == VIEW_PLOT2D ) {
    sf = new QAD_StudyFrame ( this, parent->getMainFrame(),
			      title, VIEW_PLOT2D,
			      myInterp, myMutex );
    Standard_CString name = strdup(sf->title().latin1());
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeName");
    aName = SALOMEDS::AttributeName::_narrow(anAttr);
    aName->SetValue(name);
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeComment");
    aComment = SALOMEDS::AttributeComment::_narrow(anAttr);
    aComment->SetValue("PLOT2D");
    
    anAttr = aStudyBuilder->FindOrCreateAttribute(newObj, "AttributeSelectable");
    aSelAttr = SALOMEDS::AttributeSelectable::_narrow(anAttr);
    aSelAttr->SetSelectable(false);
  } 
  //VRV: T2.5 - add default viewer

  sf->resize( int (0.8*parent->getMainFrame()->width()),
	      int (0.8*parent->getMainFrame()->height() ));
  
  sf->setEntry(newObj->GetID());
  
  addStudyFrame( sf );
  
  updateObjBrowser( true );
  
  parent->setSettings();
  
  myResult = true;
  
  if (aLocked) myStudy->GetProperties()->SetLocked(true);

  // T2.12 - add the word "locked" to study frame captions if this study is locked
  updateCaptions();

  myStudy->EnableUseCaseAutoFilling(true);

  return sf;
}

/*!
   Updates study frame captions according to IsLocked study flag
*/
void QAD_Study::updateCaptions()
{
  QString appendix("");

  if (!myStudy->_is_nil()) {
    SALOMEDS::AttributeStudyProperties_var propAttr = myStudy->GetProperties();
/*    
    if (!propAttr->_is_nil() ) {
      if ( propAttr->IsModified() )
	appendix += " (" + tr("PRP_STUDY_MODIFIED") + ")";
      else
	appendix += " (" + tr("PRP_STUDY_SAVED") + ")";
    }  
  
*/
    if (!propAttr->_is_nil() && propAttr->IsLocked()) {
      appendix += " (" + tr("PRP_STUDY_LOCKED") + ")";
    }
  }

  for ( QPtrListIterator<QAD_StudyFrame> it( myStudyFrames ); it.current(); ++it ) {
    it.current()->setCaption(it.current()->title() + appendix);
  }
  
}

/*!
    Shows the active study frame.
    Called by Desktop after 'new' and 'open' commands.
*/
void QAD_Study::show()
{
  for ( QPtrListIterator<QAD_StudyFrame> it( myStudyFrames ); it.current(); ++it ) {
    it.current()->show();
  }
}

//=======================================================================//
//		    Study properties (CAF related)			 //
//=======================================================================//

/*!
    Returns connected SALOMEDS_Study object
*/
SALOMEDS::Study_var QAD_Study::getStudyDocument()
{
  return myStudy;
}

/*!
    Returns whether the document was saved in file
*/
bool QAD_Study::isSaved() const
{
  return myStudy->IsSaved();
}

/*!
    Returns whether the document is modified
*/
bool QAD_Study::isModified() const
{
//  return myStudy->IsModified(); // VSR:05/12/02 - USE PROPERTY : MODIFIED FLAG
  if ( !myStudy->_is_nil() ) {
    SALOMEDS::AttributeStudyProperties_var propAttr = myStudy->GetProperties();
    if ( !propAttr->_is_nil() )
      return propAttr->IsModified();
  }
  return false;
}

bool QAD_Study::undo()
{
  try {
    SALOMEDS::StudyBuilder_var SB = myStudy->NewBuilder();
    SB->Undo();

    /* Update Object Browser */
    updateObjBrowser( true );

    for ( QPtrListIterator<QAD_StudyFrame> it( myStudyFrames ); it.current(); ++it ) {
      /* Update Viewer */
      it.current()->getRightFrame()->getViewFrame()->undo( this, it.current()->entry() );
    }
    updateCaptions();

    // mpv 07.03.2003 SAL1805: clear selection must be called after study structure changed
    SALOME_Selection* Sel = SALOME_Selection::Selection( getSelection() );
    Sel->ClearIObjects();
  }
  // mpv : undo command can raise exception, if study is locked
  catch ( SALOMEDS::StudyBuilder::LockProtection& ) {
    QAD_MessageBox::warn1 ((QWidget*)QAD_Application::getDesktop(),
			   QObject::tr("WRN_WARNING"), 
			   QObject::tr("WRN_STUDY_LOCKED"),
			   QObject::tr("BUT_OK"));
    return false;
  }
  catch ( Standard_Failure ) {
    return false; }
  return true;
}

/*!
    Redo last undo
*/
bool QAD_Study::redo()
{
  try {
    SALOMEDS::StudyBuilder_var SB = myStudy->NewBuilder();
    SB->Redo();

    /* Update Object Browser */
    updateObjBrowser( true );

    /* Update Viewer */
    for ( QPtrListIterator<QAD_StudyFrame> it( myStudyFrames ); it.current(); ++it ) {
      it.current()->getRightFrame()->getViewFrame()->redo( this, it.current()->entry() );
    }
    updateCaptions();

    // mpv 07.03.2003 SAL1805: clear selection must be called after study structure changed
    SALOME_Selection* Sel = SALOME_Selection::Selection( getSelection() );
    Sel->ClearIObjects();
  }
  // mpv : redo command can raise exception, if study is locked
  catch ( SALOMEDS::StudyBuilder::LockProtection& ) {
    QAD_MessageBox::warn1 ((QWidget*)QAD_Application::getDesktop(),
			   QObject::tr("WRN_WARNING"), 
			   QObject::tr("WRN_STUDY_LOCKED"),
			   QObject::tr("BUT_OK"));
    return false;
  }
  catch ( Standard_Failure ) {
    return false; }
  return true;
}

/*!
  Check if possible to perform 'undo' command
*/
bool QAD_Study::canUndo() const
{
  if ( myStudy->_is_nil() )
    return false;

  SALOMEDS::StudyBuilder_var SB = myStudy->NewBuilder();
  return (SB->GetAvailableUndos() > 0 );
}

/*!
    Check if possible to perform 'redo' command
*/
bool QAD_Study::canRedo() const
{
  if ( myStudy->_is_nil() )
    return false;

  SALOMEDS::StudyBuilder_var SB = myStudy->NewBuilder();
  return (SB->GetAvailableRedos() > 0 );
}

/*!
    Called when operation is started
    Returns 'true' if started OK, 'false' otherwise.
*/
bool QAD_Study::onOperationStarted( QAD_Operation* op )
{
  if ( !initOperation( op ) )
    return false;

  try
    {	/* start a new transaction */
      SALOMEDS::StudyBuilder_var SB = myStudy->NewBuilder();
      SB->NewCommand();
    }
  catch ( Standard_Failure )
    {
      /*  May be, a transaction is already open,
	  it's not a problem */
    }
  myOperationState = Started;
  return true;
}

/*!
    Called when operation is finished
*/
void QAD_Study::onOperationFinished( QAD_Operation* op )
{
  try {
    /* transaction is completed OK */
    SALOMEDS::StudyBuilder_var SB = myStudy->NewBuilder();
    SB->CommitCommand();
  }
  catch ( SALOMEDS::StudyBuilder::LockProtection& ) {
    QApplication::restoreOverrideCursor();
    QAD_MessageBox::warn1 ((QWidget*)QAD_Application::getDesktop(),
			   QObject::tr("WRN_WARNING"), 
			   QObject::tr("WRN_STUDY_LOCKED"),
			   QObject::tr("BUT_OK"));
    onOperationAborted(op);
    updateObjBrowser( true );
    return;
  }
  catch ( Standard_Failure ) {
  }
  myOperationState = Finished;
  emit docOperationTerminated( true );	  /* terminated successfully */

  updateCaptions();
  // mpv: any operation must call this method, otherwise study may not be saved
  updateObjBrowser( true );

  clearOperation( op );			  /* we don't need the operation anymore */
  QAD_Application::getDesktop()->putInfo ( tr("INF_DOC_OPERATIONFINISHED") );
}

/*!
    Called when operation is suspended
*/
void QAD_Study::onOperationSuspended( QAD_Operation* op )
{
  myOperationState = Suspended;
}

/*!
    Called when operation is resumed
*/
void QAD_Study::onOperationResumed( QAD_Operation* op )
{
  myOperationState = Resumed;
}

/*!
    Called when operation is aborted
*/
void QAD_Study::onOperationAborted(QAD_Operation* op)
{
  try {
    /* abort transaction */
    SALOMEDS::StudyBuilder_var SB = myStudy->NewBuilder();
    SB->AbortCommand();
  }
  catch ( Standard_Failure )
    {
    }
  myOperationState = Aborted;
  emit docOperationTerminated( false );	      /* aborted */

  updateCaptions();

  clearOperation(op);
  QAD_Application::getDesktop()->putInfo ( tr("INF_DOC_OPERATIONABORTED") );
}

/*!
    Call when the last study frame is closing
*/
void QAD_Study::onLastStudyFrameClosing( QAD_StudyFrame* sf )
{
  if ( myStudyFrames.count() == 1 ) {
    emit docClosing( this );
  } else
    removeStudyFrame( sf );

//  QWorkspace* ws = QAD_Application::getDesktop()->getMainFrame();
//  if ( !ws->activeWindow() && ws->windowList().count() > 0 )
//    ws->activatePreviousWindow();
}

/*!
    Call when created a new window3d
*/
QAD_StudyFrame* QAD_Study::newWindow3d(QString name, ViewType theViewType, bool toShow)
{
  if(name == "")  name = getNextStudyFrameName();
  QAD_StudyFrame* sf = createStudyFrame( name, theViewType );
  if ( myResult ) {
    if (toShow) showFrame(sf);
    //    sf->compressLeft(); /* compress LeftFrame */
    sf->getRightFrame()->compressBottom(); /* compress BottomFrame */
    return sf;
  }
  return 0;
}

void QAD_Study::showFrame(QAD_StudyFrame* theFrame){
  theFrame->resize( (int)(0.64*QAD_Application::getDesktop()->getMainFrame()->width()),
 		    (int)(0.64*QAD_Application::getDesktop()->getMainFrame()->height()));
  theFrame->show();

}



/*!
    Call when setted a message
*/
void QAD_Study::setMessage(const char* message)
{
  for ( QPtrListIterator<QAD_StudyFrame> it( myStudyFrames ); it.current(); ++it ) {
    it.current()->getRightFrame()->getMessage()->setMessage(message);
  }
}

/*!
    Call when updated object browser
*/
void QAD_Study::updateObjBrowser( bool updateSelection )
{
  QAD_Desktop* parent = QAD_Application::getDesktop();
  SALOME_ModuleCatalog::ModuleCatalog_var Catalog = parent->getCatalogue();

  QString ActiveComp = parent->getActiveComponent();

  for ( QPtrListIterator<QAD_StudyFrame> it( myStudyFrames ); it.current(); ++it ) {
    it.current()->getLeftFrame()->getObjectBrowser()->Update();
  }

  /* update selection */
  if ( updateSelection && (myStudyFrames.count() > 0) ) {
    SALOME_Selection* Sel = SALOME_Selection::Selection( getSelection() );
    SALOME_ListIteratorOfListIO It( Sel->StoredIObjects() );
    for(;It.More();It.Next()) {
      if ( Sel->AddIObject( It.Value() ) == 0 ) {
	highlight( It.Value(), true, false );
      }
    }
  }
  /* update viewers */
  update3dViewers();
}

/*!
  Updates only Use Case Browser
*/
void QAD_Study::updateUseCaseBrowser( bool updateSelection )
{
  for ( QPtrListIterator<QAD_StudyFrame> it( myStudyFrames ); it.current(); ++it ) {
    it.current()->getLeftFrame()->getObjectBrowser()->UpdateUseCaseBrowser();
  }
  /* update selection */
  if ( updateSelection && (myStudyFrames.count() > 0) ) {
    SALOME_Selection* Sel = SALOME_Selection::Selection( getSelection() );
    SALOME_ListIteratorOfListIO It( Sel->StoredIObjects() );
    for(;It.More();It.Next()) {
      if ( Sel->AddIObject( It.Value() ) == 0 ) {
	highlight( It.Value(), true, false );
      }
    }
    /* update viewers */
    update3dViewers();
  }
}

/*!
  unHighlight All Interactive Objects in All Devices
*/
void QAD_Study::unHighlightAll() 
{
  for ( QPtrListIterator<QAD_StudyFrame> it( myStudyFrames ); it.current(); ++it ) {
    /* Device = Viewers */
    it.current()->getRightFrame()->getViewFrame()->unHighlightAll();

    /* Device = ObjectBrowser */
    it.current()->getLeftFrame()->getObjectBrowser()->unHighlightAll();
  }
}

/*!
  Returns type of Interactive Object
*/
int QAD_Study::typeIObject( const Handle(SALOME_InteractiveObject)& IObject )
{
  bool IsStudyObject     = isInStudy( IObject );
  bool IsGraphicalObject = isInViewer( IObject );

  //    MESSAGE ( "IsStudyObject : " <<  IsStudyObject )
  //    MESSAGE ( "IsGraphicalObject : " <<  IsGraphicalObject )

  if ( IsStudyObject ) {
    if ( IsGraphicalObject )
      return 1; /* StudyObject and GraphicalObject */
    else
      return 2; /* only StudyObject */
  } else {
    if ( IsGraphicalObject )
      return 3; /* only GraphicalObject */
    else
      return 4; /* may be a component */
  }
  return 0;
}


/*!
  Renames the Interactive Object in All Devices
*/
void QAD_Study::renameIObject( const Handle(SALOME_InteractiveObject)& IObject, QString newName )
{
  if (myStudy->GetProperties()->IsLocked()) {
    QAD_MessageBox::warn1 ((QWidget*)QAD_Application::getDesktop(),
			   QObject::tr("WRN_WARNING"), 
			   QObject::tr("WRN_STUDY_LOCKED"),
			   QObject::tr("BUT_OK"));
    return;
  }

  highlight( IObject, false );
  
  int nbFrames = myStudyFrames.count();
  for ( int i = 0; i < nbFrames; i++ ) {
    QAD_StudyFrame* sf = myStudyFrames.at( i );
    switch ( typeIObject(IObject) ) {
    case 1:
      {
	/* Device = Viewers */
	sf->getRightFrame()->getViewFrame()->rename(IObject, newName);
	/* Device = ObjectBrowser */
	sf->getLeftFrame()->getObjectBrowser()->rename(IObject, newName);
	break;
      }
    case 2:
      {
	/* Device = ObjectBrowser */
	sf->getLeftFrame()->getObjectBrowser()->rename(IObject, newName); 
      break;
      }
    case 3:
      {
	/* Device = Viewers */
	sf->getRightFrame()->getViewFrame()->rename(IObject, newName);
	break;
      }
    case 4:
      {
	QString ActiveComp = QAD_Application::getDesktop()->getActiveComponent();
	if ( !ActiveComp.isEmpty() ) {
	  sf->getLeftFrame()->getObjectBrowser()->rename(IObject,newName);
	}
	break;
      }
    }
  }

  /* SALOMEDS */
  SALOMEDS::StudyBuilder_var aStudyBuilder = myStudy->NewBuilder();
  SALOMEDS::SObject_var obj = myStudy->FindObjectID( IObject->getEntry() );
  if ( !obj->_is_nil() ) {
    SALOMEDS::GenericAttribute_var anAttr;
    SALOMEDS::AttributeName_var    aName;
    anAttr = aStudyBuilder->FindOrCreateAttribute(obj, "AttributeName");
    aName = SALOMEDS::AttributeName::_narrow(anAttr);
    aName->SetValue(newName.latin1());
  }

  // VSR 13/03/03 - rename Interactive object 
  IObject->setName( ( char* )newName.latin1() );

  updateObjBrowser( true );
}

/*!
  Selects the Interactive Object in All Devices
*/
void QAD_Study::highlight( const Handle(SALOME_InteractiveObject)& IObject, bool highlight,bool immediatly ) 
{
  //    MESSAGE ( " QAD_Study::highlight : " << highlight )
  int typeIO = typeIObject( IObject );

  for ( QPtrListIterator<QAD_StudyFrame> it( myStudyFrames ); it.current(); ++it ) {
    switch ( typeIO ) {
    case 1:
      {
	/* Device = Viewer */
	if (it.current()->getTypeView() >= 0 ) { /* Device = Viewers */
	  it.current()->getRightFrame()->getViewFrame()->highlight(IObject, highlight, immediatly);
	}
	/* Device = ObjectBrowser */
	it.current()->getLeftFrame()->getObjectBrowser()->highlight(IObject, highlight);
	break;
      }
    case 2:
      {
	/* Device = ObjectBrowser */
	it.current()->getLeftFrame()->getObjectBrowser()->highlight(IObject, highlight); 
	break;
      }
    case 3:
      {
	/* Device = Viewer */
	if (it.current()->getTypeView() >= 0) { /* Device = Viewers */
	  it.current()->getRightFrame()->getViewFrame()->highlight(IObject, highlight, immediatly);
	} 
	break;
      }
    case 4:
      {
	QString ActiveComp = QAD_Application::getDesktop()->getActiveComponent();
	if ( !ActiveComp.isEmpty() ) {
	  it.current()->getLeftFrame()->getObjectBrowser()->highlight(IObject,highlight);
	}
	break;
      }
    }
  }
}

/*!
  Returns TRUE if the IObject is a Study Object. Returns FALSE otherwise.
*/
bool QAD_Study::isInStudy( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  return IObject->hasEntry();
}

/*!
  Returns true if the IObject has a Graphical Object. Returns FALSE otherwise.
*/
bool QAD_Study::isInViewer( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  bool found = false;
  for ( QPtrListIterator<QAD_StudyFrame> it( myStudyFrames ); it.current(); ++it ) {
    found = it.current()->getRightFrame()->getViewFrame()->isInViewer(IObject);
    if ( found )
      return true;
  }
  return found;
}

/*!
  Returns true if the IObject has a Graphical Object into studyframe. Returns FALSE otherwise.
*/
bool QAD_Study::isInViewer(const char* anEntry, const char* StudyFrameEntry)
{
  SALOMEDS::SObject_var RefSO;
  SALOMEDS::SObject_var SO = myStudy->FindObjectID(StudyFrameEntry);
  SALOMEDS::ChildIterator_var it = myStudy->NewChildIterator(SO);
  for (; it->More();it->Next()){
    SALOMEDS::SObject_var CSO= it->Value();
    if (CSO->ReferencedObject(RefSO))
      if ( strcmp( anEntry, RefSO->GetID() ) == 0 )
	return 1;
  }
  return 0;
}


/*!
    Returns title for the new studyframe window
*/
QString QAD_Study::getNextStudyFrameName()
{
  QString numStudyFrame;
  numStudyFrame.sprintf("%s%d", "#", ++myStudyFrameCount );
  return myTitle + numStudyFrame;   /* return unique name ( title + unique number) */
}


/*!
  Returns the Python interpreter that belongs to this study
*/
QAD_PyInterp* QAD_Study::get_PyInterp(void)
{
  return myInterp;
}

/*!
  Sets current selection.
*/
void QAD_Study::Selection( QString aSelection )
{
  if ( !SALOME_Selection::FindSelection( QString(myTitle + "_" + aSelection) ) )
    SALOME_Selection::CreateSelection( QString(myTitle + "_" + aSelection) );

  SALOME_Selection* Sel = SALOME_Selection::Selection( QString(myTitle + "_" + aSelection) );

  mySelection = aSelection;
}

/*!
  Returns the name of current selection
*/
QString QAD_Study::getSelection() const
{
  return QString (myTitle + "_" + mySelection);
}

/*!
  Returns the study Id.
*/
int QAD_Study::getStudyId()
{
  return myStudy->StudyId();
}

void QAD_Study::update3dViewers() 
{
  for ( QPtrListIterator<QAD_StudyFrame> it( myStudyFrames ); it.current(); ++it ) {
    it.current()->getRightFrame()->getViewFrame()->Repaint();
  }
}

/* Adds new child window */
void QAD_Study::addChildWidget( QWidget* child )
{
  if ( myChildWidgets.findRef( child ) < 0 ) {
    myChildWidgets.append( child );
    child->installEventFilter( this );
  }
}

/*!
  Removes child window from the list ( and deletes it )
*/
void QAD_Study::removeChildWidget( QWidget* child )
{
  myChildWidgets.removeRef( child );
}

/* Event filter */
bool QAD_Study::eventFilter( QObject* o, QEvent* e)
{
  int index = myChildWidgets.findRef( ( QWidget* )o );
  if ( index >= 0 && e->type() == QEvent::Close ) {
    myChildWidgets.at( index )->removeEventFilter( this );
    myChildWidgets.remove( ( QWidget* )o );
    return TRUE;
  }
  return QObject::eventFilter( o, e );
}

