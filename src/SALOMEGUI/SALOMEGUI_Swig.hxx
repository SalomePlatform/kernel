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
//  File   : SALOMEGUI_Swig.hxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef _SALOMEGUI_SWIG_HXX_
#define _SALOMEGUI_SWIG_HXX_

//QT Include
#include <qstring.h>

#include <Standard.hxx>

#include "SALOME_InteractiveObject.hxx"

class QAD_Study;
class QAD_ViewFrame;

namespace SALOME{
  QAD_ViewFrame* GetViewFrame(QAD_Study* theStudy);
  Handle(SALOME_InteractiveObject) FindIObject(QAD_Study* theStudy, const char *theEntry);
}


class SALOMEGUI_Swig
{
public:
  SALOMEGUI_Swig();
  ~SALOMEGUI_Swig();

  void         updateObjBrowser( bool updateSelection );
  QAD_Study*   getActiveStudy();
  int          getActiveStudyId();
  const char*  getActiveStudyName();

/* selection */
  int          SelectedCount();
  const char*  getSelected(int i);

  void AddIObject(const char *Entry);
  void RemoveIObject(const char *Entry);
  void ClearIObjects();

/* display */		
  void Display(const char *Entry);
  void DisplayOnly(const char *Entry);
  void Erase(const char *Entry);
  void DisplayAll();
  void EraseAll();

/* check */
  bool IsInCurrentView(const char *Entry);

/* component name */
  const char* getComponentName( const char* ComponentUserName );
  const char* getComponentUserName( const char* ComponentName );

protected:
  int _studyId;
  QString _name;
};


#endif
