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
//  File   : QAD_Operation.h
//  Author : UI team
//  Module : SALOME
//  $Header$

#ifndef QAD_Operation_H
#define QAD_Operation_H

#include "QAD.h"
#include <qobject.h>

class QAD_Study;
class QAD_Application;

enum OperationState { 
  Undef, Started, Finished, Suspended, Resumed, 
  Aborted
};

class QAD_EXPORT QAD_Operation : public QObject
{
  Q_OBJECT

public:
  QAD_Operation( QAD_Study* aDoc );
  ~QAD_Operation();

public:
  QAD_Study*       getDoc() const;
  QAD_Application* getApp() const;
  
  bool             start();
  void             finish();
  void             suspend();
  void             resume();
  void             abort();	
    
  virtual bool     changesData() const;
  virtual bool     supportCancelByEscape() const;
  virtual bool     compatibleWith( QAD_Operation* ) const;

protected:
  /* customization */
  virtual void    onStartOperation();        
  virtual void    onFinishOperation();
  virtual void    onSuspendOperation();
  virtual void    onResumeOperation();
  virtual void    onAbortOperation();
    
protected:
  QAD_Study*      myDoc;
};

#endif

