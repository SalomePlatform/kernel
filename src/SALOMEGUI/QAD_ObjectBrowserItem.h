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
//  File   : QAD_ObjectBrowserItem.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef QAD_OBJECTBROWSERITEM_H
#define QAD_OBJECTBROWSERITEM_H

#include <qobject.h>
#include <qlistview.h>

class QPainter;
class QColorGroup;

class QAD_ObjectBrowserItem : public QListViewItem
{
  friend class QListView;

public:
  QAD_ObjectBrowserItem( QListView* theParent );
  QAD_ObjectBrowserItem( QListView* theParent, QAD_ObjectBrowserItem* after );
  QAD_ObjectBrowserItem( QAD_ObjectBrowserItem* theParent );
  QAD_ObjectBrowserItem( QAD_ObjectBrowserItem* theParent, QAD_ObjectBrowserItem* after );
  virtual ~QAD_ObjectBrowserItem();
  
  void    setType( int type );
  void    setTextColor( const QColor& color );
  void    setTextHighlightColor( const QColor& color );

  QString getName() const;
  void    setName( const QString& name );
  QString getValue() const;
  void    setValue( const QString& value );
  QString getEntry() const;
  void    setEntry( const QString& entry );
  QString getIOR() const;
  void    setIOR( const QString& IOR );
  QString getReference() const;
  void    setReference( const QString& ref );
  
  bool    isCurrent() const;
  void    setCurrent( bool on );

  int     compare( QListViewItem* i, int col, bool ascending ) const;

protected:
  enum { BlackItem = 0, BlueItem, RedItem };
  int      myType;
  QColor   myTextColor;
  QColor   myTextHighlightColor;
  bool     myCurrent;

  void    paintCell( QPainter*, const QColorGroup &, int, int, int );
  QString text( int column ) const;
};

#endif






