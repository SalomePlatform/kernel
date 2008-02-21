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

#include <cstdio>

//Les demandes de copies vers l'espace utilisateur
//proviennent d'une procédure de lecture  

//Cas du zero copie
template <bool zerocopy >
struct Copy2UserSpace{
  
  template <class T1, class T2>
  static void apply( T1 * & data, T2 & corbaData, size_t nRead ){

    // La ligne suivante appelle à un commentaire
    // dans le cas de char *, cf CalciumPortTraits.hxx 'char *' vs 'str'
    typedef typename ProvidesPortTraits<T1>::PortType PortType;
    typedef typename PortType::DataManipulator        DataManipulator;
    typedef typename DataManipulator::InnerType       InnerType;

    // Devient propriétaire des données contenues dans la structure CORBA
    // (allouées par allocbuff() pour une séquence)
    // Le client est propriétaire des données.
    // Il doit cependant être attentif au fait que s'il les modifie,
    // une nouvelle demande de lecture lui fournira les données modifiées.
    // TODO : ? Si plusieurs lecteurs demandent la même donnée ? 
    //        ? qui devient le propriétaire? --> normalement le premier car
    //        ensuite la séquence n'est plus propriétaire.
    //      NO: Le port devrait resté propriétaire du contenu de la séquence
    //      NO: L'utilisateur doit de toute les façons utiliser les données reçues en
    //      NO: lecture seulement car si une nouvelle demande de lecture est formulée
    //      NO: pour ces données, les eventuelles modifications seraient visibles !
    // YES : La solution de donner la propriété à l'utilisateur est convenable car si
    // le port déréférence ces données (garbage collecteur, niveau) le buffer
    // reste disponible à l'ulisateur en lecture et écriture
    // Le problème est que la donnée CORBA stockée par le port est maintenant vide (cf CORBA BOOK)
    // du coup quid d'une nouvelle demande de lecture : A TESTER 
    InnerType * dataPtr  = DataManipulator::getPointer(corbaData,true);

    // Cette ligne poserait uun problème dans la méthode appelante, si elle
    // ne testait pas que les types utilisateurs et CORBA sont identiques :
    // ex :  InnerType == Corba::Long et d'un T == int
    // C'est l'objet de la procédure suivante
    data = dataPtr; 

    // En zero copie l'utilisateur doit appeler ecp_free ( cas ou un buffer intermédiaire
    // a été alloué pour cause de typage différent xor necessité de désalouer le buffer alloué par CORBA)
    // L'utilisateur doit cependant être attentif au fait qu'après désallocation, si la donnée
    // est toujours estampillée dans le port une nouvelle lecture pour cette estampille
    // rendrait un buffer vide.
  }
};

// Cas où il faut effectuer une recopie
template <>
struct Copy2UserSpace<false> {

  //Recopie le contenu de la donnée CORBA dans le buffer utilisateur de longueur nRead
  template <class T1, class T2>
  static void apply( T1 * &data, T2 & corbaData, size_t nRead){

    // La ligne suivante appelle à un commentaire
    // dans le cas de char *, cf CalciumPortTraits.hxx 'char *' vs 'str'
    typedef typename ProvidesPortTraits<T1>::PortType  PortType;
    typedef typename PortType::DataManipulator         DataManipulator;
    typedef typename DataManipulator::InnerType        InnerType;
    
  
#ifdef _DEBUG_
    InnerType * dataPtr = NULL;
      // Affiche la valeur du pointeur de la structure corba
      //  et les pointeurs contenus le cas échéant
      dataPtr  = DataManipulator::getPointer(corbaData,false);
      std::cerr << "-------- Copy2UserSpace<false> MARK 1a --dataPtr("<<dataPtr<<")[0.."<<
	DataManipulator::size(corbaData) <<"] : ----------------" << std::endl;
      std::copy(dataPtr,dataPtr+DataManipulator::size(corbaData),std::ostream_iterator<T1>(std::cerr," "));
      for (int i=0; i< DataManipulator::size(corbaData); ++i) 
	fprintf(stderr,"pointer[%d]=%p ",i, dataPtr[i]);
      std::cerr << std::endl;

      T1 * tmpData = data;
      std::cerr << "-------- Copy2UserSpace<false> MARK 1b --data("<<tmpData<<")[0.."<<
	DataManipulator::size(corbaData) <<"] : ----------------" << std::endl;
      std::copy(tmpData,tmpData+DataManipulator::size(corbaData),std::ostream_iterator<T1>(std::cerr," "));
      for (int i=0; i< DataManipulator::size(corbaData); ++i) 
	fprintf(stderr,"pointer[%d]=%p ",i, tmpData[i]);
      std::cerr << std::endl;
#endif

      // Pour les types pointeurs et ref il faut effectuer une recopie profonde.
      // On la délègue au manipulateur de données. 
      
      // Recopie des données dans le buffer allouée par l'utilisateur 
      // OU 
      // Recopie des données dans le buffer allouée par la méthode appelante (ex: lecture)
      // dans le cas d'une demande utilisateur 0 copie mais que types utilisateurs et CORBA incompatibles.
    
      //std::copy(dataPtr,dataPtr+nRead,data);
      DataManipulator::copy(corbaData,data,nRead);
      
#ifdef _DEBUG_
      tmpData = data;
      std::cerr << "-------- Copy2UserSpace<false> MARK 1c --data("<<tmpData<<")[0.."<<
	DataManipulator::size(corbaData) <<"] : ----------------" << std::endl;
      std::copy(tmpData,tmpData+DataManipulator::size(corbaData),std::ostream_iterator<T1>(std::cerr," "));
      for (int i=0; i< DataManipulator::size(corbaData); ++i) 
	fprintf(stderr,"pointer[%d]=%p ",i, tmpData[i]);
      std::cerr << std::endl;
#endif
    
  }
  
};


// Désallocation des buffers si necessaire
template <bool rel>
struct DeleteTraits {
  template <typename T> 
  static void apply(T * dataPtr) {

    typedef typename ProvidesPortTraits<T>::PortType     PortType;
    typedef typename PortType::DataManipulator          DataManipulator;
    //typedef typename DataManipulator::Type         DataType; // Attention != T
    
    // Attention : Seul CalciumCouplingPolicy via eraseDataId doit décider de supprimer ou non
    // la donnée corba associée à un DataId ! 
    // Ne pas effectuer la desallocation suivante :
    // DataManipulator::relPointer(dataPtr);
    // TODO : Il convient cependant de rendre la propriété du buffer à la séquence CORBA
    // TODO : PB   : On n'a plus de référence sur la séquence. 
    // TODO : Modifier l'API ecp_free pour indiquer le dataId associé ?
    // TODO : ??VERIF accès concurrent à la séquence stockée ?? suppression simultanée ?

  }
};

// Désalocation du buffer intermédiaire 
// dans le cas de types utilisateur/CORBA différents 
template <>
struct DeleteTraits<false>{

  template <typename T> 
  static void apply(T * dataPtr) { delete[] dataPtr; }

};

#endif
