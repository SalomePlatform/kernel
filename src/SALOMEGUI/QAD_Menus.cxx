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
//  File   : QAD_Menus.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "QAD_Menus.h"
using namespace std;

/*!
  Constructor.
*/
QAD_Menus::QAD_Menus(QMainWindow* theParent)
{
  myMainWindow=theParent;
  myToolBarList.setAutoDelete(TRUE);
}

/*!
  Desctructor.
*/
QAD_Menus::~QAD_Menus()
{
  myToolBarList.clear();
  myPopupMenuList.clear();
  myMenuBarList.clear();
  myMenuList.clear();
}

/*!
  Appends a MenuBar
*/
void QAD_Menus::append(QMenuBar* theMenuBar)
{
  myMenuBarList.append(theMenuBar);
}

/*!
  Appends a ToolBar
*/
void QAD_Menus::append(QToolBar* theToolBar)
{
  myToolBarList.append(theToolBar);
}

/*!
  Appends a PopupMenu
*/
void QAD_Menus::append(QAD_ListPopupMenu* thePopupMenu)
{
  myPopupMenuList.append(thePopupMenu);
}

/*!
  Appends a MenuId
*/
void QAD_Menus::append(QAD_ListMenuIdAction* theMenu)
{
  myMenuList.append(theMenu);
}

/*!
  Shows a ToolBar
*/
void QAD_Menus::showToolBar(QToolBar* theToolBar)
{
  if(theToolBar!=NULL && theToolBar->isHidden()) {
    theToolBar->show();
  }
}

/*!
  Shows all ToolBars
*/
void QAD_Menus::showAllToolBars()
{
  QToolBar* aToolBar;
  
  for(aToolBar=myToolBarList.first();aToolBar!=NULL;aToolBar=myToolBarList.next()) {
    if(aToolBar->isHidden()) {
      aToolBar->show();
    }
  }
}

/*!
  Hides a ToolBar
*/
void QAD_Menus::hideToolBar(QToolBar* theToolBar)
{
  if(theToolBar!=NULL && theToolBar->isVisible()) {
    theToolBar->hide();
  }
}

/*!
  Hides all ToolBars
*/
void QAD_Menus::hideAllToolBars()
{
  QToolBar* aToolBar;
  
  for(aToolBar=myToolBarList.first();aToolBar!=NULL;aToolBar=myToolBarList.next()) {
    if(aToolBar->isVisible()) {
      aToolBar->hide();
    }
  }
}

/*!
  Sets current MenuBar
*/
void QAD_Menus::setActiveMenuBar(QMenuBar* theMenuBar)
{
  myActiveMenuBar=theMenuBar;
}

/*!
  Sets current setActivePopupMenu with index \a thePopupMenuIndex
*/
void QAD_Menus::setActivePopupMenu(int thePopupMenuIndex)
{
  myActivePopupMenu=myPopupMenuList.at(thePopupMenuIndex)->getPopup();
}

/*!
  Sets current PopupMenu
*/
void QAD_Menus::setActivePopupMenu(QPopupMenu* thePopupMenu)
{
  myActivePopupMenu=thePopupMenu;
}

/*!
  Sets current ToolBar with index \a theToolBarIndex
*/
void QAD_Menus::setActiveToolBar(int theToolBarIndex)
{
  myActiveToolBar=myToolBarList.at(theToolBarIndex);
}

/*!
  Sets current ToolBar
*/
void QAD_Menus::setActiveToolBar(QToolBar* theToolBar)
{
  myActiveToolBar=theToolBar;
}
