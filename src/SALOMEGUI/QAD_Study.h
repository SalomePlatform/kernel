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
//  File   : QAD_Study.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef QAD_STUDY_H
#define QAD_STUDY_H

#include "QAD_Operation.h"
#include "QAD_StudyFrame.h"

#include "SALOME_InteractiveObject.hxx"

// IDL Headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

// QT Includes
#include <qdict.h>
#include <qobject.h>
#include <qstring.h>

class QAD_Application;

class QMutex;
class QAD_PyInterp;
class TInitStudyThread;
class QAD_ViewFrame;

class QAD_EXPORT QAD_Study : public QObject
{
  Q_OBJECT

public:
  QAD_Study(QAD_Application* app,
	    SALOMEDS::Study_var aStudy,
	    const QString& title);
  virtual void Init(); 
  ~QAD_Study();

  /* Event filter */
  bool eventFilter( QObject* o, QEvent* e);

public:
  /* studyframes management */
  QAD_StudyFrame*		   createStudyFrame(const QString& name, ViewType theViewType = VIEW_TYPE_MAX);
  void				   removeStudyFrame( QAD_StudyFrame* );
  int				   getStudyFramesCount() const;
  const QList<QAD_StudyFrame>&	   getStudyFrames() const;
  QAD_StudyFrame*		   getStudyFrameByName( const QString& name );
  QAD_StudyFrame*		   getFirstStudyFrame();
  QAD_StudyFrame*		   getLastStudyFrame();
  QAD_StudyFrame*		   getStudyFrame( unsigned i );
  QAD_StudyFrame*		   getActiveStudyFrame() const;
  
  bool		   getResult();

  /* document state management */
  bool		   isActive() const;
  bool		   isSaved() const;
  bool		   isModified() const;
  bool		   isReadOnly() const;
  void		   setReadOnly(bool state);
  bool             isLocked();
  const QString&   getTitle() const;
  int              getStudyId();
  const QString&   getPath() const;
  void		   setTitle( const QString& title);
  void             updateCaptions();
  void		   close();
  QAD_Application* getApp() const;

  /* visualization */
  void		   show();

  /* operation management */
  OperationState   getOperationState() const;
  int		   getOperationsCount() const;
  QAD_Operation*   getOperation( unsigned i );
  QAD_Operation*   getFirstOperation();
  QAD_Operation*   getLastOperation();

  void		   abortAllOperations();
  void		   resumeAllOperations();
  void		   suspendAllOperations();

  bool		  onOperationStarted( QAD_Operation* );
  void		  onOperationFinished( QAD_Operation* );
  void		  onOperationSuspended( QAD_Operation* );
  void		  onOperationResumed( QAD_Operation* );
  void		  onOperationAborted( QAD_Operation* );

  /* standard CAF document */
  SALOMEDS::Study_var	   getStudyDocument();

  bool		  undo();
  bool		  redo();
  bool		  canUndo() const;
  bool		  canRedo() const;

  QAD_StudyFrame* newWindow3d(QString name = "", ViewType theViewType = VIEW_TYPE_MAX, bool toShow = true);
  void            showFrame(QAD_StudyFrame* theFrame);

  void		  setMessage(const char* message);

  void		  updateObjBrowser( bool updateSelection = true );
  void		  updateUseCaseBrowser( bool updateSelection = true );
  void		  update3dViewers();

  /*  interactive object management */
  void            highlight( const Handle(SALOME_InteractiveObject)& IObject, 
			     bool highlight, bool immediatly=true );
  void            unHighlightAll();

  void            renameIObject( const Handle(SALOME_InteractiveObject)& IObject,
				 QString newName );
  int             typeIObject( const Handle(SALOME_InteractiveObject)& IObject );

  bool            isInStudy( const Handle(SALOME_InteractiveObject)& IObject );
  bool            isInViewer( const Handle(SALOME_InteractiveObject)& IObject );
  bool		  isInViewer(const char* anEntry,
			     const char* StudyFrameEntry);

  /* selection */
  void		  Selection( QString aSelection );
  QString	  getSelection() const;

  /* python interpreter */
  QAD_PyInterp*	  get_PyInterp(void);

  QString  getNextStudyFrameName();

  /* Child widgets handling (not QAD_StudyFrame) */
  void            addChildWidget( QWidget* child );
  void            removeChildWidget( QWidget* child );

signals:
  void		  docClosing( QAD_Study* );
  void		  docOperationTerminated( bool );
  void            closed();
  void            supervStudyFrameClosing( QAD_ViewFrame* );

public slots:
  void		  onStudyFrameActivated( QAD_StudyFrame* );
  void		  onStudyDeactivated();
  void		  onLastStudyFrameClosing( QAD_StudyFrame* );

private:
  void		  addStudyFrame( QAD_StudyFrame* );
  bool		  initOperation( QAD_Operation* );
  void		  clearOperation( QAD_Operation* );

private:
  SALOMEDS::Study_var	    myStudy;

  QList<QAD_Operation>	    myOperations;
  OperationState	    myOperationState;

  QAD_Application*	    myApp;
  QList<QAD_StudyFrame>	    myStudyFrames;
  QAD_StudyFrame*	    myActiveStudyFrame;
  int			    myStudyFrameCount;

  QList<QWidget>	    myChildWidgets;

  QString		    myPath;
  QString		    myTitle;

  QString		    mySelection;

  bool			    myIsActive;
  bool			    myIsSaved;
  bool			    myIsModified;
  bool			    myIsReadOnly;
  bool			    myResult;

  QMutex*                   myMutex;
  QAD_PyInterp*		    myInterp;
  TInitStudyThread*         myInitStudyThread;
};

#endif

