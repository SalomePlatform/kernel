//  File      : QAD_ListPopupMenu.h
//  Created   : Thu Jun 14 13:52:42 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef QAD_LISTPOPUPMENU_H
#define QAD_LISTPOPUPMENU_H

#include <qpopupmenu.h>

class QAD_ListPopupMenu
{ 

public:
  QAD_ListPopupMenu(QPopupMenu*, 
		    const QString & theContext,
		    const QString & theParent,
		    const QString & theObject);
  virtual ~QAD_ListPopupMenu();

  QPopupMenu* getPopup()   {return myPopup;}
  QString &   getContext() {return myContext;}
  QString &   getParent()  {return myParent;}
  QString &   getObject()  {return myObject;}

private:
  QPopupMenu*  myPopup;
  QString      myContext;
  QString      myParent;
  QString      myObject;
};

#endif
