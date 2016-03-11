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

//  File   : GenericUsesPort.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-28 15:26:32 +0100 (mer, 28 fÃ©v 2007) $
// Id          : $Id$
//
#ifndef _GENERIC_USES_PORT_HXX_
#define _GENERIC_USES_PORT_HXX_

#include "CorbaTypeManipulator.hxx"

#include "uses_port.hxx"
#include "SALOME_Ports.hh"

#include "DSC_Exception.hxx"

// #define GENERATE_USES_PORT(dataManip,portType,portName)                      \
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
GenericUsesPort< DataManipulator,CorbaPortType, repositoryName, UsesPort  >::~GenericUsesPort() 
{
  delete _my_ports;
}

template <typename DataManipulator,typename CorbaPortType, char * repositoryName, typename UsesPort > 
const char *
GenericUsesPort< DataManipulator,CorbaPortType, repositoryName, UsesPort  >::get_repository_id() {
  return repositoryName;
}


template <typename DataManipulator,typename CorbaPortType, char * repositoryName, typename UsesPort > 
template <typename TimeType,typename TagType>
void
GenericUsesPort< DataManipulator,CorbaPortType, repositoryName, UsesPort  >::put( CorbaInDataType data, 
                                                                                  TimeType time, 
                                                                                  TagType tag) {
  typedef typename CorbaPortType::_var_type CorbaPortTypeVar;
  if (!_my_ports)
    throw DSC_Exception(LOC("There is no connected provides port to communicate with."));

  // OLD : PB1 : Cf remarque dans CalciumInterface, si on n'effectue pas de copie
  // OLD :       du buffer ! 
  // OLD : PB2 : Si les ports provides auquels on envoie data sont collocalisés
  // OLD : ils vont partagés le même buffer (à cause de notre optim ds get_data)
  // OLD : il faut alors effectuer une copie ici.
  // OLD : Pour l'instant on résoud PB2 en créant une copie de la donnée en cas
  // OLD : de connexions multiples. Il faudra tester la collocalisation.
  // OLD :  DataType copyOfData; // = data; PB1
  for(int i = 0; i < _my_ports->length(); i++) {

    CorbaPortTypeVar port = CorbaPortType::_narrow((*_my_ports)[i]);
    //if (i) { PB1
    //OLD :   copyOfData = DataManipulator::clone(data);
#ifdef MYDEBUG
    std::cerr << "-------- GenericUsesPort::put -------- " << std::endl;
#endif
    //} PB1
    try {
      port->put(data,time,tag);
      // OLD : port->put(*copyOfData,time,tag);
    } catch(const CORBA::SystemException& ex) {
      //OLD : DataManipulator::delete_data(copyOfData);
      throw DSC_Exception(LOC(OSS() << "Can't invoke put method on port number "
                              << i << "( i>=  0)"));

    }
    //if (i) PB1 
    // La séquence est détruite avec le buffer si on n'est pas collocalisé
    // La séquence est détruite sans son buffer sinon (cf comportement de get_data
    // appelée dans put (port provides)
    //OLD : DataManipulator::delete_data(copyOfData);
  }
}


template <typename DataManipulator, typename CorbaPortType, char * repositoryName, typename UsesPort>
void 
GenericUsesPort< DataManipulator, CorbaPortType, repositoryName, UsesPort  
                 >::uses_port_changed(Engines::DSC::uses_port * new_uses_port,
                                      const Engines::DSC::Message message)
{
  if (_my_ports) delete _my_ports;

#ifdef MYDEBUG
  std::cerr << "GenericUsesPort::uses_port_changed" << std::endl;
#endif
  _my_ports = new_uses_port;
}

#endif
