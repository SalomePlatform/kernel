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

//  File   : CorbaTypeManipulator.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-07 18:26:44 +0100 (mer, 07 fÃ©v 2007) $
// Id          : $Id$
//
#ifndef _CORBA_TYPE_MANIPULATION_HXX_
#define _CORBA_TYPE_MANIPULATION_HXX_

#include <iostream>
#include <cstring>
#include <omniORB4/CORBA.h>

//#define MYDEBUG

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
// trois types :
// - Type      : Le type CORBA de la donnée manipulée
// - InType    : Le mapping CORBA pour un paramètre IN du type manipulé
// - InnerType : Type interne des valeurs d'un type contenant 

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
//                          bool giveOwnerShip=false) {
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
    std::cerr << "[atom_manipulation] Data : " << data << std::endl;
  }
};


// Gére un type sequence de taille illimitee (Unbounded)
// Ces types sont manipulés par pointeur
template <typename seq_T,typename elem_T>
class seq_u_manipulation {
  
public:
  typedef seq_T *       Type;        // Type de donnée abstrait manipulé par GenericPort::Put,Get,..
  typedef const seq_T & CorbaInType; // Mapping corba de la séquence en paramètre IN
  typedef elem_T        InnerType;   // Il n'existe pas dans CORBA de seq_T::elem_T
                                     // C'est la raison d'être du second paramètre template de seq_u_mani
 
  // Operation de recuperation des donnees venant de l'ORB
  // Remarque : On a un paramètre d'entrée de type const seq_T &
  //            et en sortie un seq_T *
  static inline Type get_data(CorbaInType data) {
    CORBA::Long len = data.length();
    CORBA::Long max = data.maximum();
    // Récupère et devient propriétaire des données reçues dans la séquence. 
    // La séquence reçue (mais pas le buffer) sera désallouée au retour 
    // de la méthode CORBA qui a reçu le type CorbaInType en paramètre
    // (ex: GenericPort::put)
    // REM : Le mapping CORBA du type séquence IN est : const seq &

    // OLD : On ne teste pas si le flag release de la séquence est à true ou false 
    // OLD : ( pour des séquences de chaines ou d'objrefs )
    // OLD :   -> Si on est collocalisé le port uses doit créer une copie pour éviter la modification
    // OLD : du contenu de la séquence lorsque l'utilisateur modifie ses données dans son programme (0 copie)
    // OLD : ATTENTION TESTER p194 si le pointeur est null (release flag==false)
    // OLD :   -> La séquence n'était pas propriétaire des données !

    // Le flag release() de la séquence est à false si elle n'est pas propriétaire du buffer
    // En  collocalité release() renvoie false car 
    // l'appelé n'est pas propriétaire de la séquence. On effectue alors
    // une copie pour éviter de perturber les structures de données de l'appelant.
    // En non collocalisé on recrée une séquence avec le buffer de la première dont on
    // a demandé la propriété.

#ifdef MYDEBUG
    std::cout << "----seq_u_manipulation::get_data(..)-- MARK 1 ------------------" << std::endl;
#endif
    if ( data.release() ) {
      InnerType * p_data = const_cast<seq_T &>(data).get_buffer(true);

    // Crée une nouvelle sequence propriétaire des données du buffer (pas de recopie)
    // Les données de la nouvelle séquence seront automatiquement désallouées 
    // par appel à la méthode freebuf dans le destructeur de la séquence (cf  delete_data).
#ifdef MYDEBUG
      std::cout << "----seq_u_manipulation::get_data(..)-- MARK 1(0 copy) bis ------"<<  p_data <<"------------" << std::endl;
#endif
    
      return  new seq_T (max, len, p_data, true);
    }
#ifdef MYDEBUG
    std::cout << "----seq_u_manipulation::get_data(..)-- MARK 1(recopie) bis ------"<<  &data <<"------------" << std::endl;
#endif
    // Crée une nouvelle sequence propriétaire des données du buffer (avec recopie)    
    return new seq_T(data);

  }

  static inline size_t size(Type data) { 
    return data->length();
  } 

  // Operation de destruction d'une donnee
  static inline void delete_data(Type data) {
    //La séquence est détruite par appel à son destructeur
    //Ce destructeur prend en compte la nécessité de détruire ou non
    //les données contenues en fonction de son flag interne release()
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

  // Permet d'obtenir un pointeur sur le buffer de la séquence :
  // Si ownerShip=True, la séquence n'est plus propriétaire du buffer
  //         (son pointeur de buffer interne est aussi réinitialisé) 
  //       On détruit également explicitement la séquence (mais pas le buffer !)
  // Si ownerShip=False, la séquence reste propriétaire du buffer
  //    et l'utilisateur devra appeler delete_data sur la séquence contenante pour
  //    détruire à la fois la séquence et le buffer contenu.
  static inline InnerType * const getPointer(Type data, bool ownerShip = false) {
    InnerType * p_data;
    if (ownerShip) {
      p_data = data->get_buffer(true);
      delete_data(data);
    } else
      p_data = data->get_buffer(false);
    return p_data;
  }

  // Permet de désallouer le buffer dont on détient le pointeur après appel
  // à la méthode getPointer avec ownerShip=True 
  static inline void relPointer(InnerType * dataPtr) {
    seq_T::freebuf(dataPtr);
  }

  // Permet d'allouer un buffer compatible avec le type séquence
  static inline InnerType *  allocPointer(size_t size ) {
    return seq_T::allocbuf(size);
  }

  // Opération de création de la séquence CORBA soit
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

  // Copie le contenu de la séquence dans le buffer idata de taille isize
  // pour les types non pointeur
  template <typename T >
  static inline void copy( Type data, T * const idata, size_t isize ) { 
    
    InnerType *dataPtr  = getPointer(data,false);

    for (int i = 0; i< isize; ++i) 
      idata[i]=dataPtr[i];

    // Le mode de recopie suivant ne permet pas  la conversion de type (ex int -> CORBA::Long)
    //OLD:     Type tmp = new seq_T(isize,isize,idata,false); 
    //OLD:     // giveOwnerShip == false -> seul le contenu du buffer data est détruit et remplacé
    //OLD:     // par celui de data dans l'affectation suivante :
    //OLD:     //       ---> ATTENTION SI LA TAILLE DU BUFFER EST TROP PETITE, QUE FAIT CORBA !
    //OLD:     //              corruption mémoire
    //OLD:     // Cependant ce cas devrait pas arrivé (on s'assure dans les couches supérieures
    //OLD:     //  de la taille correcte du buffer de recopie)
    //OLD:     // Si giveOwnerShip était == true -> le buffer et son contenu serait détruit puis une 
    //OLD:     // allocation de la taille du buffer de data serait effectué avant la copie des données  
    //OLD:     // tmp = data;
  } 

  // Copie le contenu de la séquence de char* dans le buffer idata de taille isize
  // La généralisation de la recopie profonde est difficile du fait que CORBA ne renvoie pas
  // pas des objets de haut niveau de type std::vector<std::string> (avec des  interfaces d'accès identiques) 
  // mais un type simple C comme char *Tab[N]. On doit alors utiliser une méthode de recopie spécifique
  // comme l'appel C strcpy.
  static inline void copy( Type data, char* * const idata, size_t isize ) { 

    char* *dataPtr  = getPointer(data,false);

    // Si idata[i] n'a pas été alloué suffisament grand,
    // il y a corruption de la mémoire
    for (int i = 0; i< isize; ++i) 
      strcpy(idata[i],dataPtr[i]);
  }
  
  // Dump de l'objet pour deboguage
  static void inline dump (CorbaInType data) {
    // Affiche la longueur des donnees
    std::cerr << "[seq_u_manipulation] Data length: " << data.length() << std::endl;
    // Affiche la longueur des donnees
    std::cerr << "[seq_u_manipulation] Data max: " << data.maximum() << std::endl;
  }
};


// TODO : Vérifier la conformité de l'implémentation par rapport
//        au type unbounded

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
     if ( data.release() ) {
       InnerType * p_data = const_cast<seq_T &>(data).get_buffer(true);

    // Crée une nouvelle sequence propriétaire des données du buffer (généralement pas de recopie)
    // Les données seront automatiquement désallouées par appel interne à la méthode freebuf
    // lors de la destruction de l'objet par appel à delete_data.
#ifdef MYDEBUG
    std::cout << "----seq_u_manipulation::get_data(..)-- MARK 1bis Pas de Duplication  -----------" << std::endl;
#endif
       return new seq_T (len, p_data, true);
     }
#ifdef MYDEBUG
    std::cout << "----seq_u_manipulation::get_data(..)-- MARK 1bis Duplication pour en devenir propriétaire -----------" << std::endl;
#endif
    // Crée une nouvelle sequence propriétaire des données du buffer (avec recopie)    
    return new seq_T(data);

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

  // Permet d'obtenir un pointeur sur le buffer de la séquence :
  // Si ownerShip=True, la séquence n'est plus propriétaire du buffer
  //         (son pointeur de buffer interne est aussi réinitialisé) 
  //       On détruit également explicitement la séquence (mais pas le buffer !)
  // Si ownerShip=False, la séquence reste propriétaire du buffer
  //    et l'utilisateur devra appeler delete_data sur la séquence contenante pour
  //    détruire à la fois la séquence et le buffer contenu.
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
    std::cerr << "[seq_b_manipulation] Data length: " << data.length() << std::endl;
  }
};

#endif
