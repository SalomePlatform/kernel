//  File      : QAD_Menus.h
//  Created   : Thu Jun 14 13:57:00 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef QAD_MENUS_H
#define QAD_MENUS_H

#include "QAD_ListMenuIdAction.h"
#include "QAD_ListPopupMenu.h"

#include <qlist.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qpopupmenu.h>

class QAD_MainWindow;

class QAD_Menus
{ 

public:
  QAD_Menus(QMainWindow*);
  virtual ~QAD_Menus();

  QMenuBar*   myActiveMenuBar;
  QToolBar*   myActiveToolBar;
  QStatusBar* myActiveStatusBar;
  QPopupMenu* myActivePopupMenu;
  
  QMainWindow*                 getMainWindow()      {return myMainWindow;}
  QList<QMenuBar>              getMenuBarList()     {return myMenuBarList;}
  QList<QToolBar>              getToolBarList()     {return myToolBarList;}
  QPopupMenu*                  getActivePopupMenu() {return myActivePopupMenu;}
  QList<QAD_ListMenuIdAction>  getMenuList()        {return myMenuList;}
  QList<QAD_ListPopupMenu>     getPopupMenuList()   {return myPopupMenuList;}

  void              setActiveMenuBar(QMenuBar*);
  void              setActivePopupMenu(int);
  void              setActivePopupMenu(QPopupMenu*);
  void              setActiveToolBar(int);
  void              setActiveToolBar(QToolBar*);
  
  void              append(QMenuBar*);
  void              append(QToolBar*);
  void              append(QAD_ListPopupMenu*);
  void              append(QAD_ListMenuIdAction*);
  void              clearToolBar() {myToolBarList.clear();}
  void              clearPopupMenu() {myPopupMenuList.clear();}
  void              showToolBar(QToolBar*);
  void              showAllToolBars();
  void              hideToolBar(QToolBar*);
  void              hideAllToolBars();

protected:
  QMainWindow*                myMainWindow;
  QList<QMenuBar>             myMenuBarList;
  QList<QToolBar>             myToolBarList;
  QList<QAD_ListMenuIdAction> myMenuList;
  QList<QAD_ListPopupMenu>    myPopupMenuList;

};

#endif
