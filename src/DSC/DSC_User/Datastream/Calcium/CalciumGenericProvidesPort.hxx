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

//  File   : CalciumGenericProvidesPort.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Id          : $Id$
//
#ifndef _CALCIUM_GENERIC_PROVIDES_PORT_HXX_
#define _CALCIUM_GENERIC_PROVIDES_PORT_HXX_

#include <omnithread.h>

#include "PortProperties_i.hxx"

#include "calcium_provides_port.hxx"

#include "GenericProvidesPort.hxx"
#include "CalciumCouplingPolicy.hxx"

#include "CorbaTypes2CalciumTypes.hxx"
#include "CalciumTypes2CorbaTypes.hxx"

#include "DSC_Exception.hxx"
#include <iostream>

//
// Cette macro CPP remplace une déclaration : template <typename CorbaInterface, typename CorbaDataManipulator > 
// mais permet surtout de générer différentes classes (dont le nom est spécifié par specificPortName ) 
//
// Variante: Creer un trait qui à partir de CorbaInterface déduit CorbaDataManipulator
// et simplifier la classe
//
// L'utilisation conjointe du paramètre ... et de __VA_ARGS__ au lieu d'un 'CorbaDataManipulator' permet
// d'invoquer la macro avec un troisième token contenant une virgule 
// (qui est considéré comme un séparateur d'argument par le PP ) 

#define CALCIUM_GENERIC_PROVIDES_PORT_HXX(specificPortName,CorbaInterface,...) \
  class specificPortName :   public virtual CorbaInterface ,            \
                             public virtual POA_Ports::PortProperties,  \
                             public GenericProvidesPort< __VA_ARGS__ , CalciumCouplingPolicy, calcium_provides_port > { \
  private :                                                             \
    omni_mutex     _disconnect_mutex; \
    int            _mustnotdisconnect; \
  public :                                                              \
    typedef  __VA_ARGS__               DataManipulator;                 \
    typedef  DataManipulator::Type     CorbaDataType;                   \
    typedef GenericPort< DataManipulator ,                              \
      CalciumCouplingPolicy >          Port;                            \
      specificPortName () : _mustnotdisconnect(0) {}; \
                                                                        \
    virtual ~ specificPortName ();                                      \
                                                                        \
    inline void disconnect(bool provideLastGivenValue) {                \
      _disconnect_mutex.lock();                       \
      if(_mustnotdisconnect > 1)                      \
      {                                               \
        _mustnotdisconnect--;                         \
      }                                               \
      else if(_mustnotdisconnect == 1)                \
      {                                               \
        _mustnotdisconnect--;                         \
        Port::disconnect(provideLastGivenValue);      \
      }                                               \
      _disconnect_mutex.unlock();                     \
    }                                                 \
    inline void setDependencyType(CalciumTypes::DependencyType dependencyType) { \
      Port::setDependencyType(dependencyType);                          \
    }                                                                   \
    inline CalciumTypes::DependencyType getDependencyType () const {    \
      return Port::getDependencyType();                                 \
    }                                                                   \
    inline void   setStorageLevel (size_t storageLevel) {               \
      Port::setStorageLevel(storageLevel);                              \
    }                                                                   \
    inline size_t getStorageLevel () const {                            \
      return Port::getStorageLevel();                                   \
    }                                                                   \
    inline void   setDateCalSchem (CalciumTypes::DateCalSchem   dateCalSchem) { \
      Port::setDateCalSchem (dateCalSchem);                             \
    }                                                                   \
    inline CalciumTypes::DateCalSchem getDateCalSchem () const {        \
      return Port::getDateCalSchem ();                                  \
    }                                                                   \
    inline void setAlpha(double alpha) {                                \
      Port::setAlpha(alpha);                                            \
    }                                                                   \
    inline double getAlpha() const  {                                   \
      return Port::getAlpha();                                          \
    }                                                                   \
    inline void   setDeltaT(double deltaT ) {                           \
      Port::setDeltaT(deltaT);                                          \
    }                                                                   \
    inline double getDeltaT() const  {                                  \
      return Port::getDeltaT();                                         \
    }                                                                   \
    inline void setInterpolationSchem (CalciumTypes::InterpolationSchem interpolationSchem) { \
      Port::setInterpolationSchem(interpolationSchem);                  \
    }                                                                   \
    inline void setExtrapolationSchem (CalciumTypes::ExtrapolationSchem extrapolationSchem) { \
      Port::setExtrapolationSchem(extrapolationSchem);                  \
    }                                                                   \
    inline CalciumTypes::InterpolationSchem getInterpolationSchem() const  { \
      return Port::getInterpolationSchem();                             \
    }                                                                   \
    inline CalciumTypes::ExtrapolationSchem getExtrapolationSchem() const  { \
      return Port::getExtrapolationSchem();                             \
    }                                                                   \
                                                                        \
    inline void put( DataManipulator::CorbaInType data,                 \
                     CORBA::Double time, CORBA::Long tag) {             \
      Port::put(data, time, tag);                                       \
    }                                                                   \
                                                                        \
    inline Ports::Port_ptr get_port_ref() {                             \
      return _this();                                                   \
    }                                                                   \
                                                                        \
    Ports::PortProperties_ptr get_port_properties() {                   \
      return POA_Ports::PortProperties::_this();                        \
    }                                                                   \
                                                                        \
    virtual void set_property(const char * name, const CORBA::Any& value) \
      throw (Ports::NotDefined, Ports::BadType, Ports::BadValue);       \
                                                                        \
    virtual CORBA::Any* get_property(const char* name)                  \
      throw (Ports::NotDefined);                                        \
                                                                        \
    virtual void provides_port_changed(int connection_nbr,              \
                                       const Engines::DSC::Message message) { \
      if ( message == Engines::DSC::AddingConnection)                   \
        {                                                               \
          _disconnect_mutex.lock();                                     \
          _mustnotdisconnect++;                                         \
          _disconnect_mutex.unlock();                                   \
        }                                                               \
      else if ( message == Engines::DSC::RemovingConnection )           \
        {                                                               \
          disconnect(false);                                            \
        }                                                               \
    }                                                                   \
                                                                        \
    inline void calcium_erase(float t,long i, bool before)              \
    {                                                                   \
      erase(t,i,before);                                                \
    }                                                                   \
  };                                                                    \



#define CALCIUM_GENERIC_PROVIDES_PORT_CXX(specificPortName)             \
                                                                        \
  specificPortName::~specificPortName(void) {};                         \
                                                                        \
  void specificPortName::set_property(const char * name,  const CORBA::Any& value) \
    throw (Ports::NotDefined, Ports::BadType, Ports::BadValue) {        \
                                                                        \
    const std::string key(name);                                        \
    CORBA::Long sl;                                                     \
    CORBA::Double alpha,delta;                                          \
    Ports::Calcium_Ports::DependencyType dt;                            \
    Ports::Calcium_Ports::DateCalSchem  dcs;                            \
    Ports::Calcium_Ports::InterpolationSchem is;                        \
    Ports::Calcium_Ports::ExtrapolationSchem es;                        \
    bool ok=false;                                                      \
                                                                        \
    try {                                                               \
                                                                        \
      if (key == "StorageLevel" )                                       \
        {if ( ( ok=(value >>= sl) )   ) Port::setStorageLevel(sl);}     \
      else if (key == "Alpha"      )                                    \
        {if ( ( ok=(value >>= alpha) ) ) Port::setAlpha(alpha);}                \
      else if (key == "DeltaT"     )                                    \
        {if ( ( ok=(value >>= delta) ) ) Port::setDeltaT(delta);}               \
      else if (key == "DependencyType" )                                \
        {if ( ( ok=( value >>= dt)  )  ) Port::setDependencyType(dependencyType[dt]);} \
      else if (key == "DateCalSchem" )                                  \
        {if ( ( ok=(value >>= dcs) ) )                                  \
            Port::setDateCalSchem(dateCalSchem[dcs]);}                  \
      else if (key == "InterpolationSchem")                             \
        {if ( ( ok=(value >>= is) ) )                                   \
            Port::setInterpolationSchem(interpolationSchem[is]);}       \
      else if (key == "ExtrapolationSchem")                             \
        {if ( ( ok=(value >>= es) ) )                                   \
            Port::setExtrapolationSchem(extrapolationSchem[es]);}       \
      else                                                              \
        throw Ports::NotDefined();                                      \
      if (!ok) throw Ports::BadType();                                  \
                                                                        \
    } catch ( const DSC_Exception & ex ) {                              \
                                                                        \
      std::cerr << ex.what() << std::endl;                              \
      throw Ports::BadValue();                                          \
    }                                                                   \
  }                                                                     \
                                                                        \
                                                                        \
  CORBA::Any* specificPortName::get_property(const char* name)          \
    throw (Ports::NotDefined) {                                         \
    const std::string key(name);                                        \
    CORBA::Any* value=new CORBA::Any;                                   \
    if (key == "StorageLevel"    )                                      \
      { (*value) <<= static_cast<CORBA::Long>(Port::getStorageLevel()); } \
    else if (key == "Alpha"      )                                      \
      { *value <<= (CORBA::Double) Port::getAlpha();}                   \
    else if (key == "DeltaT"     )                                      \
      { *value <<= (CORBA::Double) Port::getDeltaT();}                  \
    else if (key == "DependencyType" )                                  \
      {  *value <<= corbaDependencyType[Port::getDependencyType()];}    \
    else if (key == "DateCalSchem" )                                    \
      { *value <<= corbaDateCalSchem[Port::getDateCalSchem()];}         \
    else if (key == "InterpolationSchem")                               \
      { *value <<= corbaInterpolationSchem[Port::getInterpolationSchem()];} \
    else if (key == "ExtrapolationSchem")                               \
      { *value <<= corbaExtrapolationSchem[Port::getExtrapolationSchem()];} \
    else {                                                              \
      delete value;                                                     \
      throw Ports::NotDefined();                                        \
    }                                                                   \
    return value;                                                       \
  };                                            
                                                                        

#endif
