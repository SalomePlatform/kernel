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
//  File   : SALOMEGUI_Swig.i
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

%{
#include "SALOMEGUI_Swig.hxx"
%}

%include "pointer.i"

/* %typemap(python,out) const char** */
/* { */
/*   int len,i; */
/*   if ($source) */
/*     { */
/*       len=atoi($source[0]); */
/*       $target = PyList_New(len); */
/*       for (i=0; i<len;  i++) */
/* 	{ */
/* 	  PyList_SetItem($target,i,PyString_FromString($source[i+1])); */
/* 	} */
/*     } */
/*   else */
/*     { */
/*       $target = PyList_New(1); */
/*       PyList_SetItem($target,0,PyString_FromString("")); */
/*     } */
/* } */

class SALOMEGUI_Swig
{
 public:
  SALOMEGUI_Swig();
  ~SALOMEGUI_Swig();

  void updateObjBrowser(bool);
  int getActiveStudyId();
  const char *getActiveStudyName();

/* selection */
  int SelectedCount();
  const char *getSelected(int i);

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
};
