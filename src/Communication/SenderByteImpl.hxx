// Copyright (C) 2023-2025  CEA, EDF
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

#pragma once

#include "SALOME_Communication.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Comm)
#include "SALOME_GenericObj_i.hh"

#include <cstddef>

class COMMUNICATION_EXPORT SenderByteImpl : public virtual POA_SALOME::SenderByte,
                                            public virtual PortableServer::ServantBase,
                                            public virtual SALOME::GenericObj_i
{
public:
  //! SenderByteImpl instance does not have ownership of data
  SenderByteImpl(char *data, std::size_t size):_data(data),_size(size) { }
  CORBA::ULongLong getSize() override;
  SALOME::vectorOfByte *sendPart(CORBA::ULongLong n1, CORBA::ULongLong n2) override;
private:
  char *_data = nullptr;
  std::size_t _size = 0;
};
