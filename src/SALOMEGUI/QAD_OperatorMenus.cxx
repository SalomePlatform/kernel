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
//  File   : QAD_OperatorMenus.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "QAD_OperatorMenus.h"
#include "QAD_Desktop.h"
using namespace std;

/*!
  Constructor.
*/
QAD_OperatorMenus::QAD_OperatorMenus(QAD_Desktop* theDesktop)
{
  myDesktop=theDesktop;
}

/*!
  Destructor.
*/
QAD_OperatorMenus::~QAD_OperatorMenus()
{
}

/*!
  Builds popup menu with a 'Context', a 'Parent' and an 'Object'.
  Parent is equal to 'Objectbrowser' or 'Viewer'.
*/
QAD_ListPopupMenu* QAD_OperatorMenus::createPopupMenu(const QString & theContext,
						      const QString & theParent,
						      const QString & theObject)
{
  QAD_Menus* aMenus = myDesktop->getActiveMenus();
  QAD_ListPopupMenu* aPopup;

  QList<QAD_ListPopupMenu> PopupList = aMenus->getPopupMenuList();

  for(aPopup=PopupList.first();aPopup!=NULL;aPopup=PopupList.next()){
    if ((aPopup->getContext().compare(theContext)==0) &&
	(aPopup->getParent().compare(theParent)==0) &&
	(aPopup->getObject().compare(theObject)==0)) {
      return aPopup;
    }
  }
  return NULL;
}

QAD_ListPopupMenu* QAD_OperatorMenus::createPopupMenu(int thePopupMenuId)
{
  QAD_Menus* aMenus = myDesktop->getActiveMenus();

  if(!aMenus->getPopupMenuList().isEmpty() && 
     aMenus->getPopupMenuList().at(thePopupMenuId)!=NULL) {
    return aMenus->getPopupMenuList().at(thePopupMenuId);
  } else
    return NULL;
}

void QAD_OperatorMenus::showMenuBar(int theMenuBarId)

{
  QAD_Menus* aMenus = myDesktop->getActiveMenus();
  if(!aMenus->getMenuBarList().isEmpty() && 
     aMenus->getMenuBarList().at(theMenuBarId)!=NULL) {
    aMenus->getMenuBarList().at(theMenuBarId)->show();
  }
}

void QAD_OperatorMenus::hideMenuBar(int theMenuBarId)

{
  QAD_Menus* aMenus;
  aMenus=myDesktop->getActiveMenus();
  if(!aMenus->getMenuBarList().isEmpty() && 
     aMenus->getMenuBarList().at(theMenuBarId)!=NULL) {
    aMenus->getMenuBarList().at(theMenuBarId)->hide();
  }
}

void QAD_OperatorMenus::showToolBar(int theToolBarId)

{
  QAD_Menus* aMenus;
  
  aMenus=myDesktop->getActiveMenus();
  if(!aMenus->getToolBarList().isEmpty() && 
     aMenus->getToolBarList().at(theToolBarId)!=NULL) {
    aMenus->getToolBarList().at(theToolBarId)->show();
  }
}

void QAD_OperatorMenus::hideToolBar(int theToolBarId)

{
  QAD_Menus* aMenus;
  
  aMenus=myDesktop->getActiveMenus();
  if(!aMenus->getToolBarList().isEmpty() && 
     aMenus->getToolBarList().at(theToolBarId)!=NULL) {
    aMenus->getToolBarList().at(theToolBarId)->hide();
  }
}






