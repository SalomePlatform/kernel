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
//  File   : SALOMEDSImpl_AttributeGraphic.hxx
//  Author : Sergey LITONIN
//  Module : SALOME

#ifndef SALOMEDSImpl_AttributeGraphic_HeaderFile
#define SALOMEDSImpl_AttributeGraphic_HeaderFile

#include "DF_Attribute.hxx"
#include "DF_Label.hxx"
#include "SALOMEDSImpl_GenericAttribute.hxx"

#include <map>

/*
  Class       : SALOMEDSImpl_AttributeGraphic
  Description : This class is intended for storing information about
                graphic representation of objects in dirrent views
*/

#ifndef WNT
class Standard_EXPORT SALOMEDSImpl_AttributeGraphic : public SALOMEDSImpl_GenericAttribute
#else
class SALOMEDSImpl_AttributeGraphic : public SALOMEDSImpl_GenericAttribute
#endif
{

public:
Standard_EXPORT                                    SALOMEDSImpl_AttributeGraphic();
Standard_EXPORT                                    ~SALOMEDSImpl_AttributeGraphic();
				  
Standard_EXPORT    static const                    std::string& GetID() ;

Standard_EXPORT    void                            SetVisibility( const int, const bool );
Standard_EXPORT    bool                            GetVisibility( const int );
Standard_EXPORT    const std::string&              ID() const;
Standard_EXPORT    void                            Restore( DF_Attribute* theWith );
Standard_EXPORT    DF_Attribute*                   NewEmpty() const;
Standard_EXPORT    void                            Paste( DF_Attribute* theInto);

Standard_EXPORT    void                            SetVisibility( const std::map<int, int>& );

Standard_EXPORT    const std::map<int, int>&       GetVisibility();
  
private:
  std::map<int, int>  myVisibility;

};

#endif
