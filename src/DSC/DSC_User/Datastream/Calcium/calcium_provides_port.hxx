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
// Date        : $LastChangedDate: 2007-02-28 15:26:32 +0100 (Wed, 28 Feb 2007) $
// Id          : $Id$
//
#ifndef _CALCIUM_PROVIDES_PORT_HXX_
#define _CALCIUM_PROVIDES_PORT_HXX_

#include "provides_port.hxx"
#include "CalciumTypes.hxx"

class calcium_provides_port : public provides_port
{
public :

  typedef CalciumTypes::DependencyType       DependencyType;
  typedef CalciumTypes::DateCalSchem         DateCalSchem;
  typedef CalciumTypes::InterpolationSchem   InterpolationSchem;
  typedef CalciumTypes::ExtrapolationSchem   ExtrapolationSchem;
  typedef CalciumTypes::DisconnectDirective  DisconnectDirective;  

  calcium_provides_port();
  virtual ~calcium_provides_port();

  virtual void           setDependencyType (DependencyType dependencyType) =0;
  virtual DependencyType getDependencyType () const =0;
 
  virtual  void   setStorageLevel   (size_t storageLevel) =0;
  virtual  size_t getStorageLevel   () const =0;

  virtual  void         setDateCalSchem   (DateCalSchem   dateCalSchem) =0;
  virtual  DateCalSchem getDateCalSchem () const =0;
  
  virtual  void   setAlpha(double alpha) =0;
  virtual  double getAlpha() const  =0;

  virtual  void   setDeltaT(double deltaT ) =0;
  virtual  double getDeltaT() const  =0;

  virtual  void setInterpolationSchem (InterpolationSchem interpolationSchem) =0;
  virtual  void setExtrapolationSchem (ExtrapolationSchem extrapolationSchem) =0;
  virtual  InterpolationSchem getInterpolationSchem () const  =0;
  virtual  ExtrapolationSchem getExtrapolationSchem () const  =0;

  virtual void calcium_erase (float t,long i, bool before) {};
};

#endif

