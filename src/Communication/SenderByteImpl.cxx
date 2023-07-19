// Copyright (C) 2023  CEA, EDF
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

#include "SenderByteImpl.hxx"

CORBA::ULongLong SenderByteImpl::getSize()
{
  return _size;
}

SALOME::vectorOfByte *SenderByteImpl::sendPart(CORBA::ULongLong n1, CORBA::ULongLong n2)
{
  SALOME::vectorOfByte *ret = new SALOME::vectorOfByte;
  CORBA::ULongLong retSize(n2-n1);
  ret->length(retSize);
  for(CORBA::ULongLong i = 0 ; i < retSize ; ++i)
    (*ret)[i] = _data[n1+i];
  return ret;
}
