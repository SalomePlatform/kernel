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

//  File   : CalciumCouplingPolicy.cxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Id          : $Id$
//
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
  MESSAGE( "CalciumCouplingPolicy::setStorageLevel: " << storageLevel );
  if ( storageLevel < 1 && (storageLevel != CalciumTypes::UNLIMITED_STORAGE_LEVEL)  )
    throw CalciumException(CalciumTypes::CPRENA,LOC("StorageLevel < 1 is not allowed"));
  _storageLevel = storageLevel;
}
size_t CalciumCouplingPolicy::getStorageLevel   () const                        {return _storageLevel;}
void   CalciumCouplingPolicy::setDateCalSchem   (CalciumTypes::DateCalSchem   dateCalSchem)   {
  MESSAGE( "CalciumCouplingPolicy::setDateCalSchem: " << dateCalSchem );
  if ( _dependencyType != CalciumTypes::TIME_DEPENDENCY )
    throw CalciumException(CalciumTypes::CPITVR,LOC("Can't set a temporal scheme on a port that is not time dependent"));
  _dateCalSchem = dateCalSchem;
}

CalciumTypes::DateCalSchem CalciumCouplingPolicy::getDateCalSchem () const   { return _dateCalSchem; }

void CalciumCouplingPolicy::setAlpha(double alpha) {
  MESSAGE( "CalciumCouplingPolicy::setAlpha: " << alpha );
  if ( _dependencyType != CalciumTypes::TIME_DEPENDENCY )
    throw CalciumException(CalciumTypes::CPITVR,LOC("Can't set alpha on a port that is not time dependent"));
  
  if ( 0 <= alpha && alpha <= 1 ) _alpha = alpha; 
  else 
    throw CalciumException(CalciumTypes::CPRENA,LOC("alpha must be between [0,1]"));
}

double CalciumCouplingPolicy::getAlpha() const   { return _alpha; }

void CalciumCouplingPolicy::setDeltaT(double deltaT ) {
  if ( _dependencyType != CalciumTypes::TIME_DEPENDENCY )
    throw CalciumException(CalciumTypes::CPITVR,LOC("Can't set deltaT on a port that is not time dependent"));
  if ( 0 <= deltaT && deltaT <= 1 ) _deltaT = deltaT; 
  else 
    throw(CalciumException(CalciumTypes::CPRENA,LOC("deltaT must be between [0,1]")));
}
double CalciumCouplingPolicy::getDeltaT() const  {return _deltaT;}

void CalciumCouplingPolicy::setInterpolationSchem (CalciumTypes::InterpolationSchem interpolationSchem) {
  MESSAGE( "CalciumCouplingPolicy::setInterpolationSchem: " << interpolationSchem );
  if ( _dependencyType != CalciumTypes::TIME_DEPENDENCY )
    throw CalciumException(CalciumTypes::CPITVR,LOC("Can't set InterpolationSchem on a port that is not time dependent"));
  _interpolationSchem=interpolationSchem;
}

void CalciumCouplingPolicy::setExtrapolationSchem (CalciumTypes::ExtrapolationSchem extrapolationSchem) {
  if ( _dependencyType != CalciumTypes::TIME_DEPENDENCY )
    throw CalciumException(CalciumTypes::CPITVR,LOC("Can't set ExtrapolationSchem on a port that is not time dependent"));
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
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::disconnect CP_CONT  ------------------" << std::endl;
#endif
    _disconnectDirective = CalciumTypes::CONTINUE;
  } else {
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::disconnect CP_ARRET  ------------------" << std::endl;
#endif
    _disconnectDirective = CalciumTypes::STOP;
  }

  //Wakeup get data if any
  wakeupWaiting();  
}
