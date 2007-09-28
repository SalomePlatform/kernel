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
//  File   : CorbaTypeManipulator.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-07 18:26:44 +0100 (mer, 07 fÃ©v 2007) $
// Id          : $Id$

#ifndef _TYPE_MANIPULATION_HXX_
#define _TYPE_MANIPULATION_HXX_

#include <iostream>
#include <CORBA.h>

using namespace std;

// Classes manipulation
// -------------------
//
// Ces différentes classes permettent d'unifier la manipulation des
// différents types de données dans un port datastream
// Les données sont maniées par valeur ou par pointeur 
// pour éviter les recopies de gros volume de données 

// Les classes présentes quatre méthodes :
// - clone
// - get_data
// - delete_data
// - dump
// et
// deux type :
// - Type   : Le type CORBA de la donnée manipulée
// - InType : Le mapping CORBA pour un paramètre IN du type manipulé
 

// Cette classe permet de manipuler des types CORBA 
// any, struct, union et sequence (utiliser plutôt les seq_manipulator)
// Ces types sont manipulés par pointeur.
// Les données reçues de CORBA sont systématiquement
// dupliquées pour être conservées.
// Quelque soit le type de donnée, les données sont considérées 
// comme une donnée unique (retour de size() == 1)
template <typename T >
class user_type_manipulation
{
public:
  typedef T *       Type;
  // correspond au mapping corba des type any, struct, 
  //                  union, séquence en paramètre IN
  typedef const T & CorbaInType; 
  typedef T         InnerType;

  // Operation de recuperation des donnees venant de l'ORB et
  //  creation d'une copie (memoire spécialement allouee)
  static inline Type get_data(CorbaInType data) {
    return new T(data);
  }

  // Pb si ownerShip == True car appel par l'utilisateur de relPointer !
  static inline InnerType * const getPointer(Type data, bool ownerShip = false) {
    return data;
  }

  static inline void relPointer(InnerType * dataPtr) {
    delete dataPtr;
  }

  // Operation de clonage : par defaut, creation d'une copie en memoire allouee pour l'occasion
  static inline Type clone(Type data) { 
    return new T (* data);
  } 
  static inline Type clone(CorbaInType data) {
    return new T (data);
  }

  // Operation de création
  static inline Type create (size_t size=1) { 
    return new T();
  } 

  // Operation de destruction d'une donnee
  static inline void delete_data(Type data) {
    delete data;
  }
  
  // Renvoie la taille de la donnée
  static inline size_t size(Type data) { 
    return 1;
  } 

  // Dump de l'objet pour deboguage: neant car on ne connait pas sa structure
  static inline void dump (CorbaInType data) {}
};


// Gére les types CORBA atomiques ('Int', 'Char', 'Float', ...)
// Gére les types enums
// Gére les références d'objets CORBA
// Ces types sont manipulés par valeur
// Les méthodes getPointer ... ne devrait pas être utilisée
// pour ce types de données
template <typename T>
class atom_manipulation
{
public:
  typedef T Type;
  // correspond au mapping corba des types simples en paramètre IN
  typedef T CorbaInType; 
  typedef T InnerType; 

    
  // Operation de recuperation des donnees venant de l'ORB : une copie par affectation simple
  static inline Type get_data(CorbaInType data) {
    return data;
  }

 static inline InnerType * const getPointer(Type & data, bool getOwnerShip = false) {
//    InnerType * ptr;
//     if (getOwnerShip) {
//       ptr =new InnerType[1];*ptr=data;
//       return ptr;
//     } else
//      return &data;
   return &data;
 }

//   static inline void relPointer(InnerType * dataPtr) {
//     return;
//         delete[] dataPtr;
//   }

// Je ne sais pas comment l'implémenter sans faire
// d'allocation heap
//static inline InnerType * allocPointer(size_t size=1) {
//    return  new InnerType[1];
  //}

  // Operation de clonage : une copie par affectation simple
  static inline Type clone(Type data) {
    return data;
  }

  // Inutile car Type == CorbaInType
  //   static inline Type clone(CorbaInType data) {
  //     return data;
  //   }

  // Operation de création
//   static inline Type create(size_t size=1,InnerType * data=NULL,
// 			    bool giveOwnerShip=false) {
//     Type dummy;
//     if (dataPtr)
//       return *data;
//     else
//       return dummy;
//   } 
    
  // Operation de destruction d'une donnee: rien a faire car pas de memoire a liberer
  static inline void delete_data(Type data) {}
    // Renvoie la taille de la donnée

  static inline size_t size(Type data) { 
    return 1;
  } 

  // Dump de l'objet pour deboguage : Affiche la donnee
  static void inline dump (CorbaInType data) {
    cerr << "[atom_manipulation] Data : " << data << endl;
  }
};


// Gére un type sequence de taille illimitee (Unbounded)
// Ces types sont manipulés par pointeur
template <typename seq_T,typename elem_T>
class seq_u_manipulation {
  
public:
  typedef seq_T * Type;
  // correspond au mapping corba de la séquence en paramètre IN
  typedef const seq_T & CorbaInType; 
  typedef elem_T  InnerType;

 
  // Operation de recuperation des donnees venant de l'ORB
  // Remarque : On a un paramètre d'entrée de type const seq_T &
  //            et en sortie un seq_T *
  static inline Type get_data(CorbaInType data) {
    CORBA::Long len = data.length();
    CORBA::Long max = data.maximum();
    // Récupère et devient propriétaire des données reçues dans la séquence. 
    // La séquence sera désalloué (mais pas le buffer) au retour 
    // de la méthode put (car mapping de type IN : const seq & )
    // ATTENTION TESTER p184 si le pointeur est null
    // ATTENTION TESTER Si le flag release si la sequence contient des chaines
    // ou des object refs
#ifdef _DEBUG_
    std::cout << "----seq_u_manipulation::get_data(..)-- MARK 1 ------------------" << std::endl;
#endif
    InnerType * p_data = const_cast<seq_T &>(data).get_buffer(true); 
#ifdef _DEBUG_
    std::cout << "----seq_u_manipulation::get_data(..)-- MARK 2 ------"<<  p_data <<"------------" << std::endl;
#endif

    // Crée une nouvelle sequence propriétaire des données du buffer (pas de recopie)
    // Les données seront automatiquement désallouées par appel interne à la méthode freebuf
    // lors de la destruction de l'objet par appel à delete_data.
    return  new seq_T (max, len, p_data, true);
  }

  static inline size_t size(Type data) { 
    return data->length();
  } 

  // Operation de destruction d'une donnee
  static inline void delete_data(Type data) {
    delete data;
  }

  // Operation de clonage : par defaut creation d'une copie en memoire allouee pour l'occasion
  // Utilisation du constructeur du type seq_T
  static inline Type clone(Type data) {
    return new seq_T (*data) ;
  }
  static inline Type clone(CorbaInType data) {
    return new seq_T (data);
  }

  // Permet de désallouer le buffer dont on détient le pointeur par appel
  // à la méthode getPointer avec ownerShip=True si la séquence contenante
  // à été détruite.
  static inline InnerType * const getPointer(Type data, bool ownerShip = false) {
    InnerType * p_data;
    if (ownerShip) {
      p_data = data->get_buffer(true);
      delete_data(data);
    } else
      p_data = data->get_buffer(false);
    return p_data;
  }

  // Permet de désallouer le buffer dont on détient le pointeur par appel
  // à la méthode getPointer avec ownerShip=True si la séquence contenante
  // à été détruite.
  static inline void relPointer(InnerType * dataPtr) {
    seq_T::freebuf(dataPtr);
  }

  // Permet d'allouer un buffer pour la séquence
  static inline InnerType *  allocPointer(size_t size ) {
    return seq_T::allocbuf(size);
  }

  // Operation de création du type corba soit
  // - Vide et de taille size
  // - Utilisant les données du pointeur *data de taille size 
  // (généralement pas de recopie qlq soit l'ownership )
  // data doit avoir été alloué par allocPointer si giveOwnerShip = true  
  static inline Type create(size_t size, InnerType * const data = NULL,
			    bool giveOwnerShip = false ) { 
    Type tmp;
    if (!data) {
      tmp = new seq_T();
      tmp->length(size);
    } else {
      tmp = new seq_T(size,size,data,giveOwnerShip); 
    }
    return tmp;
  } 
  
  // Dump de l'objet pour deboguage
  static void inline dump (CorbaInType data) {
    // Affiche la longueur des donnees
    cerr << "[seq_u_manipulation] Data length: " << data.length() << endl;
    // Affiche la longueur des donnees
    cerr << "[seq_u_manipulation] Data max: " << data.maximum() << endl;
  }
};


// Gére un type sequence de taille limitée (bounded)
// Ces types sont manipulés par pointeur
// Cette classe diffère de la seq_u_manipulation
// par la signature du constructeur de la séquence
// utilisé dans le methode get_data
template <typename seq_T,typename elem_T>
class seq_b_manipulation {
  
public:
  typedef seq_T *       Type;
  typedef const seq_T & CorbaInType;
  typedef elem_T        InnerType;


  // Operation de recuperation des donnees venant de l'ORB
  // Sans opération de notre part, ces données seraient perdues
  // au retour de la méthode put de GenericPort.
  // Remarque : On a un paramètre d'entrée de type const seq_T &
  //            et en sortie un seq_T *
  static inline Type get_data(CorbaInType data) {
    CORBA::Long len = data.length();
    // Récupère et devient propriétaire des données reçues dans la séquence 
    // la séquence sera désalloué (mais pas le buffer)
    // au retour de la méthode put (car mapping de type IN : const seq & )
    InnerType * p_data = const_cast<seq_T &>(data).get_buffer(true);
    // Crée une nouvelle sequence propriétaire des données du buffer (généralement pas de recopie)
    // Les données seront automatiquement désallouées par appel interne à la méthode freebuf
    // lors de la destruction de l'objet par appel à delete_data.
    return new seq_T (len, p_data, true);
  }

  static inline size_t size(Type data) { 
    return data->length();
  } 

  // Operation de clonage : par defaut creation d'une copie en memoire allouee pour l'occasion
  // Utilisation du constructeur du type seq_T  
  static inline Type clone(Type data) {
    return new seq_T (* data);
  }
  static inline Type clone(CorbaInType data) {
    return new seq_T (data);
  }

  // Operation de destruction d'une donnee CORBA
  static inline void delete_data(Type data) {
    delete data;
  }

  // Récupère un pointeur sur les données de type InnerType contenue dans la séquence
  // si ownership=True, l'utilisateur devra appeler relPointer
  // si ownership=False, l'utilisateur devra appeler delete_data sur la séquence contenante
  static inline InnerType * const getPointer(Type data, bool getOwnerShip = false) {
    InnerType * p_data;
    if (getOwnerShip) {
      p_data = data->get_buffer(true);
      delete_data(data);
    } else
      p_data = data->get_buffer(false);
    return p_data;
  }

  // Permet de désallouer le buffer dont on détient le pointeur par appel
  // à la méthode getPointer avec ownerShip=True si la séquence contenante
  // à été détruite.
  static inline void relPointer(InnerType * dataPtr) {
    seq_T::freebuf(dataPtr);
  }

  // Permet d'allouer un buffer pour la séquence
  static inline InnerType *  allocPointer(size_t size ) {
    return seq_T::allocbuf(size);
  }

  // Operation de création du type corba soit
  // - Vide et de taille size
  // - Utilisant les données du pointeur *data de taille size 
  // (généralement pas de recopie qlq soit l'ownership )
  // data doit avoir été alloué par allocPointer si giveOwnerShip = true  
  static inline Type create(size_t size, InnerType * const data = NULL,
			    bool giveOwnerShip = false ) { 
    Type tmp;
    if (!data) {
      tmp = new seq_T();
      tmp->length(size);
    } else {
      tmp = new seq_T(size,data,giveOwnerShip); 
    }
    return tmp;
  } 

  
  // Dump de l'objet pour deboguage
  static inline void dump (CorbaInType data) {
    // Affiche la longueur des donnees
    cerr << "[seq_b_manipulation] Data length: " << data.length() << endl;
  }
};

#endif
