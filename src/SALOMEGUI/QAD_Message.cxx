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
//  File   : QAD_Message.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "QAD_Message.h"
#include "QAD_Application.h"
#include "QAD_Config.h"
#include "QAD_Tools.h"
#include <stdio.h>
#include <qstring.h>
#include <qpopupmenu.h>
#include <qmap.h>
using namespace std;

enum { IdCopy, IdClear, IdSelectAll };

/*!
  \class QAD_Message QAD_Message.h
  \brief Messages window for Study. 
*/

/*!
  Constructor - creates Message Log window
*/
QAD_Message::QAD_Message(QWidget *parent, const char* name)
  : QMultiLineEdit(parent, name)
{
  // setting default title
  myTitle = tr("MESSAGE_LOG_NAME");
  // setting default prompt
  myPrompt = ">>> ";
  // setting font ( from preferences )
  QString fntSet = QAD_CONFIG->getSetting("Viewer:ConsoleFont");
  QFont myFont = QAD_Tools::stringToFont( fntSet );
//  QFont myFont("Courier", 11);
  setFont(myFont);
  setReadOnly(TRUE);
  // appending title
  setMessage(myTitle);
  // setting palette (special)
  setPalette( QAD_Application::getPalette(true) );
  setFocusPolicy( NoFocus );
  // currently no text wrap
  setWordWrap(NoWrap);
}

/*!
    Destructor
*/
QAD_Message::~QAD_Message()
{
}

/*!
  Adds message
*/
void QAD_Message::setMessage(const QString& s)
{
  int line = numLines() > 0 ? numLines()-1 : 0;
  int col  = lineLength(line);
  if (!s.isNull() && s.length() > 0) {
    QString ss = s;
    insertAt(ss, line, col, false);
  }
  append(myPrompt);
}

/*
   Processes own popup manu
*/
void QAD_Message::mousePressEvent(QMouseEvent* event)
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
    id = popup->insertItem( tr( "EDIT_CLEAR_CMD" ) );
    idMap.insert(IdClear, id);
    popup->insertSeparator();
    id = popup->insertItem( tr( "EDIT_SELECTALL_CMD" ) );
    idMap.insert(IdSelectAll, id);
    popup->setItemEnabled( idMap[ IdCopy ], hasMarkedText() );
    popup->setItemEnabled( idMap[ IdSelectAll ], !allSelected );
    popup->setItemEnabled( idMap[ IdClear ], textLine(1)!=myPrompt );
        
    int r = popup->exec( event->globalPos() );
    delete popup;
    
    if ( r == idMap[ IdCopy ] ) {
      copy();
    }
    else if ( r == idMap [ IdClear ] ) {
      clear();
      setMessage(myTitle);
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

/*
   Sets prompt ( default is '>>> ' )
*/
void QAD_Message::setPrompt(const QString& prompt) 
{
  myPrompt = prompt;
  clear();
  setMessage(myTitle);
}
