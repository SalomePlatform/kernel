//  File      : QAD_ObjectBrowserItem.h
//  Created   : Thu Jun 14 17:06:54 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
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






