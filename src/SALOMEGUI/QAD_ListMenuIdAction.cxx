using namespace std;
//  File      : QAD_ListMenuIdAction.cxx
//  Created   : Thu Jun 14 13:54:44 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#include "QAD_ListMenuIdAction.h"

QAD_ListMenuIdAction::QAD_ListMenuIdAction(QPopupMenu* theMenu,
					   int id,
					   QAction* theAction)
{
  myMenu   = theMenu;
  myId     = id;
  myAction = theAction;
}

QAD_ListMenuIdAction::~QAD_ListMenuIdAction()
{
}
