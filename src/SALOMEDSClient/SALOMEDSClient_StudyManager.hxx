// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

//  File   : SALOMEDSClient_StudyManager.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDSClient_STUDYMANAGER_H__
#define __SALOMEDSClient_STUDYMANAGER_H__

#include <vector>
#include <string>

#include "SALOMEDSClient_definitions.hxx"
#include "SALOMEDSClient_Study.hxx"

class SALOMEDSClient_StudyManager
{
public:
  virtual ~SALOMEDSClient_StudyManager() {}

  virtual _PTR(Study) NewStudy(const std::string& study_name) = 0;
  virtual _PTR(Study) Open(const std::string& theStudyUrl) = 0;
  virtual void Close(const _PTR(Study)& theStudy) = 0;
  virtual bool Save(const _PTR(Study)& theStudy, bool theMultiFile) = 0;
  virtual bool SaveASCII(const _PTR(Study)& theStudy, bool theMultiFile) = 0;
  virtual bool SaveAs(const std::string& theUrl,  const _PTR(Study)& theStudy, bool theMultiFile) = 0;
  virtual bool SaveAsASCII(const std::string& theUrl,  const _PTR(Study)& theStudy, bool theMultiFile) = 0;
  virtual std::vector<std::string> GetOpenStudies() = 0;
  virtual _PTR(Study) GetStudyByName(const std::string& theStudyName) = 0;
  virtual _PTR(Study) GetStudyByID(int theStudyID) = 0;
  virtual bool CanCopy(const _PTR(SObject)& theSO) = 0;
  virtual bool Copy(const _PTR(SObject)& theSO) = 0;
  virtual bool CanPaste(const _PTR(SObject)& theSO) = 0;
  virtual _PTR(SObject) Paste(const _PTR(SObject)& theSO) = 0;
};

#endif
