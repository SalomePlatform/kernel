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
using namespace std;

int ToolsGUI::runCommand(string & arg)
{ 
  int res;
  res = system(arg.c_str());
 
  if (res == -1)
    MESSAGE("fork failed (system command result = 0x" << hex << res << ")" << dec) 
  else
    if (res == 217)
      MESSAGE("shell exec failed (system command result = 0x" << hex << res << ")" << dec)
	
  return res;
}

bool ToolsGUI::OnGUIEvent(int theCommandID,  QAD_Desktop* parent)
{
  QAD_Study* myActiveStudy = parent->getActiveStudy();
  switch (theCommandID)
    {
    case 5102 :
      {
	ToolsGUI_CatalogGeneratorDlg* aDlg = new ToolsGUI_CatalogGeneratorDlg(parent);
	aDlg->exec();
	delete aDlg;
	break;
      }

    default:
      MESSAGE (" No command associated with this id = " << theCommandID )
      break;
    }
  return true;
}

extern "C"
{
  bool OnGUIEvent(int theCommandID, QAD_Desktop* parent)
  {
    //MESSAGE("ToolsGUI::OnGUIEvent "<< theCommandID);
    return ToolsGUI::OnGUIEvent(theCommandID, parent);
  }

}
