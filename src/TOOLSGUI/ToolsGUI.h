//  File      : ToolsGUI.h
//  Created   : Wed Nov 14 22:00:04 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : TOOLSGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef ToolsGUI_HeaderFile
#define ToolsGUI_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#include "QAD_Desktop.h"

class ToolsGUI : public QObject
{
  Q_OBJECT

public :

// Methods PUBLIC
// 
    Standard_EXPORT static int ToolsGUI::runCommand(string & arg);
Standard_EXPORT static bool OnGUIEvent (int theCommandID, QAD_Desktop* parent);
 
protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 


 // Fields PRIVATE
 //

};

#endif
