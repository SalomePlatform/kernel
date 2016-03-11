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

// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-03-01 13:40:26 +0100 (Thu, 01 Mar 2007) $
// Id          : $Id$
//
#ifndef _CORBATYPES_TO_CALCIUMTYPES_HXX_
#define _CORBATYPES_TO_CALCIUMTYPES_HXX_

#include "Calcium_Ports.hh"
#include "CalciumTypes.hxx"

#include <map>
#include <cassert>


class DATE_CAL_SCHEM : public std::map<Ports::Calcium_Ports::DateCalSchem,
                                       CalciumTypes::DateCalSchem> 
{
public :

  DATE_CAL_SCHEM();
  CalciumTypes::DateCalSchem  operator[]( const Ports::Calcium_Ports::DateCalSchem &c ) const;
};


extern const DATE_CAL_SCHEM dateCalSchem ;

class DEPENDENCY_TYPE : public std::map<Ports::Calcium_Ports::DependencyType,
                                        CalciumTypes::DependencyType>
{
public :

  DEPENDENCY_TYPE();
  CalciumTypes::DependencyType  operator[]( const Ports::Calcium_Ports::DependencyType &c ) const;
};

extern const DEPENDENCY_TYPE dependencyType ;

class INTERPOLATION_SCHEM : public std::map<Ports::Calcium_Ports::InterpolationSchem,
                                            CalciumTypes::InterpolationSchem>
{
public :

  INTERPOLATION_SCHEM();
  CalciumTypes::InterpolationSchem  operator[]( const Ports::Calcium_Ports::InterpolationSchem &c ) const;
} ;

extern const INTERPOLATION_SCHEM interpolationSchem ;

class EXTRAPOLATION_SCHEM : public std::map<Ports::Calcium_Ports::ExtrapolationSchem,
                                            CalciumTypes::ExtrapolationSchem>
{
public :

  EXTRAPOLATION_SCHEM();
  CalciumTypes::ExtrapolationSchem  operator[]( const Ports::Calcium_Ports::ExtrapolationSchem &c ) const;
} ;

extern const EXTRAPOLATION_SCHEM extrapolationSchem ;


#endif
