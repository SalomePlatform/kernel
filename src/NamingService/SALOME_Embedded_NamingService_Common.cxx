// Copyright (C) 2021-2023  CEA, EDF
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

#include "SALOME_Embedded_NamingService_Common.hxx"
#include "SALOME_KernelORB.hxx"

#include <memory>
#include <cstring>

CORBA::Object_var IORToObject(const Engines::IORType& ObjRef)
{
  CORBA::ORB_ptr orb(KERNEL::getORB());
  CORBA::ULong size(ObjRef.length());
  std::unique_ptr<char[]> pt(new char[size+1]);
  pt[size] = '\0';
  for(CORBA::ULong i = 0 ; i < size ; ++i)
    pt[i] = ObjRef[i];
  CORBA::Object_var obj = orb->string_to_object(pt.get());
  return obj;
}

Engines::IORType *ObjectToIOR(CORBA::Object_ptr obj)
{
  std::unique_ptr<Engines::IORType> ret(new Engines::IORType);
  CORBA::ORB_ptr orb(KERNEL::getORB());
  CORBA::String_var ior = orb->object_to_string(obj);
  auto len( strlen(ior) );
  ret->length( len );
  for(std::size_t i = 0 ; i < len ; ++i)
    (*ret)[i] = ior[i];
  return ret.release();
}
