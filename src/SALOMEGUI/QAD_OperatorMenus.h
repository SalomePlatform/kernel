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
//  File   : QAD_OperatorMenus.h
//  Author : Nicolas REJNERI
//  Module : SALOME
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

