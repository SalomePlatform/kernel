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
//  File   : Copy2UserSpace.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-13 11:09:09 +0100 (mar, 13 fÃ©v 2007) $
// Id          : $Id$

#ifndef _COPY_TO_USER_SPACE_HXX_
#define _COPY_TO_USER_SPACE_HXX_

#include <string>
#include <iostream>
#include "CalciumPortTraits.hxx"

//Cas du zero copie
template <bool zerocopy >
struct Copy2UserSpace{
  
  template <class T1, class T2>
  static void apply( T1 * & data, T2 & corbaData, size_t nRead ){

    typedef typename ProvidesPortTraits<T1>::PortType PortType;
    typedef typename PortType::DataManipulator        DataManipulator;
    typedef typename DataManipulator::InnerType       InnerType;

    // Devient propriétaire des données contenues dans la structure CORBA
    // (allouées par allocbuff() pour une séquence)
    InnerType * dataPtr  = DataManipulator::getPointer(corbaData,true);

    // Cette ligne poserait uun problème dans la méthode appelante, si elle
    // ne testait pas que les types utilisateurs et CORBA sont identiques :
    // ex :  InnerType == Corba::Long et d'un T == int
    data = dataPtr; 

    // L'utilisateur a la charge de la desallocation
    // il devra appeler la méthode ecp_free pour désallouer le contenu de la séquence CORBA
    // La structure CORBA sera désallouer le cas échéant dans la méthode appelante
  }
};

// Cas où il faut effectuer une recopie
template <>
struct Copy2UserSpace<false>{

  //Recopie le contenu de la donnée CORBA dans le buffer utilisateur de longueur
  //nRead
  template <class T1, class T2>
  static void apply( T1 * &data, T2 & corbaData, size_t nRead){

    typedef typename ProvidesPortTraits<T1>::PortType  PortType;
    typedef typename PortType::DataManipulator         DataManipulator;
    typedef typename DataManipulator::InnerType        InnerType;

    size_t corbaDataSize = DataManipulator::size      (corbaData);
    // Récupère un pointeur sur les données contenues dans la structure CORBA sans en devenir propriétaire
    InnerType * dataPtr  = DataManipulator::getPointer(corbaData,false);

    // Attention : Pour les chaines ou tout autre object complexe il faut utiliser une recopie profonde !
    // Recopie des données dans le buffer allouée par l'utilisateur 
    // OU 
    // Recopie des données dans le buffer allouée la méthode appelante
    // dans le cas d'une demande utilisateur 0 copie mais sur 
    // des types utilisateurs et CORBA incompatibles.
#ifdef _DEBUG_
    std::cerr << "-------- Copy2UserSpace<false> MARK 1 --(dataPtr : " <<dataPtr<<")----------------" << std::endl;
#endif
    
    std::copy(dataPtr,dataPtr+nRead,data);
 
#ifdef _DEBUG_
    std::cerr << "-------- Copy2UserSpace<false> MARK 2 --(nRead: "<<nRead<<")-------------" << std::endl;
 
    std::cerr << "-------- Copy2UserSpace<false> MARK 3  --(data : " <<data<<") :";
    std::copy(data,data+nRead,std::ostream_iterator<T1>(std::cout," "));
    std::cout << std::endl;
#endif

 }
};


template <bool rel>
struct DeleteTraits {
  template <typename T> 
  static void apply(T * dataPtr) {

    typedef typename ProvidesPortTraits<T>::PortType     PortType;
    typedef typename PortType::DataManipulator          DataManipulator;
    //typedef typename DataManipulator::Type         DataType; // Attention != T
    
    // Attention : Seul CalciumCouplingPolicy via eraseDataId doit décider de supprimer ou non
    // la donnée corba associée à un DataId ! Ne pas effectuer la desallocation suivante :
    // DataManipulator::relPointer(dataPtr);
  }
};

// Cas où une recopie avait été effectuée
template <>
struct DeleteTraits<false>{
  template <typename T> 
  static void apply(T * dataPtr) {
    delete[] dataPtr;
  }
};

#endif
