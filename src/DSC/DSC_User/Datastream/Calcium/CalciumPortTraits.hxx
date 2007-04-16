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
//  File   : CalciumPortTraits.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-28 15:26:32 +0100 (mer, 28 f√©v 2007) $
// Id          : $Id$

#ifndef _PORT_TRAITS_HXX_
#define _PORT_TRAITS_HXX_

#include "Superv_Component_i.hxx"


struct UnknownPortType {};
template <class T> struct ProvidesPortTraits {
  typedef  UnknownPortType PortType;
};
template <> struct ProvidesPortTraits<int> {
  typedef  calcium_integer_port_provides PortType;
};
template <> struct ProvidesPortTraits<long> {
  typedef  calcium_integer_port_provides PortType;
};
template <> struct ProvidesPortTraits<float> {
  typedef  calcium_real_port_provides PortType;
};
template <> struct ProvidesPortTraits<double> {
  typedef  calcium_double_port_provides PortType;
};
template <> struct ProvidesPortTraits<char *> {
  typedef  calcium_string_port_provides PortType;
};
template <> struct ProvidesPortTraits<bool> {
  typedef  calcium_logical_port_provides PortType;
};

// DÈfinition du type cplx pour disciminer ce type de port
// de celui du float 
struct cplx {};
template <> struct ProvidesPortTraits<cplx> {
   typedef calcium_complex_port_provides PortType;
};

template < typename T > struct StarTrait        { typedef  T NonStarType; };
template < typename T > struct StarTrait< T * > { typedef  T NonStarType; };

template <class T> struct UsesPortTraits {
  typedef  UnknownPortType PortType;
};
template <> struct UsesPortTraits<int> {
  typedef  calcium_integer_port_uses PortType;
};
template <> struct UsesPortTraits<long> {
  typedef  calcium_integer_port_uses PortType;
};
template <> struct UsesPortTraits<float> {
  typedef  calcium_real_port_uses PortType;
};
template <> struct UsesPortTraits<double> {
  typedef  calcium_double_port_uses PortType;
};
template <> struct UsesPortTraits<char *> {
  typedef  calcium_string_port_uses PortType;
};
template <> struct UsesPortTraits<bool> {
  typedef  calcium_logical_port_uses PortType;
};
template <> struct UsesPortTraits<cplx> {
   typedef  calcium_complex_port_uses PortType;
};

#endif
