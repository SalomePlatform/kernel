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

#include "SALOME_KernelServices.hxx"

void RegisterCompoInternal(const std::string& compoName, const std::string& compoIOR)
{
    CORBA::ORB_ptr orb = KERNEL::getORB();
    CORBA::Object_var obj = orb->string_to_object(compoIOR.c_str());
    KERNEL::RegisterCompo(compoName,obj);
}

std::string RetrieveCompoInternal(const std::string& compoName)
{
    CORBA::Object_var obj = KERNEL::RetrieveCompo(compoName);
    CORBA::ORB_ptr orb = KERNEL::getORB();
    CORBA::String_var ior = orb->object_to_string(obj);
    return std::string(ior.in());
}

/*!
 * This method generates a SIGSEGV to test robustness of higher level layers.
 */
void GenerateViolentMemoryFaultForTestPurpose()
{
    double *a = nullptr;
    *a = 0;
}
