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
//  File   : QAD_Splitter.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "QAD_Splitter.h"
using namespace std;

/*!
  Constructs an horizontal splitter.
*/

QAD_Splitter::QAD_Splitter( QWidget *parent, const char *name )
  :QSplitterP(parent,name)
{
}

/*!
  Constructs splitter with orientation \a o.
*/
QAD_Splitter::QAD_Splitter( Orientation o, QWidget *parent, const char *name )
  :QSplitterP(o, parent, name)
{
}

/*!
  Destructs the splitter.
*/
QAD_Splitter::~QAD_Splitter()
{
}
