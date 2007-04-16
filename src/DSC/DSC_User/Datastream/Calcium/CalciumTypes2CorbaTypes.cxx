// Eric Fayolle - EDF R&D
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-03-01 13:40:26 +0100 (Thu, 01 Mar 2007) $
// Id          : $Id$

#include "CalciumTypes2CorbaTypes.hxx"
#include <iostream>

using namespace std;

CORBA_DATE_CAL_SCHEM::CORBA_DATE_CAL_SCHEM() : map<CalciumTypes::DateCalSchem,
						   Ports::Calcium_Ports::DateCalSchem>()
{
  map<CalciumTypes::DateCalSchem, 
    Ports::Calcium_Ports::DateCalSchem > &
    table  = ( map<CalciumTypes::DateCalSchem, 
	       Ports::Calcium_Ports::DateCalSchem > & ) *this ;

table[CalciumTypes::TI_SCHEM ] = Ports::Calcium_Ports::TI_SCHEM ;
table[CalciumTypes::TF_SCHEM ] = Ports::Calcium_Ports::TF_SCHEM ;
table[CalciumTypes::ALPHA_SCHEM ] = Ports::Calcium_Ports::ALPHA_SCHEM ;
}


Ports::Calcium_Ports::DateCalSchem CORBA_DATE_CAL_SCHEM::operator[]( const CalciumTypes::DateCalSchem &c ) const
{
  map<CalciumTypes::DateCalSchem,
    Ports::Calcium_Ports::DateCalSchem > &table = (map<CalciumTypes::DateCalSchem,
						   Ports::Calcium_Ports::DateCalSchem >&)*this ;
  assert( table.find( (CalciumTypes::DateCalSchem)c ) != table.end() ) ;
  return table[ (CalciumTypes::DateCalSchem)c ] ;
}

const CORBA_DATE_CAL_SCHEM corbaDateCalSchem ;



CORBA_DEPENDENCY_TYPE::CORBA_DEPENDENCY_TYPE() : map<CalciumTypes::DependencyType,
						     Ports::Calcium_Ports::DependencyType >()
{
  map<CalciumTypes::DependencyType,
    Ports::Calcium_Ports::DependencyType > &
    table  = ( map<CalciumTypes::DependencyType,
	       Ports::Calcium_Ports::DependencyType > & ) *this ;

table[CalciumTypes::TIME_DEPENDENCY ]      = Ports::Calcium_Ports::TIME_DEPENDENCY ;
table[CalciumTypes::ITERATION_DEPENDENCY ] = Ports::Calcium_Ports::ITERATION_DEPENDENCY ;
table[CalciumTypes::UNDEFINED_DEPENDENCY ] = Ports::Calcium_Ports::UNDEFINED_DEPENDENCY ;


std::cout << "CORBA_DEPENDENCY_TYPE() : table["<<CalciumTypes::TIME_DEPENDENCY<<"] : "<< 
  table[CalciumTypes::TIME_DEPENDENCY] << std::endl;
std::cout << "CORBA_DEPENDENCY_TYPE() : table["<<CalciumTypes::ITERATION_DEPENDENCY<<"] : "<< 
  table[CalciumTypes::ITERATION_DEPENDENCY] << std::endl;
std::cout << "CORBA_DEPENDENCY_TYPE() : table["<<CalciumTypes::UNDEFINED_DEPENDENCY<<"] : "<< 
  table[CalciumTypes::UNDEFINED_DEPENDENCY] << std::endl;
}


Ports::Calcium_Ports::DependencyType CORBA_DEPENDENCY_TYPE::operator[]( const CalciumTypes::DependencyType &c ) const
{
  map<CalciumTypes::DependencyType,
    Ports::Calcium_Ports::DependencyType > &
    table = (map<CalciumTypes::DependencyType,
	     Ports::Calcium_Ports::DependencyType >& ) *this ;

std::cout << "CORBA_DEPENDENCY_TYPE() : ::operator["<<c<<"]: " << table[c] << std::endl;

  assert( table.find( (CalciumTypes::DependencyType)c ) != table.end() ) ;
  return table[ (CalciumTypes::DependencyType)c ] ;
}

const CORBA_DEPENDENCY_TYPE corbaDependencyType ;




CORBA_INTERPOLATION_SCHEM::CORBA_INTERPOLATION_SCHEM() : map<CalciumTypes::InterpolationSchem,
							     Ports::Calcium_Ports::InterpolationSchem > () 
{
  map<CalciumTypes::InterpolationSchem,
    Ports::Calcium_Ports::InterpolationSchem > &
    table  = ( map<CalciumTypes::InterpolationSchem,
	       Ports::Calcium_Ports::InterpolationSchem > & ) *this ;

  table[CalciumTypes::L0_SCHEM ] = Ports::Calcium_Ports::L0_SCHEM ;
  table[CalciumTypes::L1_SCHEM ] = Ports::Calcium_Ports::L1_SCHEM ;
}


Ports::Calcium_Ports::InterpolationSchem CORBA_INTERPOLATION_SCHEM::operator[]( const CalciumTypes::InterpolationSchem &c ) const
{
  map<CalciumTypes::InterpolationSchem,
    Ports::Calcium_Ports::InterpolationSchem > &table = 
    (map<CalciumTypes::InterpolationSchem,
     Ports::Calcium_Ports::InterpolationSchem >& ) *this ;

  assert( table.find( (CalciumTypes::InterpolationSchem)c ) != table.end() ) ;
  return table[ (CalciumTypes::InterpolationSchem)c ] ;
}

const CORBA_INTERPOLATION_SCHEM corbaInterpolationSchem ;



CORBA_EXTRAPOLATION_SCHEM::CORBA_EXTRAPOLATION_SCHEM() : map<CalciumTypes::ExtrapolationSchem,
							     Ports::Calcium_Ports::ExtrapolationSchem > () 
{
  map<CalciumTypes::ExtrapolationSchem,
    Ports::Calcium_Ports::ExtrapolationSchem > &
    table  = ( map<CalciumTypes::ExtrapolationSchem,
	       Ports::Calcium_Ports::ExtrapolationSchem > & ) *this ;

  table[CalciumTypes::E0_SCHEM ] = Ports::Calcium_Ports::E0_SCHEM ;
  table[CalciumTypes::E1_SCHEM ] = Ports::Calcium_Ports::E1_SCHEM ;
  table[CalciumTypes::UNDEFINED_EXTRA_SCHEM ] = Ports::Calcium_Ports::UNDEFINED_EXTRA_SCHEM ;
}


Ports::Calcium_Ports::ExtrapolationSchem CORBA_EXTRAPOLATION_SCHEM::operator[]( const CalciumTypes::ExtrapolationSchem &c ) const
{
  map<CalciumTypes::ExtrapolationSchem,
    Ports::Calcium_Ports::ExtrapolationSchem > &table = 
    (map<CalciumTypes::ExtrapolationSchem,
     Ports::Calcium_Ports::ExtrapolationSchem >& ) *this ;

  assert( table.find( (CalciumTypes::ExtrapolationSchem)c ) != table.end() ) ;
  return table[ (CalciumTypes::ExtrapolationSchem)c ] ;
}

const CORBA_EXTRAPOLATION_SCHEM corbaExtrapolationSchem ;
