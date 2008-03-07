// Eric Fayolle - EDF R&D
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-03-01 13:40:26 +0100 (Thu, 01 Mar 2007) $
// Id          : $Id$

#ifndef _CALCIUM_PORT_PROVIDES_HXX_
#define _CALCIUM_PORT_PROVIDES_HXX_

#include <SALOMEconfig.h>

#include "Calcium_Ports.hh"
#include "CalciumGenericProvidesPort.hxx"
#include "CalciumCouplingPolicy.hxx"


CALCIUM_GENERIC_PROVIDES_PORT_HXX(calcium_integer_port_provides,		\
			      POA_Ports::Calcium_Ports::Calcium_Integer_Port, \
			      seq_u_manipulation<Ports::Calcium_Ports::seq_long,CORBA::Long> ) \

CALCIUM_GENERIC_PROVIDES_PORT_HXX(calcium_real_port_provides,		\
			      POA_Ports::Calcium_Ports::Calcium_Real_Port, \
			      seq_u_manipulation<Ports::Calcium_Ports::seq_float,CORBA::Float> ) \

CALCIUM_GENERIC_PROVIDES_PORT_HXX(calcium_double_port_provides,		\
			      POA_Ports::Calcium_Ports::Calcium_Double_Port, \
			      seq_u_manipulation<Ports::Calcium_Ports::seq_double,CORBA::Double> ) \

CALCIUM_GENERIC_PROVIDES_PORT_HXX(calcium_complex_port_provides,		\
			      POA_Ports::Calcium_Ports::Calcium_Complex_Port, \
			      seq_u_manipulation<Ports::Calcium_Ports::seq_float,CORBA::Float> ) \

CALCIUM_GENERIC_PROVIDES_PORT_HXX(calcium_logical_port_provides,		\
			      POA_Ports::Calcium_Ports::Calcium_Logical_Port, \
			      seq_u_manipulation<Ports::Calcium_Ports::seq_boolean,CORBA::Boolean> ) \

CALCIUM_GENERIC_PROVIDES_PORT_HXX(calcium_string_port_provides,		\
			      POA_Ports::Calcium_Ports::Calcium_String_Port, \
			      seq_u_manipulation<Ports::Calcium_Ports::seq_string,char *> ) \

#endif
