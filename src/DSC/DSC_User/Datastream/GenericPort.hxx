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

//  File   : GenericPort.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-28 15:26:32 +0100 (mer, 28 fÃ©v 2007) $
// Id          : $Id$
//
#ifndef _GENERIC_PORT_HXX_
#define _GENERIC_PORT_HXX_

#include "CorbaTypeManipulator.hxx"

#include "Superv_Component_i.hxx"
// SALOME CORBA Exception
#include "Utils_CorbaException.hxx"
// SALOME C++   Exception
#include "Utils_SALOME_Exception.hxx"
#include "DSC_Exception.hxx"
#include "utilities.h"

#include <iostream>
#include <map>

// Inclusions pour l'affichage
#include <algorithm>
#include <iterator>

//#define MYDEBUG

// Classe GenericPort
// --------------------------------
//
// Definition: Implemente un port de type "data-stream"
// Cette implémentation gère tous les types de données définies par DataManipulator::type
// Ce port est soumis à une politique d'itération sur les identificateurs de données (DataId)
// Un identificateur de données est construit à partir d'un ou plusieurs paramètres de la méthode put
// tels que :  une date, une itération, un pas de temps ou une combinaison de ces paramètres.

template < typename DataManipulator, class COUPLING_POLICY >
class GenericPort : public COUPLING_POLICY  {
public:
  // Type de données manipulés 
  typedef typename DataManipulator::Type         DataType;
  typedef typename DataManipulator::CorbaInType  CorbaInDataType;

  GenericPort(); 
  virtual ~GenericPort();

  template <typename TimeType,typename TagType> void     put(CorbaInDataType data,  TimeType time, TagType tag);
  template <typename TimeType,typename TagType> DataType get(TimeType time, TagType tag);
  template <typename TimeType,typename TagType> DataType get(TimeType& ti, TimeType tf, TagType tag = 0);
  template <typename TimeType,typename TagType> DataType next(TimeType &t, TagType  &tag );
  void      close (PortableServer::POA_var poa, PortableServer::ObjectId_var id);
  void wakeupWaiting();
  template <typename TimeType,typename TagType> void erase(TimeType time, TagType tag, bool before );

private:

  // Type identifiant une instance de donnee. Exemple (time,tag) 
  typedef typename COUPLING_POLICY::DataId DataId;
  typedef std::map< DataId, DataType>      DataTable;

  // Stockage des donnees recues et non encore distribuées
  DataTable storedDatas ;

  // Indicateur que le destinataire attend une instance particuliere de données
  bool     waitingForConvenientDataId;
  // Indicateur que le destinataire attend n'importe qu'elle instance de données
  bool     waitingForAnyDataId;

  // Identificateur de la donné que le destinataire (propriétaire du port) attend
  DataId   expectedDataId ;
  // Sauvegarde du DataId courant pour la méthode next 
  DataId   lastDataId;
  bool     lastDataIdSet;
  // Exclusion mutuelle d'acces a la table des données reçues
  omni_mutex     storedDatas_mutex;
  // Condition d'attente d'une instance (Le processus du Get attend la condition declaree par le processus Put)
  omni_condition cond_instance;

};

template < typename DataManipulator, typename COUPLING_POLICY >
GenericPort<DataManipulator, COUPLING_POLICY >::GenericPort() :
  cond_instance(& this->storedDatas_mutex),waitingForConvenientDataId(false),
  waitingForAnyDataId(false),lastDataIdSet(false) {}

template < typename DataManipulator, typename COUPLING_POLICY>
GenericPort<DataManipulator, COUPLING_POLICY>::~GenericPort() {
  typename DataTable::iterator it;
  for (it=storedDatas.begin(); it!=storedDatas.end(); ++it) {
#ifdef MYDEBUG
    std::cerr << "~GenericPort() : destruction de la donnnée associée au DataId :"<<  (*it).first << std::endl;
#endif
    DataManipulator::delete_data( (*it).second );
  }
}

template < typename DataManipulator, typename COUPLING_POLICY> void 
GenericPort<DataManipulator, COUPLING_POLICY>::close (PortableServer::POA_var poa, 
                                                      PortableServer::ObjectId_var id) {
  // Ferme le port en supprimant le servant
  // La desactivation du servant du POA provoque sa suppression
  poa->deactivate_object (id);
}

template < typename DataManipulator, typename COUPLING_POLICY> void
GenericPort<DataManipulator, COUPLING_POLICY>::wakeupWaiting()
{
#ifdef MYDEBUG
  std::cout << "-------- wakeupWaiting ------------------" << std::endl;
#endif
  storedDatas_mutex.lock();
  if (waitingForAnyDataId || waitingForConvenientDataId) {
#ifdef MYDEBUG
    std::cout << "-------- wakeupWaiting:signal --------" << std::endl;
    std::cout << std::flush;
#endif
    cond_instance.signal();
   }
  storedDatas_mutex.unlock();

}

/* Methode put_generique
 *
 * Stocke en memoire une instance de donnee (pointeur) que l'emetteur donne a l'intention du destinataire.
 * Reveille le destinataire, si il y a lieu.
 */
template < typename DataManipulator, typename COUPLING_POLICY>
template < typename TimeType,typename TagType>
void GenericPort<DataManipulator, COUPLING_POLICY>::put(CorbaInDataType dataParam, 
                                                        TimeType time, 
                                                        TagType  tag) {
  fflush(stdout);
  fflush(stderr);
  try {
#ifdef MYDEBUG
    // Affichage des donnees pour DEBUGging
    std::cerr << "parametres emis: " << time << ", " << tag << std::endl;
    DataManipulator::dump(dataParam);
#endif
  
    // L'intérêt des paramètres time et tag pour ce port est décidé dans la politique de couplage
    // Il est possible de filtrer en prenant en compte uniquement un paramètre time/tag ou les deux
    // Il est également possible de convertir les données recues ou bien de les dupliquer
    // pour plusieurs  valeurs de time et/ou tag (d'où la notion de container dans la politique de couplage)
    typedef typename COUPLING_POLICY::DataIdContainer DataIdContainer;  
    typedef typename COUPLING_POLICY::DataId          DataId;

    DataId          dataId(time,tag);
    // Effectue les traitements spécifiques à la politique de couplage 
    // pour construire une liste d'ids (par filtrage, conversion ...)
    // DataIdContainer dataIds(dataId,*(static_cast<const COUPLING_POLICY *>(this)));   
    DataIdContainer dataIds(dataId, *this);   

    typename DataIdContainer::iterator dataIdIt = dataIds.begin();

    bool expectedDataReceived = false;

#ifdef MYDEBUG
    std::cout << "-------- Put : MARK 1 ------------------" << std::endl;
#endif
    if ( dataIds.empty() ) return;
#ifdef MYDEBUG
    std::cout << "-------- Put : MARK 1bis ------------------" << std::endl;
#endif

    // Recupere les donnees venant de l'ORB et relâche les structures CORBA 
    // qui n'auraient plus cours en sortie de méthode put
    DataType data = DataManipulator::get_data(dataParam);


    int nbOfIter = 0;

#ifdef MYDEBUG
    std::cout << "-------- Put : MARK 2 ------ "<< (dataIdIt == dataIds.end()) << "------------" << std::endl;
    std::cout << "-------- Put : MARK 2bis "<< (*dataIdIt) <<"------------------" << std::endl;
#endif
    storedDatas_mutex.lock();

    for (;dataIdIt != dataIds.end();++dataIdIt) {

#ifdef MYDEBUG
      std::cout << "-------- Put : MARK 3 ------------------" << std::endl;
#endif
      // Duplique l'instance de donnée pour les autres dataIds 
      if (nbOfIter > 0) data = DataManipulator::clone(data);
#ifdef MYDEBUG
      std::cout << "-------- Put : MARK 3bis -----"<< dataIdIt.operator*() <<"------------" << std::endl;
#endif
    
      DataId currentDataId=*dataIdIt;

#ifdef MYDEBUG
      std::cerr << "processing dataId : "<< currentDataId << std::endl;

      std::cout << "-------- Put : MARK 4 ------------------" << std::endl;
#endif
 
      // Ajoute l'instance de la donnee a sa place dans la table de données
      // ou remplace une instance précédente si elle existe
    
      // Recherche la première clé telle quelle ne soit pas <  currentDataId
      // pour celà l'opérateur de comparaison storedDatas.key_comp() est utilisé
      // <=> premier emplacement où l'on pourrait insérer notre DataId
      // <=> en général équivaux à (*wDataIt).first >= currentDataId
      typename DataTable::iterator wDataIt = storedDatas.lower_bound(currentDataId);
#ifdef MYDEBUG
      std::cout << "-------- Put : MARK 5 ------------------" << std::endl;
#endif

      // On n'a pas trouvé de dataId supérieur au notre ou 
      // on a trouvé une clé >  à cet Id          
      if (wDataIt == storedDatas.end() || storedDatas.key_comp()(currentDataId,(*wDataIt).first) ) {
#ifdef MYDEBUG
        std::cout << "-------- Put : MARK 6 ------------------" << std::endl;
#endif
        // Ajoute la donnee dans la table
        wDataIt = storedDatas.insert(wDataIt, make_pair (currentDataId, data));
      } else  {
        // Si on n'est pas en fin de liste et qu'il n'y a pas de relation d'ordre strict
        // entre notre dataId et le DataId pointé c'est qu'ils sont identiques
#ifdef MYDEBUG
        std::cout << "-------- Put : MARK 7 ------------------" << std::endl;
#endif
        // Les données sont remplacées par les nouvelles valeurs
        // lorsque que le dataId existe déjà
        DataType old_data = (*wDataIt).second;
        (*wDataIt).second = data;
        // Detruit la vieille donnee
        DataManipulator::delete_data (old_data);
      }
  
#ifdef MYDEBUG
      std::cout << "-------- Put : MARK 8 ------------------" << std::endl;
#endif
      // Compte le nombre de dataIds à traiter
      ++nbOfIter;

#ifdef MYDEBUG
      std::cout << "-------- Put : waitingForConvenientDataId : " << waitingForConvenientDataId <<"---" << std::endl;
      std::cout << "-------- Put : waitingForAnyDataId : " << waitingForAnyDataId <<"---" << std::endl;
      std::cout << "-------- Put : currentDataId  : " << currentDataId <<"---" << std::endl;
      std::cout << "-------- Put : expectedDataId : " << expectedDataId <<"---" << std::endl;
      std::cout << "-------- Put : MARK 9 ------------------" << std::endl;
#endif

      // A simplifier mais :
      // - pas possible de mettre des arguments optionnels à cause
      //   du type itérator qui n'est pas connu (pas de possibilité de déclarer un static )
      // - compliquer de créer une méthode sans les paramètres inutiles tout en réutilisant
      //   la méthode initiale car cette dernière ne peut pas être déclarée virtuelle 
      //   à cause de ses paramètres templates. Du coup, il faudrait aussi redéfinir la
      //   méthode simplifiée dans les classes définissant une politique 
      //   de couplage particulière ...
      bool dummy1,dummy2; typename DataTable::iterator dummy3;
      // Par construction, les valeurs de waitingForAnyDataId, waitingForConvenientDataId et de 
      // expectedDataId ne peuvent pas être modifiées pendant le traitement de la boucle
      // sur les dataIds (à cause du lock utilisé dans la méthode put et les méthodes get )
      // rem : Utilisation de l'évaluation gauche droite du logical C or
      if ( waitingForAnyDataId || 
           ( waitingForConvenientDataId && 
             this->isDataIdConveniant(storedDatas, expectedDataId, dummy1, dummy2, dummy3) ) 
           ) {
#ifdef MYDEBUG
        std::cout << "-------- Put : MARK 10 ------------------" << std::endl;
#endif
        //Doit pouvoir réveiller le get ici (a vérifier)
        expectedDataReceived = true;
      }
    }
   
    if (expectedDataReceived) {
#ifdef MYDEBUG
      std::cout << "-------- Put : MARK 11 ------------------" << std::endl;
#endif
      // si waitingForAnyDataId était positionné, c'est forcément lui qui a activer
      // expectedDataReceived à true
      if (waitingForAnyDataId) 
        waitingForAnyDataId        = false;
      else 
        waitingForConvenientDataId = false;
      // Reveille le thread du destinataire (stoppe son attente)
      // Ne faudrait-il pas réveiller plutôt tous les threads ?
      // Celui  réveillé ne correspond pas forcément à celui qui demande
      // cet expectedDataReceived.
      // Pb1 : cas d'un un get séquentiel et d'un get sur un dataId que l'on vient de recevoir.
      // Si l'on reveille le mauvais thread, l'autre va attendre indéfiniment ! (sauf timeout)
      // Pb2 : également si deux attentes de DataIds même différents car on n'en stocke qu'un !
      // Conclusion : Pour l'instant on ne gère pas un service multithreadé qui effectue
      // des lectures simultanées sur le même port !
#ifdef MYDEBUG
      std::cerr << "-------- Put : new datas available ------------------" << std::endl;
#endif
      fflush(stdout);fflush(stderr);
      cond_instance.signal();
    }
#ifdef MYDEBUG
    std::cout << "-------- Put : MARK 12 ------------------" << std::endl;
#endif

    // Deverouille l'acces a la table : On peut remonter l'appel au dessus de expected...
    storedDatas_mutex.unlock();

#ifdef MYDEBUG
    std::cout << "-------- Put : MARK 13 ------------------" << std::endl;
#endif
    fflush(stdout);
    fflush(stderr);

  } // Catch les exceptions SALOME//C++ pour la transformer en une exception SALOME//CORBA  
  catch ( const SALOME_Exception & ex ) {
    // On évite de laisser un  mutex
    storedDatas_mutex.unlock();
    THROW_SALOME_CORBA_EXCEPTION(ex.what(), SALOME::INTERNAL_ERROR);
  }

}

// erase data before time or tag
template < typename DataManipulator, typename COUPLING_POLICY >
template <typename TimeType,typename TagType>
void
GenericPort<DataManipulator, COUPLING_POLICY>::erase(TimeType time, TagType  tag, bool before)
{
  typename COUPLING_POLICY::template EraseDataIdBeforeOrAfterTagProcessor<DataManipulator> processEraseDataId(*this);
  processEraseDataId.apply(storedDatas,time,tag,before);
}

// Version du Get en 0 copy
// ( n'effectue pas de recopie de la donnée trouvée dans storedDatas )
// ( L'utilisateur devra être attentif à la politique de gestion de l'historique
//   spécifique au mode de couplage car il peut y avoir une suppression potentielle 
//   d'une donnée utilisée directement dans le code utilisateur )
//  Le code doit prendre connaissance du transfert de propriété ou non des données
//  auprès du mode de couplage choisi. 
template < typename DataManipulator, typename COUPLING_POLICY >
template < typename TimeType,typename TagType>
typename DataManipulator::Type 
GenericPort<DataManipulator, COUPLING_POLICY>::get(TimeType time, 
                                                   TagType  tag)
// REM : Laisse passer toutes les exceptions
//       En particulier les SALOME_Exceptions qui viennent de la COUPLING_POLICY
//       Pour déclarer le throw avec l'exception spécifique il faut que je vérifie
//       qu'un setunexpeted est positionné sinon le C++ arrête tout par appel à terminate
{
  typedef typename COUPLING_POLICY::DataId DataId;
  // (Pointeur sur séquence) ou valeur..
  DataType dataToTransmit ;
  bool     isEqual, isBounded;
  typedef typename DataManipulator::InnerType InnerType;

#ifdef MYDEBUG
  std::cout << "-------- Get : MARK 1 ------------------" << std::endl;
#endif
  expectedDataId   = DataId(time,tag);
#ifdef MYDEBUG
  std::cout << "-------- Get : MARK 2 ------------------" << std::endl;
#endif
 
  typename DataTable::iterator wDataIt1;

  try {
    storedDatas_mutex.lock(); // Gérer les Exceptions ds le corps de la méthode
  
    while ( true ) {
 
      // Renvoie isEqual si le dataId attendu est trouvé dans storedDatas :
      //   - l'itérateur wDataIt1 pointe alors sur ce dataId
      // Renvoie isBounded si le dataId attendu n'est pas trouvé mais encadrable et 
      // que la politique  gére ce cas de figure 
      //   - l'itérateur wDataIt1 est tel que wDataIt1->first < wdataId < (wDataIt1+1)->first
      // Méthode provenant de la COUPLING_POLICY
      this->isDataIdConveniant(storedDatas,expectedDataId,isEqual,isBounded,wDataIt1);
#ifdef MYDEBUG
      std::cout << "-------- Get : MARK 3 ------------------" << std::endl;
#endif

      // L'ordre des différents tests est important
      if ( isEqual ) {
 
#ifdef MYDEBUG
        std::cout << "-------- Get : MARK 4 ------------------" << std::endl;
#endif
        // La propriété de la données N'EST PAS transmise à l'utilisateur en mode CALCIUM.
        // Si l'utilisateur supprime la donnée, storedDataIds devient incohérent
        // C'est EraseDataId qui choisi ou non de supprimer la donnée
        // Du coup interaction potentielle entre le 0 copy et gestion de l'historique
        dataToTransmit = (*wDataIt1).second; 

#ifdef MYDEBUG
        std::cout << "-------- Get : MARK 5 ------------------" << std::endl;
        std::cout << "-------- Get : Données trouvées à t : " << std::endl;
        typename DataManipulator::InnerType const * const InIt1 = DataManipulator::getPointer(dataToTransmit);
        size_t   N = DataManipulator::size(dataToTransmit);
        std::copy(InIt1,        InIt1 + N,
                  std::ostream_iterator< InnerType > (std::cout," "));
        std::cout << std::endl;
#endif

        // Décide de la suppression de certaines  instances de données 
        // La donnée contenu dans la structure CORBA et son dataId sont désallouées
        // Méthode provenant de la COUPLING_POLICY 
        typename COUPLING_POLICY::template EraseDataIdProcessor<DataManipulator> processEraseDataId(*this);
        processEraseDataId.apply(storedDatas,wDataIt1);
#ifdef MYDEBUG
        std::cout << "-------- Get : MARK 6 ------------------" << std::endl;
#endif
        break;

      }
#ifdef MYDEBUG
      std::cout << "-------- Get : MARK 7 ------------------" << std::endl;
#endif

      //if (  isBounded() && COUPLING_POLICY::template needToProcessBoundedDataId() ) {
      // Le DataId demandé n'est pas trouvé mais est encadré ET la politique de couplage
      // implémente une méthode processBoundedDataId capable de générer les données à retourner
      if (  isBounded ) {
        // Pour être cohérent avec la politique du bloc précédent
        // on stocke la paire (dataId,données interpolées ).
        // CALCIUM ne stockait pas les données interpolées. 
        // Cependant  comme les données sont censées être produites
        // par ordre croissant de DataId, de nouvelles données ne devrait pas améliorer
        // l'interpolation.
#ifdef MYDEBUG
        std::cout << "-------- Get : MARK 8 ------------------" << std::endl;
#endif

        typedef typename COUPLING_POLICY::template BoundedDataIdProcessor<DataManipulator> BDI;
        BDI processBoundedDataId(*this);
        //        typename COUPLING_POLICY::template BoundedDataIdProcessor<DataManipulator> processBoundedDataId(*this);
        //si static BDIP::apply(dataToTransmit,expectedDataId,wDataIt1);
        //ancienne version template processBoundedDataId<DataManipulator>(dataToTransmit,expectedDataId,wDataIt1);
        //BDIP processBoundedDataId;
        processBoundedDataId.apply(dataToTransmit,expectedDataId,wDataIt1);
  
        // Il ne peut pas y avoir déjà une clé expectedDataId dans storedDatas (utilisation de la notation [] )
        // La nouvelle donnée produite est stockée, ce n'était pas le cas dans CALCIUM
        // Cette opération n'a peut être pas un caractère générique.
        // A déplacer en paramètre de la méthode précédente ? ou déléguer ce choix au mode de couplage ?
        storedDatas[expectedDataId]=dataToTransmit;

#ifdef MYDEBUG
        std::cout << "-------- Get : Données calculées à t : " << std::endl;
        typename DataManipulator::InnerType const * const InIt1 = DataManipulator::getPointer(dataToTransmit);
        size_t   N = DataManipulator::size(dataToTransmit);
 
        std::copy(InIt1,        InIt1 + N,
                  std::ostream_iterator< InnerType > (std::cout," "));
        std::cout << std::endl;
        std::cout << "-------- Get : MARK 9 ------------------" << std::endl;
#endif

        typename COUPLING_POLICY::template EraseDataIdProcessor<DataManipulator> processEraseDataId(*this);
        processEraseDataId.apply(storedDatas,wDataIt1);
   
        break;
      }
  
      // Délègue au mode de couplage la gestion d'une demande de donnée non disponible 
      // si le port est deconnecté
      typename COUPLING_POLICY::template DisconnectProcessor<DataManipulator> processDisconnect(*this);
      if ( processDisconnect.apply(storedDatas, expectedDataId, wDataIt1) ) continue;
    
      // Réception bloquante sur le dataId demandé
      // Si l'instance de donnée n'est pas trouvee
#ifdef MYDEBUG
      std::cout << "-------- Get : MARK 10 ------------------" << std::endl;
#endif
      //Positionné à faux dans la méthode put
      waitingForConvenientDataId = true; 
#ifdef MYDEBUG
      std::cout << "-------- Get : MARK 11 ------------------" << std::endl;
     
      // Ici on attend que la méthode put recoive la donnée 
      std::cout << "-------- Get : waiting datas ------------------" << std::endl;
#endif
      fflush(stdout);fflush(stderr);
      unsigned long ts, tns,rs=Superv_Component_i::dscTimeOut;
      if(rs==0)
        cond_instance.wait();
      else
        {
          //Timed wait on omni condition
          omni_thread::get_time(&ts,&tns, rs,0);
          int success=cond_instance.timedwait(ts,tns);
          if(!success)
            {
              // Waiting too long probably blocking
              std::stringstream msg;
              msg<<"Timeout ("<<rs<<" s) exceeded";
              Engines_DSC_interface::writeEvent("BLOCKING","","","","Probably blocking",msg.str().c_str());
              throw DSC_Exception(msg.str());
            }
        }


#ifdef MYDEBUG
      std::cout << "-------- Get : MARK 12 ------------------" << std::endl;
#endif
    }

  } catch (...) {
    waitingForConvenientDataId = true;
    storedDatas_mutex.unlock();
    throw;
  }

  // Deverouille l'acces a la table
  storedDatas_mutex.unlock();
#ifdef MYDEBUG
  std::cout << "-------- Get : MARK 13 ------------------" << std::endl;
#endif

  // La propriété de la données N'EST PAS transmise à l'utilisateur en mode CALCIUM
  // Si l'utilisateur supprime la donnée, storedDataIds devient incohérent
  // c'est eraseDataId qui choisi ou non de supprimer la donnée
  // Du coup interaction potentielle entre le 0 copy et gestion des niveaux 
  return dataToTransmit; 

}

template < typename DataManipulator, typename COUPLING_POLICY >
template < typename TimeType,typename TagType>
typename DataManipulator::Type 
GenericPort<DataManipulator, COUPLING_POLICY>::get(TimeType& ti,
                                                   TimeType tf, 
                                                   TagType  tag ) {
  ti = COUPLING_POLICY::getEffectiveTime(ti,tf);
  return get(ti,tag);
}


// Version du next en 0 copy
// ( n'effectue pas de recopie de la donnée trouvée dans storedDatas )
template < typename DataManipulator, typename COUPLING_POLICY >
template < typename TimeType,typename TagType>
typename DataManipulator::Type 
GenericPort<DataManipulator, COUPLING_POLICY>::next(TimeType &t,
                                                    TagType  &tag ) {
 
  typedef  typename COUPLING_POLICY::DataId DataId;

  DataType dataToTransmit;
  DataId   dataId;

  try {
    storedDatas_mutex.lock();// Gérer les Exceptions ds le corps de la méthode

#ifdef MYDEBUG
    std::cout << "-------- Next : MARK 1 ---lastDataIdSet ("<<lastDataIdSet<<")---------------" << std::endl;
#endif

    typename DataTable::iterator wDataIt1;
    wDataIt1 = storedDatas.end();

    //Recherche le prochain dataId à renvoyer
    // - lastDataIdset == true indique que lastDataId
    // contient le dernier DataId renvoyé
    // - lastDataIdset == false indique que l'on renverra
    //   le premier dataId trouvé
    // - upper_bound(lastDataId) situe le prochain DataId
    // à renvoyer
    // Rem : les données renvoyées ne sont effacées par eraseDataIds
    //       si necessaire
    if (lastDataIdSet) 
      wDataIt1 = storedDatas.upper_bound(lastDataId);
    else if ( !storedDatas.empty() ) {
      lastDataIdSet = true;
      wDataIt1      = storedDatas.begin();
    }

    typename COUPLING_POLICY::template DisconnectProcessor<DataManipulator> processDisconnect(*this);

    while ( storedDatas.empty() || wDataIt1 == storedDatas.end() ) {

      // Délègue au mode de couplage la gestion d'une demande de donnée non disponible 
      // si le port est deconnecté
      if ( processDisconnect.apply(storedDatas, lastDataId, wDataIt1) )  {
        waitingForAnyDataId = false; break;
      }
  
#ifdef MYDEBUG
      std::cout << "-------- Next : MARK 2 ------------------" << std::endl;
#endif
      //Positionné à faux dans la méthode put
      waitingForAnyDataId   = true;
#ifdef MYDEBUG
      std::cout << "-------- Next : MARK 3 ------------------" << std::endl;
      // Ici on attend que la méthode put recoive la donnée 
      std::cout << "-------- Next : waiting datas ------------------" << std::endl;
#endif
      fflush(stdout);fflush(stderr);
      unsigned long ts, tns,rs=Superv_Component_i::dscTimeOut;
      if(rs==0)
        cond_instance.wait();
      else
        {
          //Timed wait on omni condition
          omni_thread::get_time(&ts,&tns, rs,0);
          int success=cond_instance.timedwait(ts,tns);
          if(!success)
            {
              // Waiting too long probably blocking
              std::stringstream msg;
              msg<<"Timeout ("<<rs<<" s) exceeded";
              Engines_DSC_interface::writeEvent("BLOCKING","","","","Probably blocking",msg.str().c_str());
              throw DSC_Exception(msg.str());
            }
        }

      if (lastDataIdSet) {
#ifdef MYDEBUG
        std::cout << "-------- Next : MARK 4 ------------------" << std::endl;
#endif
        wDataIt1 = storedDatas.upper_bound(lastDataId);
      } else  {
#ifdef MYDEBUG
        std::cout << "-------- Next : MARK 5 ------------------" << std::endl;
#endif
        lastDataIdSet = true;
        wDataIt1      = storedDatas.begin();
      }
    }

#ifdef MYDEBUG
    std::cout << "-------- Next : MARK 6 ------------------" << std::endl;
#endif

    t   = this->getTime( (*wDataIt1).first );
    tag = this->getTag ( (*wDataIt1).first );
    dataToTransmit = (*wDataIt1).second;
 
#ifdef MYDEBUG
    std::cout << "-------- Next : MARK 7 ------------------" << std::endl;
#endif
    lastDataId    = (*wDataIt1).first;

    typename COUPLING_POLICY::template EraseDataIdProcessor<DataManipulator> processEraseDataId(*this);
    processEraseDataId.apply(storedDatas, wDataIt1);

#ifdef MYDEBUG
    std::cout << "-------- Next : MARK 8 ------------------" << std::endl;   
#endif
  } catch (...) {
#ifdef MYDEBUG
    std::cout << "-------- Next : MARK 8bis ------------------" << std::endl;
#endif
    waitingForAnyDataId = false;
    storedDatas_mutex.unlock();
    throw;
  }
  storedDatas_mutex.unlock();
  
#ifdef MYDEBUG
  std::cout << "-------- Next : MARK 9 ------------------" << std::endl;
#endif

  // La propriété de la données N'EST PAS transmise à l'utilisateur en mode CALCIUM
  // Si l'utilisateur supprime la donnée, storedDataIds devient incohérent
  // c'est eraseDataId qui choisi ou non de supprimer la donnée
  // Du coup interaction potentielle entre le 0 copy et gestion des niveaux 
  return dataToTransmit; 

};

#endif
