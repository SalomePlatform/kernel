using namespace std;
//  File      : QAD_ListPopupMenu.cxx
//  Created   : Thu Jun 14 13:53:41 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2001
//  $Header$


#include "QAD_ListPopupMenu.h"

/*!
  Constructor.
*/
QAD_ListPopupMenu::QAD_ListPopupMenu(QPopupMenu* thePopup,
				     const QString & theContext,
				     const QString & theParent,
				     const QString & theObject)
{
  myPopup   = thePopup;
  myContext = theContext;
  myParent  = theParent;
  myObject  = theObject;
}

/*!
  Destructor.
*/
QAD_ListPopupMenu::~QAD_ListPopupMenu()
{
}
