//  File      : QAD_OperatorMenus.h
//  Created   : Thu Jun 14 14:38:48 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef QAD_OPERATOR_MENUS_H
#define QAD_OPERATOR_MENUS_H

#include "QAD_ListPopupMenu.h"

class QAD_Desktop;

class QAD_OperatorMenus
{
public:
  QAD_OperatorMenus(QAD_Desktop *);
  virtual ~QAD_OperatorMenus();
  
  QAD_Desktop* myDesktop;
  QAD_ListPopupMenu*  createPopupMenu(int);
  QAD_ListPopupMenu*  createPopupMenu(const QString & theContext,
				      const QString & theParent,
				      const QString & theObject);

  void showMenuBar(int);
  void hideMenuBar(int);
  void showToolBar(int);
  void hideToolBar(int);
};

#endif

