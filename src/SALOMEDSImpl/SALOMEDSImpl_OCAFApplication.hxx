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
// See http://www.salome-platform.org/
//
//  File   : SALOMEDSImpl_OCAFApplication.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_OCAFApplication_HeaderFile
#define _SALOMEDSImpl_OCAFApplication_HeaderFile


//Handle definition
#include <TDocStd_Application.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_OCAFApplication,  TDocStd_Application)

#include <Standard_CString.hxx>
#include <TColStd_SequenceOfExtendedString.hxx>


class SALOMEDSImpl_OCAFApplication : public TDocStd_Application {

public:

Standard_EXPORT SALOMEDSImpl_OCAFApplication();
Standard_EXPORT virtual  void Formats(TColStd_SequenceOfExtendedString& Formats) ;
Standard_EXPORT   Standard_CString ResourcesName() ;
Standard_EXPORT ~SALOMEDSImpl_OCAFApplication() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_OCAFApplication )

};

#endif
