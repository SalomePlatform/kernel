// Eric Fayolle - EDF R&D
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-28 15:26:32 +0100 (Wed, 28 Feb 2007) $
// Id          : $Id$

#ifndef _GENERIC_PROVIDES_PORT_HXX_
#define _GENERIC_PROVIDES_PORT_HXX_

#include "provides_port.hxx"
#include "GenericPort.hxx"


template <typename DataManipulator, typename CouplingPolicy, typename ProvidesPort=provides_port> 
class GenericProvidesPort : public ProvidesPort,
			    public GenericPort< DataManipulator , CouplingPolicy > {
public :
  // Type de données manipulés 
  typedef typename DataManipulator::Type         DataType;
  typedef typename DataManipulator::CorbaInType  CorbaInDataType;

  virtual ~GenericProvidesPort() {};

};


#endif
