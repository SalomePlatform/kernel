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

//  File   : ProcessTimeIntervalTraits.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-01-08 19:01:14 +0100 (lun, 08 jan 2007) $
// Id          : $Id$
//
#ifndef _PROCESS_TIME_INTERVAL_TRAIT_HXX_
#define _PROCESS_TIME_INTERVAL_TRAIT_HXX_

#include "IteratorTraits.hxx"

// Par défaut les classes définissant un mode de couplage n'implémentent pas
// de traitement particulier pour une de demande de données identifiées à partir
// d'un dataId non reçu mais encadré par deux dataIds
// Ce trait peut être spécialisé pour la classe de couplage pour effectuer
// le traitement de son choix
template <class WhatEver > struct ProcessTimeIntervalTraits {
    
  template < typename Iterator > 
  std::pair<bool, typename iterator_t<Iterator>::value_type > 
  static inline apply(typename WhatEver::DataId dataId, Iterator it1) {
    typedef typename iterator_t<Iterator>::value_type value_type; 
    return std::make_pair<bool,value_type> (0,0);
  }
};

// class CalciumCoulpingPolicy;
// template < typename Iterator > 
// typename iterator_t<Iterator>::value_type
// CalciumCoulpingPolicy::processTimeInterval (DataId & dataId,
//                                          Iterator  & it1, Iterator  & it2);
#include "CalciumCouplingPolicy.hxx" 
template <> struct ProcessTimeIntervalTraits<CalciumCouplingPolicy> {
    
  template < class Iterator > 
  std::pair<bool,typename iterator_t<Iterator>::value_type> 
  static inline apply(CalciumCouplingPolicy::DataId dataId, Iterator it1) {

    Iterator it2=it1; it2++;

    typedef typename iterator_t<Iterator>::value_type value_type; 
    return std::make_pair<bool,value_type> (1,processTimeInterval(dataId,it1,it2));
  }
};

#endif
