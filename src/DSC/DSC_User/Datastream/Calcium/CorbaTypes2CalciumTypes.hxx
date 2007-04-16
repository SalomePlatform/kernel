// Eric Fayolle - EDF R&D
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-03-01 13:40:26 +0100 (Thu, 01 Mar 2007) $
// Id          : $Id$

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
