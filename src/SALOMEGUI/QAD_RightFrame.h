//  File      : QAD_RightFrame.h
//  Created   : Thu Jun 14 14:52:28 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef QAD_RightFrame_H
#define QAD_RightFrame_H

#include "QAD_ViewFrame.h"
#include "QAD_Message.h"
#include "QAD_PyEditor.h"
#include "QAD_Splitter.h"
#include "QAD_PyInterp.h"

class QAD_EXPORT QAD_RightFrame : public QAD_Splitter
{
 public:
  QAD_RightFrame(QWidget *parent, const char *name,
		 QAD_PyInterp* interp, ViewType vt);
  ~QAD_RightFrame();

  QAD_ViewFrame*    getViewFrame() const;
  QAD_PyInterp*     get_PyInterp(void);
  QAD_Message*      getMessage() const;
  QAD_PyEditor*     getPyEditor() const;

  void              compressBottom();
  void              compressUp();
  void              unCompressBottom();
  void              unCompressUp();
  
protected:
  ViewType          myViewType;
  
  QAD_ViewFrame*    myViewFrame; 
  QAD_PyEditor*     myPyEditor;
  QAD_Message*      myMessage;
  QAD_Splitter*     vsplitter;
  QAD_PyInterp*     _interp;
};

#endif

