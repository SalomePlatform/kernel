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
//  File   : CalciumInterface.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-03-01 13:27:58 +0100 (jeu, 01 mar 2007) $
// Id          : $Id$

#ifndef _CALCIUM_INTERFACE_HXX_
#define _CALCIUM_INTERFACE_HXX_

#include <string>
#include <vector>
#include <iostream>
#include "Superv_Component_i.hxx"
#include "DatastreamException.hxx"
#include "CalciumTypes.hxx"
#include "CalciumGenericUsesPort.hxx"
#include "Copy2UserSpace.hxx"
#include "Copy2CorbaSpace.hxx"
#include "CalciumPortTraits.hxx"

#include <stdio.h>


// Déplacer cette information dans CorbaTypeManipulator
// Gérer en même temps la recopie profonde.

template <typename T1, typename T2>
struct IsSameType {
  static const bool value = false;
};
template <typename T1>
struct IsSameType<T1,T1> {
  static const bool value = true;
};




class CalciumInterface {
public :


  static void
  ecp_fin (Superv_Component_i & component, bool provideLastGivenValue)
  { 
    std::vector<std::string> usesPortNames;
    std::vector<std::string>::const_iterator it;
    component.get_uses_port_names(usesPortNames);    
    
    //récupérer le type de réel su port est un peu difficile
    //car l'interface nous donne aucune indication
    uses_port *myUsesPort;
    
    for (it=usesPortNames.begin(); it != usesPortNames.end(); ++it) {
      try {
	component.Superv_Component_i::get_port(myUsesPort,(*it).c_str());
	calcium_uses_port* myCalciumUsesPort=
	  dynamic_cast<calcium_uses_port*>(myUsesPort);
	std::cerr << "-------- CalciumInterface(ecp_fin) MARK 1 -|"<< *it <<"|----"<< 
	  typeid(myUsesPort).name() <<"-------------" <<
	  typeid(myCalciumUsesPort).name() <<"-------------" << std::endl;
	if ( !myCalciumUsesPort )
	  throw Superv_Component_i::BadCast(LOC(OSS()<<"Impossible de convertir le port "
						<< *it << " en port de type calcium_uses_port." ));
	myCalciumUsesPort->disconnect(provideLastGivenValue);
      } catch ( const Superv_Component_i::PortNotDefined & ex) {
	std::cerr << ex.what() << std::endl;
	throw (DatastreamException(CalciumTypes::CPNMVR,ex));
      } catch ( const Superv_Component_i::PortNotConnected & ex) {
	std::cerr << ex.what() << std::endl;;
	throw (DatastreamException(CalciumTypes::CPLIEN,ex)); 
       } catch ( const Superv_Component_i::BadCast & ex) {
 	std::cerr << ex.what() << std::endl;
 	throw (DatastreamException(CalciumTypes::CPTPVR,ex));
      } // Laisse passer les autres exceptions.
    }
  }


  // Uniquement appelé par l'utilisateur s'il a passé un pointeur de données NULL
  // à l'appel de ecp_lecture (demande de 0 copie)
  template <typename T1 > static void
  ecp_free ( T1 * dataPtr )
  { 
    ecp_free<T1,T1> ( dataPtr );
  }
  
  template <typename T1,typename T2 > static void
  ecp_free ( T1 * dataPtr )
  { 

    typedef typename ProvidesPortTraits<T2>::PortType      PortType;
    typedef typename PortType::DataManipulator            DataManipulator;
    typedef typename DataManipulator::Type                DataType; // Attention != T
    typedef typename DataManipulator::InnerType           InnerType;

    DeleteTraits<IsSameType<T1,InnerType>::value >::apply(dataPtr);

  }

  template <typename T1 > static void
  ecp_lecture ( Superv_Component_i & component,
	       CalciumTypes::DependencyType dependencyType,
	       double        & ti,
	       double const  & tf,
	       long          & i,
	       const string  & nomVar, 
	       size_t bufferLength,
	       size_t & nRead, 
	       T1 * &data )
  {
    ecp_lecture<T1,T1> (component,dependencyType,ti,tf,
			i,nomVar,bufferLength,nRead,data);
  
  }

  template <typename T1, typename T2 > static void
  ecp_lecture ( Superv_Component_i & component,
	       CalciumTypes::DependencyType dependencyType,
	       double        & ti,
	       double const  & tf,
	       long          & i,
	       const string  & nomVar, 
	       size_t bufferLength,
	       size_t & nRead, 
	       T1 * &data )
  {

    assert(&component);

    typedef typename ProvidesPortTraits<T2>::PortType     PortType;
    typedef typename PortType::DataManipulator            DataManipulator;
    typedef typename DataManipulator::Type                CorbaDataType; // Attention != T
    typedef typename DataManipulator::InnerType           InnerType;

    CorbaDataType     corbaData;
    long         ilong;

    std::cerr << "-------- CalciumInterface(ecp_lecture) MARK 1 ------------------" << std::endl;

    if (nomVar.empty())
      throw DatastreamException(CalciumTypes::CPNMVR,
				LOC("Le nom de la variable est <nul>"));
    PortType * port;
    std::cout << "-------- CalciumInterface(ecp_lecture) MARK 2 ------------------" << std::endl;

    try {
      port  = component.Superv_Component_i::get_port< PortType > (nomVar.c_str());
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 3 ------------------" << std::endl;
    } catch ( const Superv_Component_i::PortNotDefined & ex) {
      std::cerr << ex.what() << std::endl;
      throw (DatastreamException(CalciumTypes::CPNMVR,ex));
    } catch ( const Superv_Component_i::PortNotConnected & ex) {
      std::cerr << ex.what() << std::endl;;
      throw (DatastreamException(CalciumTypes::CPLIEN,ex)); 
      // VERIFIER LES CAS DES CODES : CPINARRET, CPSTOPSEQ, CPCTVR, CPLIEN
    } catch ( const Superv_Component_i::BadCast & ex) {
      std::cerr << ex.what() << std::endl;
      throw (DatastreamException(CalciumTypes::CPTPVR,ex));
    }
  
    // mode == mode du port 
    CalciumTypes::DependencyType portDependencyType;
    try {
      portDependencyType = port->getDependencyType();
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 4 ------------------" << std::endl;
    } catch ( const DSC_Exception & ex ) {
      std::cerr << ex.what() << std::endl;;
      throw (DatastreamException(CalciumTypes::CPIT,ex));
    }

    if ( portDependencyType == CalciumTypes::UNDEFINED_DEPENDENCY )
      throw DatastreamException(CalciumTypes::CPIT,
				LOC(OSS()<<"Le mode de dépendance de la variable " 
				    << nomVar << " est indéfini."));

    if ( ( portDependencyType != dependencyType ) && 
	 ( dependencyType != CalciumTypes::SEQUENCE_DEPENDENCY ) ) 
      throw DatastreamException(CalciumTypes::CPITVR,
				LOC(OSS()<<"Le mode de dépendance de la variable " 
				    << nomVar << " ne correspond pas au mode demandé."));

  
    if ( dependencyType == CalciumTypes::TIME_DEPENDENCY ) {
      corbaData = port->get(ti,tf, 0);
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 5 ------------------" << std::endl;
    } 
    else if ( dependencyType == CalciumTypes::ITERATION_DEPENDENCY ) {
      corbaData = port->get(0, i);
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 6 ------------------" << std::endl;
    } else {
      // Lecture en séquence
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 7 ------------------" << std::endl;
      corbaData = port->next(ti,i);
    }
 
    std::cout << "-------- CalciumInterface(ecp_lecture) MARK 8 ------------------" << std::endl;
    size_t corbaDataSize = DataManipulator::size(corbaData);
    std::cout << "-------- CalciumInterface(ecp_lecture) corbaDataSize : " << corbaDataSize << std::endl;
   
    // Vérifie si l'utilisateur demande du 0 copie
    if ( data == NULL ) {
      if ( bufferLength != 0 ) {
	MESSAGE("bufferLength devrait valoir 0 pour l'utilisation du mode sans copie (data==NULL)");
      }
      nRead = corbaDataSize;
      // Si les types T et InnerType sont différents, il faudra effectuer tout de même une recopie
      if (!IsSameType<T1,InnerType>::value) data = new T1[nRead];
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 9 ------------------" << std::endl;
      // On essaye de faire du 0 copy si les types T et InnerType sont les mêmes
      Copy2UserSpace< IsSameType<T1,InnerType>::value >::apply(data,corbaData,nRead);
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 10 ------------------" << std::endl;
      // Attention : Seul CalciumCouplingPolicy via eraseDataId doit décider de supprimer ou non
      // la donnée corba associée à un DataId ! Ne pas effectuer la desallocation suivante :
      //  old : Dans les deux cas la structure CORBA n'est plus utile 
      //  old : Si !IsSameType<T1,InnerType>::value l'objet CORBA est détruit avec son contenu
      //  old : Dans l'autre cas seul la coquille CORBA est détruite 
      //  tjrs correct : Dans les deux cas l'utilisateur devra appeler ecp_free (version modifiée)
      // DataManipulator::delete_data(corbaData);
   } else {
      nRead = std::min < size_t > (corbaDataSize,bufferLength);
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 11 ------------------" << std::endl;
      Copy2UserSpace<false>::apply(data,corbaData,nRead);
      std::cout << "-------- CalciumInterface(ecp_lecture) MARK 12 ------------------" << std::endl;
      // Attention : Seul CalciumCouplingPolicy via eraseDataId doit décider de supprimer ou non
      // la donnée corba associée à un DataId ! Ne pas effectuer la desallocation suivante :
      //      DataManipulator::delete_data(corbaData);
   }
    std::cout << "-------- CalciumInterface(ecp_lecture), Valeur de data : " << std::endl;
    std::copy(data,data+nRead,std::ostream_iterator<T1>(std::cout," "));
    std::cout << "Ptr :" << data << std::endl;

    std::cout << "-------- CalciumInterface(ecp_lecture) MARK 13 ------------------" << std::endl;
 
  
    return;
  }


  template <typename T1> static void
  ecp_ecriture ( Superv_Component_i & component,
		 CalciumTypes::DependencyType dependencyType,
		 double const  & t,
		 long   const  & i,
		 const string  & nomVar, 
		 size_t bufferLength,
		 T1  & data ) {
    ecp_ecriture<T1,T1> (component,dependencyType,t,i,nomVar,bufferLength,data); 
  }

  template <typename T1, typename T2> static void
  ecp_ecriture ( Superv_Component_i & component,
		 CalciumTypes::DependencyType dependencyType,
		 double const  & t,
		 long   const  & i,
		 const string  & nomVar, 
		 size_t bufferLength,
		 T1  & data ) 
  {
    
    assert(&component);

    //typedef typename StarTrait<TT>::NonStarType           T;
    typedef typename UsesPortTraits<T2>::PortType          PortType;
    typedef typename ProvidesPortTraits<T2>::PortType      ProvidesPortType;
    typedef typename ProvidesPortType::DataManipulator     DataManipulator;
    // Verifier que l'on peut définir UsesPortType::DataManipulator
    //    typedef typename PortType::DataManipulator            DataManipulator;
    typedef typename DataManipulator::Type                CorbaDataType; // Attention != T1
    typedef typename DataManipulator::InnerType           InnerType;

      std::cerr << "-------- CalciumInterface(ecriture) MARK 1 ------------------" << std::endl;
    if ( nomVar.empty() ) throw DatastreamException(CalciumTypes::CPNMVR,
						    LOC("Le nom de la variable est <nul>"));
    PortType * port;
    std::cout << "-------- CalciumInterface(ecriture) MARK 2 ------------------" << std::endl;

    try {
      port  = component.Superv_Component_i::get_port< PortType > (nomVar.c_str());
      std::cout << "-------- CalciumInterface(ecriture) MARK 3 ------------------" << std::endl;
    } catch ( const Superv_Component_i::PortNotDefined & ex) {
      std::cerr << ex.what() << std::endl;
      throw (DatastreamException(CalciumTypes::CPNMVR,ex));
    } catch ( const Superv_Component_i::PortNotConnected & ex) {
      std::cerr << ex.what() << std::endl;;
      throw (DatastreamException(CalciumTypes::CPLIEN,ex)); 
      // VERIFIER LES CAS DES CODES : CPINARRET, CPSTOPSEQ, CPCTVR, CPLIEN
    } catch ( const Superv_Component_i::BadCast & ex) {
      std::cerr << ex.what() << std::endl;
      throw (DatastreamException(CalciumTypes::CPTPVR,ex));
    }
 
    // mode == mode du port 
    // On pourrait créer la méthode CORBA dans le mode de Couplage CALCIUM.
    // et donc ajouter cette cette méthode uniquement dans l'IDL calcium !

//     CalciumTypes::DependencyType portDependencyType;
//     try {
//       portDependencyType = port->getDependencyType();
//       std::cout << "-------- CalciumInterface(ecriture) MARK 4 ------------------" << std::endl;
//     } catch ( const DSC_Exception & ex ) {
//       std::cerr << ex.what() << std::endl;;
//       throw (DatastreamException(CalciumTypes::CPIT,ex));
//     }

    if ( dependencyType == CalciumTypes::UNDEFINED_DEPENDENCY )
      throw DatastreamException(CalciumTypes::CPIT,
				LOC(OSS()<<"Le mode de dépendance demandé pour la variable " 
				    << nomVar << " est indéfini."));

    if ( dependencyType == CalciumTypes::SEQUENCE_DEPENDENCY )
      throw DatastreamException(CalciumTypes::CPIT,
				LOC(OSS()<<"Le mode de dépendance SEQUENCE_DEPENDENCY pour la variable " 
				    << nomVar << " est impossible en écriture."));

    // Il faudrait que le port provides génère une exception si le mode donnée n'est pas
    // le bon. La seule façon de le faire est d'envoyer -1 en temps si on n'est en itération
    // et vice-versa pour informer les provides port du mode dans lequel on est. Sinon il faut
    // modifier l'interface IDL pour y ajouter un mode de dépendance !
    // ---->
//     if ( portDependencyType != dependencyType ) 
//       throw DatastreamException(CalciumTypes::CPITVR,
// 				LOC(OSS()<<"Le mode de dépendance de la variable " 
// 				    << nomVar << " ne correspond pas au mode demandé."));

  
    if ( bufferLength < 1 )
      throw DatastreamException(CalciumTypes::CPNTNULL,
				LOC(OSS()<<"Le buffer a envoyer est de taille nulle "));


    std::cout << "-------- CalciumInterface(ecriture) MARK 7 ------------------" << std::endl;
    CorbaDataType corbaData;

    
    // Si les types Utilisateurs et CORBA sont différents
    // il faut effectuer une recopie sinon on utilise directement le
    // buffer data pour constituer la séquence
    // TODO : 
    // - Attention en mode asynchrone il faudra eventuellement
    //   faire une copie des données même si elles sont de même type.
    // - En cas de collocalisation (du port provide et du port uses)
    //   il est necessaire d'effectuer une recopie du buffer car la
    //   séquence est envoyée au port provide par une référence sur 
    //   la séquence locale. Or la méthode put récupère le buffer directement
    //   qui est alors le buffer utilisateur. Il pourrait alors arrivé que :
    //     * Le recepteur efface le buffer emetteur
    //     * Le port lui-même efface le buffer de l'ulisateur !
    //   Cette copie est effectuée dans GenericPortUses::put 
    //   en fonction de la collocalisation ou non.
    // - En cas de connection multiples d'un port uses distant vers plusieurs port provides
    //   collocalisés les ports provides partagent la même copie de la donnée ! 
    //   Il faut effectuer une copie dans le port provides.
    //   Cette copie est effectuée dans GenericPortUses::put 
    //   en fonction de la collocalisation ou non.
    Copy2CorbaSpace<IsSameType<T1,InnerType>::value >::apply(corbaData,data,bufferLength);
 
    //TODO : GERER LES EXCEPTIONS ICI : ex le port n'est pas connecté
    if ( dependencyType == CalciumTypes::TIME_DEPENDENCY ) {
      port->put(*corbaData,t, -1); 
      //Le -1 peut être traité par le cst DataIdContainer et transformé en 0 
      //Etre obligé de mettre une étoile ds (*corbadata) va poser des pb pour les types <> seq
      std::cout << "-------- CalciumInterface(ecriture) MARK 5 ------------------" << std::endl;
    } 
    else if ( dependencyType == CalciumTypes::ITERATION_DEPENDENCY ) {
      port->put(*corbaData,-1, i);
      std::cout << "-------- CalciumInterface(ecriture) MARK 6 ------------------" << std::endl;
    } 

    
    std::cout << "-------- CalciumInterface(ecriture), Valeur de corbaData : " << std::endl;
    for (int i = 0; i < corbaData->length(); ++i)
      cout << "-------- CalciumInterface(ecriture), corbaData[" << i << "] = " << (*corbaData)[i] << endl;
    
    //    if ( !IsSameType<T1,InnerType>::value ) delete corbaData;
    // Supprime l'objet CORBA avec eventuellement les données qu'il contient (case de la recopie)
    delete corbaData;

    std::cout << "-------- CalciumInterface(ecriture) MARK 7 ------------------" << std::endl;
   
    return;
  }

};

// Interface C/C++

// En CALCIUM l'utilisation de données de type double
// implique des dates de type double, pour les autres
// types de données les dates sont de type float
template <class T> struct CalTimeType {
  typedef float TimeType;
};

template <> struct CalTimeType<double> {
  typedef double TimeType;
};

extern "C"  CalciumTypes::InfoType 
ecp_fin_ (void * component, int code) {

  Superv_Component_i * _component = static_cast<Superv_Component_i *>(component); 

  bool provideLastGivenValue = false;
  if (code == CalciumTypes::CP_CONT ) provideLastGivenValue = true;

  try {									
    CalciumInterface::ecp_fin( *_component,				
			       provideLastGivenValue); 
  } catch ( const DatastreamException & ex) { //tester l'arrêt par exception
    std::cerr << ex.what() << std::endl;				
    return ex.getInfo();						
  }									
  return CalciumTypes::CPOK;
};


#define CALCIUM_C2CPP_INTERFACE_(_name,_type,_qual)			\
  extern "C" CalciumTypes::InfoType ecp_lecture_##_name (void * component, int dependencyType, \
							 CalTimeType< _type _qual >::TimeType * ti, \
							 CalTimeType< _type _qual >::TimeType * tf, long * i, \
							 const char * const nomvar, size_t bufferLength, \
							 size_t * nRead, _type _qual ** data ) { \
    Superv_Component_i * _component = static_cast<Superv_Component_i *>(component); \
    double         _ti=*ti;						\
    double         _tf=*tf;						\
    size_t         _nRead=0;						\
    size_t         _bufferLength=bufferLength;				\
    CalciumTypes::DependencyType _dependencyType=     \
          static_cast<CalciumTypes::DependencyType>(dependencyType);							\
                                                                                  \
    if ( IsSameType< _name , cplx >::value ) _bufferLength*=2;		\
    std::cout << "-------- CalciumInterface(lecture Inter Part) MARK 1 ------------------" << std::endl; \
    try {								\
      CalciumInterface::ecp_lecture< _type, _name >( *_component,	\
						     _dependencyType, \
						     _ti, _tf, *i,	\
						     nomvar,		\
						     _bufferLength, _nRead, *data); \
    } catch ( const DatastreamException & ex) {				\
      std::cerr << ex.what() << std::endl;				\
      return ex.getInfo();						\
    }									\
    if ( IsSameType< _name , cplx >::value ) { *nRead=_nRead/2;		\
      std::cout << "-------- CalciumInterface(lecture Inter Part) IsSameType cplx -------------" << std::endl; \
      std::cout << "-------- CalciumInterface(lecture Inter Part) _nRead  : " << _nRead << std::endl; \
      std::cout << "-------- CalciumInterface(lecture Inter Part) *nRead  : " << *nRead << std::endl; \
    } else *nRead = _nRead;						\
    if (_dependencyType == CalciumTypes::CP_SEQUENTIEL ) \
        *ti=(CalTimeType< _type _qual >::TimeType)(_ti);			\
    std::cout << "-------- CalciumInterface(lecture Inter Part), Data Ptr :" << *data << std::endl; \
    for (int i=0; i<_nRead;++i)						\
      printf("-------- CalciumInterface(lecture Inter Part), Valeur de data (typage entier) data[%d] : %d \n",i,(*data)[i]); \
    std::cout << "-------- CalciumInterface(lecture Inter Part), Data Ptr :" << *data << std::endl; \
    return CalciumTypes::CPOK;						\
  };									\
  extern "C" void ecp_lecture_##_name##_free ( _type _qual * data) {	\
    CalciumInterface::ecp_free< _type, _name >(data);			\
  };		                                                        \
  extern "C" CalciumTypes::InfoType ecp_ecriture_##_name (void * component, int dependencyType, \
							  CalTimeType< _type _qual >::TimeType *t, \
							  long  i,	\
							  const char * const nomvar, size_t bufferLength, \
							  _type _qual * data ) { \
    Superv_Component_i * _component = static_cast<Superv_Component_i *>(component); \
    /* Je ne sais pas pourquoi, je n'arrive pas à passer t par valeur : corruption de la pile*/ \
    double         _t=*t;						\
    size_t         _bufferLength=bufferLength;				\
    if ( IsSameType< _name , cplx >::value ) _bufferLength=_bufferLength*2; \
    std::cout << "-------- CalciumInterface(ecriture Inter Part) MARK 1 ------------------" << std::endl; \
    try {								\
      printf("-------- CalciumInterface(ecriture Inter Part), cp_name : Nom de la var. de type %s : %s\n",#_type,nomvar); \
      std::string essai(nomvar);					\
      std::cout << "----------->-" << nomvar << std::endl;		\
      CalciumInterface::ecp_ecriture< _type, _name >( *_component,	\
						      static_cast<CalciumTypes::DependencyType>(dependencyType), \
						      _t,i,nomvar,_bufferLength,*data); \
    } catch ( const DatastreamException & ex) {				\
      std::cerr << ex.what() << std::endl;				\
      return ex.getInfo();						\
    }									\
    std::cout << "-------- CalciumInterface(ecriture Inter Part), Valeur de data : " << std::endl; \
    std::cout << "-------- CalciumInterface(ecriture Inter Part), Ptr(1) :" << data << std::endl; \
    for (int i=0; i<_bufferLength;++i)					\
      printf("-------- CalciumInterface(ecriture Inter Part), Valeur de data (typage entier) data[%d] : %d \n",i,data[i]); \
    std::cout << "-------- CalciumInterface(ecriture Inter Part), Ptr(2) :" << data << std::endl; \
    return CalciumTypes::CPOK;						\
  };									\



#define STAR *
CALCIUM_C2CPP_INTERFACE_(int,int,);
CALCIUM_C2CPP_INTERFACE_(float,float, );
CALCIUM_C2CPP_INTERFACE_(double,double,);
CALCIUM_C2CPP_INTERFACE_(bool,bool,);
CALCIUM_C2CPP_INTERFACE_(cplx,float,);

#endif
