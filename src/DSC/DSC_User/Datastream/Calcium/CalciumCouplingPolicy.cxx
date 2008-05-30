//  Copyright (C) 2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : CalciumCouplingPolicy.cxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Id          : $Id$

#include "CalciumCouplingPolicy.hxx"

CalciumCouplingPolicy::CalciumCouplingPolicy():_dependencyType(CalciumTypes::UNDEFINED_DEPENDENCY),
					       _storageLevel(CalciumTypes::UNLIMITED_STORAGE_LEVEL),
					       _dateCalSchem(CalciumTypes::TI_SCHEM),
					       _interpolationSchem(CalciumTypes::L1_SCHEM),
					       _extrapolationSchem(CalciumTypes::UNDEFINED_EXTRA_SCHEM),
					       _alpha(0.0),_deltaT(CalciumTypes::EPSILON),
					       _disconnectDirective(CalciumTypes::UNDEFINED_DIRECTIVE){};

void CalciumCouplingPolicy::setDependencyType (CalciumTypes::DependencyType dependencyType) {_dependencyType=dependencyType;}
CalciumTypes::DependencyType CalciumCouplingPolicy::getDependencyType () const              { return _dependencyType;}
 
void   CalciumCouplingPolicy::setStorageLevel   (size_t         storageLevel)   {
#ifdef _DEBUG_
  std::cerr << "CalciumCouplingPolicy::setStorageLevel: " << storageLevel << std::endl;
#endif
  if ( storageLevel < 1 && (storageLevel != CalciumTypes::UNLIMITED_STORAGE_LEVEL)  )
    throw CalciumException(CalciumTypes::CPRENA,LOC("Un niveau < 1 n'est pas autorisé"));
  _storageLevel = storageLevel;
}
size_t CalciumCouplingPolicy::getStorageLevel   () const                        {return _storageLevel;}
void   CalciumCouplingPolicy::setDateCalSchem   (CalciumTypes::DateCalSchem   dateCalSchem)   {
#ifdef _DEBUG_
  std::cerr << "CalciumCouplingPolicy::setDateCalSchem: " << dateCalSchem << std::endl;
#endif
  if ( _dependencyType != CalciumTypes::TIME_DEPENDENCY )
    throw CalciumException(CalciumTypes::CPITVR,LOC("Il est impossible de positionner un schéma temporel sur un port qui n'est pas en dépendance temporelle"));
  _dateCalSchem = dateCalSchem;
}

CalciumTypes::DateCalSchem CalciumCouplingPolicy::getDateCalSchem () const   { return _dateCalSchem; }

void CalciumCouplingPolicy::setAlpha(double alpha) {
#ifdef _DEBUG_
  std::cerr << "CalciumCouplingPolicy::setAlpha: " << alpha << std::endl;
#endif
  if ( _dependencyType != CalciumTypes::TIME_DEPENDENCY )
    throw CalciumException(CalciumTypes::CPITVR,LOC("Il est impossible de positionner alpha sur un port qui n'est pas en dépendance temporelle"));
  
  if ( 0 <= alpha <= 1 ) _alpha = alpha; 
  else 
    throw CalciumException(CalciumTypes::CPRENA,LOC("Le paramètre alpha doit être compris entre [0,1]"));
}

double CalciumCouplingPolicy::getAlpha() const   { return _alpha; }

void CalciumCouplingPolicy::setDeltaT(double deltaT ) {
  if ( _dependencyType != CalciumTypes::TIME_DEPENDENCY )
    throw CalciumException(CalciumTypes::CPITVR,LOC("Le paramètre deltaT sur un port qui n'est pas en dépendance temporelle n'a pas de sens"));
  if ( 0 <= deltaT <= 1 ) _deltaT = deltaT; 
  else 
    throw(CalciumException(CalciumTypes::CPRENA,LOC("Le paramètre deltaT doit être compris entre [0,1]")));
}
double CalciumCouplingPolicy::getDeltaT() const  {return _deltaT;}

void CalciumCouplingPolicy::setInterpolationSchem (CalciumTypes::InterpolationSchem interpolationSchem) {
#ifdef _DEBUG_
  std::cerr << "CalciumCouplingPolicy::setInterpolationSchem: " << interpolationSchem << std::endl;
#endif
  if ( _dependencyType != CalciumTypes::TIME_DEPENDENCY )
    throw CalciumException(CalciumTypes::CPITVR,LOC("Le paramètre InterpolationSchem sur un port qui n'est pas en dépendance temporelle n'a pas de sens"));
  _interpolationSchem=interpolationSchem;
}

void CalciumCouplingPolicy::setExtrapolationSchem (CalciumTypes::ExtrapolationSchem extrapolationSchem) {
  if ( _dependencyType != CalciumTypes::TIME_DEPENDENCY )
    throw CalciumException(CalciumTypes::CPITVR,LOC("Le paramètre ExtrapolationSchem sur un port qui n'est pas en dépendance temporelle n'a pas de sens"));
_extrapolationSchem=extrapolationSchem;
}

CalciumTypes::InterpolationSchem CalciumCouplingPolicy::getInterpolationSchem () const { return _interpolationSchem; };
CalciumTypes::ExtrapolationSchem CalciumCouplingPolicy::getExtrapolationSchem () const { return _extrapolationSchem; };


CalciumCouplingPolicy::TimeType 
CalciumCouplingPolicy::getEffectiveTime(CalciumCouplingPolicy::TimeType ti, 
					CalciumCouplingPolicy::TimeType tf) {
  if ( _dateCalSchem == CalciumTypes::TI_SCHEM )  return ti;
  if ( _dateCalSchem == CalciumTypes::TF_SCHEM )  return tf;
  
  //CalciumTypes::ALPHA_SCHEM
  return tf*_alpha + ti*(1-_alpha);
}

void CalciumCouplingPolicy::disconnect(bool provideLastGivenValue) {

  if (provideLastGivenValue) {
    std::cout << "-------- CalciumCouplingPolicy::disconnect CP_CONT  ------------------" << std::endl;
    _disconnectDirective = CalciumTypes::CONTINUE;
  } else {
    std::cout << "-------- CalciumCouplingPolicy::disconnect CP_ARRET  ------------------" << std::endl;
    _disconnectDirective = CalciumTypes::STOP;
  }

  //Wakeup get data if any
  wakeupWaiting();  
}
