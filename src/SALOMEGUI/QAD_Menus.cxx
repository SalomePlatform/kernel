using namespace std;
//  File      : QAD_Menus.cxx
//  Created   : Thu Jun 14 13:56:16 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$


#include "QAD_Menus.h"

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
