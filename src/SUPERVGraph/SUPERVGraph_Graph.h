//  SALOME SUPERVGraph : build Supervisor viewer into desktop
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
//  File   : SUPERVGraph_Graph.h
//  Author : Francis KLOSS
//  Module : SALOME

#ifndef SUPERVGraph_Graph_HXX
#define SUPERVGraph_Graph_HXX

#include "QAD.h"
#include "QAD_Popup.h"

#include <qscrollview.h>
#include <qpopupmenu.h>

class QAD_EXPORT SUPERVGraph_Graph: public QScrollView, public QAD_PopupClientServer
{
  Q_OBJECT

public:
  SUPERVGraph_Graph(QWidget* parent);
  ~SUPERVGraph_Graph();

  /* popup management */
  void onCreatePopup();

  void sync();
  void contentsMousePressEvent(QMouseEvent* e);
  void contentsMouseReleaseEvent(QMouseEvent* e);
  void contentsMouseMoveEvent(QMouseEvent* e);
  
  QPoint getPoint();
  
public slots:
  void Draw(int x, int y);
  
private slots:
  void addPoint();
  void deleteLink();
  void showInformation();
  void changeInformation();

protected slots:
  void onChangeBackgroundColor();

protected:
  bool eventFilter(QObject* object, QEvent* event);
  
private:
//    SUPERVGraph_Point* findPoint(int x, int y);
  float distance(int xp, int yp, int xl, int yl, int xn, int yn);
  
//    SUPERVGraph_Main* main;
  QPoint          where;
  QPopupMenu*     popup;
  int             pci;
  int             pan;
  int             pap;
  int             pdl;
  int             pif;
};

#endif
