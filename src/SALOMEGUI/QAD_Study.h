//  File      : QAD_Study.cxx
//  Created   : UI team, 05.09.00
//  Descrip   : Study for QAD-based application

//  Modified  : Mon Dec 03 15:37:21 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2001
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
class QAD_PyInterp;

class QAD_EXPORT QAD_Study : public QObject
{
  Q_OBJECT

public:
  QAD_Study(QAD_Application* app,
	    SALOMEDS::Study_var aStudy,
	    const QString& title);
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
  QString	  getSelection();

  /* python interpreter */
  QAD_PyInterp*	  get_PyInterp(void);

  QString  getNextStudyFrameName();

  /* Child widgets handling (not QAD_StudyFrame) */
  void            addChildWidget( QWidget* child );
  void            removeChildWidget( QWidget* child );

signals:
  void		  docClosing( QAD_Study* );
  void		  docOperationTerminated( bool );

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
  QAD_PyInterp*		    _interp;
};

#endif

