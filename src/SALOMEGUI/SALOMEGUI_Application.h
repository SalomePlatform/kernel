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
//  File   : SALOMEGUI_Application.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef SALOMEGUI_APPLICATION_H
#define SALOMEGUI_APPLICATION_H

#include "QAD_Application.h"
#include "QAD_ViewFrame.h"
#include "QAD_Action.h"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

// QT Includes
#include <qlist.h>
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
    ActionMap           myViewActions;
    QAD_Operation*	myCurOperation;
};

#endif

