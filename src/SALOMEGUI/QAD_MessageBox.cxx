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
//  File   : QAD_MessageBox.cxx
//  Author : UI team
//  Module : SALOME
//  $Header$

/*!
  \class QAD_MessageBox QAD_MessageBox.h
  \brief Message dialog box for QAD-based application.
*/

#include "QAD_MessageBox.h"

// QT Includes
#include <qmessagebox.h>
#include <qapplication.h>
using namespace std;

/*!
    Shows info message box with one button [ static ]
*/
int QAD_MessageBox::info1 ( QWidget* parent, const QString& caption, const QString& text,
			    const QString& textButton0 )
{
    int ret = QMessageBox::information ( parent, caption, text, textButton0,
					 QString::null, QString::null, 0, 0);
    qApp->processEvents();  /* update desktop */
    return ret;
}

/*!
    Shows warning message box with one button [ static ]
*/
int QAD_MessageBox::warn1 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0 )
{
    int ret = QMessageBox::warning ( parent, caption, text, textButton0,
				     QString::null, QString::null, 0, 0);
    qApp->processEvents();  /* update desktop */
    return ret;
}

/*!
    Shows error message box with one button [ static ]
*/
int QAD_MessageBox::error1 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0 )
{
    int ret = QMessageBox::critical ( parent, caption, text, textButton0,
				      QString::null, QString::null, 0, 0);
    qApp->processEvents();  /* update desktop */
    return ret;
}

/*!
    Shows info message box with two buttons.
    Returns id of the pressed button or -1 if escaped [ static ]
*/
int QAD_MessageBox::info2 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       int idButton0, int idButton1, int idDefault )
{
    if ( idDefault == idButton0 ) idDefault = 0;
    else if ( idDefault == idButton1 ) idDefault = 1;
    else idDefault = 0;
    
    int ret = QMessageBox::information ( parent, caption, text, textButton0,
        textButton1, QString::null, idDefault );        
    qApp->processEvents();  /* update desktop */
    return ( ret == 0 ? idButton0 : idButton1 );
}

/*!
    Shows warning message box with two buttons.
    Returns id of the pressed button or -1 if escaped [ static ]
*/
int QAD_MessageBox::warn2 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       int idButton0, int idButton1, int idDefault )
{
    if ( idDefault == idButton0 ) idDefault = 0;
    else if ( idDefault == idButton1 ) idDefault = 1;
    else idDefault = 0;
    
    int ret = QMessageBox::warning ( parent, caption, text, textButton0,
        textButton1, QString::null, idDefault );
    qApp->processEvents();  /* update desktop */
    return ( ret == 0 ? idButton0 : idButton1 );
}

/*!
    Shows error message box with two buttons 
    Returns id of the pressed button or -1 if escaped [ static ]
*/
int QAD_MessageBox::error2 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       int idButton0, int idButton1, int idDefault )
{
    if ( idDefault == idButton0 ) idDefault = 0;
    else if ( idDefault == idButton1 ) idDefault = 1;
    else idDefault = 0;
    
    int ret = QMessageBox::critical ( parent, caption, text, textButton0,
        textButton1, QString::null, idDefault );
    qApp->processEvents();  /* update desktop */
    return ( ret == 0 ? idButton0 : idButton1 );
}

/*!
    Shows info message box with three buttons.
    Returns id of the pressed button or -1 if escaped [ static ]
*/
int QAD_MessageBox::info3 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       const QString& textButton2, int idButton0, int idButton1,
                       int idButton2, int idDefault )
{
    if ( idDefault == idButton0 ) idDefault = 0;
    else if ( idDefault == idButton1 ) idDefault = 1;
    else if ( idDefault == idButton2 ) idDefault = 2;
    else idDefault = 0;
    
    int ret = QMessageBox::information ( parent, caption, text, textButton0,
            textButton1, textButton2, idDefault );
    qApp->processEvents();  /* update desktop */
    switch ( ret ) 
    {
        case 0: return idButton0;
        case 1: return idButton1;    
        case 2: return idButton2;
    }
    return -1;   
}

/*!
    Shows warning message box with three buttons.
    Returns id of the pressed button or -1 if escaped [ static ]
*/
int QAD_MessageBox::warn3 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       const QString& textButton2, int idButton0, int idButton1,
                       int idButton2, int idDefault )
{
    if ( idDefault == idButton0 ) idDefault = 0;
    else if ( idDefault == idButton1 ) idDefault = 1;
    else if ( idDefault == idButton2 ) idDefault = 2;
    else idDefault = 0;
    
    int ret = QMessageBox::warning ( parent, caption, text, textButton0,
            textButton1, textButton2, idDefault );
    qApp->processEvents();  /* update desktop */
    switch ( ret ) 
    {
        case 0: return idButton0;
        case 1: return idButton1;    
        case 2: return idButton2;
    }
    return -1;   
}

/*!
    Shows error message box with three buttons.
    Returns id of the pressed button or -1 if escaped [ static ]
*/
int QAD_MessageBox::error3 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       const QString& textButton2, int idButton0, int idButton1,
                       int idButton2, int idDefault )
{
    if ( idDefault == idButton0 ) idDefault = 0;
    else if ( idDefault == idButton1 ) idDefault = 1;
    else if ( idDefault == idButton2 ) idDefault = 2;
    else idDefault = 0;

    int ret = QMessageBox::critical ( parent, caption, text, textButton0,
            textButton1, textButton2, idDefault );
    qApp->processEvents();  /* update desktop */
    switch ( ret ) 
    {
        case 0: return idButton0;
        case 1: return idButton1;    
        case 2: return idButton2;
    }
    return -1;   
}
