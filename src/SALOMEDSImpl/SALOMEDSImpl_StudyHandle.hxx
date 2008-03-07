// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
//  File   : SALOMEDSImpl_StudyHandle.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSImpl_StudyHandle_HeaderFile
#define SALOMEDSImpl_StudyHandle_HeaderFile

#include "DF_Attribute.hxx"
#include "DF_Label.hxx"

/*
  Class       : SALOMEDSImpl_StudyHandle
  Description : PRIVATE: This class is intended for storing of the study handle 
*/

#include "SALOMEDSImpl_Study.hxx"

#ifndef WNT
class Standard_EXPORT SALOMEDSImpl_StudyHandle : public DF_Attribute
#else
class SALOMEDSImpl_StudyHandle : public DF_Attribute
#endif
{

public:
Standard_EXPORT                                  SALOMEDSImpl_StudyHandle();
Standard_EXPORT                                  ~SALOMEDSImpl_StudyHandle() {; }

Standard_EXPORT  static SALOMEDSImpl_StudyHandle* Set(const DF_Label& theLabel, SALOMEDSImpl_Study* theStudy); 
Standard_EXPORT  static const                    std::string& GetID() ;

Standard_EXPORT  void                            Set(SALOMEDSImpl_Study* theStudy) { myHandle = theStudy; }
Standard_EXPORT  SALOMEDSImpl_Study*             Get() { return myHandle; }
Standard_EXPORT  const std::string&              ID() const;
Standard_EXPORT  void                            Restore( DF_Attribute* theWith );
Standard_EXPORT  DF_Attribute*                   NewEmpty() const;
Standard_EXPORT  void                            Paste( DF_Attribute* theInto);
  
private:
  SALOMEDSImpl_Study* myHandle;

};

#endif
