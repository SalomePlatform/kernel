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
//  File   : QAD_Menus.h
//  Author : Nicolas REJNERI
//  Module : SALOME
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
