//=============================================================================
// File      : SALOMEGUI_Swig.i
// Created   : ven oct 12 15:44:16 CEST 2001
// Author    : Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

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
