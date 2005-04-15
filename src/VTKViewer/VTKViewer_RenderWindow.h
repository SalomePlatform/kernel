//  SALOME VTKViewer : build VTK viewer into Salome desktop
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
//  File   : VTKViewer_RenderWindow.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef VTKViewer_RenderWindow_h
#define VTKViewer_RenderWindow_h

#include <stdlib.h>
#include <vtkRenderWindow.h>
//#include <qgl.h>
#include <vtkRenderWindow.h>

#include "QAD_Popup.h"

class VTK_EXPORT VTKViewer_RenderWindow : 
public QWidget, /*virtual public vtkRenderWindow, */
  public QAD_PopupClientServer
{
  Q_OBJECT 
    public:
  /* popup management */
  void 	                  onCreatePopup();

  const char *GetClassName() {return "VTKViewer_RenderWindow";};

  protected slots:
    void onChangeBackgroundColor();

 public:
  VTKViewer_RenderWindow(QWidget *parent, const char *name);
  ~VTKViewer_RenderWindow() ;

  virtual void mouseMoveEvent( QMouseEvent * ) ;
  virtual void mousePressEvent( QMouseEvent * ) ;
  virtual void mouseReleaseEvent( QMouseEvent * ) ;
  virtual void keyPressEvent( QKeyEvent * ) ;

  vtkRenderWindow* getRenderWindow() { return myRW; }

 protected:
  virtual void paintEvent (QPaintEvent* theEvent);
  virtual void resizeEvent (QResizeEvent* theEvent);


 signals:
  void MouseMove(const QMouseEvent *event) ;
  void LeftButtonPressed(const QMouseEvent *event) ;
  void LeftButtonReleased(const QMouseEvent *event) ;
  void MiddleButtonPressed(const QMouseEvent *event) ;
  void MiddleButtonReleased(const QMouseEvent *event) ;
  void RightButtonPressed(const QMouseEvent *event) ;
  void RightButtonReleased(const QMouseEvent *event) ;
  void ButtonPressed(const QMouseEvent *event) ;
  void ButtonReleased(const QMouseEvent *event) ;
  void KeyPressed(QKeyEvent *event) ;
  void DumpView();

 protected:
  vtkRenderWindow* myRW;
};

#endif
