// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
#ifndef _RECEIVERFACTORY_HXX_
#define _RECEIVERFACTORY_HXX_

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Comm)
#include "MultiCommException.hxx"

/*!
  This class internally builds a receiver associated with the sender given. It also performs transfert completely and clean up the objects.
  This is the only class used client side of an array.
 */
class ReceiverFactory
{
public:
  static double *getValue(SALOME::SenderDouble_ptr sender,long &size)throw(MultiCommException);
  static int *getValue(SALOME::SenderInt_ptr sender,long &size)throw(MultiCommException);
private:
  static double *getValueOneShot(SALOME::SenderDouble_ptr sender,long &size)throw(MultiCommException);
  static int *getValueOneShot(SALOME::SenderInt_ptr sender,long &size)throw(MultiCommException);
};

#endif

