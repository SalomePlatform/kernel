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
//  File   : QAD_Action.h
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#ifndef QAD_ACTION_H
#define QAD_ACTION_H

#include "qactionP.h"
#include <qmap.h>

class ActionMap {
 public:
     ActionMap() {}
     ~ActionMap() 
     { 
       QMapIterator<int, QActionP*> it;
       for ( it = myMap.begin(); it != myMap.end(); it++ ) delete (QActionP*)it.data();
       myMap.clear();
     }
     bool isEmpty()                                  { return myMap.isEmpty(); }
     void insert( const int id, QActionP* action )   { myMap[id] = action;     }
     QActionP* at( const int id )                    { return myMap[id];       }
     void clear()                                             { myMap.clear();          }
     QActionP*& operator[] (const int id)            { return myMap[id];       }
     bool hasAction( const int id )                  { return myMap.contains(id); }

 private:
     QMap<int, QActionP*> myMap;

};

class ToggleAction : public QActionP {
  Q_OBJECT
 public:
  ToggleAction( const QString& text,     const QString& menuText, 
	        QKeySequence   accel,    QObject*       parent, 
	        const char*    name = 0, bool           toggle = FALSE)
    : QActionP( text, menuText, accel, parent, name, toggle ) 
      {
	connect( this, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
      }
 private slots:
  void onToggled( bool on)
    {
      on ? emit( toggledOn() ) : emit( toggledOff() );
    }
  
 signals:
  void toggledOn();
  void toggledOff();
};

#endif
