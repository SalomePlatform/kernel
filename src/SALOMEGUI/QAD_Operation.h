//  File      : QAD_Operation.h
//  Created   : UI team, 22.09.00
//  Author    : UI team
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
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

