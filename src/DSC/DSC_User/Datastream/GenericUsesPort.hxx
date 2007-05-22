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
//  File   : GenericUsesPort.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-28 15:26:32 +0100 (mer, 28 fÃ©v 2007) $
// Id          : $Id$

#ifndef _GENERIC_USES_PORT_HXX_
#define _GENERIC_USES_PORT_HXX_

#include "CorbaTypeManipulator.hxx"

#include "uses_port.hxx"
#include "SALOME_Ports.hh"

#include "DSC_Exception.hxx"

// #define GENERATE_USES_PORT(dataManip,portType,portName)			\
//   const char * _repository_##portType##_name_ = "IDL:Ports/##portType##:1.0"; \
//   GenericUsesPort< dataManip, portType, _repository_##portType##_name_ > portName;

//ex : GENERATE_USES_PORT(Ports::Data_Short_Port,data_short_port);

template <typename DataManipulator, typename CorbaPortType, char * repositoryName, 
	  typename UsesPort=uses_port > 
class GenericUsesPort : public UsesPort
{
public :
  // Type de données manipulés 
  typedef typename DataManipulator::Type         DataType;
  typedef typename DataManipulator::CorbaInType  CorbaInDataType;

  GenericUsesPort();
  virtual ~GenericUsesPort();

  virtual const char * get_repository_id();
  virtual bool  set_port(Ports::Port_ptr port);
  template <typename TimeType,typename TagType>
  void  put(CorbaInDataType data,  TimeType time, TagType tag); 

  virtual void uses_port_changed(Engines::DSC::uses_port * new_uses_port,
				 const Engines::DSC::Message message);

protected :
  Engines::DSC::uses_port * _my_ports;
};


template <typename DataManipulator,typename CorbaPortType, char * repositoryName, typename UsesPort > 
GenericUsesPort< DataManipulator,CorbaPortType, repositoryName, UsesPort  >::GenericUsesPort() {
  _my_ports = NULL;
}

template <typename DataManipulator,typename CorbaPortType, char * repositoryName, typename UsesPort > 
GenericUsesPort< DataManipulator,CorbaPortType, repositoryName, UsesPort  >::~GenericUsesPort() {}

template <typename DataManipulator,typename CorbaPortType, char * repositoryName, typename UsesPort > 
const char *
GenericUsesPort< DataManipulator,CorbaPortType, repositoryName, UsesPort  >::get_repository_id() {
  return repositoryName;
}


template <typename DataManipulator,typename CorbaPortType, char * repositoryName, typename UsesPort > 
bool
GenericUsesPort< DataManipulator,CorbaPortType, repositoryName, UsesPort  >::set_port(Ports::Port_ptr port) {
  if (_my_ports) {
    size_t n = _my_ports->length()+1;
    _my_ports->length(n);
    (*_my_ports)[n]=CorbaPortType::_narrow(port);
    return true;
  }  else {
    // Vérifier si port is_nil
    _my_ports = new Engines::DSC::uses_port();
    _my_ports->length(1);
    (*_my_ports)[0]=CorbaPortType::_narrow(port);
    return true;
  }
  return false;
}

template <typename DataManipulator,typename CorbaPortType, char * repositoryName, typename UsesPort > 
template <typename TimeType,typename TagType>
void
GenericUsesPort< DataManipulator,CorbaPortType, repositoryName, UsesPort  >::put( CorbaInDataType data, 
								       TimeType time, 
								       TagType tag) {
  typedef typename CorbaPortType::_ptr_type CorbaPortTypePtr;
  if (!_my_ports)
    throw DSC_Exception(LOC("There is no connected provides port to communicate with."));

  // PB1 : Cf remarque dans CalciumInterface, si on n'effectue pas de copie
  //       du buffer ! 
  // PB2 : Si les ports provides auquels on envoie data sont collocalisés
  // ils vont partagés le même buffer (à cause de notre optim ds get_data)
  // il faut alors effectuer une copie ici.
  // Pour l'instant on résoud PB2 en créant une copie de la donnée en cas
  // de connexions multiples. Il faudra tester la collocalisation.
  DataType copyOfData; // = data; PB1
  for(int i = 0; i < _my_ports->length(); i++) {

    CorbaPortTypePtr port = CorbaPortType::_narrow((*_my_ports)[i]);
    //if (i) { PB1
    copyOfData = DataManipulator::clone(data);
    std::cout << "-------- GenericUsesPort::put : Copie de data("
      //<< DataManipulator::getPointer(data)
	      <<") vers copyOfData ("
	      <<DataManipulator::getPointer(copyOfData)
	      <<")------------------" << std::endl;
    //} PB1
    try {
      port->put(*copyOfData,time,tag); // catcher les exceptions
    }   catch(const CORBA::SystemException& ex){
      DataManipulator::delete_data(copyOfData);
      throw DSC_Exception(LOC(OSS() << "Impossible d'invoquer la méthode put sur le port n°"
			      << i << "( i>=  0)"));

    }
    //if (i) PB1 
    // La séquence est détruite avec le buffer si on n'est pas collocalisé
    // La séquence est détruite sans son buffer sinon (cf comportement de get_data
    // appelée dans put (port provides)
    DataManipulator::delete_data(copyOfData);
    }
}


template <typename DataManipulator, typename CorbaPortType, char * repositoryName, typename UsesPort>
void 
GenericUsesPort< DataManipulator, CorbaPortType, repositoryName, UsesPort  
		 >::uses_port_changed(Engines::DSC::uses_port * new_uses_port,
				      const Engines::DSC::Message message)
{
  if (_my_ports) delete _my_ports;

  std::cerr << "GenericUsesPort::uses_port_changed" << endl;
  _my_ports = new Engines::DSC::uses_port(*new_uses_port);
}

#endif
