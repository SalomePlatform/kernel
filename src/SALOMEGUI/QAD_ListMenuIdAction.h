//  File      : QAD_ListMenuIdAction.h
//  Created   : Thu Jun 14 13:55:29 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef QAD_LISTMENUIDACTION_H
#define QAD_LISTMENUIDACTION_H

#include <qpopupmenu.h>
#include <qaction.h>

class QAD_ListMenuIdAction
{ 

public:
  QAD_ListMenuIdAction(QPopupMenu*,int,QAction*);
  virtual ~QAD_ListMenuIdAction();

  int         getId()     {return myId;}
  QPopupMenu* getMenu()   {return myMenu;}
  QAction*    getAction() {return myAction;}

private:
  QPopupMenu*  myMenu;
  int          myId;
  QAction*     myAction;
};

#endif
