using namespace std;
//  File      : QAD_OperatorMenus.cxx
//  Created   : Thu Jun 14 14:37:17 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#include "QAD_OperatorMenus.h"
#include "QAD_Desktop.h"

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






