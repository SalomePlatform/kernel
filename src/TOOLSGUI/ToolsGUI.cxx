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
//  File   : ToolsGUI.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "ToolsGUI.h"
#include "ToolsGUI_CatalogGeneratorDlg.h"

#include "utilities.h"

#include <stdlib.h>

#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

using namespace std;

//============================================================================
// function : runCommand
// purpose  : Run command
//============================================================================
int ToolsGUI::runCommand(string & arg)
{ 
  int res;
  res = system( arg.c_str() );
 
  if ( res == -1 )
    MESSAGE( "fork failed (system command result = 0x" << hex << res << ")" << dec ) 
  else
    if ( res == 217 )
      MESSAGE( "shell exec failed (system command result = 0x" << hex << res << ")" << dec )
  return res;
}

//============================================================================
// function : OnGUIEvent
// purpose  : Process events
//============================================================================
bool ToolsGUI::OnGUIEvent( int theCommandID,  QAD_Desktop* parent )
{
  switch ( theCommandID )
  {
  case 5102 :
    {
      ToolsGUI_CatalogGeneratorDlg* aDlg = new ToolsGUI_CatalogGeneratorDlg( parent );
      aDlg->exec();
      delete aDlg;
      break;
    }

  default:
    MESSAGE ( " No command associated with this id = " << theCommandID )
    break;
  }
  return true;
}

extern "C"
{
  bool OnGUIEvent( int theCommandID, QAD_Desktop* parent )
  {
    return ToolsGUI::OnGUIEvent(theCommandID, parent);
  }
}

//=======================================================================
// name    : GetVisibility
// Purpose : Verify whether object is visible or not
//=======================================================================
bool ToolsGUI::GetVisibility( SALOMEDS::Study_var   theStudy,
                              SALOMEDS::SObject_var theObj,
                              void*                 theId )
{
  SALOMEDS::GenericAttribute_var anAttr;
  if ( !theObj->_is_nil() && theObj->FindAttribute( anAttr, "AttributeGraphic" ) )
  {
    SALOMEDS::AttributeGraphic_var aGraphic =
      SALOMEDS::AttributeGraphic::_narrow( anAttr );
    return aGraphic->GetVisibility( (unsigned long)theId );
  }

  return false;
}

//=======================================================================
// name    : SetVisibility
// Purpose : Set flag visibility of object
//=======================================================================
bool ToolsGUI::SetVisibility( SALOMEDS::Study_var theStudy,
                              const char*         theEntry,
                              const bool          theValue,
                              void*               theId )
{
  SALOMEDS::SObject_var anObj = theStudy->FindObjectID( theEntry );

  if ( !anObj->_is_nil() )
  {
    SALOMEDS::GenericAttribute_var aGAttr;
    if ( anObj->FindAttribute( aGAttr, "AttributeGraphic" ) )
    {
      SALOMEDS::AttributeGraphic_var anAttr = SALOMEDS::AttributeGraphic::_narrow( aGAttr );
      anAttr->SetVisibility( (unsigned long)theId, theValue );
    }
    else if ( theValue )
    {
      SALOMEDS::StudyBuilder_var aBuilder = theStudy->NewBuilder();
      SALOMEDS::AttributeGraphic_var anAttr = SALOMEDS::AttributeGraphic::_narrow(
        aBuilder->FindOrCreateAttribute( anObj, "AttributeGraphic" ) );
      anAttr->SetVisibility( (unsigned long)theId, theValue );
    }
    return true;
  }

  return false;
}






