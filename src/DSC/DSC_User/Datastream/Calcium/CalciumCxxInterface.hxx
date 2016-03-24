// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

//  File   : CalciumCxxInterface.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-03-01 13:27:58 +0100 (jeu, 01 mar 2007) $
// Id          : $Id$
//
#ifndef _CALCIUM_CXXINTERFACE_HXX_
#define _CALCIUM_CXXINTERFACE_HXX_

#if defined(__CONST_H) || defined(__CALCIUM_H)
#error "The header CalciumCxxInterface.hxx must be included before calcium.h"
#endif

#include <string>
#include <vector>
#include <iostream>
#include "Superv_Component_i.hxx"
#include "CalciumException.hxx"
#include "CalciumTypes.hxx"
#include "CalciumGenericUsesPort.hxx"
#include "Copy2UserSpace.hxx"
#include "Copy2CorbaSpace.hxx"
#include "CalciumPortTraits.hxx"

#include <stdio.h>

#include <typeinfo>

template <typename T1, typename T2>
struct IsSameType {
  static const bool value = false;
};
template <typename T1>
struct IsSameType<T1,T1> {
  static const bool value = true;
};

extern const char * CPMESSAGE[];

//#define MYDEBUG

#include <boost/type_traits/remove_all_extents.hpp>

namespace CalciumInterface {
  
  /********************* CONNECTION INTERFACE *****************/

  static inline void
  ecp_cd (Superv_Component_i & component, std::string & instanceName)
  { 
    /* TODO : Trouver le nom de l'instance SALOME*/
    CORBA::String_var componentName=component.instanceName();
    std::string containerName=component.getContainerName();
    if (instanceName.empty()) instanceName=componentName;
    Engines_DSC_interface::writeEvent("CP_CD",containerName,componentName,"","","");
  }

  static inline void
  ecp_fin (Superv_Component_i & component, bool provideLastGivenValue)
  { 
    CORBA::String_var componentName=component.instanceName();
    std::string containerName=component.getContainerName();
    Engines_DSC_interface::writeEvent("CP_FIN",containerName,componentName,"","","");

    std::vector<std::string> usesPortNames;
    std::vector<std::string>::const_iterator it;
    component.get_uses_port_names(usesPortNames);    
    
    //Récupérer le type de réel du port est un peu difficile
    //car l'interface ne donne aucune indication

    //     uses_port *myUsesPort;
    calcium_uses_port* myCalciumUsesPort;
      
    for (it=usesPortNames.begin(); it != usesPortNames.end(); ++it) 
      {
        try 
          {
            myCalciumUsesPort= component.Superv_Component_i::get_port< calcium_uses_port >((*it).c_str());

//         component.Superv_Component_i::get_port(myUsesPort,(*it).c_str());
//         calcium_uses_port* myCalciumUsesPort=
//           dynamic_cast<calcium_uses_port*>(myUsesPort);

#ifdef MYDEBUG
            std::cerr << "-------- CalciumInterface(ecp_fin) MARK 1 -|"<< *it <<"|----"<< 
          //          typeid(myUsesPort).name() <<"-------------" <<
              typeid(myCalciumUsesPort).name() <<"-------------" << std::endl;
#endif
        
//         if ( !myCalciumUsesPort )
//           throw Superv_Component_i::BadCast(LOC(OSS()<<"Impossible de convertir le port "
//                                                 << *it << " en port de type calcium_uses_port." ));

            myCalciumUsesPort->disconnect(provideLastGivenValue);
          }
        catch ( const Superv_Component_i::BadCast & ex) 
          {
            Engines_DSC_interface::writeEvent("CP_FIN",containerName,componentName,"",CPMESSAGE[CalciumTypes::CPTPVR],ex.what());
            throw (CalciumException(CalciumTypes::CPTPVR,ex));
          }
        catch ( const DSC_Exception & ex) 
          {
            Engines_DSC_interface::writeEvent("CP_FIN",containerName,componentName,"",CPMESSAGE[CalciumTypes::CPOK],ex.what());
            // Exception venant de SupervComponent :
            //   PortNotDefined(CPNMVR), PortNotConnected(CPLIEN)  
            // ou du port uses : Dsc_Exception
            // On continue à traiter la deconnexion des autres ports uses
          }
        catch (...) 
          {
            Engines_DSC_interface::writeEvent("CP_FIN",containerName,componentName,"",CPMESSAGE[CalciumTypes::CPATAL],"Unexpected exception");
            throw (CalciumException(CalciumTypes::CPATAL,"Unexpected exception"));
            // En fonction du mode de gestion des erreurs throw;
          }
      }
  }


  /********************* INTERFACES DE DESALLOCATION  *****************/

  // Uniquement appelé par l'utilisateur s'il utilise la 0 copie
  //   ( pointeur de données data==NULL à l'appel de ecp_lecture )
  // Une désallocation aura lieu uniquement si un buffer intermédiaire
  // était necessaire (type utilisateur et corba diffférent)
  // La propriété du buffer est rendue à CORBA sinon  
  template <typename T1, typename T2> static void
  ecp_free ( T1 * dataPtr )
  {
    typedef typename ProvidesPortTraits<T2>::PortType     PortType;
    typedef typename PortType::DataManipulator            DataManipulator;
    typedef typename DataManipulator::Type                DataType; // Attention != T1
    typedef typename DataManipulator::InnerType           InnerType;

    DeleteTraits<IsSameType<T1,InnerType>::value, DataManipulator >::apply(dataPtr);
  }

  template <typename T1> static void
  ecp_free ( T1 * dataPtr )
  {
    ecp_free<T1,T1> ( dataPtr );
  }


  /********************* READING INTERFACE *****************/


  // T1 est le type de données
  // T2 est un <nom> de type Calcium permettant de sélectionner le port CORBA correspondant 
  // T1 et T2 sont dissociés pour discriminer par exemple le cas des nombres complexes
  //  -> Les données des nombres complexes sont de type float mais
  //     le port à utiliser est le port cplx
  template <typename T1, typename T2 > static void
  ecp_lecture ( Superv_Component_i & component,
               int    const  & dependencyType,
               double        & ti,
               double const  & tf,
               long          & i,
               const std::string  & nomVar, 
               size_t          bufferLength,
               size_t        & nRead, 
               T1            * &data )
  {

    assert(&component);
    CORBA::String_var componentName=component.instanceName();
    std::string containerName=component.getContainerName();

    typedef typename ProvidesPortTraits<T2>::PortType     PortType;
    typedef typename PortType::DataManipulator            DataManipulator;
    typedef typename DataManipulator::Type                CorbaDataType; // Attention != T1
    typedef typename DataManipulator::InnerType           InnerType;
    CalciumTypes::DependencyType _dependencyType=                
      static_cast<CalciumTypes::DependencyType>(dependencyType);
    
    CorbaDataType     corbaData;

#ifdef MYDEBUG
    std::cerr << "-------- CalciumInterface(ecp_lecture) MARK 1 ------------------" << std::endl;
#endif

    if (nomVar.empty())
      {
        Engines_DSC_interface::writeEvent("BEGIN_READ",containerName,componentName,"",CPMESSAGE[CalciumTypes::CPNMVR],"");
        throw CalciumException(CalciumTypes::CPNMVR, LOC("Empty variable name"));
      }
    PortType * port;
#ifdef MYDEBUG
    std::cout << "-------- CalciumInterface(lecture) MARK 2 --"<<typeid(port).name()<<"----------------" << std::endl;
    T1 t1;
    T2 t2;
    std::cout << "-------- CalciumInterface(lecture) MARK 2b1 -----" << typeid(t1).name() << "-------------" << std::endl;
    std::cout << "-------- CalciumInterface(lecture) MARK 2b2 -----" << typeid(t2).name() << "-------------" << std::endl;
#endif

    try 
      {
        port  = component.Superv_Component_i::get_port< PortType > (nomVar.c_str());
#ifdef MYDEBUG
        std::cout << "-------- CalciumInterface(ecp_lecture) MARK 3 ------------------" << std::endl;
#endif
      }
    catch ( const Superv_Component_i::PortNotDefined & ex) 
      {
        Engines_DSC_interface::writeEvent("BEGIN_READ",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPNMVR],ex.what());
        throw (CalciumException(CalciumTypes::CPNMVR,ex));
      }
    catch ( const Superv_Component_i::PortNotConnected & ex) 
      {
        Engines_DSC_interface::writeEvent("BEGIN_READ",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPLIEN],ex.what());
        throw (CalciumException(CalciumTypes::CPLIEN,ex)); 
        // VERIFIER LES CAS DES CODES : CPINARRET, CPSTOPSEQ, CPCTVR, CPLIEN
      }
    catch ( const Superv_Component_i::BadCast & ex) 
      {
        Engines_DSC_interface::writeEvent("BEGIN_READ",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPTPVR],ex.what());
        throw (CalciumException(CalciumTypes::CPTPVR,ex));
      }
  
    // mode == mode du port 
    CalciumTypes::DependencyType portDependencyType = port->getDependencyType();

    if ( portDependencyType == CalciumTypes::UNDEFINED_DEPENDENCY )
      {
        Engines_DSC_interface::writeEvent("BEGIN_READ",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPIT],"Dependency mode is undefined");
        throw CalciumException(CalciumTypes::CPIT, LOC(OSS()<<"Dependency mode of variable " << nomVar << " is undefined."));
      }

    if ( ( portDependencyType != _dependencyType ) && ( _dependencyType != CalciumTypes::SEQUENCE_DEPENDENCY ) ) 
      {
        Engines_DSC_interface::writeEvent("BEGIN_READ",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPIT],
                   "Dependency mode is not the same as the required one");
        throw CalciumException(CalciumTypes::CPITVR, LOC(OSS()<<"Dependency mode of variable " << nomVar << ": " 
                                << portDependencyType << " is not the same as the required one."));
      }

  
    std::stringstream msgout,msg;
    if ( _dependencyType == CalciumTypes::TIME_DEPENDENCY ) 
      {
        try
          {
            double   tt=ti;
            msg << "ti=" << ti << ", tf=" << tf ;
            Engines_DSC_interface::writeEvent("BEGIN_READ",containerName,componentName,nomVar.c_str(),"",msg.str().c_str());
            corbaData = port->get(tt,tf, 0);
            msgout << "read t=" << tt ;
#ifdef MYDEBUG
            std::cout << "-------- CalciumInterface(ecp_lecture) MARK 5 ------------------" << std::endl;
#endif
          }
        catch ( const DSC_Exception & ex)
          {
            Engines_DSC_interface::writeEvent("END_READ",containerName,componentName,nomVar.c_str(),"",ex.what());
            throw;
          }
      } 
    else if ( _dependencyType == CalciumTypes::ITERATION_DEPENDENCY ) 
      {
        try
          {
            msg << "i=" << i ;
            Engines_DSC_interface::writeEvent("BEGIN_READ",containerName,componentName,nomVar.c_str(),"",msg.str().c_str());
            corbaData = port->get(0, i);
            msgout << "read i=" << i ;
#ifdef MYDEBUG
            std::cout << "-------- CalciumInterface(ecp_lecture) MARK 6 ------------------" << std::endl;
#endif
          }
        catch ( const DSC_Exception & ex)
          {
            Engines_DSC_interface::writeEvent("END_READ",containerName,componentName,nomVar.c_str(),"",ex.what());
            throw;
          }
      } 
    else 
      {
        // Sequential read
        try
          {
#ifdef MYDEBUG
            std::cout << "-------- CalciumInterface(ecp_lecture) MARK 7 ------------------" << std::endl;
#endif
            Engines_DSC_interface::writeEvent("BEGIN_READ",containerName,componentName,nomVar.c_str(),"","Sequential read");
            corbaData = port->next(ti,i);
            msgout << "read ";
            if(i==0)msgout<< "t=" <<ti;
            else msgout<< "i=" <<i;
          }
        catch ( const DSC_Exception & ex)
          {
            Engines_DSC_interface::writeEvent("END_READ",containerName,componentName,nomVar.c_str(),"",ex.what());
            throw;
          }
      }
 
#ifdef MYDEBUG
    std::cout << "-------- CalciumInterface(ecp_lecture) MARK 8 ------------------" << std::endl;
#endif
    size_t corbaDataSize = DataManipulator::size(corbaData);
#ifdef MYDEBUG
    std::cout << "-------- CalciumInterface(ecp_lecture) corbaDataSize : " << corbaDataSize << std::endl;
#endif
   
    // Vérifie si l'utilisateur demande du 0 copie
    if ( data == NULL ) 
      {
        if ( bufferLength != 0 ) 
          {
            MESSAGE("bufferLength devrait valoir 0 pour l'utilisation du mode sans copie (data==NULL)");
          }
        nRead = corbaDataSize;
        // Si les types T1 et InnerType sont différents, il faudra effectuer tout de même une recopie
        if (!IsSameType<T1,InnerType>::value) data = new T1[nRead];
#ifdef MYDEBUG
        std::cout << "-------- CalciumInterface(ecp_lecture) MARK 9 ------------------" << std::endl;
#endif
        // On essaye de faire du 0 copy si les types T1 et InnerType sont les mêmes.
        // Copy2UserSpace : 
        // La raison d'être du foncteur Copy2UserSpace est qu'il n'est pas possible de compiler
        // une expression d'affectation sur des types incompatibles ; même 
        // si cette expression se trouve dans une branche non exécuté d'un test
        // portant sur la compatibilité des types.
        // En utilisant le foncteur Copy2UserSpace, seule la spécialisation en adéquation
        // avec la compatibilité des types sera compilée 
        Copy2UserSpace< IsSameType<T1,InnerType>::value, DataManipulator >::apply(data,corbaData,nRead);
#ifdef MYDEBUG
        std::cout << "-------- CalciumInterface(ecp_lecture) MARK 10 ------------------" << std::endl;
#endif
        // Attention : Seul CalciumCouplingPolicy via eraseDataId doit décider de supprimer ou non
        // la donnée corba associée à un DataId ! Ne pas effectuer la desallocation suivante :
        // DataManipulator::delete_data(corbaData); 
        // ni DataManipulator::getPointer(corbaData,true); qui détruit la sequence lorsque l'on
        // prend la propriété du buffer
        //  old : Dans les deux cas la structure CORBA n'est plus utile 
        //  old : Si !IsSameType<T1,InnerType>::value l'objet CORBA est détruit avec son contenu
        //  old : Dans l'autre cas seul la coquille CORBA est détruite 
        //  L'utilisateur devra appeler ecp_free qui déterminera s'il est necessaire
        //  de désallouer un buffer intermédiaire ( types différents) ou de rendre la propriété
      }
    else 
      {
        nRead = std::min < size_t > (corbaDataSize,bufferLength);
#ifdef MYDEBUG
        std::cout << "-------- CalciumInterface(ecp_lecture) MARK 11 ------------------" << std::endl;
#endif
        Copy2UserSpace<false, DataManipulator >::apply(data,corbaData,nRead);
        //Déjà fait ci-dessus : 
        //DataManipulator::copy(corbaData,data,nRead);
#ifdef MYDEBUG
        std::cout << "-------- CalciumInterface(ecp_lecture) MARK 12 ------------------" << std::endl;
#endif
      }
#ifdef MYDEBUG
    std::cout << "-------- CalciumInterface(ecp_lecture), Valeur de data : " << std::endl;
    std::copy(data,data+nRead,std::ostream_iterator<T1>(std::cout," "));
    std::cout << "Ptr :" << data << std::endl;
    std::cout << "-------- CalciumInterface(ecp_lecture) MARK 13 ------------------" << std::endl;
#endif
    Engines_DSC_interface::writeEvent("END_READ",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPOK],msgout.str().c_str());
    return;
  }

  // T1 is the data type
  template <typename T1 > static void
  ecp_lecture ( Superv_Component_i & component,
               int    const      & dependencyType,
               double            & ti,
               double const      & tf,
               long              & i,
               const std::string & nomVar, 
               size_t              bufferLength,
               size_t            & nRead, 
               T1                * &data )
  {
    ecp_lecture<T1,T1> (component,dependencyType,ti,tf,
                        i,nomVar,bufferLength,nRead,data);
  
  }

  /********************* WRITING INTERFACE *****************/

  // T1 : DataType
  // T2 : PortType
  template <typename T1, typename T2> static void
  ecp_ecriture ( Superv_Component_i & component,
                 int    const      & dependencyType,
                 double const      & t,
                 long   const      & i,
                 const std::string & nomVar, 
                 size_t              bufferLength,
                 T1                  const  & data ) 
  {
    
    assert(&component);
    CORBA::String_var componentName=component.instanceName();
    std::string containerName=component.getContainerName();

    //typedef typename StarTrait<TT>::NonStarType                    T;
    typedef typename boost::remove_all_extents< T1 >::type           T1_without_extent;
    typedef typename boost::remove_all_extents< T2 >::type           T2_without_extent;
    typedef typename UsesPortTraits    <T2_without_extent>::PortType UsesPortType;
    typedef typename ProvidesPortTraits<T2_without_extent>::PortType ProvidesPortType;// pour obtenir un manipulateur de données
    typedef typename ProvidesPortType::DataManipulator               DataManipulator;
    // Verifier que l'on peut définir UsesPortType::DataManipulator
    //    typedef typename PortType::DataManipulator            DataManipulator;
    typedef typename DataManipulator::Type                           CorbaDataType; // Attention != T1
    typedef typename DataManipulator::InnerType                      InnerType;
    
    T1_without_extent const & _data = data;

    CalciumTypes::DependencyType _dependencyType=                
      static_cast<CalciumTypes::DependencyType>(dependencyType);

#ifdef MYDEBUG
    std::cerr << "-------- CalciumInterface(ecriture) MARK 1 ------------------" << std::endl;
#endif
    if ( nomVar.empty() )
      {
        Engines_DSC_interface::writeEvent("WRITE",containerName,componentName,"",CPMESSAGE[CalciumTypes::CPNMVR],"");
        throw CalciumException(CalciumTypes::CPNMVR, LOC("Empty variable name"));
      }
    UsesPortType * port;
#ifdef MYDEBUG
    std::cout << "-------- CalciumInterface(ecriture) MARK 2 ---"<<typeid(port).name()<<"---------------" << std::endl;
    T1 t1;
    T2 t2;
    std::cout << "-------- CalciumInterface(ecriture) MARK 2b1 -----" << typeid(t1).name() << "-------------" << std::endl;
    std::cout << "-------- CalciumInterface(ecriture) MARK 2b2 -----" << typeid(t2).name() << "-------------" << std::endl;
#endif

    try 
      {
        port  = component.Superv_Component_i::get_port< UsesPortType > (nomVar.c_str());
#ifdef MYDEBUG
        std::cout << "-------- CalciumInterface(ecriture) MARK 3 ------------------" << std::endl;
#endif
      }
    catch ( const Superv_Component_i::PortNotDefined & ex) 
      {
        Engines_DSC_interface::writeEvent("WRITE",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPNMVR],ex.what());
        throw (CalciumException(CalciumTypes::CPNMVR,ex));
      }
    catch ( const Superv_Component_i::PortNotConnected & ex) 
      {
        Engines_DSC_interface::writeEvent("WRITE",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPLIEN],ex.what());
        throw (CalciumException(CalciumTypes::CPLIEN,ex)); 
        // VERIFIER LES CAS DES CODES : CPINARRET, CPSTOPSEQ, CPCTVR, CPLIEN
      }
    catch ( const Superv_Component_i::BadCast & ex) 
      {
        Engines_DSC_interface::writeEvent("WRITE",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPTPVR],ex.what());
        throw (CalciumException(CalciumTypes::CPTPVR,ex));
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
//       throw (CalciumException(CalciumTypes::CPIT,ex));
//     }

    if ( _dependencyType == CalciumTypes::UNDEFINED_DEPENDENCY )
      {
        Engines_DSC_interface::writeEvent("WRITE",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPIT],"Dependency mode is undefined");
        throw CalciumException(CalciumTypes::CPIT, LOC(OSS()<<"Dependency mode of variable " << nomVar << " is undefined."));
      }

    if ( _dependencyType == CalciumTypes::SEQUENCE_DEPENDENCY )
      {
        Engines_DSC_interface::writeEvent("WRITE",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPIT],
                   "SEQUENCE_DEPENDENCY mode is not possible when writing");
        throw CalciumException(CalciumTypes::CPIT, LOC(OSS()<<"Dependency mode SEQUENCE_DEPENDENCY for variable " << nomVar 
                               << " is not possible when writing."));
      }

    // Il faudrait que le port provides génère une exception si le mode donnée n'est pas
    // le bon. La seule façon de le faire est d'envoyer -1 en temps si on n'est en itération
    // et vice-versa pour informer les provides port du mode dans lequel on est. Sinon il faut
    // modifier l'interface IDL pour y ajouter un mode de dépendance !
    // ---->
//     if ( portDependencyType != _dependencyType ) 
//       throw CalciumException(CalciumTypes::CPITVR,
//                                 LOC(OSS()<<"Le mode de dépendance de la variable " 
//                                     << nomVar << " ne correspond pas au mode demandé."));

  
    if ( bufferLength < 1 )
      {
        Engines_DSC_interface::writeEvent("WRITE",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPNTNULL],"Buffer to send is empty");
        throw CalciumException(CalciumTypes::CPNTNULL, LOC(OSS()<<"Buffer to send is empty"));
      }

    CorbaDataType corbaData;
#ifdef MYDEBUG
    std::cout << "-------- CalciumInterface(ecriture) MARK 4 ------------------" << std::endl;
#endif
    
    // Si les types Utilisateurs et CORBA sont différents
    // il faut effectuer une recopie sinon on utilise directement le
    // buffer data pour constituer la séquence
    // TODO : 
    // - Attention en mode asynchrone il faudra eventuellement
    //   faire une copie des données même si elles sont de même type.
    // - OLD : En cas de collocalisation (du port provide et du port uses)
    //   OLD : il est necessaire d'effectuer une recopie du buffer car la
    //   OLD : séquence est envoyée au port provide par une référence sur 
    //   OLD : la séquence locale. Or la méthode put récupère le buffer directement
    //   OLD : qui est alors le buffer utilisateur. Il pourrait alors arriver que :
    //   OLD :   * Le recepteur efface le buffer emetteur
    //   OLD :   * Le port lui-même efface le buffer de l'utilisateur !
    //   OLD : Cette copie est effectuée dans GenericPortUses::put 
    //   OLD : en fonction de la collocalisation ou non.
    // - OLD :En cas de connection multiples d'un port uses distant vers plusieurs port provides
    //   OLD : collocalisés les ports provides partagent la même copie de la donnée ! 
    //   OLD : Il faut effectuer une copie dans le port provides.
    //   OLD : Cette copie est effectuée dans GenericPortUses::put 
    //   OLD : en fonction de la collocalisation ou non.
#ifdef MYDEBUG
    T1_without_extent t1b;
    InnerType         t2b;
    std::cout << "-------- CalciumInterface(ecriture) MARK 4b1 -----" << typeid(t1b).name() << "-------------" << std::endl;
    std::cout << "-------- CalciumInterface(ecriture) MARK 4b2 -----" << typeid(t2b).name() << "-------------" << std::endl;
#endif

    Copy2CorbaSpace<IsSameType<T1_without_extent,InnerType>::value, DataManipulator >::apply(corbaData,_data,bufferLength);
 
    //TODO : GERER LES EXCEPTIONS ICI : ex le port n'est pas connecte
    if ( _dependencyType == CalciumTypes::TIME_DEPENDENCY ) 
      {
        try
          {
            port->put(*corbaData,t, -1); 
            std::stringstream msg;
            msg << "t=" << t ;
            Engines_DSC_interface::writeEvent("WRITE",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPOK],msg.str().c_str());
          }
        catch ( const DSC_Exception & ex) 
          {
            Engines_DSC_interface::writeEvent("WRITE",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPATAL],ex.what());
            throw (CalciumException(CalciumTypes::CPATAL,ex.what()));
          }
        //Le -1 peut être traité par le cst DataIdContainer et transformé en 0 
        //Etre obligé de mettre une étoile ds (*corbadata) va poser des pb pour les types <> seq
#ifdef MYDEBUG
        std::cout << "-------- CalciumInterface(ecriture) MARK 5 ------------------" << std::endl;
#endif
      } 
    else if ( _dependencyType == CalciumTypes::ITERATION_DEPENDENCY ) 
      {
        try
          {
            port->put(*corbaData,-1, i);
            std::stringstream msg;
            msg << "i=" << i ;
            Engines_DSC_interface::writeEvent("WRITE",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPOK],msg.str().c_str());
          }
        catch ( const DSC_Exception & ex) 
          {
            Engines_DSC_interface::writeEvent("WRITE",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPATAL],ex.what());
            throw (CalciumException(CalciumTypes::CPATAL,ex.what()));
          }
#ifdef MYDEBUG
        std::cout << "-------- CalciumInterface(ecriture) MARK 6 ------------------" << std::endl;
#endif
      } 
    
#ifdef MYDEBUG
    std::cout << "-------- CalciumInterface(ecriture), Valeur de corbaData : " << std::endl;
    for (int i = 0; i < corbaData->length(); ++i)
      std::cout << "-------- CalciumInterface(ecriture), corbaData[" << i << "] = " << (*corbaData)[i] << std::endl;
#endif
    
    //    if ( !IsSameType<T1,InnerType>::value ) delete corbaData;
    // Supprime l'objet CORBA avec eventuellement les données qu'il contient (cas de la recopie)
    delete corbaData;

#ifdef MYDEBUG
    std::cout << "-------- CalciumInterface(ecriture) MARK 7 ------------------" << std::endl;
#endif
   
    return;
  };
  
  template <typename T1> static void
  ecp_ecriture ( Superv_Component_i & component,
                 int    const  & dependencyType,
                 double const  & t,
                 long   const  & i,
                 const std::string  & nomVar, 
                 size_t bufferLength,
                 T1 const & data ) 
  {
    ecp_ecriture<T1,T1> (component,dependencyType,t,i,nomVar,bufferLength,data); 
  };

  static inline void
  ecp_fini(Superv_Component_i & component,const std::string  & nomVar,long const  & i)
  {
    CORBA::String_var componentName=component.instanceName();
    std::string containerName=component.getContainerName();

    if (nomVar.empty())
      {
        Engines_DSC_interface::writeEvent("CP_FINI",containerName,componentName,"",CPMESSAGE[CalciumTypes::CPNMVR],"");
        throw CalciumException(CalciumTypes::CPNMVR, LOC("Empty variable name"));
      }

    calcium_provides_port* port;

    try
      {
        port = component.Superv_Component_i::get_port< calcium_provides_port >(nomVar.c_str());
      }
    catch ( const Superv_Component_i::PortNotDefined & ex)
      {
        Engines_DSC_interface::writeEvent("CP_FINI",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPNMVR],ex.what());
        throw (CalciumException(CalciumTypes::CPNMVR,ex));
      }
    catch ( const Superv_Component_i::PortNotConnected & ex)
      {
        Engines_DSC_interface::writeEvent("CP_FINI",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPLIEN],ex.what());
        throw (CalciumException(CalciumTypes::CPLIEN,ex));
      }
    catch ( const Superv_Component_i::BadCast & ex)
      {
        Engines_DSC_interface::writeEvent("CP_FINI",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPTPVR],ex.what());
        throw (CalciumException(CalciumTypes::CPTPVR,ex));
      }

    // get dependency mode
    CalciumTypes::DependencyType portDependencyType = port->getDependencyType();

    if ( portDependencyType == CalciumTypes::UNDEFINED_DEPENDENCY )
      {
        Engines_DSC_interface::writeEvent("CP_FINI",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPIT],"Dependency mode is undefined");
        throw CalciumException(CalciumTypes::CPIT, LOC(OSS()<<"Dependency mode of variable " << nomVar << " is undefined."));
      }

    if ( portDependencyType != CalciumTypes::ITERATION_DEPENDENCY )
      {
        Engines_DSC_interface::writeEvent("CP_FINI",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPIT],
                   "Dependency mode must be iteration mode");
        throw CalciumException(CalciumTypes::CPITVR, LOC(OSS()<<"Dependency mode of variable " << nomVar << ": "
                                << portDependencyType << " must be iteration mode."));
      }

    port->calcium_erase(0., i,true);

    std::stringstream msg;
    msg << "i<=" << i ;
    Engines_DSC_interface::writeEvent("CP_FINI",containerName,componentName,nomVar.c_str(),"",msg.str().c_str());

  };

  static inline void
  ecp_fint(Superv_Component_i & component,const std::string  & nomVar,double const  & t)
  {
    CORBA::String_var componentName=component.instanceName();
    std::string containerName=component.getContainerName();

    if (nomVar.empty())
      {
        Engines_DSC_interface::writeEvent("CP_FINT",containerName,componentName,"",CPMESSAGE[CalciumTypes::CPNMVR],"");
        throw CalciumException(CalciumTypes::CPNMVR, LOC("Empty variable name"));
      }

    calcium_provides_port* port;

    try
      {
        port = component.Superv_Component_i::get_port< calcium_provides_port >(nomVar.c_str());
      }
    catch ( const Superv_Component_i::PortNotDefined & ex)
      {
        Engines_DSC_interface::writeEvent("CP_FINT",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPNMVR],ex.what());
        throw (CalciumException(CalciumTypes::CPNMVR,ex));
      }
    catch ( const Superv_Component_i::PortNotConnected & ex)
      {
        Engines_DSC_interface::writeEvent("CP_FINT",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPLIEN],ex.what());
        throw (CalciumException(CalciumTypes::CPLIEN,ex));
      }
    catch ( const Superv_Component_i::BadCast & ex)
      {
        Engines_DSC_interface::writeEvent("CP_FINT",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPTPVR],ex.what());
        throw (CalciumException(CalciumTypes::CPTPVR,ex));
      }

    // get dependency mode
    CalciumTypes::DependencyType portDependencyType = port->getDependencyType();

    if ( portDependencyType == CalciumTypes::UNDEFINED_DEPENDENCY )
      {
        Engines_DSC_interface::writeEvent("CP_FINT",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPIT],"Dependency mode is undefined");
        throw CalciumException(CalciumTypes::CPIT, LOC(OSS()<<"Dependency mode of variable " << nomVar << " is undefined."));
      }

    if ( portDependencyType != CalciumTypes::TIME_DEPENDENCY )
      {
        Engines_DSC_interface::writeEvent("CP_FINT",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPIT],
                   "Dependency mode must be time mode");
        throw CalciumException(CalciumTypes::CPITVR, LOC(OSS()<<"Dependency mode of variable " << nomVar << ": "
                                << portDependencyType << " must be time mode."));
      }

    port->calcium_erase(t, 0,true);

    std::stringstream msg;
    msg << "t<=" << t ;
    Engines_DSC_interface::writeEvent("CP_FINT",containerName,componentName,nomVar.c_str(),"",msg.str().c_str());

  };

  static inline void
  ecp_effi(Superv_Component_i & component,const std::string  & nomVar,long const  & i)
  {
    CORBA::String_var componentName=component.instanceName();
    std::string containerName=component.getContainerName();

    if (nomVar.empty())
      {
        Engines_DSC_interface::writeEvent("CP_EFFI",containerName,componentName,"",CPMESSAGE[CalciumTypes::CPNMVR],"");
        throw CalciumException(CalciumTypes::CPNMVR, LOC("Empty variable name"));
      }

    calcium_provides_port* port;

    try
      {
        port = component.Superv_Component_i::get_port< calcium_provides_port >(nomVar.c_str());
      }
    catch ( const Superv_Component_i::PortNotDefined & ex)
      {
        Engines_DSC_interface::writeEvent("CP_EFFI",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPNMVR],ex.what());
        throw (CalciumException(CalciumTypes::CPNMVR,ex));
      }
    catch ( const Superv_Component_i::PortNotConnected & ex)
      {
        Engines_DSC_interface::writeEvent("CP_EFFI",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPLIEN],ex.what());
        throw (CalciumException(CalciumTypes::CPLIEN,ex));
      }
    catch ( const Superv_Component_i::BadCast & ex)
      {
        Engines_DSC_interface::writeEvent("CP_EFFI",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPTPVR],ex.what());
        throw (CalciumException(CalciumTypes::CPTPVR,ex));
      }

    // get dependency mode
    CalciumTypes::DependencyType portDependencyType = port->getDependencyType();

    if ( portDependencyType == CalciumTypes::UNDEFINED_DEPENDENCY )
      {
        Engines_DSC_interface::writeEvent("CP_EFFI",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPIT],"Dependency mode is undefined");
        throw CalciumException(CalciumTypes::CPIT, LOC(OSS()<<"Dependency mode of variable " << nomVar << " is undefined."));
      }

    if ( portDependencyType != CalciumTypes::ITERATION_DEPENDENCY )
      {
        Engines_DSC_interface::writeEvent("CP_EFFI",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPIT],
                   "Dependency mode must be iteration mode");
        throw CalciumException(CalciumTypes::CPITVR, LOC(OSS()<<"Dependency mode of variable " << nomVar << ": "
                                << portDependencyType << " must be iteration mode."));
      }

    port->calcium_erase(0., i,false);

    std::stringstream msg;
    msg << "i>=" << i ;
    Engines_DSC_interface::writeEvent("CP_EFFI",containerName,componentName,nomVar.c_str(),"",msg.str().c_str());

  };

  static inline void
  ecp_efft(Superv_Component_i & component,const std::string  & nomVar,double const  & t)
  {
    CORBA::String_var componentName=component.instanceName();
    std::string containerName=component.getContainerName();

    if (nomVar.empty())
      {
        Engines_DSC_interface::writeEvent("CP_EFFT",containerName,componentName,"",CPMESSAGE[CalciumTypes::CPNMVR],"");
        throw CalciumException(CalciumTypes::CPNMVR, LOC("Empty variable name"));
      }

    calcium_provides_port* port;

    try
      {
        port = component.Superv_Component_i::get_port< calcium_provides_port >(nomVar.c_str());
      }
    catch ( const Superv_Component_i::PortNotDefined & ex)
      {
        Engines_DSC_interface::writeEvent("CP_EFFT",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPNMVR],ex.what());
        throw (CalciumException(CalciumTypes::CPNMVR,ex));
      }
    catch ( const Superv_Component_i::PortNotConnected & ex)
      {
        Engines_DSC_interface::writeEvent("CP_EFFT",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPLIEN],ex.what());
        throw (CalciumException(CalciumTypes::CPLIEN,ex));
      }
    catch ( const Superv_Component_i::BadCast & ex)
      {
        Engines_DSC_interface::writeEvent("CP_EFFT",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPTPVR],ex.what());
        throw (CalciumException(CalciumTypes::CPTPVR,ex));
      }

    // get dependency mode
    CalciumTypes::DependencyType portDependencyType = port->getDependencyType();

    if ( portDependencyType == CalciumTypes::UNDEFINED_DEPENDENCY )
      {
        Engines_DSC_interface::writeEvent("CP_EFFT",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPIT],"Dependency mode is undefined");
        throw CalciumException(CalciumTypes::CPIT, LOC(OSS()<<"Dependency mode of variable " << nomVar << " is undefined."));
      }

    if ( portDependencyType != CalciumTypes::TIME_DEPENDENCY )
      {
        Engines_DSC_interface::writeEvent("CP_EFFT",containerName,componentName,nomVar.c_str(),CPMESSAGE[CalciumTypes::CPIT],
                   "Dependency mode must be time mode");
        throw CalciumException(CalciumTypes::CPITVR, LOC(OSS()<<"Dependency mode of variable " << nomVar << ": "
                                << portDependencyType << " must be time mode."));
      }

    port->calcium_erase(t, 0,false);

    std::stringstream msg;
    msg << "t>=" << t ;
    Engines_DSC_interface::writeEvent("CP_EFFT",containerName,componentName,nomVar.c_str(),"",msg.str().c_str());

  };

};

#endif
