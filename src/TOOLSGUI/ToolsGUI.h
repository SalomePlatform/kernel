//  SALOME TOOLSGUI : implementation of desktop "Tools" optioins
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
//  File   : ToolsGUI.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef ToolsGUI_HeaderFile
#define ToolsGUI_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#include "QAD_Desktop.h"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class Standard_EXPORT ToolsGUI  
{

public :

  static int                runCommand( string&  );
  static bool               OnGUIEvent( int theCommandID, QAD_Desktop* parent );

  static bool               GetVisibility( SALOMEDS::Study_var   theStudy,
                                           SALOMEDS::SObject_var theObj,
                                           void*                 theId );
  static bool               SetVisibility( SALOMEDS::Study_var theStudy,
                                           const char*         theEntry,
                                           const bool          theValue,
                                           void*               theId );
};

#endif
