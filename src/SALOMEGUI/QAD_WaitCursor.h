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
     QAD_WaitCursor() : myStarted( false ), myDepth( 1 ) { start(); }
// destructor
     ~QAD_WaitCursor()   { if ( !myStarted ) { myDepth--; start(); } else { QApplication::restoreOverrideCursor(); } }

// sets WAIT cursor if not set yet
     void    start()     { if ( !myStarted ) { while( myDepth-- ) QApplication::setOverrideCursor( Qt::waitCursor ); myStarted = true; } myDepth = 0; }
// clears WAIT cursor if set
     void    stop()      { if ( myStarted )  { while( QApplication::overrideCursor() ) { QApplication::restoreOverrideCursor(); myDepth++; } myStarted = false; } } 
// returns true if WAIT cursor is active
     bool    isStarted() { return myStarted; }

 private:
     bool  myStarted;
     int   myDepth;
};

#endif
