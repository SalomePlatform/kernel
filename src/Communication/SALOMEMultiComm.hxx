// Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
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

#ifndef _SALOMEMULTICOMM_HXX_
#define _SALOMEMULTICOMM_HXX_

#include "SALOME_Communication.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Comm)

/*!
  Class is designed to ease the use of multi communication.\n
  Simply inherite from it your servant class you want to emit data with senders.
 */
class COMMUNICATION_EXPORT SALOMEMultiComm : public virtual POA_SALOME_CMOD::MultiCommClass {
protected:
  SALOME_CMOD::TypeOfCommunication _type;
public:
  SALOMEMultiComm();
  SALOMEMultiComm(SALOME_CMOD::TypeOfCommunication type);
  virtual void setProtocol(SALOME_CMOD::TypeOfCommunication type);
  SALOME_CMOD::TypeOfCommunication getProtocol() const;
};

#endif
