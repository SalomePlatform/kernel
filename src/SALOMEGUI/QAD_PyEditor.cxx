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

#include "QAD_PyEditor.h" // this include must be first (see PyInterp_base.h)!
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_Config.h"
#include "QAD_Tools.h"
#include "QAD_MessageBox.h"

#include <qapplication.h>
#include <qmap.h>
#include <qclipboard.h>
#include <qthread.h>
#include <qdragobject.h>

// NRI : Temporary added
// IDL Headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
//NRI

#include "utilities.h"
using namespace std;


#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif


enum { IdCopy, IdPaste, IdClear, IdSelectAll };


static QString READY_PROMPT = ">>> ";
static QString DOTS_PROMPT  = "... ";
#define PROMPT_SIZE _currentPrompt.length()

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
      else if(ret > 0)
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

  _currentPrompt = READY_PROMPT;
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
  int para=paragraphs()-2;

  // NRI : Temporary added
  SALOMEDS::Study_var aStudy = QAD_Application::getDesktop()->getActiveStudy()->getStudyDocument();
  
  if ( aStudy->GetProperties()->IsLocked() ) {
    QApplication::restoreOverrideCursor();
    QAD_MessageBox::warn1 ( (QWidget*)QAD_Application::getDesktop(),
			    QObject::tr("WARNING"), 
			    QObject::tr("WRN_STUDY_LOCKED"),
			    QObject::tr("BUT_OK") );

    _currentPrompt = READY_PROMPT;
    setText(_currentPrompt);
    
    return;
  }  
  // NRI

  _buf.append(text(para).remove(0,PROMPT_SIZE));
  _buf.truncate( _buf.length() - 1 );
  setReadOnly( true );
  viewport()->setCursor( waitCursor );
  myExecCommandThread->exec(_buf.latin1());
}

/*
   Processes drop event: paste dragged text
*/
void QAD_PyEditor::contentsDropEvent( QDropEvent* event )
{
  event->acceptAction();
  QString text;
  if ( QTextDrag::decode( event, text ) ) {
    int par, col;
    int endLine = paragraphs() -1;
    col = charAt( event->pos(), &par );
    
    if ( col >= 0 && par >= 0 ) {
      if ( par != endLine || col < PROMPT_SIZE ) {
	par = endLine;
	col = paragraphLength( endLine );
      }
      setCursorPosition( par, col );
      insertAt( text, par, col );
      removeSelection();
    }
  }
}

/*
   Processes middle button release event - paste clipboard's contents
*/
void QAD_PyEditor::contentsMouseReleaseEvent( QMouseEvent* event )
{
  if ( event->button() == LeftButton ) {
    QTextEdit::contentsMouseReleaseEvent(event);
    copy();
  }
  if ( event->button() == MidButton ) {
    if (QApplication::clipboard()->supportsSelection()) {
      int par, col;
      int endLine = paragraphs() -1;
      col = charAt( event->pos(), &par );
      if ( col >= 0 && par >= 0 ) {
	if ( par != endLine || col < PROMPT_SIZE )
	  setCursorPosition( endLine, paragraphLength( endLine ) );
	else
	  setCursorPosition( par, col );
	QApplication::clipboard()->setSelectionMode(TRUE);
	paste();
	QApplication::clipboard()->setSelectionMode(FALSE);
      }
    }
  }
  else {
    QTextEdit::contentsMouseReleaseEvent(event);
  }
}

/*
   Processes own popup menu
*/
void QAD_PyEditor::mousePressEvent (QMouseEvent* event)
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
      _currentPrompt = READY_PROMPT;
      setText(_currentPrompt);
    }
    else if ( r == idMap[ IdSelectAll ] ) {
      selectAll();
    }
  }
  else {
    QTextEdit::mousePressEvent(event);
  }
}

/*!
   Checks, is the string a command line or not.
*/

bool QAD_PyEditor::isCommand( const QString& str) const
{
  // prompt may be '>>> ' or for '... '
  return ( str.find( READY_PROMPT ) == 0 || str.find( DOTS_PROMPT ) == 0 );
}


/*!
    Called when a keyPress event
*/
void QAD_PyEditor::keyPressEvent( QKeyEvent* e )
{
  // get cursor position
  int curLine, curCol;
  getCursorPosition(&curLine, &curCol);

  // get last edited line
  int endLine = paragraphs() -1;

  // get pressed key code
  int aKey = e->key();

  // check if <Ctrl> is pressed
  bool ctrlPressed = e->state() & ControlButton;
  // check if <Shift> is pressed
  bool shftPressed = e->state() & ShiftButton;
  // check if <Alt> is pressed
  bool altPressed = e->state() & AltButton;

  // process <Ctrl>+<C> key-bindings
  if ( aKey == Key_C && ctrlPressed ) {
    _buf.truncate(0);
    setText("\n");
    _currentPrompt = READY_PROMPT;
    setText(_currentPrompt);
    return;
  }

  // check for printed key
  aKey = ( aKey < Key_Space || aKey > Key_ydiaeresis ) ? aKey : 0;

  switch ( aKey ) {
  case 0 :
    // any printed key
    {
      if ( curLine < endLine || curCol < PROMPT_SIZE )
	moveCursor( QTextEdit::MoveEnd, false );
      QTextEdit::keyPressEvent( e );
      break;
    }
  case Key_Return:
  case Key_Enter:
    // <Enter> key
    {
      moveCursor( QTextEdit::MoveEnd, false );
      QTextEdit::keyPressEvent( e );
      break;
    }
  case Key_Up:
    // <Up> arrow key: process as follows:
    // - without <Ctrl>, <Shift> modifiers: previous command in history
    // - with <Ctrl> modifier key pressed:  move cursor one row up without selection
    // - with <Shift> modifier key pressed: move cursor one row up with selection
    // - with <Ctrl>+<Shift> modifier keys pressed: scroll one row up
    {
      if ( ctrlPressed && shftPressed ) {
	scrollBy( 0, -QFontMetrics( font() ).lineSpacing() );
      }
      else if ( shftPressed ) {
	if ( curLine > 0 )
	  moveCursor( QTextEdit::MoveUp, true );
      }
      else if ( ctrlPressed ) {
	moveCursor( QTextEdit::MoveUp, false );
      }
      else { 
	QString histLine = _currentPrompt;
	if ( ! _isInHistory ) {
	  _isInHistory = true;
	  _currentCommand = text( endLine ).remove( 0, PROMPT_SIZE );
	  _currentCommand.truncate( _currentCommand.length() - 1 );
	}
	QString previousCommand = myInterp->getPrevious();
	if ( previousCommand.compare( BEGIN_HISTORY_PY ) != 0 ) {
	  removeParagraph( endLine );
	  histLine.append( previousCommand );
	  insertParagraph( histLine, -1 );
	}
	moveCursor( QTextEdit::MoveEnd, false );
      }
      break;
    }
  case Key_Down:
    // <Down> arrow key: process as follows:
    // - without <Ctrl>, <Shift> modifiers: next command in history
    // - with <Ctrl> modifier key pressed:  move cursor one row down without selection
    // - with <Shift> modifier key pressed: move cursor one row down with selection
    // - with <Ctrl>+<Shift> modifier keys pressed: scroll one row down
    {
      if ( ctrlPressed && shftPressed ) {
	scrollBy( 0, QFontMetrics( font() ).lineSpacing() );
      }
      else if ( shftPressed ) {
	if ( curLine < endLine )
	  moveCursor( QTextEdit::MoveDown, true );
      }
      else if ( ctrlPressed ) {
	moveCursor( QTextEdit::MoveDown, false );
      }
      else { 
	QString histLine = _currentPrompt;
	QString nextCommand = myInterp->getNext();
	if ( nextCommand.compare( TOP_HISTORY_PY ) != 0 ) {
	  removeParagraph( endLine );
	  histLine.append( nextCommand );
	  insertParagraph( histLine, -1 );
	}
	else {
	  if (_isInHistory) {
	    _isInHistory = false;
	    removeParagraph( endLine );
	    histLine.append( _currentCommand );
	    insertParagraph( histLine, -1 );
	  }
	}
	moveCursor( QTextEdit::MoveEnd, false );
      }
      break;
    }
  case Key_Left:
    // <Left> arrow key: process as follows:
    // - without <Ctrl>, <Shift> modifiers: move one symbol left (taking into account prompt)
    // - with <Ctrl> modifier key pressed:  move one word left (taking into account prompt)
    // - with <Shift> modifier key pressed: move one symbol left with selection
    // - with <Ctrl>+<Shift> modifier keys pressed: move one word left with selection
    {
      if ( !shftPressed && isCommand( text( curLine ) ) && curCol <= PROMPT_SIZE ) {
	setCursorPosition( curLine-1, 0 );
	moveCursor( QTextEdit::MoveLineEnd, false );
      }
      else {
	QTextEdit::keyPressEvent( e );
      }
      break;
    }
  case Key_Right:
    // <Right> arrow key: process as follows:
    // - without <Ctrl>, <Shift> modifiers: move one symbol right (taking into account prompt)
    // - with <Ctrl> modifier key pressed:  move one word right (taking into account prompt)
    // - with <Shift> modifier key pressed: move one symbol right with selection
    // - with <Ctrl>+<Shift> modifier keys pressed: move one word right with selection
    {
      if ( !shftPressed ) {
	if ( curCol < paragraphLength( curLine ) ) {
	  if ( isCommand( text( curLine ) ) && curCol < PROMPT_SIZE ) {
	    setCursorPosition( curLine, PROMPT_SIZE );
	    break;
	  }
	}
	else {
	  if ( curLine < endLine && isCommand( text( curLine+1 ) ) ) {
	    setCursorPosition( curLine+1, PROMPT_SIZE );
	    break;
	  }
	}
      }
      QTextEdit::keyPressEvent( e );
      break;
    }
  case Key_PageUp:
    // <PageUp> key: process as follows:
    // - without <Ctrl>, <Shift> modifiers: first command in history
    // - with <Ctrl> modifier key pressed:  move cursor one page up without selection
    // - with <Shift> modifier key pressed: move cursor one page up with selection
    // - with <Ctrl>+<Shift> modifier keys pressed: scroll one page up
    {
      if ( ctrlPressed && shftPressed ) {
	scrollBy( 0, -visibleHeight() );
      }
      else if ( shftPressed ) {
	if ( curLine > 0 )
	  moveCursor( QTextEdit::MovePgUp, true );
      }
      else if ( ctrlPressed ) {
	moveCursor( QTextEdit::MovePgUp, false );
      }
      else { 
	QString histLine = _currentPrompt;
	if ( ! _isInHistory ) {
	  _isInHistory = true;
	  _currentCommand = text( endLine ).remove( 0, PROMPT_SIZE );
	  _currentCommand.truncate( _currentCommand.length() - 1 );
	}
	QString firstCommand = myInterp->getPrevious();
	QString pcmd;
	while ( ( pcmd = QString( myInterp->getPrevious() ) ).compare( BEGIN_HISTORY_PY ) != 0 )
	  firstCommand = pcmd;
	if ( firstCommand.compare( BEGIN_HISTORY_PY ) != 0 ) {
	  removeParagraph( endLine );
	  histLine.append( firstCommand );
	  insertParagraph( histLine, -1 );
	}
	moveCursor( QTextEdit::MoveEnd, false );
      }
      break;
    }
  case Key_PageDown:
    // <PageDown> key: process as follows:
    // - without <Ctrl>, <Shift> modifiers: last command in history
    // - with <Ctrl> modifier key pressed:  move cursor one page down without selection
    // - with <Shift> modifier key pressed: move cursor one page down with selection
    // - with <Ctrl>+<Shift> modifier keys pressed: scroll one page down
    {
      if ( ctrlPressed && shftPressed ) {
	scrollBy( 0, visibleHeight() );
      }
      else if ( shftPressed ) {
	if ( curLine < endLine )
	  moveCursor( QTextEdit::MovePgDown, true );
      }
      else if ( ctrlPressed ) {
	moveCursor( QTextEdit::MovePgDown, false );
      }
      else { 
	if ( _isInHistory ) {
	  QString histLine = _currentPrompt;
	  while ( QString( myInterp->getNext() ).compare( TOP_HISTORY_PY ) != 0 );
	  _isInHistory = false;
	  removeParagraph( endLine );
	  histLine.append( _currentCommand );
	  insertParagraph( histLine, -1 );
	}
	moveCursor( QTextEdit::MoveEnd, false );
      }
      break;
    }
  case Key_Home: 
    // <Home> key: process as follows:
    // - without <Ctrl>, <Shift> modifiers: move cursor to the beginning of the current line without selection
    // - with <Ctrl> modifier key pressed:  move cursor to the very first symbol without selection
    // - with <Shift> modifier key pressed: move cursor to the beginning of the current line with selection
    // - with <Ctrl>+<Shift> modifier keys pressed: move cursor to the very first symbol with selection
    {
      if ( ctrlPressed ) { 
	moveCursor( QTextEdit::MoveHome, shftPressed );
      }
      else {
	if ( isCommand( text( curLine ) ) ) {
	  int ps1, ps2, cs1, cs2;
	  bool hasSelection = hasSelectedText();
	  if ( hasSelection )
	    getSelection( &ps1, &cs1, &ps2, &cs2 );
	  removeSelection();
	  horizontalScrollBar()->setValue( horizontalScrollBar()->minValue() );
	  if ( curCol > PROMPT_SIZE && shftPressed ) 
	    setSelection( curLine, PROMPT_SIZE, curLine, ( hasSelection && ps1 == ps2 && ps1 == curLine && cs2 > PROMPT_SIZE ) ? cs2 : curCol );
	  setCursorPosition( curLine, PROMPT_SIZE );
	}
	else {
	  moveCursor( QTextEdit::MoveLineStart, shftPressed );
	}
      }
      break;
    }
  case Key_End:
    // <End> key: process as follows:
    // - without <Ctrl>, <Shift> modifiers: move cursor to the end of the current line without selection
    // - with <Ctrl> modifier key pressed:  move cursor to the very last symbol without selection
    // - with <Shift> modifier key pressed: move cursor to the end of the current line with selection
    // - with <Ctrl>+<Shift> modifier keys pressed: move cursor to the very last symbol with selection
    {
      if ( ctrlPressed ) { 
	moveCursor( QTextEdit::MoveEnd, shftPressed );
      }
      else {
	moveCursor( QTextEdit::MoveLineEnd, shftPressed );
      }
      break;
    }  
  case Key_Backspace :
    // <Backspace> key: process as follows
    // - without any modifiers : delete symbol before the cursor / selection (taking into account prompt)
    // - with <Ctrl> modifier key pressed: delete previous word
    // works only for last (command) line
    {
      if ( curLine == endLine && ( curCol > PROMPT_SIZE || curCol >= PROMPT_SIZE && hasSelectedText() ) ) {
	if ( ctrlPressed && !hasSelectedText() ) {
	  QString txt = text( curLine );
	  int ind = curCol-1;
	  while ( ind > 0 && txt[ ind ] == ' ' ) ind--;
	  ind = txt.findRev( ' ', ind ) + 1;
	  if ( ind > PROMPT_SIZE-1 ) {
	    setSelection( curLine, ind, curLine, curCol );
	    removeSelectedText();
	  }
	  else {
	    QTextEdit::keyPressEvent( e );
	  }
	}
	else {
	  QTextEdit::keyPressEvent( e );
	}
      }
      break;
    }
  case Key_Delete :
    // <Delete> key: process as follows
    // - without any modifiers : delete symbol after the cursor / selection (taking into account prompt)
    // - with <Ctrl> modifier key pressed: delete next word
    // works only for last (command) line
    {
      if ( curLine == endLine && curCol > PROMPT_SIZE-1 ) {
	if ( ctrlPressed && !hasSelectedText() ) {
	  QString txt = text( curLine );
	  int ind = curCol;
	  while ( ind < txt.length()-1 && txt[ ind ] == ' ' ) ind++;
	  ind = txt.find( ' ', ind );
	  while ( ind < txt.length()-1 && txt[ ind ] == ' ' ) ind++;
	  if ( ind > PROMPT_SIZE-1 ) {
	    setSelection( curLine, curCol, curLine, ind );
	    removeSelectedText();
	  }
	  else {
	    QTextEdit::keyPressEvent( e );
	  }
	}
	else {
	  QTextEdit::keyPressEvent( e );
	}
      }
      break;
    }
  case Key_Insert :
    // <Insert> key: process as follows
    // - with <Ctrl> modifier key pressed:  copy()
    // - with <Shift> modifier key pressed: paste() to the command line
    {
      if ( ctrlPressed ) {
	copy();
      }
      else if ( shftPressed ) {
	if ( curLine != endLine || curCol < PROMPT_SIZE )
	  moveCursor( QTextEdit::MoveEnd, false );
	paste();
      }
      else
	QTextEdit::keyPressEvent( e );
      break;
    }
  }
  // NRI : DEBUG PAS TERRIBLE //
  if (( e->key() == Key_F3) || 
      ( e->key() == Key_F4) ||
      ( e->key() == Key_Return) ||
      ( e->key() == Key_Escape))
    QAD_Application::getDesktop()->onKeyPress( e );
  // NRI //
}

void QAD_PyEditor::customEvent(QCustomEvent* e)
{
  switch( e->type() ) {
  case PYTHON_OK:
  case PYTHON_ERROR:
    {
      _buf.truncate(0);
      setText(myOutput);
      setText(myError);
      _currentPrompt = READY_PROMPT;
      setText(_currentPrompt);
      break;
    }
  case PYTHON_INCOMPLETE:
    {
      _buf.append("\n");
      _currentPrompt = DOTS_PROMPT;
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
