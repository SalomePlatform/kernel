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

//  File   : Copy2CorbaSpace.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-07 18:26:44 +0100 (mer, 07 f√©v 2007) $
// Id          : $Id$
//
#ifndef _COPY_TO_CORBA_SPACE_HXX_
#define _COPY_TO_CORBA_SPACE_HXX_


#include <string>
#include <iostream>
#include "CalciumPortTraits.hxx"

//#define MYDEBUG

template <bool zerocopy, typename DataManipulator> 
struct Copy2CorbaSpace  {

  template <class T1, class T2>
  static void apply( T1 * & corbaData, T2 const & data, size_t nRead){

    typedef typename ProvidesPortTraits<T2>::PortType  PortType;
    //typedef typename UsesPortTraits<T2>::PortType      PortType;
    //ESSAI:     typedef typename PortType::DataManipulator         DataManipulator;
    typedef typename DataManipulator::InnerType        InnerType;

#ifdef MYDEBUG
    std::cerr << "-------- Copy2CorbaSpace<true> MARK 1 ------------------" << std::endl;
#endif
    // CrÈe le type corba ‡ partir du data sans lui en donner la propriÈtÈ.
    // Le const_cast supprime le caractËre const du type T2 const & de data car 
    // DataManipulator::create n'a pas le caractËre const sur son paramËtre data pour le
    // cas de figure o˘  la propriÈtÈ de la donnÈe lui est donnÈe.
    corbaData = DataManipulator::create(nRead,const_cast<T2 * > (&data),false);
#ifdef MYDEBUG
    std::cerr << "-------- Copy2CorbaSpace<true> MARK 2 --(dataPtr : " 
              << DataManipulator::getPointer(corbaData,false)<<")----------------" << std::endl;
#endif

  }
};

// Cas ou il faut effectuer une recopie
template <typename DataManipulator> struct
Copy2CorbaSpace<false, DataManipulator>  {
  
  template <class T1, class T2>
  static void apply( T1 * & corbaData,  T2 const & data, size_t nRead){

    typedef typename ProvidesPortTraits<T2>::PortType  PortType;
    // typedef typename UsesPortTraits<T2>::PortType     PortType;
//ESSAI:    typedef typename PortType::DataManipulator        DataManipulator;
    typedef typename DataManipulator::InnerType       InnerType;

    corbaData = DataManipulator::create(nRead);
    InnerType * dataPtr  = DataManipulator::getPointer(corbaData,false);

#ifdef MYDEBUG
    std::cerr << "-------- Copy2CorbaSpace<false> MARK 1 --(dataPtr : " <<
      dataPtr<<")----------------" << std::endl;
#endif
    // Attention : Pour les chaines ou tout autre object complexe il faut utiliser une recopie profonde !   
    std::copy(&data,&data+nRead,dataPtr);
 
#ifdef MYDEBUG
    std::cerr << "-------- Copy2CorbaSpace<false> MARK 2 --(nRead: "<<nRead<<")-------------" << std::endl;
 
    std::cerr << "-------- Copy2CorbaSpace<false> MARK 3 : " ;
    std::copy(dataPtr,dataPtr+nRead,std::ostream_iterator<InnerType>(std::cout," "));
    std::cout << std::endl;
    std::cerr << "-------- Copy2CorbaSpace<false> MARK 4 --(data : " <<data<<") :" ;
    for (int i=0; i<nRead; ++i)
      std::cerr << (*corbaData)[i] << " ";
    std::cout << std::endl;
#endif
    
  }
};

#endif
