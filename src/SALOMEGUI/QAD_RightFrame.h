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
//  File   : QAD_RightFrame.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef QAD_RightFrame_H
#define QAD_RightFrame_H

#include "QAD_ViewFrame.h"
#include "QAD_Message.h"
#include "QAD_Splitter.h"

class QMutex;

class QAD_PyEditor;
class QAD_PyInterp;

class QAD_EXPORT QAD_RightFrame : public QAD_Splitter
{
 public:
  QAD_RightFrame(QWidget *theParent, 
		 const char *theTitle, ViewType theTypeView,
		 QAD_PyInterp*& theInterp, QMutex* theMutex);
  ~QAD_RightFrame();

  QAD_ViewFrame*    getViewFrame() const;
  QAD_PyInterp*     get_PyInterp(void);
  QAD_Message*      getMessage() const;
  QAD_PyEditor*     getPyEditor() const;

  void              compressBottom();
  void              compressUp();
  void              unCompressBottom();
  void              unCompressUp();

  void              compressLeft();
  void              compressRight();
  void              unCompressLeft();
  void              unCompressRight();

  bool              isCompressedViewFrame() const;
  bool              isCompressedPython() const;
  bool              isCompressedMessage() const;
    
protected:
  ViewType          myViewType;
  
  QAD_ViewFrame*    myViewFrame; 
  QAD_PyEditor*     myPyEditor;
  QAD_Message*      myMessage;
  QAD_Splitter*     mySplitter;
  QAD_PyInterp*&    myInterp;
};

#endif

