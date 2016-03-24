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
#include "CalciumTypes2CorbaTypes.hxx"
#include "utilities.h"
#include <iostream>


CORBA_DATE_CAL_SCHEM::CORBA_DATE_CAL_SCHEM() : std::map<CalciumTypes::DateCalSchem,
                                                   Ports::Calcium_Ports::DateCalSchem>()
{
  std::map<CalciumTypes::DateCalSchem, 
    Ports::Calcium_Ports::DateCalSchem > &
    table  = ( std::map<CalciumTypes::DateCalSchem, 
               Ports::Calcium_Ports::DateCalSchem > & ) *this ;

table[CalciumTypes::TI_SCHEM ] = Ports::Calcium_Ports::TI_SCHEM ;
table[CalciumTypes::TF_SCHEM ] = Ports::Calcium_Ports::TF_SCHEM ;
table[CalciumTypes::ALPHA_SCHEM ] = Ports::Calcium_Ports::ALPHA_SCHEM ;
}


Ports::Calcium_Ports::DateCalSchem CORBA_DATE_CAL_SCHEM::operator[]( const CalciumTypes::DateCalSchem &c ) const
{
  std::map<CalciumTypes::DateCalSchem,
    Ports::Calcium_Ports::DateCalSchem > &table = (std::map<CalciumTypes::DateCalSchem,
                                                   Ports::Calcium_Ports::DateCalSchem >&)*this ;
  assert( table.find( (CalciumTypes::DateCalSchem)c ) != table.end() ) ;
  return table[ (CalciumTypes::DateCalSchem)c ] ;
}

const CORBA_DATE_CAL_SCHEM corbaDateCalSchem ;



CORBA_DEPENDENCY_TYPE::CORBA_DEPENDENCY_TYPE() : std::map<CalciumTypes::DependencyType,
                                                     Ports::Calcium_Ports::DependencyType >()
{
  std::map<CalciumTypes::DependencyType,
    Ports::Calcium_Ports::DependencyType > &
    table  = ( std::map<CalciumTypes::DependencyType,
               Ports::Calcium_Ports::DependencyType > & ) *this ;

  table[CalciumTypes::TIME_DEPENDENCY ]      = Ports::Calcium_Ports::TIME_DEPENDENCY ;
  table[CalciumTypes::ITERATION_DEPENDENCY ] = Ports::Calcium_Ports::ITERATION_DEPENDENCY ;
  table[CalciumTypes::UNDEFINED_DEPENDENCY ] = Ports::Calcium_Ports::UNDEFINED_DEPENDENCY ;


  MESSAGE("CORBA_DEPENDENCY_TYPE() : table["<<CalciumTypes::TIME_DEPENDENCY<<"] : "<< table[CalciumTypes::TIME_DEPENDENCY]);
  MESSAGE("CORBA_DEPENDENCY_TYPE() : table["<<CalciumTypes::ITERATION_DEPENDENCY<<"] : "<< table[CalciumTypes::ITERATION_DEPENDENCY]);
  MESSAGE("CORBA_DEPENDENCY_TYPE() : table["<<CalciumTypes::UNDEFINED_DEPENDENCY<<"] : "<< table[CalciumTypes::UNDEFINED_DEPENDENCY]);
}


Ports::Calcium_Ports::DependencyType CORBA_DEPENDENCY_TYPE::operator[]( const CalciumTypes::DependencyType &c ) const
{
  std::map<CalciumTypes::DependencyType,
    Ports::Calcium_Ports::DependencyType > &
    table = (std::map<CalciumTypes::DependencyType,
             Ports::Calcium_Ports::DependencyType >& ) *this ;

  MESSAGE("CORBA_DEPENDENCY_TYPE() : ::operator["<<c<<"]: " << table[c]);

  assert( table.find( (CalciumTypes::DependencyType)c ) != table.end() ) ;
  return table[ (CalciumTypes::DependencyType)c ] ;
}

const CORBA_DEPENDENCY_TYPE corbaDependencyType ;




CORBA_INTERPOLATION_SCHEM::CORBA_INTERPOLATION_SCHEM() : std::map<CalciumTypes::InterpolationSchem,
                                                             Ports::Calcium_Ports::InterpolationSchem > () 
{
  std::map<CalciumTypes::InterpolationSchem,
    Ports::Calcium_Ports::InterpolationSchem > &
    table  = ( std::map<CalciumTypes::InterpolationSchem,
               Ports::Calcium_Ports::InterpolationSchem > & ) *this ;

  table[CalciumTypes::L0_SCHEM ] = Ports::Calcium_Ports::L0_SCHEM ;
  table[CalciumTypes::L1_SCHEM ] = Ports::Calcium_Ports::L1_SCHEM ;
}


Ports::Calcium_Ports::InterpolationSchem CORBA_INTERPOLATION_SCHEM::operator[]( const CalciumTypes::InterpolationSchem &c ) const
{
  std::map<CalciumTypes::InterpolationSchem,
    Ports::Calcium_Ports::InterpolationSchem > &table = 
    (std::map<CalciumTypes::InterpolationSchem,
     Ports::Calcium_Ports::InterpolationSchem >& ) *this ;

  assert( table.find( (CalciumTypes::InterpolationSchem)c ) != table.end() ) ;
  return table[ (CalciumTypes::InterpolationSchem)c ] ;
}

const CORBA_INTERPOLATION_SCHEM corbaInterpolationSchem ;



CORBA_EXTRAPOLATION_SCHEM::CORBA_EXTRAPOLATION_SCHEM() : std::map<CalciumTypes::ExtrapolationSchem,
                                                             Ports::Calcium_Ports::ExtrapolationSchem > () 
{
  std::map<CalciumTypes::ExtrapolationSchem,
    Ports::Calcium_Ports::ExtrapolationSchem > &
    table  = ( std::map<CalciumTypes::ExtrapolationSchem,
               Ports::Calcium_Ports::ExtrapolationSchem > & ) *this ;

  table[CalciumTypes::E0_SCHEM ] = Ports::Calcium_Ports::E0_SCHEM ;
  table[CalciumTypes::E1_SCHEM ] = Ports::Calcium_Ports::E1_SCHEM ;
  table[CalciumTypes::UNDEFINED_EXTRA_SCHEM ] = Ports::Calcium_Ports::UNDEFINED_EXTRA_SCHEM ;
}


Ports::Calcium_Ports::ExtrapolationSchem CORBA_EXTRAPOLATION_SCHEM::operator[]( const CalciumTypes::ExtrapolationSchem &c ) const
{
  std::map<CalciumTypes::ExtrapolationSchem,
    Ports::Calcium_Ports::ExtrapolationSchem > &table = 
    (std::map<CalciumTypes::ExtrapolationSchem,
     Ports::Calcium_Ports::ExtrapolationSchem >& ) *this ;

  assert( table.find( (CalciumTypes::ExtrapolationSchem)c ) != table.end() ) ;
  return table[ (CalciumTypes::ExtrapolationSchem)c ] ;
}

const CORBA_EXTRAPOLATION_SCHEM corbaExtrapolationSchem ;
