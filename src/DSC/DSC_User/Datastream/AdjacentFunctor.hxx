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

//  File   : AdjacentFunctor.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-01-24 16:30:34 +0100 (mer, 24 jan 2007) $
// Id          : $Id$
//
#ifndef _ADJACENT_FUNCTOR_HXX_
#define _ADJACENT_FUNCTOR_HXX_

#include "ConstTraits.hxx"
// Pour affichage
#include "DisplayPair.hxx"
//

//#define MYDEBUG

// Suppose que le container est trié
template < typename T > struct AdjacentFunctor {

  typedef typename ConstTrait<T>::NonConstType TNoConst;
  const T & _minValue;
  T         _maxValue;
  TNoConst  _max;
  TNoConst  _min;
  bool      _minFound,_maxFound,_equal;
  
  AdjacentFunctor(const T& value):_minValue(value),_maxValue(value),
                                  _minFound(false),_maxFound(false),
                                  _equal(false) {}

  // Suppose que les valeurs passées en paramètres sont triées par ordre croissant
  bool operator()(const T &v1) {
#ifdef MYDEBUG
    std::cout << "AdjacentFunctor: " << _minValue << _maxValue << std::endl;
    std::cout << "AdjacentFunctor: " << _min << _max << std::endl;
#endif
    if ( v1 <= _minValue && v1 >= _maxValue)    
    {
      _equal= true;
#ifdef MYDEBUG
      std::cout << "AdjacentFunctor: _equal : " << v1 << std::endl;   
#endif
      return true; 
    }
    if ( v1 < _minValue )    
    {
      _min=v1;_minFound=true;
#ifdef MYDEBUG
      std::cout << "AdjacentFunctor: _minFound : " <<_min << std::endl;
#endif
    }
    else if ( v1 > _maxValue )
    {
      _max=v1;_maxFound=true;
#ifdef MYDEBUG
      std::cout << "AdjacentFunctor: _maxFound : " <<_max << std::endl;
#endif
    }


    /*
    if ( v1 < _minValue)    {
      std::cout << "EE1: _min : " << _min << std::endl;
      _min=v1;_minFound=true;
      std::cout << "AdjacentFunctor: _minFound : " <<_min << ", _minValue " << _minValue << std::endl;
    } else if ( v1 > _maxValue ) {
      _max=v1;_maxFound=true;
      std::cout << "AdjacentFunctor: _maxFound : " <<_max << ", _maxValue " << _maxValue << std::endl;
    } else {
      _equal= true;
      std::cout << "AdjacentFunctor: _equal : " << v1<< ", _minValue " << _minValue << ", _maxValue " << _maxValue << std::endl;   
      return true; 
    } // end if
    */
    
    //std::cout << "AdjacentFunctor: _minFound : " <<_min << ", _maxFound " << _max << std::endl;
    return  ( _minFound && _maxFound );
  }

  void setMaxValue(const T & value) {_maxValue = value;}
  bool isEqual()   const { return _equal;}
  bool isBounded() const { return _minFound && _maxFound;}
  bool getBounds(TNoConst & min, TNoConst & max) const {
#ifdef MYDEBUG
    std::cout << "_minFound : " <<_minFound << ", _maxFound " << _maxFound << std::endl;
#endif
    if (_minFound && _maxFound ) { min=_min; max=_max; return true; }
    return false;
  }
  void reset() { _minFound = false; _maxFound = false; _equal = false; };
};

#endif
