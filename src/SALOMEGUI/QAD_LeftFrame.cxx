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
//  File   : QAD_LeftFrame.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "QAD_LeftFrame.h"
#include "QAD_ObjectBrowser.h"
#include <qlayout.h>
using namespace std;

/*!
  \class QAD_LeftFrame QAD_LeftFrame.h
  \brief Frame window which contains QAD_ObjectBrowser.
*/

/*!
  Constructor
*/
QAD_LeftFrame::QAD_LeftFrame( SALOMEDS::Study_var study, QWidget *parent, const char *name)
     : QWidget( parent, name )
{
  QVBoxLayout* layout = new QVBoxLayout( this );
  layout->setMargin( 0 ); layout->setSpacing( 6 );
  myObjectBrowser = new QAD_ObjectBrowser( study, this );
  layout->addWidget( myObjectBrowser );
}

/*!
  Destructor
*/
QAD_LeftFrame::~QAD_LeftFrame()
{
}

/*!
  Returns reference to Object Browser [ static ]
*/
QAD_ObjectBrowser* QAD_LeftFrame::getObjectBrowser() const
{
  return myObjectBrowser;
}
