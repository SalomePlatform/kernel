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
//  File   : SALOMEGUI.hxx
//  Author : Sergey ANIKIN
//  Module : SALOME
//  $Header$

#ifndef SALOMEGUI_HeaderFile
#define SALOMEGUI_HeaderFile

#include <Standard_Macro.hxx>

#include <qobject.h>
#include <qstring.h>

class QAD_Desktop;
class QAD_StudyFrame;
class SALOME_Event;

class Handle_SALOME_InteractiveObject;

class QKeyEvent;
class QMouseEvent;
class QPopupMenu;
class QString;


class Standard_EXPORT SALOMEGUI : public QObject
{
  Q_OBJECT

public:
  SALOMEGUI( const QString& name = "", QObject* parent = 0 );
  virtual ~SALOMEGUI();

  virtual bool OnGUIEvent        (int theCommandID, QAD_Desktop* parent);
  virtual bool OnKeyPress        (QKeyEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame);
  virtual bool OnMousePress      (QMouseEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame);
  virtual bool OnMouseMove       (QMouseEvent* pe, QAD_Desktop* parent, QAD_StudyFrame* studyFrame);
  virtual bool SetSettings       ( QAD_Desktop* parent );
  virtual bool SetSettings       ( QAD_Desktop* parent, char* compName );
  virtual bool CustomPopup       ( QAD_Desktop* parent, QPopupMenu* popup, const QString & theContext,
                                   const QString & theParent, const QString & theObject );
  virtual void DefinePopup       ( QString & theContext, QString & theParent, QString & theObject );
  virtual bool ActiveStudyChanged( QAD_Desktop* parent );
  virtual void BuildPresentation ( const Handle(SALOME_InteractiveObject)& theIO );
  virtual void SupportedViewType (int* buffer, int bufferSize);
  virtual void Deactivate        ();

signals:
  void SignalDeactivateActiveDialog();
  void SignalCloseAllDialogs       ();  

private:
  QString myName;
};

#endif
