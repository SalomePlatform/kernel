//=============================================================================
// File      : SALOMEGUI_Swig.hxx
// Created   : ven oct 12 15:44:16 CEST 2001
// Author    : Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2001

// Modified  : Mon Jul 29 21:38:07 2002
// Author    : Nicolas REJNERI
// Copyright : OPEN CASCADE 2002
// $Header$
//=============================================================================

#ifndef _SALOMEGUI_SWIG_HXX_
#define _SALOMEGUI_SWIG_HXX_

//QT Include
#include <qstring.h>

#include <Standard.hxx>

class vtkRenderer;

class SALOMEGUI_Swig
{
public:
  SALOMEGUI_Swig();
  ~SALOMEGUI_Swig();

  static vtkRenderer* getRenderer(int viewId = -1);

  void        updateObjBrowser( bool updateSelection );
  int         getActiveStudyId();
  const char* getActiveStudyName();

/* selection */
  int         SelectedCount();
  const char* getSelected(int i);

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
