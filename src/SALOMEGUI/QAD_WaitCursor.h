//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : QAD_WaitCursor.h
//  Author : Vadim SANDLER
//  Module : SALOME
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
