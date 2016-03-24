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

//  File   : CalciumPortTraits.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-28 15:26:32 +0100 (mer, 28 f√©v 2007) $
// Id          : $Id$
//
#ifndef _PORT_TRAITS_HXX_
#define _PORT_TRAITS_HXX_

#include "Superv_Component_i.hxx"
#include "calcium_port_factory.hxx"

// PROVIDES PORT TRAITS
template <class T> struct UnknownProvidesPortType {};

template <class T> struct ProvidesPortTraits {
  typedef  UnknownProvidesPortType<T> PortType;
};
struct integer {};
template <> struct ProvidesPortTraits<integer> {
  typedef  calcium_integer_port_provides PortType;
};
template <> struct ProvidesPortTraits<int> {
  typedef  calcium_intc_port_provides PortType;
};
template <> struct ProvidesPortTraits<long> {
  typedef  calcium_long_port_provides PortType;
};
template <> struct ProvidesPortTraits<float> {
  typedef  calcium_real_port_provides PortType;
};
template <> struct ProvidesPortTraits<double> {
  typedef  calcium_double_port_provides PortType;
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
// DÈfinition du type str pour obtenir le type de port
// correspondant
struct str {};
template <> struct ProvidesPortTraits<str> {
   typedef calcium_string_port_provides PortType;
};
// DÈfinition du type intc pour obtenir le type de port
// correspondant
struct intc {};
template <> struct ProvidesPortTraits<intc> {
   typedef calcium_intc_port_provides PortType;
};


template < typename T > struct StarTrait        { typedef  T NonStarType; };
template < typename T > struct StarTrait< T * > { typedef  T NonStarType; };


// USES PORT TRAITS
template <class T> struct UnknownUsesPortType {};
template <class T> struct UsesPortTraits {
  typedef  UnknownUsesPortType<T> PortType;
};
template <> struct UsesPortTraits<integer> {
  typedef  calcium_integer_port_uses PortType;
};
template <> struct UsesPortTraits<int> {
  typedef  calcium_intc_port_uses PortType;
};
template <> struct UsesPortTraits<long> {
  typedef  calcium_long_port_uses PortType;
};
template <> struct UsesPortTraits<float> {
  typedef  calcium_real_port_uses PortType;
};
template <> struct UsesPortTraits<double> {
  typedef  calcium_double_port_uses PortType;
};
template <> struct UsesPortTraits<str> {
  typedef  calcium_string_port_uses PortType;
};
template <> struct UsesPortTraits<bool> {
  typedef  calcium_logical_port_uses PortType;
};
template <> struct UsesPortTraits<cplx> {
   typedef  calcium_complex_port_uses PortType;
};
template <> struct UsesPortTraits<intc> {
   typedef  calcium_intc_port_uses PortType;
};

#endif
