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
//  File   : QAD_MessageBox.h
//  Module : SALOME

/********************************************************************
**  Class:   QAD_MessageBox
**  Descr:   Message dialog box for QAD-based application
**  Module:  QAD
**  Created: UI team, 02.10.00
*********************************************************************/
#ifndef QAD_MESSAGEBOX_H
#define QAD_MESSAGEBOX_H

#include <qstring.h>
#include <qwidget.h>

#include "QAD.h"

class QAD_EXPORT QAD_MessageBox
{
public:
    /* show a box with one button */
    static int info1 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0 );
    static int warn1 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0 );
    static int error1 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0 );

    /* show a box with two buttons */
    static int info2 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       int idButton0, int idButton1, int idDefault );
    static int warn2 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       int idButton0, int idButton1, int idDefault );
    static int error2 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       int idButton0, int idButton1, int idDefault );

    /* show a box with three buttons */
    static int info3 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       const QString& textButton2, int idButton0, int idButton1,
                       int idButton2, int idDefault );
    static int warn3 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       const QString& textButton2, int idButton0, int idButton1,
                       int idButton2, int idDefault );
    static int error3 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       const QString& textButton2, int idButton0, int idButton1,
                       int idButton2, int idDefault );
};

#endif

