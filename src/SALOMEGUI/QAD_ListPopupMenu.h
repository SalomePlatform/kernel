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
//  File   : QAD_ListPopupMenu.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef QAD_LISTPOPUPMENU_H
#define QAD_LISTPOPUPMENU_H

#include <qpopupmenu.h>

class QAD_ListPopupMenu
{ 

public:
  QAD_ListPopupMenu(QPopupMenu*, 
		    const QString & theContext,
		    const QString & theParent,
		    const QString & theObject);
  virtual ~QAD_ListPopupMenu();

  QPopupMenu* getPopup()   {return myPopup;}
  QString &   getContext() {return myContext;}
  QString &   getParent()  {return myParent;}
  QString &   getObject()  {return myObject;}

private:
  QPopupMenu*  myPopup;
  QString      myContext;
  QString      myParent;
  QString      myObject;
};

#endif
