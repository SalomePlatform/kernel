//  File      : SALOMEGUI_Application.h
//  Created   : Thu Jun 14 12:01:23 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef SALOMEGUI_APPLICATION_H
#define SALOMEGUI_APPLICATION_H

#include "QAD_Application.h"
#include "QAD_ViewFrame.h"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

// QT Includes
#include <qlist.h>
#include <qaction.h>
#include <qstring.h>
#include <qtoolbar.h>
#include <qpopupmenu.h>

class SALOMEGUI_Application : public QAD_Application
{
  Q_OBJECT
    enum { ViewToolBarId = 100 };
  
    enum { ViewDumpId, ViewTrihedronId, ViewFitAllId, ViewFitAreaId,
	   ViewZoomId, ViewPanId, ViewRotateId, ViewGlobalPanId,
	   ViewFrontId, ViewBackId, ViewTopId, ViewBottomId, ViewLeftId,
	   ViewRightId, ViewResetId };
public:
	SALOMEGUI_Application ( const QString& format,
				const QString& description,
				const QString& filters);
	~SALOMEGUI_Application();
public slots:
  // Common display services
  void    onDisplay(int);
  void    onErase();

protected:
    /* application management */
    bool    initApp( SALOME_NamingService* name_service );
    bool    createDesktop( SALOME_NamingService* name_service );

    /* popup */
    void    onCreatePopup();

    /* document management */
    void    onStudyCreated( QAD_Study* );
    void    onStudyOpened( QAD_Study* );
    void    onActiveStudyChanged( QAD_Study* oldActiveStudy,
				  QAD_Study* newActiveStudy );
    void    onStudyShown( QAD_Study* );

    /* menu management */
    int	    onUpdateViewActions ( QPopupMenu*, bool add, int index );

private slots:
    void    onDesktopCreated();

private:
    void    connectToViewer( QAD_ViewFrame* );
    void    disconnectFromViewer( QAD_ViewFrame* );
    void    createActions();
    void    updateActions();

private:
    QList<QAction>	    myViewActions;
    QAD_Operation*	    myCurOperation;
};

#endif

