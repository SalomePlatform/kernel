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
//#include "QAD_RightFrame.h"
using namespace std;

#include <qapplication.h>
#include <qmap.h>
#include <qclipboard.h>

// NRI : Temporary added
// IDL Headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
//NRI

#define SIZEPR 4
enum { IdCopy, IdPaste, IdClear, IdSelectAll };

/*!
    Constructor
*/
QAD_PyEditor::QAD_PyEditor(QAD_PyInterp* interp, 
			   QWidget *parent, const char *name)
  : QMultiLineEdit(parent,name)
{
  QString fntSet = QAD_CONFIG->getSetting("Viewer:ConsoleFont");
  QFont myFont = QAD_Tools::stringToFont( fntSet );
//  QFont myFont("Courier",11);
  setFont(myFont);
  _interp = interp;
  string banner = _interp->getbanner();
  setText(banner.c_str());
  _isInHistory = false;
  _currentPrompt = ">>> ";
  // put error messages of interpreter if they exist.
  _buf.truncate(0);
  setText(_interp->getverr());
  setText(_currentPrompt);
  setPalette( QAD_Application::getPalette(true) );
  setWordWrap(NoWrap);
  connect(this,SIGNAL(returnPressed()),this,SLOT(handleReturn()) );
}

/*!
    Destructor
*/
QAD_PyEditor::~QAD_PyEditor()
{
}

/*!
    Called to insert a string s 
*/
void QAD_PyEditor::setText(QString s)
{
//   MESSAGE("setText");
  int line=numLines()-1;
  int col=lineLength(line);
  insertAt(s,line,col);
  int n = numLines()-1;  
  setCursorPosition( n, textLine(n).length()); 
}

/*!
    Called when an handleReturn
*/
void QAD_PyEditor::handleReturn()
{
  QApplication::setOverrideCursor( Qt::waitCursor );
  int ret;
  int line=numLines()-2;

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

  _buf.append(textLine(line).remove(0,SIZEPR));
  ret = _interp->run(_buf);
  if(ret <= 0)
    {
      _buf.truncate(0);
      setText(_interp->getvout());
      setText(_interp->getverr());
      _currentPrompt = ">>> ";
      setText(_currentPrompt);
    }
  if(ret == 1)
    {
      _buf.append("\n");
      _currentPrompt = "... ";
      setText(_currentPrompt);
    }
  _isInHistory = false;
  QApplication::restoreOverrideCursor();
}

/*
   Processes own popup menu
*/
void QAD_PyEditor::mousePressEvent (QMouseEvent * event)
{
  if ( event->button() == RightButton ) {
    QPopupMenu *popup = new QPopupMenu( this );
    QMap<int, int> idMap;

    int line1, col1, line2, col2;
    getMarkedRegion(&line1, &col1, &line2, &col2);
    bool allSelected = getMarkedRegion(&line1, &col1, &line2, &col2) &&
      line1 == 0 && line2 == numLines()-1 && col1 == 0 && col2 == lineLength(line2);
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
    popup->setItemEnabled( idMap[ IdCopy ],  hasMarkedText() );
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
      string banner = _interp->getbanner();
      setText(banner.c_str());
      setText(_currentPrompt);
    }
    else if ( r == idMap[ IdSelectAll ] ) {
      selectAll();
    }
    return;
  }
  else {
    QMultiLineEdit::mousePressEvent(event);
  }
}

/*!
    Called when a Mouse release event
*/
void QAD_PyEditor::mouseReleaseEvent ( QMouseEvent * e )
{
  //  MESSAGE("mouseReleaseEvent");
  int curLine, curCol; // for cursor position
  int endLine, endCol; // for last edited line
  getCursorPosition(&curLine, &curCol);
  endLine = numLines() -1;
  if (e->button() != MidButton)
    QMultiLineEdit::mouseReleaseEvent(e);
  else if ((curLine == endLine) && (curCol >= SIZEPR))
    QMultiLineEdit::mouseReleaseEvent(e);
}

/*!
    Called when a drop event (Drag & Drop)
*/
  void QAD_PyEditor::dropEvent (QDropEvent *e)
{
  INFOS("dropEvent : not handled");
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
  endLine = numLines() -1;
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
	if (curLine <endLine)
	  {
	    setCursorPosition(endLine, SIZEPR);
	    end();
	  }
	QMultiLineEdit::keyPressEvent( e );
	break;
      }
    case Key_Return:
    case Key_Enter:
      {
	if (curLine <endLine)
	  {
	    setCursorPosition(endLine, SIZEPR);
	  }
	end();
	QMultiLineEdit::keyPressEvent( e );
	break;
      }
    case Key_Up:
      {
	// if Cntr+Key_Up event then move cursor up
	if (ctrlPressed) {
	  QMultiLineEdit::cursorUp( );
        }
	// if Shift+Key_Up event then move cursor up and select the text
	else if ( shftPressed && curLine > 0 ){
	   setCursorPosition(curLine-1, curCol, true);
	}
	// scroll the commands stack up
	else { 
	   QString histLine = _currentPrompt;
	  if (! _isInHistory)
	    {
	      _isInHistory = true;
	      _currentCommand = textLine(endLine).remove(0,SIZEPR);
	      SCRUTE(_currentCommand);
	    }
	  QString previousCommand = _interp->getPrevious();
	  if (previousCommand.compare(BEGIN_HISTORY_PY) != 0)
	    {
	      removeLine(endLine);
	      histLine.append(previousCommand);
	      insertLine(histLine);
	    }
	  endLine = numLines() -1;
	  setCursorPosition(endLine, lineLength(endLine));
	}
	break;
      }
    case Key_Down:
      {
	// if Cntr+Key_Down event then move cursor down
	if (ctrlPressed) {
	  QMultiLineEdit::cursorDown( );
	}
	// if Shift+Key_Down event then move cursor down and select the text
	else if ( shftPressed && curLine < endLine ) {
	   setCursorPosition(curLine+1, curCol, true);
	}
	// scroll the commands stack down
	else {
	QString histLine = _currentPrompt;
	  QString nextCommand = _interp->getNext();
	  if (nextCommand.compare(TOP_HISTORY_PY) != 0)
	    {
	      removeLine(endLine);
	      histLine.append(nextCommand);
	      insertLine(histLine);
	    }
	  else
	    if (_isInHistory)
	      {
		_isInHistory = false;
		removeLine(endLine);
		histLine.append(_currentCommand);
		insertLine(histLine);
	      }
	  endLine = numLines() -1;
	  setCursorPosition(endLine, lineLength(endLine));
	}
	break;
      }
    case Key_Left:
      {
	if (!shftPressed && isCommand(textLine(curLine)) && curCol <= SIZEPR )
	  {
	    setCursorPosition((curLine -1), SIZEPR);
	    end();
	  }
	else QMultiLineEdit::keyPressEvent( e );
	break;
      }
    case Key_Right:
      {
	if (!shftPressed && isCommand(textLine(curLine)) 
	    && curCol < SIZEPR) setCursorPosition(curLine, SIZEPR-1);
	QMultiLineEdit::keyPressEvent( e );
	break;
      }
    case Key_Home: 
      {
	if (isCommand(textLine(curLine)) && curCol <= SIZEPR)
	  setCursorPosition(curLine, SIZEPR, shftPressed);
	else setCursorPosition(curLine, 0, shftPressed);
	break;
      }
    case Key_End:
      {
	setCursorPosition(curLine, textLine(curLine).length(), shftPressed);
	break;
      }  
    case Key_Backspace :
      {
	if ((curLine == endLine) && (curCol > SIZEPR))
	  QMultiLineEdit::keyPressEvent( e );
	break;
      }
    case Key_Delete :
      {
	if ((curLine == endLine) && (curCol > SIZEPR-1))
	  QMultiLineEdit::keyPressEvent( e );
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
