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
//  File   : QAD_Desktop.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef QAD_DESKTOP_H
#define QAD_DESKTOP_H

#include "QAD.h"
#include "QAD_ResourceMgr.h"
#include "QAD_Application.h"
#include "QAD_OperatorMenus.h"
#include "QAD_Menus.h"
#include "QAD_HelpWindow.h"
#include "QAD_Action.h"

#include "SALOME_NamingService.hxx"
#include "SALOME_LifeCycleCORBA.hxx"

// IDL Headers 
#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_ModuleCatalog)
#include CORBA_CLIENT_HEADER(SALOME_Component)
#include CORBA_SERVER_HEADER(SALOMEDS)

// QT Includes
#include <qlist.h>
#include <qmap.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qcombobox.h>
#include <qstatusbar.h>
#include <qpopupmenu.h>
#include "qworkspaceP.h"
#include <qmainwindow.h>
#include <qfiledialog.h>
#include <qtoolbutton.h>

// Open CASCADE Includes
#include <OSD_SharedLibrary.hxx>

class QAD_XmlHandler;

class QAD_EXPORT QAD_Desktop : public QMainWindow
{
  Q_OBJECT

protected:
  //NRI - unused - :  enum ComponentType {GEOM, MESH, SOLVER, DATA, VISU, OTHER} ;

  enum {  FileNewId = 0, FileOpenId, FileLoadId, FileCloseId, FileSaveId, FileSaveAsId, FilePropsId, FileExitId,
	  ViewStatusBarId,
	  SelectionPointId, SelectionEdgeId, SelectionCellId, SelectionActorId,
	  PrefViewerOCCId, PrefViewerVTKId, PrefGraphSupervisorId, PrefViewerPlot2dId,
	  //VRV: T2.5 - add default viewer
	  DefaultViewerOCCId, DefaultViewerVTKId, DefaultGraphSupervisorId, DefaultPlot2dId,
	  //VRV: T2.5 - add default viewer
	  PrefViewerTrihedronId, PrefConsoleFontId, PrefObjectBrowserEntryId, 
	  PrefObjectBrowserIAPPId, PrefObjectBrowserValueId, PrefObjectBrowserCHRONO_SORTId, 
	  PrefDirsId, PrefMultiFileSave, PrefASCIISave, PrefSaveId,
	  //NRI : SAL2214
	  WindowNew3dId, ViewOCCId, ViewVTKId, ViewPlot2dId,
	  //NRI : SAL2214
	  WindowCascadeId, WindowTileId, 
	  HelpContentsId, HelpSearchId, HelpWhatsThisId, HelpAboutId };
  enum UpdateCommand { ActivateApp, DeactivateApp };

    /* Desktop can be created only by application */
    friend  bool QAD_Application::createDesktop( SALOME_NamingService* name_service );
    QAD_Desktop( SALOME_NamingService* name_service );

public:
    ~QAD_Desktop();

    /* global event filter for qapplication */
    bool eventFilter( QObject* o, QEvent* e );

    /* resources */
    static QAD_ResourceMgr* getResourceManager();
    static QAD_ResourceMgr* createResourceManager();

    /* palette */
    static QPalette* getPalette();
    static QPalette* createPalette();

    /* activeStudyChanged */
    void onActiveStudyChanged();

    /* popup */
    void     definePopup(QString & theContext,
			 QString & theParent, 
			 QString & theObject );

    void     createPopup(QPopupMenu* popupFather, QPopupMenu* popup,
			 QString text, int popupID, bool separator = false);
    void     createPopup(QPopupMenu* popup, const QString & theContext,
			 const QString & theParent, const QString & theObject );
    void     customPopup(QPopupMenu* popup, const QString & theContext,
			 const QString & theParent, const QString & theObject);

    /* selection mode */
    void SetSelectionMode(int mode, bool activeCompOnly = false);

    /* desktop */
    void	      showDesktop();
    void              closeDesktop( bool forceClose = false );
    void	      putInfo ( const QString& );
    void	      putInfo ( const QString&, int );
    QToolBar*	      getStdToolBar() const;
    QMenuBar*	      getMainMenuBar() const;

    /* application */
    void	      addApplication(QAD_Application* app);

    /* studies */
    const QString&    getDefaultTitle() const;
    const QPixmap&    getDefaultIcon() const;
    const QString&    getDefaultStudyName() const;
    QAD_Application*  getActiveApp() const;
    QAD_Study*	      getActiveStudy() const;
    QWorkspaceP*      getMainFrame() const;
    QString	      getNewStudyName();
    QAD_Study*        findStudy( SALOMEDS::Study_ptr theStudy );

    /* help actions */
    virtual void      helpAbout();    
    virtual void      helpSearch();
    virtual void      helpContents();
    virtual void      helpContext(const QString& path, const QString& dir = QString::null);

    bool	      loadComponent(QString);
    void	      activateComponent(const QString& theName, bool isLoadData = true);
    void              createStudy();

    void              setSettings();

    SALOME_ModuleCatalog::ModuleCatalog_var getCatalogue();

    QAD_Menus*		  getActiveMenus()      {return myActiveMenus;}
    QAD_OperatorMenus*	  getOperatorMenus()    {return myOperatorMenus;}
    const OSD_SharedLibrary&  getHandle() const {return mySharedLibrary;}// never return sych objects "by value"
    const QString&	  getActiveComponent() const;
    SALOME_NamingService* getNameService()      {return myNameService;}

    Engines::Component_var getEngine(const char *containerName,
				     const char *componentName);

    QString getComponentName(const char *componentUserName);
    QString getComponentUserName(const char *componentName);
    
    QAD_HelpWindow*   getHelpWindow();

    void	      onMouseMove(QMouseEvent*);
    bool	      onMousePress(QMouseEvent*);
    void	      onKeyPress(QKeyEvent*);

    virtual QAD_Study* loadStudy(QString theStudyName);

protected:
    bool	      closeAllStudies( QAD_Application* );
    void	      closeEvent( QCloseEvent* );
    void	      resizeEvent( QResizeEvent* );

    virtual  QString  selectFileName ( bool open );

    virtual   void    updateDesktop( UpdateCommand  );
    virtual   void    updateStatusBar( UpdateCommand );
    virtual   void    updateToolBars( UpdateCommand );
    virtual   void    updateCaption( UpdateCommand );
    virtual   void    updateMenu( UpdateCommand );

    bool              loadComponentData( const QString& );

protected slots:
    virtual void      onNewStudy();
    virtual void      onNewStudy(QAD_Application* app);
    virtual void      onLoadStudy();
    virtual void      onOpenStudy();
    virtual bool      onSaveStudy();
    virtual bool      onSaveStudy(QAD_Study* doc);
    virtual bool      onSaveAsStudy();
    virtual bool      onSaveAsStudy(QAD_Study* doc);
    virtual bool      onCloseStudy();
    virtual bool      onCloseStudy(QAD_Study* doc);
    //ask user to remove study from study manager permanently
    virtual bool      onCloseStudy(QAD_Study* doc, bool ask);

    virtual void      onStudyProperties();

    virtual void      onViewStatusBar();
    virtual void      onToolBarPopupAboutToShow();

    virtual void      onExit();

    virtual void      onWindowPopupAboutToShow();
    void	      onWindowsPopupActivated( int id );
    void	      onWindowsPopupStatusText( int id );

    void	      onSelectionMode();

    void              onDirList();
    void	      onSavePref();
    void	      onViewerOCC();
    void	      onViewerVTK();
    void	      onGraphSupervisor();
    void              onPlot2d();
    void              onConsoleFontAction();
  //VRV: T2.5 - add default viewer
    void	      onDefaultViewer( QActionP * theAction);
  //VRV: T2.5 - add default viewer

    void	      onViewerTrihedron();
    void	      onObjectBrowser();
    void              onMultiFileSave();
    void              onASCIISave();

    void	      onActivateApp(QAD_Application* app);
    void	      onDeactivateApp(QAD_Application* app);

    void	      onHelpContents();
    void	      onHelpSearch();
    void	      onHelpAbout();

    void	      onNewWindow3d();

    void	      onOpenWith();

    void	      onDispatchTools(int id);
    void	      onDispatch(int id);
    void	      onDispatch();
    void	      clearMenus();
    void	      onButtonActiveComponent();
    void	      onComboActiveComponent( const QString & );
    void	      onComboActiveComponent( const QString & , bool isLoadData);
    void              onCascade(); 

    void              onHelpWindowClosed();

protected:
    QMenuBar*	      myMainMenu;	    /* the main menu bar */
    int		      myMainMenuPos;	    /* index after which application inserts its items */
    QPopupMenu	      myFilePopup;
    int		      myFilePos;
    QPopupMenu	      myEditPopup;
    int		      myEditPos;
    QPopupMenu	      myViewPopup;
    int		      myViewPos;
    QPopupMenu	      myHelpPopup;
    int		      myHelpPos;

  //VRV: T2.5 - add default viewer
    QPopupMenu	      myDefaultViewer;
  //VRV: T2.5 - add default viewer
    QPopupMenu	      myObjBrowserPopup;
    QPopupMenu	      myViewerPopup;
  //NRI : SAL2214
    QPopupMenu	      myNewViewPopup;
  //NRI : SAL2214
    QPopupMenu	      myToolsPopup;
    QPopupMenu	      myPrefPopup;
    QPopupMenu	      mySelectionModePopup;
    QPopupMenu	      myWindowPopup;	    /* not customized by an application */

    ActionMap         myStdActions;	    /* default actions */
    QToolBar*	      myStdToolBar;	    /* standard toolbar */
    QStatusBar*	      myStatusBar;	    /* the only status bar */

    QList<ToggleAction> myToolBarAction;
    QPopupMenu          myToolBarsPopup;

    QList<QAD_Application> myApps;	    /* running applications */
    QAD_Application*  myActiveApp;	    /* the active application */
    QAD_Study*	      myActiveStudy;	    /* the active document */

    int		      myCntUntitled;	    /* number of untitled docs */
    QWorkspaceP*      myWorkspace;	    /* workspace */

    QPixmap	      myDefaultIcon;	    /* if no application is active */
    QString	      myDefaultTitle;	    /* desktop will use them */

    QAD_OperatorMenus* myOperatorMenus;
    QList<QAD_Menus>   myMenusList;
    QAD_Menus*	       myActiveMenus;

    QAD_HelpWindow*    myHelpWindow;

    QList<QToolButton> myComponentButton;

    QMap<QString,QString> mapComponentName;

private:
    static QAD_ResourceMgr*		    resourceMgr;
    static QPalette*			    palette;
    void				    createActions();
    void				    updateActions();
    OSD_SharedLibrary			    mySharedLibrary;
    QAD_XmlHandler*			    myXmlHandler;
    QString				    myActiveComp;
    SALOME_NamingService*		    myNameService;
    SALOME_ModuleCatalog::ModuleCatalog_var myCatalogue;
    SALOME_LifeCycleCORBA *		    myEnginesLifeCycle;
    QComboBox *				    myCombo;
    bool                                    myQueryClose;
    bool                                    _islibso;
};

/********************************************************************
** Class: AppSelectionDlg
** Descr: Dialog for the selection of the application when several
**	  applications exist in the desktop and new document creation
**	  is activated by user.
** Level: Internal
*********************************************************************/
class Desktop_AppSelectionDlg : public QDialog
{
   Q_OBJECT

public:
   Desktop_AppSelectionDlg( QAD_Desktop*, QList<QAD_Application>& );
   ~Desktop_AppSelectionDlg();

public:
    QAD_Application* selectedApp()  const;

protected:
    void	     resizeEvent( QResizeEvent* );

protected slots:
    void	     onAppSelected(int);
    void	     onHelp();

protected:
    int					    myAppId;
    QList<QAD_Application>&		    myApps;
};

#endif

