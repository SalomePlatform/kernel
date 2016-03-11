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

//  File   : PalmCouplingPolicy.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-01-29 16:12:41 +0100 (lun, 29 jan 2007) $
// Id          : $Id$
//
#ifndef _PALM_COUPLING_POLICIES_HXX_
#define _PALM_COUPLING_POLICIES_HXX_

#include <SALOMEconfig.h>

#include <omniORB4/CORBA.h>
#include <vector>
#include "DataIdFilter.hxx"
#include "DisplayPair.hxx"
#include "CouplingPolicy.hxx"

class PalmCouplingPolicy: public CouplingPolicy {

  // Objet de filtrage et conversion d'un TIME
  filtre_conversion filtre_convert_TIME ;
  // Objet de filtrage et conversion d'un TAG
  filtre_conversion filtre_convert_TAG ;

public:

  template <typename T_TIME, typename T_TAG >        class InternalDataIdContainer;
  template <typename T_TIME, typename T_TAG > friend class InternalDataIdContainer;

  filtre_conversion * get_filtre_convert_TIME() { return &filtre_convert_TIME; }
  filtre_conversion * get_filtre_convert_TAG()  { return &filtre_convert_TAG;  }
 
  // Classe DataId rassemblant les paramètres de la méthode PORT::put 
  // qui identifient l'instance d'une donnée pour Palm
  // Rem : Le DataId doit pouvoir être une key dans une map stl
  typedef CORBA::Long TimeType;
  typedef CORBA::Long TagType;
  typedef std::pair               < TimeType , TagType > DataId;
  typedef InternalDataIdContainer < TimeType , TagType > DataIdContainer;

  TimeType getTime(const DataId &dataId) const { return dataId.first;}
  TagType  getTag (const DataId &dataId) const { return dataId.second;}

  // Container qui permet d'itérer sur l'ensemble des DataId générés par les
  // eventuelles conversions
  template <typename T_TIME, typename T_TAG > class InternalDataIdContainer {

    std::vector<T_TIME>  _lTime;
    std::vector<T_TAG>   _lTag;
    
    // Interdit la construction d'un container vide car
    // aucune méthode d'ajout d'élément n'a été implémentée
    InternalDataIdContainer() {}
 
  public:
    typedef DataId                                   value_type;
    template <typename TTIME, typename TTAG>   class DataIdIterator;
    typedef  DataIdIterator<T_TIME, T_TAG>           iterator;                  
    typedef value_type &                             reference;
    typedef value_type const &                       const_reference;
 
    // Remplit le container d'identificateurs après avoir appliqué
    // les filtres/convertions  des paramètres "TIMEs" et des "TAGs"
    InternalDataIdContainer(const DataId & dataId, const PalmCouplingPolicy & policy ) {
      std::cout << "----Cst ----  InternalDataIdContainer(const DataId & dataId..) " << dataId <<std::endl;
      policy.filtre_convert_TIME.applique_filtre_conversion(dataId.first, _lTime);
      int c=0;
      for(std::vector<CORBA::Long>::iterator i=_lTime.begin();i!=_lTime.end();++i)
        std::cout << "_lTime["<< c++ << "] : " << *i << std::endl;
      policy.filtre_convert_TAG.applique_filtre_conversion(dataId.second, _lTag);
      c=0;
      for(std::vector<CORBA::Long>::iterator i=_lTag.begin();i!=_lTag.end();++i)
        std::cout << "_lTag["<< c++ << "] : " << *i << std::endl;
    }

    // Pas encore testé
    InternalDataIdContainer(std::vector<T_TIME> const & lTime, std::vector<T_TAG> const & lTag):
      _lTime(lTime),_lTag(lTag) {}
  

    // Pas encore testé
    InternalDataIdContainer( InternalDataIdContainer const & pc):
      _lTime(pc._lTime), _lTag(pc._lTag) {}


    // Pas encore testé
    InternalDataIdContainer & operator=(const InternalDataIdContainer & pc) {
      if (this != &pc) { _lTime=pc._lTime; _lTag=pc._lTag; }
      return *this;
    }
    
    virtual ~InternalDataIdContainer() {}

    iterator begin() { return iterator(*this);  }
    iterator end()   { return iterator(*this,_lTime.end(),_lTag.end()); }
    // En Palm les deux paramètres du couple (temps,tag) doivent
    // exister (un seul paramètre n'est pas recevable) 
    bool     empty() const { return _lTime.empty() || _lTag.empty(); }

    // Définition de l'itérateur du container
    template <typename TTIME, typename TTAG> class DataIdIterator {

    public:

      typedef typename std::vector<TTIME>::const_iterator ItTime;
      typedef typename std::vector<TTAG >::const_iterator ItTag;

    private:
      ItTime _itTime;
      ItTag  _itTag;
      const InternalDataIdContainer<TTIME,TTAG> & _pc;
      DataIdIterator() {}

    public:
 
 
      // Pas encore testé
      DataIdIterator(const InternalDataIdContainer<TTIME,TTAG> & pc):
        _pc(pc),_itTime(pc._lTime.begin()),_itTag(pc._lTag.begin()) {}
      DataIdIterator(const InternalDataIdContainer<TTIME,TTAG> & pc, ItTime itTime, ItTag itTag):
        _pc(pc),_itTime(itTime),_itTag(itTag) {}
      // Pas encore testé
      DataIdIterator(const DataIdIterator & dIt):
        _pc(dIt._pc),_itTime(dIt._itTime),_itTag(dIt._itTag) {}
      ~DataIdIterator() {}
      

      // Pas encore testé
      DataIdIterator & operator=(const DataIdIterator & dIt) {
        if (this != &dIt) {
          _pc=dIt._pc;_itTime=dIt._itTime;_itTag=dIt._itTag;
        }
        return *this;
      }

      DataId operator*() const {
        std::cout << "-------- operator*(),  *_itTime : " << *_itTime << " *_itTag " <<  *_itTag <<std::endl;
        return DataId(*_itTime,*_itTag); }
   
      bool operator==( const DataIdIterator  & dIt) const {
        return (_itTime == dIt._itTime) && (_itTag == dIt._itTag) && (&_pc == &dIt._pc);
      }

      bool operator!=( const DataIdIterator  & dIt) const { 
        return (_itTime != dIt._itTime) || (_itTag != dIt._itTag) || (&_pc != &dIt._pc);
      }

      // Notation préfixé
      DataIdIterator & operator++() {
        if ( _itTag != _pc._lTag.end() ) { 
          ++_itTag;
          if (_itTag != _pc._lTag.end() ) return *this;
        }
        if ( _itTime != _pc._lTime.end() )  { 
          ++_itTime;
          if (_itTime != _pc._lTime.end() ) _itTag=_pc._lTag.begin();
        }
        return *this;
      }

      // Notation postfixé
      DataIdIterator operator++(int fake) { 
        DataIdIterator tmpIt=*this; ++*this; return tmpIt; 
      }
      
    }; //Fin de la classe de l'itérateur
  };   //Fin de la classe du container



};

#endif
