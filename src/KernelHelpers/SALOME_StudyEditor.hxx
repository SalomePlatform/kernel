// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
// Author: Guillaume Boulant (EDF/R&D)

#ifndef __STUDY_EDITOR_HXX__
#define __STUDY_EDITOR_HXX__

#include "KernelHelpers.hxx"

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOME_Component)

#include <vector>

class KERNELHELPERS_EXPORT SALOME_StudyEditor {

public:
  SALOME_StudyEditor(int studyId);
  SALOME_StudyEditor(SALOMEDS::Study_ptr study);

  typedef std::vector<SALOMEDS::SObject_ptr> SObjectList;

  SALOMEDS::SComponent_ptr newRoot(const char * moduleName);  
  SALOMEDS::SComponent_ptr findRoot(const char * moduleName);
  bool bindEngine(SALOMEDS::SComponent_var studyRoot,Engines::EngineComponent_var engine);

  SALOMEDS::SObject_ptr newObject(SALOMEDS::SObject_ptr parent);
  SALOMEDS::SObject_ptr findObject(const char * entry);
  void setName(SALOMEDS::SObject_var sobject, const char * value);
  const char * getName(SALOMEDS::SObject_var sobject);

  void setIcon(SALOMEDS::SObject_var sobject, const char * resourcename);

  void setParameterInt(SALOMEDS::SObject_var sobject, const char * name, int value);
  int  getParameterInt(SALOMEDS::SObject_var sobject, const char * name);
  void setParameterBool(SALOMEDS::SObject_var sobject, const char * name, bool value);
  bool getParameterBool(SALOMEDS::SObject_var sobject, const char * name);

  static const int UNDEFINED = -1;

protected:
  SALOME_StudyEditor();
  void setStudy(SALOMEDS::Study_ptr study);
  void setStudyById(int studyId);
  int getStudyId();

  SALOMEDS::Study_var _study;

private:
  SALOMEDS::StudyBuilder_var _sbuilder;
};

#endif // __STUDY_EDITOR_HXX__
