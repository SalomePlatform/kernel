//=============================================================================
// File      : SALOME_PYQT_GUI.hxx
// Created   : mer jun  4 17:17:20 UTC 2003
// Author    : Paul RASCLE, EDF 
// Project   : SALOME
// Copyright : EDF 2003
// $Header$
//=============================================================================

#ifndef _SALOME_PYQT_GUI_HXX_
#define _SALOME_PYQT_GUI_HXX_

#include "QAD_Desktop.h"

class SALOME_PYQT_GUI: public QObject
{
  Q_OBJECT
    
private:
  
public:
  
  static bool OnGUIEvent   (int theCommandID, QAD_Desktop* parent);
  static bool OnMousePress (QMouseEvent* pe, QAD_Desktop* parent,
			    QAD_StudyFrame* studyFrame);
  static bool OnMouseMove  (QMouseEvent* pe, QAD_Desktop* parent,
			    QAD_StudyFrame* studyFrame);
  static bool OnKeyPress   (QKeyEvent* pe, QAD_Desktop* parent,
			    QAD_StudyFrame* studyFrame);
  static bool SetSettings  (QAD_Desktop* parent, char* moduleName);
  static bool CustomPopup  (QAD_Desktop* parent, QPopupMenu* popup,
			    const QString & theContext,
			    const QString & theParent,
			    const QString & theObject);
  static void DefinePopup  (QString & theContext, QString & theParent,
			    QString & theObject) ;
  static void ActiveStudyChanged (QAD_Desktop* parent);
  
protected:
  
};

#endif
