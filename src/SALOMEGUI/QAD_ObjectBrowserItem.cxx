using namespace std;
//  File      : QAD_ObjectBrowserItem.cxx
//  Created   : Thu Jun 14 17:07:42 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#include "QAD_ObjectBrowserItem.h"

#include <qcolor.h>
#include <qlistview.h>
#include <qpixmap.h>
#include <qstringlist.h>
#include <qpainter.h>

/*!
  Constructors
*/
QAD_ObjectBrowserItem::QAD_ObjectBrowserItem( QListView *theParent )
  : QListViewItem(theParent)
{
  myType = BlackItem;
  myCurrent = false;
}
QAD_ObjectBrowserItem::QAD_ObjectBrowserItem( QListView* theParent, QAD_ObjectBrowserItem* after )
  : QListViewItem(theParent, after)
{
  myType = BlackItem;
  myCurrent = false;
}
QAD_ObjectBrowserItem::QAD_ObjectBrowserItem( QAD_ObjectBrowserItem *theParent )
  : QListViewItem(theParent)           
{
  myType = BlackItem;
  myCurrent = false;
}
QAD_ObjectBrowserItem::QAD_ObjectBrowserItem( QAD_ObjectBrowserItem *theParent, QAD_ObjectBrowserItem* after )
  : QListViewItem(theParent, after)
{
  myType = BlackItem;
  myCurrent = false;
}

/*!
  Destructor
*/
QAD_ObjectBrowserItem::~QAD_ObjectBrowserItem()
{
}

/*!
  Sets type
*/
void QAD_ObjectBrowserItem::setType( int type )
{
  myType = type;
  repaint();
}

/*!
  Sets text color
*/
void QAD_ObjectBrowserItem::setTextColor( const QColor& color )
{
  myTextColor = color;
  repaint();
}

/*!
  Sets text highlight color
*/
void QAD_ObjectBrowserItem::setTextHighlightColor(const QColor& color)
{  
  myTextHighlightColor = color;
}

/*!
  Gets item name
*/
QString QAD_ObjectBrowserItem::getName() const
{
  return QListViewItem::text( 0 );
}

/*!
  Sets name
*/
void  QAD_ObjectBrowserItem::setName( const QString& name )
{
  setText( 0, name );
}

/*!
  Gets value
*/
QString QAD_ObjectBrowserItem::getValue() const
{
  return QListViewItem::text( 1 );
}

/*!
  Sets value
*/
void QAD_ObjectBrowserItem::setValue( const QString& value )
{
  setText( 1, value );
}

/*!
  Gets entry
*/
QString QAD_ObjectBrowserItem::getEntry() const
{
  return QListViewItem::text( 2 );
}

/*!
  Sets entry
*/
void QAD_ObjectBrowserItem::setEntry( const QString& entry )
{
  setText( 2, entry );
}

/*!
  Gets IOR
*/
QString QAD_ObjectBrowserItem::getIOR() const
{
  return QListViewItem::text( 3 );
}

/*!
  Sets IOR
*/
  void QAD_ObjectBrowserItem::setIOR( const QString& IOR )
{
  setText( 3, IOR );
}

/*!
  Gets reference
*/
QString QAD_ObjectBrowserItem::getReference() const
{
  return QListViewItem::text( 4 );
}

/*!
  Sets reference
*/
void QAD_ObjectBrowserItem::setReference( const QString& ref )
{
  setText( 4, ref );
}
/*!
  Returns true if this item is current (for UseCase browser - it is bold)
*/
bool QAD_ObjectBrowserItem::isCurrent() const
{
  return myCurrent;
}
/*!
  Sets this item to be current (for UseCase browser - it is bold)
*/
void QAD_ObjectBrowserItem::setCurrent( bool on )
{
  myCurrent = on;
  repaint();
}

/*!
  Paints cell [ redefined ]
*/
void QAD_ObjectBrowserItem::paintCell( QPainter*          thePainter,
				       const QColorGroup& theColorGroup,
				       int                theColumn,
				       int                theWidth,
				       int                theAlignment )
{ 
  QColorGroup aColorGroup(theColorGroup);
  QColor      aColor          = aColorGroup.text();
  QColor      aColorHighlight = aColorGroup.highlight();
  
  switch (myType)
    {
    case 1 :
      {
	aColorGroup.setColor(QColorGroup::Text,       Qt::blue);
	aColorGroup.setColor(QColorGroup::Highlight,  Qt::blue);
	break;
      }
    case 2 :
      {
	aColorGroup.setColor(QColorGroup::Text,       Qt::red);
	aColorGroup.setColor(QColorGroup::Highlight,  Qt::red);
	break;
      }
    default :
      {
	aColorGroup.setColor(QColorGroup::Text,       Qt::black);
	aColorGroup.setColor(QColorGroup::Highlight,  Qt::black);
	break;
      }
    }
      
  if (myTextColor.isValid())
    aColorGroup.setColor(QColorGroup::Text,       myTextColor);
  if (myTextHighlightColor.isValid())
    aColorGroup.setColor(QColorGroup::Highlight,  myTextHighlightColor);
  if (myCurrent) { // UseCase's current entry 
    QFont font = thePainter->font();
    font.setBold(true); 
    thePainter->setFont(font);
  }
  
  QListViewItem::paintCell(thePainter,
                           aColorGroup,
                           theColumn,
                           theWidth,
                           theAlignment);
  aColorGroup.setColor(QColorGroup::Text,       aColor);
  aColorGroup.setColor(QColorGroup::Highlight,  aColorHighlight);
}

/*!
  Returns item's column text
  Overrided to protect access outside
*/
QString QAD_ObjectBrowserItem::text( int column ) const
{
  return QListViewItem::text( column );
}

/*!
  Compares two item to sort list view data by column
*/
int QAD_ObjectBrowserItem::compare( QListViewItem* i, int col, bool ascending ) const
{
  // Additional check is necessary for Entry or Reference entry (2 and 4) columns
  if ( ( col == 2 || col == 4 ) && !key( col, ascending ).isEmpty() && !i->key( col, ascending ).isEmpty() ) { 
    QStringList mylist = QStringList::split( ":", key( col, ascending ),    false );
    QStringList ilist  = QStringList::split( ":", i->key( col, ascending ), false );
    if ( mylist.count() != ilist.count() || mylist.count() == 0 )
      return ( mylist.count() < ilist.count() ) ? -1 : 1;
    for ( int i = 0; i < mylist.count(); i++ ) {
      if ( mylist[i].toInt() < ilist[i].toInt() )
	return -1;
      else if ( mylist[i].toInt() > ilist[i].toInt() )
	return 1;
    }
    return 0;
  }
  return QListViewItem::compare( i, col, ascending );
}
