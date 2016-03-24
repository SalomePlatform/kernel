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

//  File   : Copy2UserSpace.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-13 11:09:09 +0100 (mar, 13 fÃ©v 2007) $
// Id          : $Id$
//
#ifndef _COPY_TO_USER_SPACE_HXX_
#define _COPY_TO_USER_SPACE_HXX_

#include <string>
#include <iostream>
#include "CalciumPortTraits.hxx"

#include <cstdio>

//#define MYDEBUG

//Les demandes de copies vers l'espace utilisateur
//proviennent d'une procédure de lecture  


//Cas du zero copie
template <bool zerocopy, typename DataManipulator >
struct Copy2UserSpace{
  
  template <class T1, class T2>
  static void apply( T1 * & data, T2 & corbaData, size_t nRead ){

     typedef typename DataManipulator::InnerType       InnerType;

    // OLD:Devient propriétaire des données contenues dans la structure CORBA
    // OLD:(allouées par allocbuff() pour une séquence)
    // OLD:Le client est propriétaire des données.
    // OLD:Il doit cependant être attentif au fait que s'il les modifie,
    // OLD:une nouvelle demande de lecture lui fournira les données modifiées.
    // OLD:TODO : Si plusieurs lecteurs demandent la même donnée, 
    // OLD:       ? qui devient le propriétaire? --> Forcément le premier car
    // OLD:       ensuite la séquence n'est plus propriétaire et rendra un pointeur NULL.
    // OLD:     NO: Le port devrait resté propriétaire du contenu de la séquence
    // OLD:     NO: L'utilisateur doit de toute les façons utiliser les données reçues en
    // OLD:     NO: lecture seulement car si une nouvelle demande de lecture est formulée
    // OLD:     NO: pour ces données, les eventuelles modifications seraient visibles !
    // OLD:YES : La solution de donner la propriété à l'utilisateur est convenable car si
    // OLD:le port déréférence ces données (garbage collecteur, niveau) le buffer
    // OLD:reste disponible à l'ulisateur en lecture et écriture
    // OLD:Le problème est que la donnée CORBA stockée par le port est maintenant vide (cf CORBA BOOK)
    // OLD:du coup quid d'une nouvelle demande de lecture : A TESTER 

     // Le PORT doit être capable de répondre aux demandes de lecture
     // multiples d'une donnée pour une même estampille et doit donc garder un pointeur valide
     // sur le buffer. Il se pose cependant un problème s'il décide
     // de supprimer la donnée alors que des client utilise le buffer (historique calcium) !
     // La seule façon de gérer proprement cette situation est d'utiliser un shared_pointer (TODO).
     // Pour l'instant l'utilisateur du mode zero copie doit s'assurer que le niveau d'historique
     // utilisé par le port est compatible avec son utilisation des buffers. Il doit
     // être également conscient que s'il modifie le buffer, il est modifié pour tous les
     // utilisateurs actuels et futurs.
    
     //REF:    InnerType * dataPtr  = DataManipulator::getPointer(corbaData,true);
     // Laisse la propriété des données à la structure CORBA
     // (buffer allouée par allocbuff() pour une séquence)
     InnerType * dataPtr  = DataManipulator::getPointer(corbaData,false);

    // Cette ligne poserait uun problème dans la méthode appelante, si elle
    // ne testait pas que les types utilisateurs et CORBA sont identiques :
    // ex :  InnerType == Corba::Long et d'un T == int
    // C'est l'objet de la spécialisation ci-dessous.
    data = dataPtr; 

    // En zero copie l'utilisateur doit appeler ecp_free ( cas ou un buffer intermédiaire
    // a été alloué pour cause de typage différent xor necessité de désalouer le buffer alloué par CORBA)
    // L'utilisateur doit cependant être attentif au fait qu'après désallocation, si la donnée
    // est toujours estampillée dans le port une nouvelle lecture pour cette estampille
    // rendrait un buffer vide.
  }
};

// Cas où il faut effectuer une recopie
template <typename DataManipulator>
struct Copy2UserSpace<false, DataManipulator> {

  //Recopie le contenu de la donnée CORBA dans le buffer utilisateur de longueur nRead
  template <class T1, class T2>
  static void apply( T1 * &data, T2 & corbaData, size_t nRead){

    typedef typename DataManipulator::InnerType        InnerType;
    
  
#ifdef MYDEBUG
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
    //Cette affichage peut provoquer la détection d'écriture d'un espace non initailisé.
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
      
#ifdef MYDEBUG
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
template <bool rel, typename DataManipulator >
struct DeleteTraits {
  template <typename T> 
  static void apply(T * dataPtr) {

    typedef typename DataManipulator::Type         DataType; // Attention != T
    
    // Attention : Seul CalciumCouplingPolicy via eraseDataId doit décider de supprimer ou non
    // la donnée corba associée à un DataId ! 
    // Ne pas effectuer la desallocation suivante :
    // DataManipulator::relPointer(dataPtr);
  }
};

// Désalocation du buffer intermédiaire 
// dans le cas d'un type Utilisateur différent du type CORBA 
template <typename DataManipulator>
struct DeleteTraits< false, DataManipulator > {

  template <typename T> 
  static void apply(T * dataPtr) { delete[] dataPtr; }

};

#endif
