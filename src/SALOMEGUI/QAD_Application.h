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
//  File   : QAD_Application.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef QAD_Application_H
#define QAD_Application_H

#include "QAD.h"
#include "QAD_Popup.h"
#include "QAD_Study.h"
#include "QAD_Action.h"

#include "SALOME_NamingService.hxx"

// QT Includes
#include <qlist.h>
#include <qpixmap.h>
#include <qstringlist.h>
#include <qtranslator.h>
#include <qtoolbar.h>
#include <qmap.h>

class QAD_Operation;
class QAD_Desktop;
class QAD_EXPORT QAD_Application : public QObject, public QAD_PopupServer
{
  Q_OBJECT
    enum { EditUndoId, EditRedoId, EditCopyId, EditPasteId };

friend class QAD_Desktop;

public:
  QAD_Application ( const QString& format, const QString& description, 
		    const QString& filter); 
  ~QAD_Application();

public:
    static void         run();
    static bool         addToDesktop( QAD_Application* app, SALOME_NamingService* name_service );    
    static QAD_Desktop* getDesktop();    

    static QPalette     getPalette(bool alternative = false);

    const QString&               getApplicationName() const;
    const QPixmap&               getApplicationIcon() const;

    /* studies management */
    const QString&  getStudyFormat() const;     
    const QString&  getStudyExtension() const;
    const QString&  getStudyDescription() const;    

    /* studies */
    bool              isStudyOpened( const QString& studyname );
    QAD_Study*        getStudyByName( const QString& studyname );    
    QAD_Study*        getActiveStudy() const;
    QList<QAD_Study>& getStudies();
    
    QAD_Study*      newStudy();
    QAD_Study*      loadStudy( const QString& fileName);
    QAD_Study*      openStudy( const QString& fileName);
    bool            saveStudy( QAD_Study* study );
    bool            saveAsStudy( QAD_Study* study, const QString& fileName );
    bool            closeStudy( QAD_Study* study, bool permanently = false );
    
    /* customization */
    void            updateFilePopup( QPopupMenu* menu, bool add, int index = -1 );
    void            updateEditPopup( QPopupMenu* menu, bool add, int index = -1 );
    void            updateViewPopup( QPopupMenu* menu, bool add, int index = -1 );
    void            updateHelpPopup( QPopupMenu* menu, bool add, int index = -1 );
    void            updateMainMenu( QMenuBar* menu, bool add, int index = -1 );
    void            updateStatusBar( QStatusBar*, bool add );
    void            updateToolBars( bool add );
  
    /* toolbars */ 
    QToolBar*       createToolBar( int id,
				   const QString& label,  Dock dock = DockTop, 
	  			   bool  dockTop       = true, bool dockBottom   = true, 
				   bool  dockLeft      = true, bool dockRight    = true,
				   bool  dockMinimized = true, bool dockTornOff  = true );
    void            removeToolBar( int id );
    QToolBar*       getToolBar( int id );
    QList<QToolBar> getToolBars();

    virtual void    onStudyActivated( QAD_Study* );
    virtual void    onStudyDeactivated( QAD_Study* );
    virtual void    startOperation( QAD_Operation* op );    

    virtual void    helpAbout();    
    virtual void    helpSearch();
    virtual void    helpContents();

public slots:
    void    updateActions();

signals:
    void            appActivated ( QAD_Application* );
    void            appDeactivated ( QAD_Application* );
    void            desktopCreated();

protected slots:
    virtual bool    onUndo();
    virtual bool    onRedo();
    virtual bool    onCopy();
    virtual bool    onPaste();
    virtual void    onOperationTerminated( bool );

private slots:
    void            onDesktopCreated();

protected:    
    /* menu */
    virtual int     onUpdateMainMenuItems ( QMenuBar*, bool add, int index );    
    virtual int     onUpdateFileActions ( QPopupMenu*, bool add, int index );
    virtual int     onUpdateEditActions ( QPopupMenu*, bool add, int index );
    virtual int     onUpdateViewActions ( QPopupMenu*, bool add, int index );
    virtual int     onUpdateHelpActions ( QPopupMenu*, bool add, int index );          
    virtual void    onUpdateStatusBar( QStatusBar*, bool activate );
    virtual void    onUpdateToolBars( bool activate );

    /* study */
    virtual void    onStudyCreated( QAD_Study* );
    virtual void    onStudyOpened( QAD_Study* );
    virtual void    onStudyClosed( QAD_Study* );
    virtual void    onStudyShown( QAD_Study* );
    virtual void    onActiveStudyChanged( QAD_Study* oldActiveStudy, 
					  QAD_Study* newActiveStudy );
    /* specific initialization ( CAF etc. ) */
    virtual bool    initApp(SALOME_NamingService* name_service) = 0;    
    virtual bool    createDesktop(SALOME_NamingService* name_service); 

    /* undo/redo */
    virtual bool    undo( QAD_Study* study );
    virtual bool    redo( QAD_Study* study );
    /* copy/paste */
    bool            canCopy();
    bool            canPaste();
 
    /* popup */
    void            onCreatePopup();

protected:

    SALOMEDS::StudyManager_var     myStudyMgr;

    QAD_Study*                     myActiveStudy;
    QList<QAD_Study>               myStudies;
    QPixmap                        myIcon;
    QString                        myName; 
    
    QString                        myStudyFormat;
    QString                        myStudyExtension;
    QString                        myStudyDescription;    

#if defined SOLARIS          
/*  SUN C++ v4.1 compiler BUG ? 
    Error when using protected 'desktop' in subclasses. 
    */   
public: 
#endif
    static QAD_Desktop* desktop;        /* the only QAD desktop */    

private:
    void                createActions();

    void                addStudy( QAD_Study* study );
    void                removeStudy( QAD_Study* study );    

private :
    ActionMap            myEditActions;
    ActionMap            myViewActions;
    ActionMap            myHelpActions;
    QMap<int, QToolBar*> myToolBars;
};

#endif

