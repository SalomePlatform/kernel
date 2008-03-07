// Eric Fayolle - EDF R&D
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-03-01 13:40:26 +0100 (Thu, 01 Mar 2007) $
// Id          : $Id$

#ifndef _CALCIUMTYPES_TO_CORBATYPES_HXX_
#define _CALCIUMTYPES_TO_CORBATYPES_HXX_

#include "Calcium_Ports.hh"
#include "CalciumTypes.hxx"

#include <map>
#include <cassert>


class CORBA_DATE_CAL_SCHEM : public std::map<CalciumTypes::DateCalSchem,
					     Ports::Calcium_Ports::DateCalSchem > 
{
public :

  CORBA_DATE_CAL_SCHEM();
  Ports::Calcium_Ports::DateCalSchem operator[]( const CalciumTypes::DateCalSchem &c ) const;
};


extern const CORBA_DATE_CAL_SCHEM corbaDateCalSchem ;

class CORBA_DEPENDENCY_TYPE : public std::map<CalciumTypes::DependencyType,
					      Ports::Calcium_Ports::DependencyType>
{
public :

  CORBA_DEPENDENCY_TYPE();
  Ports::Calcium_Ports::DependencyType operator[]( const CalciumTypes::DependencyType  &c ) const;
};

extern const CORBA_DEPENDENCY_TYPE corbaDependencyType ;

class CORBA_INTERPOLATION_SCHEM : public std::map<CalciumTypes::InterpolationSchem,
						  Ports::Calcium_Ports::InterpolationSchem>
{
public :

  CORBA_INTERPOLATION_SCHEM();
  Ports::Calcium_Ports::InterpolationSchem operator[]( const CalciumTypes::InterpolationSchem &c ) const;
} ;

extern const CORBA_INTERPOLATION_SCHEM corbaInterpolationSchem ;

class CORBA_EXTRAPOLATION_SCHEM : public std::map<CalciumTypes::ExtrapolationSchem,
						  Ports::Calcium_Ports::ExtrapolationSchem>
{
public :

  CORBA_EXTRAPOLATION_SCHEM();
  Ports::Calcium_Ports::ExtrapolationSchem operator[]( const CalciumTypes::ExtrapolationSchem &c ) const;
} ;

extern const CORBA_EXTRAPOLATION_SCHEM corbaExtrapolationSchem ;


#endif
