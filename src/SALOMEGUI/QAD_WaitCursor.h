//  File      : QAD_WaitCursor.h
//  Created   : 02/10/2002
//  Descr     : Use this class to set wait cursor on long operatins

//  Author    : Vadim SANDLER
//  Project   : SALOME Pro
//  Module    : SALOMEGUI 
//  Copyright : Open CASCADE
//  $Header$

#ifndef QAD_WAITCURSOR_H
#define QAD_WAITCURSOR_H

#include <qapplication.h>
#include "QAD.h"

class QAD_EXPORT QAD_WaitCursor {
 public:
// constructor
     QAD_WaitCursor() : myStarted(false) { start(); }
// destructor
     ~QAD_WaitCursor()   { stop();  }

// sets WAIT cursor if not set yet
     void    start()     { if (!myStarted) { QApplication::setOverrideCursor(Qt::waitCursor); myStarted = true; } }
// clears WAIT cursor if set
     void    stop()      { if (myStarted)  { QApplication::restoreOverrideCursor(); myStarted = false; } } 
// returns true if WAIT cursor is active
     bool    isStarted() { return myStarted; }

 private:
     bool  myStarted;
};

#endif
