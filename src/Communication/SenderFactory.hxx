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

#ifndef _SENDERFACTORY_HXX_
#define _SENDERFACTORY_HXX_

#include "SALOME_Communication.hxx"

#include "MultiCommException.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Comm)

class SALOMEMultiComm;

class SALOME_SenderDouble_i;
class SALOME_SenderInt_i;

/*!
  This class implements the factory pattern of GoF by making a sender by giving an array and a communicator.It completely hides the type of sender from the user.
 */
class COMMUNICATION_EXPORT SenderFactory
{
public:
  static SALOME::SenderDouble_ptr buildSender(SALOMEMultiComm &multiCommunicator,const double *tab,long lgr,bool ownTab=false) throw(MultiCommException);
  static SALOME::SenderInt_ptr buildSender(SALOMEMultiComm &multiCommunicator,const int *tab,long lgr,bool ownTab=false) throw(MultiCommException);
  static SALOME::SenderDouble_ptr buildSender(SALOME::TypeOfCommunication NewType,SALOME_SenderDouble_i *src);
  static SALOME::SenderInt_ptr buildSender(SALOME::TypeOfCommunication NewType,SALOME_SenderInt_i *src);
};

template<class T>
struct mapCppSender {
  typedef T SenderVarType;
};

template<>
struct mapCppSender<int>
{
  typedef SALOME::SenderInt_var SenderVarType;
};

template<>
struct mapCppSender<double>
{
  typedef SALOME::SenderDouble_var SenderVarType;
};

#endif

