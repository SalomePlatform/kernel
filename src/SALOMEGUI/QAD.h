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
//  File   : QAD.h
//  Module : SALOME

/***********************************************************
**  File:    QAD.h
**  Descr:   General definitions for QAD
**  Created: UI team, 20.09.00
************************************************************/
#ifndef QAD_H
#define QAD_H

#include <assert.h>

#if defined QAD_DLL
#if defined WNT
#define QAD_EXPORT  _declspec( dllexport )
#else
#define QAD_EXPORT
#endif
#else
#if defined WNT
#define QAD_EXPORT  _declspec( dllimport )
#else
#define QAD_EXPORT  
#endif
#endif

#if defined SOLARIS
#define bool  int
#define false 0
#define true  1
#endif

#if defined WNT
#pragma warning ( disable: 4251 )
#endif

#if defined (_DEBUG) 
#define QAD_ASSERT(x)			assert(x)
#define QAD_ASSERT_DEBUG_ONLY(x)	assert(x)

#else	/* _DEBUG */
#define QAD_ASSERT(x)			x
#define QAD_ASSERT_DEBUG_ONLY(x)	
#endif

/*  Message box button's IDs */
#define QAD_OK                      1
#define QAD_CANCEL                  2
#define QAD_YES                     3
#define QAD_NO                      4
#define QAD_HELP                    5

#endif
