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
//  File   : QAD_PyEditor.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "QAD_PyEditor.h"
#include "QAD_PyInterp.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_Config.h"
#include "QAD_Tools.h"
#include "QAD_MessageBox.h"

#include <qapplication.h>
#include <qmap.h>
#include <qclipboard.h>
#include <qthread.h>

// NRI : Temporary added
// IDL Headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
//NRI

using namespace std;


#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif


#define SIZEPR 4
enum { IdCopy, IdPaste, IdClear, IdSelectAll };


static QString PROMPT = ">>> ";


class TInitEditorThread : public QThread
{
public:
  TInitEditorThread(QAD_PyInterp*& theInterp, 
		    QMutex* theStudyMutex, QMutex* theMutex,
		    QAD_PyEditor* theListener):
    myInterp(theInterp), 
    myMutex(theMutex),
    myStudyMutex(theStudyMutex),
    myListener(theListener)
  {
    // san - commented as inefficient: sometimes event is processed significant period of time after this moment
    //QThread::postEvent(myListener, new QCustomEvent(QAD_PyEditor::SET_WAIT_CURSOR));
  }

  virtual ~TInitEditorThread(){}

protected:
  virtual void run(){
    ThreadLock anEditorLock(myMutex,"TInitEditorThread::anEditorLock");
    ThreadLock aStudyLock(myStudyMutex,"TInitEditorThread::aStudyLock");
    ThreadLock aPyLock = GetPyThreadLock("TInitEditorThread::aPyLock");
    if(MYDEBUG) MESSAGE("TInitEditorThread::run() - myInterp = "<<myInterp<<"; myMutex = "<<myMutex);
    myListener->myBanner = myInterp->getbanner().c_str();
    QThread::postEvent(myListener, new QCustomEvent(QAD_PyEditor::INITIALIZE));
    QThread::postEvent(myListener, new QCustomEvent(QAD_PyEditor::PYTHON_OK));
    QThread::postEvent(myListener, new QCustomEvent(QAD_PyEditor::UNSET_CURSOR));
  }

private:
  QMutex* myMutex;
  QMutex* myStudyMutex;
  QAD_PyInterp*& myInterp;
  QAD_PyEditor* myListener;
};


class TExecCommandThread : public QThread
{
public:
  TExecCommandThread(QAD_PyInterp*& theInterp, 
		     QMutex* theStudyMutex, QMutex* theMutex,
		     QAD_PyEditor* theListener): 
    myInterp(theInterp), 
    myMutex(theMutex),
    myStudyMutex(theStudyMutex),
    myListener(theListener), 
    myCommand("")
  {
    //QThread::postEvent(myListener, new QCustomEvent(QAD_PyEditor::SET_WAIT_CURSOR));
  }

  virtual ~TExecCommandThread() {}

  void exec(const char* theCommand){
    myCommand = theCommand;
    start();
  }

protected:
  virtual void run(){
    //QThread::postEvent(myListener, new QCustomEvent(QAD_PyEditor::SET_WAIT_CURSOR));
    int anId = QAD_PyEditor::PYTHON_OK;
    if(myCommand != ""){
      ThreadLock anEditorLock(myMutex,"TExecCommandThread::anEditorLock");
      //ThreadLock aStudyLock(myStudyMutex,"TExecCommandThread::aStudyLock");
      ThreadLock aPyLock = GetPyThreadLock("TExecCommandThread::aPyLock");
      int ret = myInterp->run( myCommand.latin1() );
      if(MYDEBUG) MESSAGE("TExecCommand::run() - myInterp = "<<myInterp<<"; myCommand = '"<<myCommand.latin1()<<"' - "<<ret);
      if(ret < 0)
	anId = QAD_PyEditor::PYTHON_ERROR;
      if(ret > 0)
	anId = QAD_PyEditor::PYTHON_INCOMPLETE;
      myListener->myError = myInterp->getverr().c_str();
      myListener->myOutput = myInterp->getvout().c_str();
    }else{
      myListener->myError = "";
      myListener->myOutput = "";
    }
    QThread::postEvent(myListener, new QCustomEvent(anId));
    QThread::postEvent(myListener, new QCustomEvent(QAD_PyEditor::UNSET_CURSOR));
  }

private:
  QMutex* myMutex;
  QMutex* myStudyMutex;
  QAD_PyInterp*& myInterp;
  QAD_PyEditor* myListener;
  QString myCommand;
};


/*!
    Constructor
*/
QAD_PyEditor::QAD_PyEditor(QAD_PyInterp*& theInterp, QMutex* theMutex,
			   QWidget *theParent, const char* theName): 
  QTextEdit(theParent,theName),
  myStudyMutex(theMutex),
  myInitEditorMutex(new QMutex),
  myExecCommandMutex(new QMutex),
  myInterp(theInterp),
  myInitEditorThread(0),
  myExecCommandThread(0)
{
  QString fntSet = QAD_CONFIG->getSetting("Viewer:ConsoleFont");
  QFont myFont = QAD_Tools::stringToFont( fntSet );
//  QFont myFont("Courier",11);
  setFont(myFont);
  setTextFormat(QTextEdit::PlainText);

  // san - This is necessary for troubleless initialization
  setReadOnly( true );
  viewport()->setCursor( waitCursor );

  myInitEditorThread = new TInitEditorThread(myInterp,myStudyMutex,myInitEditorMutex,this);
  myExecCommandThread = new TExecCommandThread(myInterp,myStudyMutex,myExecCommandMutex,this);

  _currentPrompt = PROMPT;
  setPalette( QAD_Application::getPalette(true) );
  setWordWrap(NoWrap);

  connect(this,SIGNAL(returnPressed()),this,SLOT(handleReturn()) );
}


void QAD_PyEditor::Init()
{
  myInitEditorThread->start();
}


/*!
    Destructor
*/
QAD_PyEditor::~QAD_PyEditor()
{
  if(MYDEBUG) MESSAGE("QAD_PyEditor::~QAD_PyEditor()");
  {
    {
      ThreadLock aLock(myInitEditorMutex,"myInitEditorMutex");
      delete myInitEditorThread;
    }
    delete myInitEditorMutex;
  }
  {
    {
      ThreadLock aLock(myExecCommandMutex,"myExecCommandMutex");
      delete myExecCommandThread;
    }
    delete myExecCommandMutex;
  }
}

/*!
    Called to insert a string s 
*/
void QAD_PyEditor::setText(QString s)
{
  int para=paragraphs()-1;
  int col=paragraphLength(para);
  insertAt(s,para,col);
  int n = paragraphs()-1;  
  setCursorPosition( n, paragraphLength(n)); 
}

/*!
    Called when an handleReturn
*/
void QAD_PyEditor::handleReturn()
{
  int ret;
  int para=paragraphs()-2;

  // NRI : Temporary added
  SALOMEDS::Study_var aStudy = QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();
  
  if ( aStudy->GetProperties()->IsLocked() ) {
    QApplication::restoreOverrideCursor();
    QAD_MessageBox::warn1 ( (QWidget*)QAD_Application::getDesktop(),
			    QObject::tr("WARNING"), 
			    QObject::tr("WRN_STUDY_LOCKED"),
			    QObject::tr("BUT_OK") );

    _currentPrompt = ">>> ";
    setText(_currentPrompt);
    
    return;
  }  
  // NRI

  _buf.append(text(para).remove(0,SIZEPR));
  _buf.truncate( _buf.length() - 1 );
  setReadOnly( true );
  viewport()->setCursor( waitCursor );
  myExecCommandThread->exec(_buf.latin1());
}

/*
   Processes own popup menu
*/
void QAD_PyEditor::mousePressEvent (QMouseEvent * event)
{
  if ( event->button() == RightButton ) {
    QPopupMenu *popup = new QPopupMenu( this );
    QMap<int, int> idMap;

    int para1, col1, para2, col2;
    getSelection(&para1, &col1, &para2, &col2);
    bool allSelected = hasSelectedText() &&
      para1 == 0 && para2 == paragraphs()-1 && col1 == 0 && para2 == paragraphLength(para2);
    int id;
    id = popup->insertItem( tr( "EDIT_COPY_CMD" ) );
    idMap.insert(IdCopy, id);
    id = popup->insertItem( tr( "EDIT_PASTE_CMD" ) );
    idMap.insert(IdPaste, id);
    id = popup->insertItem( tr( "EDIT_CLEAR_CMD" ) );
    idMap.insert(IdClear, id);
    popup->insertSeparator();
    id = popup->insertItem( tr( "EDIT_SELECTALL_CMD" ) );
    idMap.insert(IdSelectAll, id);
    popup->setItemEnabled( idMap[ IdCopy ],  hasSelectedText() );
    popup->setItemEnabled( idMap[ IdPaste ],
			  !isReadOnly() && (bool)QApplication::clipboard()->text().length() );
    popup->setItemEnabled( idMap[ IdSelectAll ],
			  (bool)text().length() && !allSelected );
    
    int r = popup->exec( event->globalPos() );
    delete popup;
    
    if ( r == idMap[ IdCopy ] ) {
      copy();
    }
    else if ( r == idMap[ IdPaste ] ) {
      paste();
    }
    else if ( r == idMap[ IdClear ] ) {
      clear();
      setText(myBanner);
      setText(_currentPrompt);
    }
    else if ( r == idMap[ IdSelectAll ] ) {
      selectAll();
    }
    return;
  }
  else {
    QTextEdit::mousePressEvent(event);
  }
}

/*!
    Called when a Mouse release event
*/
void QAD_PyEditor::mouseReleaseEvent ( QMouseEvent * e )
{
  //  MESSAGE("mouseReleaseEvent");
  int curPara, curCol; // for cursor position
  int endPara, endCol; // for last edited line
  getCursorPosition(&curPara, &curCol);
  endPara = paragraphs() -1;
  if (e->button() != MidButton)
    QTextEdit::mouseReleaseEvent(e);
  else if ((curPara == endPara) && (curCol >= SIZEPR))
    QTextEdit::mouseReleaseEvent(e);
}

/*!
    Called when a drop event (Drag & Drop)
*/
  void QAD_PyEditor::dropEvent (QDropEvent *e)
{
  MESSAGE("dropEvent : not handled");
}

/*!
   Checks, is the string a command line or not.
*/

bool QAD_PyEditor::isCommand( const QString& str) const
{
  if (str.find(_currentPrompt)==0)
    return true;
  return false;
}


/*!
    Called when a keyPress event
*/
void QAD_PyEditor::keyPressEvent( QKeyEvent *e )
{
  int curLine, curCol; // for cursor position
  int endLine, endCol; // for last edited line
  getCursorPosition(&curLine, &curCol);
  endLine = paragraphs() -1;
  //MESSAGE("current position " << curLine << ", " << curCol);
  //MESSAGE("last line " << endLine);
  //MESSAGE(e->key());
  int aKey=e->key();
  int keyRange=0;
  if ((aKey >= Key_Space) && (aKey <= Key_ydiaeresis))
    keyRange = 0;
  else
    keyRange = aKey;

  bool ctrlPressed = ( (e->state() & ControlButton) == ControlButton );
  bool shftPressed = ( (e->state() & ShiftButton) ==  ShiftButton );

  switch (keyRange)
    {
    case 0 :
      {
	if (curLine <endLine || curCol < SIZEPR )
	  moveCursor(QTextEdit::MoveEnd, false);
	QTextEdit::keyPressEvent( e );
	break;
      }
    case Key_Return:
    case Key_Enter:
      {
	if (curLine <endLine)
	  moveCursor(QTextEdit::MoveEnd, false);
	else
	  moveCursor(QTextEdit::MoveLineEnd, false);
	QTextEdit::keyPressEvent( e );
	break;
      }
    case Key_Up:
      {
	// if Cntr+Key_Up event then move cursor up
	if (ctrlPressed) {
	    moveCursor(QTextEdit::MoveUp, false);
        }
	// if Shift+Key_Up event then move cursor up and select the text
	else if ( shftPressed && curLine > 0 ){
	    moveCursor(QTextEdit::MoveUp, true);
	}
	// scroll the commands stack up
	else { 
	   QString histLine = _currentPrompt;
	  if (! _isInHistory)
	    {
	      _isInHistory = true;
	      _currentCommand = text(endLine).remove(0,SIZEPR);
	      _currentCommand.truncate( _currentCommand.length() - 1 );
	      SCRUTE(_currentCommand);
	    }
	  QString previousCommand = myInterp->getPrevious();
	  if (previousCommand.compare(BEGIN_HISTORY_PY) != 0)
	    {
	      removeParagraph(endLine);
	      histLine.append(previousCommand);
	      insertParagraph(histLine, -1);
	    }
	  moveCursor(QTextEdit::MoveEnd, false);
	}
	break;
      }
    case Key_Down:
      {
	// if Cntr+Key_Down event then move cursor down
	if (ctrlPressed) {
	  moveCursor(QTextEdit::MoveDown, false);
	}
	// if Shift+Key_Down event then move cursor down and select the text
	else if ( shftPressed && curLine < endLine ) {
	  moveCursor(QTextEdit::MoveDown, true);
	}
	// scroll the commands stack down
	else {
	QString histLine = _currentPrompt;
	  QString nextCommand = myInterp->getNext();
	  if (nextCommand.compare(TOP_HISTORY_PY) != 0)
	    {
	      removeParagraph(endLine);
	      histLine.append(nextCommand);
	      insertParagraph(histLine, -1);
	    }
	  else
	    if (_isInHistory)
	      {
		_isInHistory = false;
		removeParagraph(endLine);
		histLine.append(_currentCommand);
		insertParagraph(histLine, -1);
	      }
	  moveCursor(QTextEdit::MoveEnd, false);
	}
	break;
      }
    case Key_Left:
      {
	if (!shftPressed && isCommand(text(curLine)) && curCol <= SIZEPR )
	  {
	    setCursorPosition((curLine -1), SIZEPR);
	    moveCursor(QTextEdit::MoveLineEnd, false);
	  }
	else QTextEdit::keyPressEvent( e );
	break;
      }
    case Key_Right:
      {
	if (!shftPressed && isCommand(text(curLine)) 
	    && curCol < SIZEPR) setCursorPosition(curLine, SIZEPR);
	QTextEdit::keyPressEvent( e );
	break;
      }
    case Key_Home: 
      {
	horizontalScrollBar()->setValue( horizontalScrollBar()->minValue() );
	if (isCommand(text(curLine))) {
	  setCursorPosition(curLine, SIZEPR);
	  if ( curCol > SIZEPR && shftPressed )
	    setSelection( curLine, SIZEPR, curLine, curCol );
	  else
	    selectAll( false );
	}
	else moveCursor(QTextEdit::MoveLineStart, shftPressed);
	break;
      }
    case Key_End:
      {
	moveCursor(QTextEdit::MoveLineEnd, shftPressed);
	break;
      }  
    case Key_Backspace :
      {
	if ((curLine == endLine) && (curCol > SIZEPR))
	  QTextEdit::keyPressEvent( e );
	break;
      }
    case Key_Delete :
      {
	if ((curLine == endLine) && (curCol > SIZEPR-1))
	  QTextEdit::keyPressEvent( e );
	break;
      }
    case Key_Insert :
      {
	if ( ctrlPressed )
	  copy();
	else if ( shftPressed ) {
	  moveCursor(QTextEdit::MoveEnd, false);
	  paste();
	}
	else
	  QTextEdit::keyPressEvent( e );
	break;
      }
    }
  if ( e->key() == Key_C && ( e->state() & ControlButton ) )
    {
      _buf.truncate(0);
      setText("\n");
      _currentPrompt = ">>> ";
      setText(_currentPrompt);
    }

  // NRI : DEBUG PAS TERRIBLE //
  if (( e->key() == Key_F3) || 
      ( e->key() == Key_F4) ||
      ( e->key() == Key_Return) ||
      ( e->key() == Key_Escape))
    QAD_Application::getDesktop()->onKeyPress( e );
  // NRI //
}

void QAD_PyEditor::customEvent(QCustomEvent *e)
{
  switch( e->type() ) {
  case PYTHON_OK:
  case PYTHON_ERROR:
    {
      _buf.truncate(0);
      setText(myOutput);
      setText(myError);
      _currentPrompt = ">>> ";
      setText(_currentPrompt);
      break;
    }
  case PYTHON_INCOMPLETE:
    {
      _buf.append("\n");
      _currentPrompt = "... ";
      setText(_currentPrompt);
      break;
    }
  case INITIALIZE:
    {
      setText(myInterp->getbanner().c_str());
      _buf.truncate(0);
      QApplication::restoreOverrideCursor();
      break;
    }  
  case SET_WAIT_CURSOR:
    {
      viewport()->setCursor( waitCursor );
      break;
    }  
  case UNSET_CURSOR:
    {
      viewport()->unsetCursor();
      break;
    }  
  default:
    QTextEdit::customEvent( e );
  }

  setReadOnly( false );
  _isInHistory = false;
}
