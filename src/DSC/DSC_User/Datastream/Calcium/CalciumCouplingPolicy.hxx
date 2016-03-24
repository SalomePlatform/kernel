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

//  File   : CalciumCouplingPolicy.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Id          : $Id$
//
#ifndef __CALCIUM_COUPLING_POLICY__ 
#define __CALCIUM_COUPLING_POLICY__

#include <vector>
#include <map>

#include "DisplayPair.hxx"
#include "CouplingPolicy.hxx"
#include "AdjacentFunctor.hxx"
#include <boost/lambda/lambda.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include "CalciumTypes.hxx"
#include "CalciumException.hxx"

//#define MYDEBUG

class CalciumCouplingPolicy : public CouplingPolicy  {


public:

  template <typename T_TIME, typename T_TAG >        class InternalDataIdContainer;
  template <typename T_TIME, typename T_TAG > friend class InternalDataIdContainer;
  template <typename DataManipulator, 
    class EnableIf >                  friend class BoundedDataIdProcessor;
  template <typename DataManipulator >        friend class EraseDataIdProcessor;
  template <typename DataManipulator >        friend class EraseDataIdBeforeOrAfterTagProcessor;
  template <typename DataManipulator >        friend class DisconnectProcessor;

  typedef CalciumTypes::DependencyType       DependencyType;
  typedef CalciumTypes::DateCalSchem         DateCalSchem;
  typedef CalciumTypes::InterpolationSchem   InterpolationSchem;
  typedef CalciumTypes::ExtrapolationSchem   ExtrapolationSchem;
  typedef CalciumTypes::DisconnectDirective  DisconnectDirective;  

private:

  DependencyType      _dependencyType;
  size_t              _storageLevel;
  DateCalSchem        _dateCalSchem;
  InterpolationSchem  _interpolationSchem;
  ExtrapolationSchem  _extrapolationSchem;
  double              _alpha;
  double              _deltaT;
  DisconnectDirective _disconnectDirective;

public:
  CalciumCouplingPolicy();

  void           setDependencyType (DependencyType dependencyType);
  DependencyType getDependencyType () const;
 
  void   setStorageLevel   (size_t storageLevel);
  size_t getStorageLevel   () const;

  void         setDateCalSchem   (DateCalSchem   dateCalSchem);
  DateCalSchem getDateCalSchem () const;

  void   setAlpha(double alpha);
  double getAlpha() const ;

  void   setDeltaT(double deltaT );
  double getDeltaT() const ;

  void setInterpolationSchem (InterpolationSchem interpolationSchem);
  void setExtrapolationSchem (ExtrapolationSchem extrapolationSchem);
  InterpolationSchem getInterpolationSchem () const ;
  ExtrapolationSchem getExtrapolationSchem () const ;

  // Classe DataId rassemblant les paramètres de la méthode PORT::put 
  // qui identifient l'instance d'une donnée pour Calcium
  // Rem : Le DataId doit pouvoir être une key dans une map stl
  typedef double TimeType;
  typedef long   TagType;
  typedef std::pair< TimeType , TagType >     DataId;
  typedef InternalDataIdContainer < TimeType , TagType >  DataIdContainer;
  typedef std::vector< DataId >::iterator  iterator;

  template <typename T_TIME, typename T_TAG >  
  struct InternalDataIdContainer;

  inline TimeType getTime(const DataId &dataId) const { return dataId.first;}
  inline TagType  getTag (const DataId &dataId) const { return dataId.second;}

  template <typename DataManipulator, 
            class EnableIf = void >    struct BoundedDataIdProcessor;
  //template <typename DataManipulator>  struct BoundedDataIdProcessor;
  template <typename DataManipulator>  struct EraseDataIdProcessor;
  template <typename DataManipulator>  struct EraseDataIdBeforeOrAfterTagProcessor;
  template <typename DataManipulator>  struct DisconnectProcessor;

  // Renvoie isEqual si le dataId attendu est trouvé dans storedDataIds :
  //   - l'itérateur wDataIt1 pointe alors sur ce dataId
  // Renvoie isBounded si le dataId attendu n'est pas trouvé mais encadrable et 
  // que la politique de couplage gére ce cas de figure 
  //   - l'itérateur wDataIt1 est tel que wDataIt1->first < wdataId < (wDataIt1+1)->first
  // Le container doit être associatif
  template < typename AssocContainer >
  bool isDataIdConveniant( AssocContainer & storedDatas, 
                           const typename AssocContainer::key_type & expectedDataId,
                           bool & isEqual, bool & isBounded, 
                           typename AssocContainer::iterator & wDataIt1) const;

  TimeType getEffectiveTime(TimeType ti, TimeType tf);

  void disconnect(bool provideLastGivenValue);

}; //Fin de CalciumCouplingPolicy



//*************   DEFINITION DES METHODES ET OBJETS TEMPLATES *************// 



// Définition du container de DataId pour répondre au concept
// de mode de couplage
template <typename T_TIME, typename T_TAG > 
struct CalciumCouplingPolicy::InternalDataIdContainer : public std::vector< std::pair< T_TIME,T_TAG> >  {
  typedef std::vector < DataId >        DataIdVect;
    
  InternalDataIdContainer(const DataId & dataId, 
                          const CalciumCouplingPolicy & policy
                          ):std::vector< std::pair< T_TIME,T_TAG> >() {
    // Ignore les paramètres qui ne sont pas en rapport avec le type de dépendance
    switch (policy._dependencyType) {
    case CalciumTypes::TIME_DEPENDENCY:
      this->push_back(DataId(dataId.first,0));
      break;
    case CalciumTypes::ITERATION_DEPENDENCY:
      this->push_back(DataId(0,dataId.second));
      break;
    default:
      throw(CalciumException(CalciumTypes::CPIT,LOC("The dependency type must be set by setDependencyType before calling DataIdContainer contructor")));
      break;
    }
  };
};


template <typename DataManipulator, class EnableIf >
struct CalciumCouplingPolicy::BoundedDataIdProcessor{
  BoundedDataIdProcessor(const CouplingPolicy & couplingPolicy) {};
  template < typename Iterator, typename DataId > 
  void inline apply(typename iterator_t<Iterator>::value_type & data,
                    const DataId & dataId,
                    const Iterator  & it1) const {
    typedef typename iterator_t<Iterator>::value_type value_type;
#ifdef MYDEBUG
    std::cout << "-------- Calcium Generic BoundedDataIdProcessor.apply() called " << std::endl;
#endif

  }
};


template <typename DataManipulator >
struct CalciumCouplingPolicy::BoundedDataIdProcessor<
  DataManipulator, 
  typename boost::enable_if< boost::is_float< typename DataManipulator::InnerType> >::type > {
    
  const CalciumCouplingPolicy & _couplingPolicy;
    
  BoundedDataIdProcessor(const CalciumCouplingPolicy &couplingPolicy):
    _couplingPolicy(couplingPolicy) {};
    
  // Méthode implémentant l'interpolation temporelle
  template < typename MapIterator > 
  void inline apply (typename iterator_t<MapIterator>::value_type & data,
                     const DataId & dataId, const MapIterator & it1) const {
      
    typedef typename iterator_t<MapIterator>::value_type value_type;
    typedef typename DataManipulator::InnerType InnerType;
    typedef typename DataManipulator::Type Type;

    MapIterator it2=it1; ++it2;
    size_t   dataSize1 = DataManipulator::size(it1->second);
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::BoundedDataIdProcessor : Taille de donnée dataId1 : " << dataSize1 << std::endl;
#endif
 
    // Gérer dans calcium la limite de la taille du buffer donnée par
    // l'utilisateur.
    size_t   dataSize2 = DataManipulator::size(it2->second);
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::BoundedDataIdProcessor : Taille de donnée dataId2 : " << dataSize2 << std::endl;
#endif

    size_t   dataSize  = std::min< size_t >( dataSize1, dataSize2 );
    DataId   dataId2 = it2->first;
    DataId   dataId1 = it1->first;
    TimeType t2      = dataId2.first;
    TimeType t1      = dataId1.first;
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::BoundedDataIdProcessor : Valeur de t1 : " << t1 << std::endl;
    std::cout << "-------- CalciumCouplingPolicy::BoundedDataIdProcessor : Valeur de t2 : " << t2 << std::endl;
#endif
    TimeType t       = dataId.first;
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::BoundedDataIdProcessor : Valeur de t : " << t << std::endl;
#endif
    TimeType timeDiff  = t2-t1;
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::BoundedDataIdProcessor : Valeur de timeDiff : " << timeDiff << std::endl;
#endif
    TimeType coeff   = (t2-t)/timeDiff;
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::BoundedDataIdProcessor : Valeur de coeff : " << coeff << std::endl;
#endif

    InnerType const * const InIt1 = DataManipulator::getPointer(it1->second);
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::BoundedDataIdProcessor : Données à t1 : " << std::endl;
    std::copy(InIt1,InIt1+dataSize1,std::ostream_iterator<InnerType>(std::cout," "));
    std::cout << std::endl;
#endif
    InnerType const * const InIt2 = DataManipulator::getPointer(it2->second);
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::BoundedDataIdProcessor : Données à t2 : " << std::endl;
    std::copy(InIt2,InIt2+dataSize2,std::ostream_iterator<InnerType>(std::cout," "));
    std::cout << std::endl;
#endif
    Type              dataOut = DataManipulator::create(dataSize);
    InnerType * const OutIt   = DataManipulator::getPointer(dataOut);
 
#ifdef MYDEBUG
    std::cerr << "-------- CalciumCouplingPolicy::BoundedDataIdProcessor : interpolationSchem : " << _couplingPolicy._interpolationSchem << std::endl;
    std::cerr << "-------- CalciumCouplingPolicy::BoundedDataIdProcessor : alpha : " << _couplingPolicy._alpha << std::endl;
    std::cerr << "-------- CalciumCouplingPolicy::BoundedDataIdProcessor : datecalschem : " << _couplingPolicy._dateCalSchem << std::endl;
    std::cerr << "-------- CalciumCouplingPolicy::BoundedDataIdProcessor : storageLevel : " << _couplingPolicy._storageLevel << std::endl;
#endif
    if ( timeDiff == 0.0 ||  _couplingPolicy._interpolationSchem == CalciumTypes::L0_SCHEM ) {
      std::copy(InIt1,InIt1+dataSize,OutIt);
    } else {

      boost::lambda::placeholder1_type _1;
      boost::lambda::placeholder2_type _2;
      // OLD: REM : Pour des buffers de type int
      // OLD: le compilo indiquera warning: converting to `long int' from `Double'
      std::transform(InIt1,InIt1+dataSize,InIt2,OutIt,
                     ( _1 - _2 ) * coeff + _2 );
//       for(size_t i =0;  i < dataSize3; ++i) {
//      OutIt[i]=(InIt1[i] - InIt2[i]) * coeff + InIt2[i];
//       }

    }
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::BoundedDataIdProcessor : Données calculées à t : " << std::endl;
    std::copy(OutIt,OutIt+dataSize,std::ostream_iterator<InnerType>(std::cout," "));
    std::cout << std::endl;
#endif
    data = dataOut;
    
  }
};

// Renvoie isEqual si le dataId attendu est trouvé dans storedDataIds :
//   - l'itérateur wDataIt1 pointe alors sur ce dataId
// Renvoie isBounded si le dataId attendu n'est pas trouvé mais encadrable et 
// que la politique de couplage gére ce cas de figure 
//   - l'itérateur wDataIt1 est tel que wDataIt1->first < wdataId < (wDataIt1+1)->first
// Le container doit être associatif
template < typename AssocContainer >
bool CalciumCouplingPolicy::isDataIdConveniant( AssocContainer & storedDatas, const typename AssocContainer::key_type & expectedDataId,
                                                bool & isEqual, bool & isBounded, typename AssocContainer::iterator & wDataIt1) const {
 
  // Rem : le type key_type == DataId
  typedef typename AssocContainer::key_type key_type;
  AdjacentFunctor< key_type > af(expectedDataId);
  if ( _dependencyType == CalciumTypes::TIME_DEPENDENCY )
  {
#ifdef MYDEBUG
    std::cout << "-------- time expected : " << expectedDataId.first << std::endl;
    std::cout << "-------- time expected corrected : " << expectedDataId.first*(1.0-_deltaT) << std::endl;
#endif
    af.setMaxValue(key_type(expectedDataId.first*(1.0-_deltaT),0));
  }
  isBounded = false;

  // Rem 1 :
  // L'algo adjacent_find ne peut être utilisé avec l'AdjacentPredicate 
  //   - si la table contient un seul élément l'algorithme adjacent_find retourne end()
  //     que se soit l'élément attendu ou non
  //   - si la table contient deux éléments dont le dernier est celui recherché
  //     l'algorithme adjacent_find retourne end() aussi
  //   d'ou la necessité d'effectuer  un find avant ou d'écrire un algorithme ad hoc
 
  // Rem 2 :
  //
  // L'algo find_if ne peut être utilisé car il recopie l'AdjacentFunctor
  // qui ne peut alors pas mémoriser ses états précédents
  //    
 
  // Un codage en reverse serait plus efficace
  typename AssocContainer::iterator prev    = storedDatas.begin();
  typename AssocContainer::iterator current = prev;
  while ( (current != storedDatas.end()) && !af(current->first)  ) 
  {
#ifdef MYDEBUG
    std::cerr << "------- stored time : " << current->first << std::endl;
#endif
    //  if ( af(current->first) ) break;
    prev = current++;
  }

  isEqual = af.isEqual();
    
  // On considère qu'il n'est pas possible d'encadrer en dépendance itérative,
  // on se veut pas calculer d'interpolation. 
  if  ( _dependencyType == CalciumTypes::TIME_DEPENDENCY)  isBounded = af.isBounded();

  if ( isEqual ) wDataIt1 = current;
  else 
    if (isBounded) wDataIt1 = prev;
    else
      wDataIt1 = storedDatas.end();

#ifdef MYDEBUG
  std::cout << "-------- isDataIdConvenient : isEqual : " << isEqual << " , isBounded " << isBounded << std::endl;
#endif

  return isEqual || isBounded;
}

//Remove DataId before or after a given time or tag
template < typename DataManipulator > 
struct CalciumCouplingPolicy::EraseDataIdBeforeOrAfterTagProcessor
{
  CalciumCouplingPolicy &_couplingPolicy;
    
  EraseDataIdBeforeOrAfterTagProcessor(CalciumCouplingPolicy &couplingPolicy):
    _couplingPolicy(couplingPolicy) {};

  template < typename Container,typename TimeType,typename TagType >
  void apply(Container & storedDatas, TimeType time, TagType tag, bool before) const 
    {
      typedef typename Container::iterator   iterator;
      typedef typename Container::reverse_iterator   riterator;

      if(_couplingPolicy._dependencyType == CalciumTypes::TIME_DEPENDENCY)
        {
          if(before)
            {
              iterator it=storedDatas.begin();
              while(it != storedDatas.end() && it->first.first <= time)
                {
                  DataManipulator::delete_data(it->second);
                  storedDatas.erase(it);
                  it=storedDatas.begin();
                }
            }
          else
            {
              riterator it=storedDatas.rbegin();
              while(it != storedDatas.rend() && it->first.first >= time)
                {
                  DataManipulator::delete_data(it->second);
                  storedDatas.erase(it->first);
                  it=storedDatas.rbegin();
                }
            }
        }
      else
        {
          if(before)
            {
              iterator it=storedDatas.begin();
              while(it != storedDatas.end() && it->first.second <= tag)
                {
                  DataManipulator::delete_data(it->second);
                  storedDatas.erase(it);
                  it=storedDatas.begin();
                }
            }
          else
            {
              riterator it=storedDatas.rbegin();
              while(it != storedDatas.rend() && it->first.second >= tag)
                {
                  DataManipulator::delete_data(it->second);
                  storedDatas.erase(it->first);
                  it=storedDatas.rbegin();
                }
            }
        }
    }
};

// TODO :PAS ENCORE TESTE AVEC UN NIVEAU POSITIONNE
// Supprime les DataId et les données associées
// du container associatif quand le nombre
// de données stockées dépasse le niveau CALCIUM.
// Cette méthode est appelée de GenericPort::get et GenericPort::next 
// TODO : Elle devrait également être appelée dans GenericPort::Put
// mais il faut étudier les interactions avec GenericPort::Get et GenericPort::next
template < typename DataManipulator > 
struct CalciumCouplingPolicy::EraseDataIdProcessor {

  CalciumCouplingPolicy &_couplingPolicy;
    
  EraseDataIdProcessor(CalciumCouplingPolicy &couplingPolicy):
    _couplingPolicy(couplingPolicy) {};

  template < typename Container >
  void apply(Container & storedDatas, 
             typename Container::iterator & wDataIt1 ) const {

    typedef typename Container::key_type   key_type;
    typedef typename Container::value_type value_type;
    typedef typename Container::iterator iterator;

#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::eraseDataId, storedDatasSize : " << storedDatas.size() << std::endl;
#endif
 
    if ( _couplingPolicy._storageLevel == CalciumTypes::UNLIMITED_STORAGE_LEVEL ) return;
 
    size_t storedDatasSize = storedDatas.size();
    long   s = storedDatasSize - _couplingPolicy._storageLevel;
    if (s > 0 ) {
      size_t dist=distance(storedDatas.begin(),wDataIt1);
      for (int i=0; i<s; ++i) {
              //no bug if removed : DataManipulator::delete_data((*storedDatas.begin()).second);
              DataManipulator::delete_data((*storedDatas.begin()).second);
              storedDatas.erase(storedDatas.begin());
      }
      // Si l'itérateur pointait sur une valeur que l'on vient de supprimer
      if (dist < s ) {
        throw(CalciumException(CalciumTypes::CPNTNULL,LOC(OSS()<< "StorageLevel management " 
                                            << _couplingPolicy._storageLevel << 
                                            " has just removed the data to send")));
      }
    }
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::eraseDataId, new storedDatasSize : " << storedDatas.size() << std::endl;
#endif
    return;

  }
};


// Lorsque cette méthode est appelée depuis GenericPort::Get 
// l'expectedDataId n'a pas été trouvé et n'est pas non plus 
// encadré (en mode temporel).
// Si apply n'effectue pas de traitement particulier la méthode renvoie false
// Si le port a déjà reçu une directive de deconnexion STOP une exception est levée
// Si le port a déjà reçu une directive de deconnexion CONTINUE, 
// on donne la dernière valeur connu et on renvoie true.
template < typename DataManipulator > 
struct CalciumCouplingPolicy::DisconnectProcessor {

  const CalciumCouplingPolicy  & _couplingPolicy;
    
  DisconnectProcessor(const CalciumCouplingPolicy & couplingPolicy):
    _couplingPolicy(couplingPolicy) {};

  template < typename Container, typename DataId >
  bool apply(Container & storedDatas,
             const DataId & expectedDataId,
             typename Container::iterator & wDataIt1 ) const {

    typedef typename Container::key_type   key_type;
    typedef typename Container::value_type value_type;
    typedef typename Container::iterator   iterator;

    // Pas de traitement particulier a effectuer
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::DisconnectProcessor MARK1 ("<< _couplingPolicy._disconnectDirective<<") --------" << std::endl;
#endif
    if ( (_couplingPolicy._disconnectDirective) == (CalciumTypes::UNDEFINED_DIRECTIVE) ) return false;
  
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::DisconnectProcessor MARK2 --------" << std::endl;
#endif

    // TODO : Ds GenericPort::next il faut convertir en CPSTOPSEQ
    if ( _couplingPolicy._disconnectDirective == CalciumTypes::CP_ARRET )
      throw(CalciumException(CalciumTypes::CPINARRET,LOC(OSS()<< "CP_ARRET directive" 
                                           << " interrupts all further data reading")));
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::DisconnectProcessor MARK3 --------" << std::endl;
#endif


    // S'il n'y a plus de données indique que l'on a pas pu effectuer de traitement
    // TODO : Dans la gestion des niveaux il faut peut être interdire un niveau ==  0
    if ( storedDatas.empty() ) 
      throw(CalciumException(CalciumTypes::CPNTNULL,LOC(OSS()<< "CP_CONT directive" 
                                          << " is active but no data is available.")));
    
    // expectedDataId n'a ni été trouvé dans storedDataIds ni encadré mais il se peut
    // qu'en mode itératif il ne soit pas plus grand que le plus grand DataId stocké auquel
    // cas on doit renvoyer une expection car on n'est plus connecté et on ne pourra jamais
    // fournir de données pour ce dataId.
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::DisconnectProcessor MARK4  " << expectedDataId <<" --------" << std::endl;
#endif

    // >= expectedDataId
    iterator it1 = storedDatas.lower_bound(expectedDataId);
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::DisconnectProcessor MARK5  " << std::endl;
    for (iterator it=storedDatas.begin();it!=storedDatas.end();++it)
      std::cout <<" "<<(*it).first ;
    std::cout <<std::endl;
#endif

    // TODO : Il faut en fait renvoyer le plus proche cf IT ou DT
    if (it1 == storedDatas.end())
      throw(CalciumException(CalciumTypes::CPNTNULL,LOC(OSS()<< "CP_CONT directive" 
                                          << " is active but the requested dataId is less or equal to the last one received.")));
  
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::DisconnectProcessor MARK6 " << std::endl;
#endif

    wDataIt1 = storedDatas.end();
    --wDataIt1;
#ifdef MYDEBUG
    std::cout << "-------- CalciumCouplingPolicy::DisconnectProcessor, CP_CONT : " << (*wDataIt1).first << std::endl;
#endif

    return true;
  }
};

#endif
