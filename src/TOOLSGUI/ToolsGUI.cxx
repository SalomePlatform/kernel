using namespace std;
//  File      : ToolsGUI.cxx
//  Created   : Wed Nov 14 21:59:24 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : TOOLSGUI
//  Copyright : Open CASCADE
//  $Header$

#include "ToolsGUI.h"
#include "ToolsGUI_CatalogGeneratorDlg.h"

#include "utilities.h"

#include <stdlib.h>

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
