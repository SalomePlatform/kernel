//  Copyright (C) 2007-2011  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  SALOME Utils : general SALOME's definitions and tools
//  File   : Basics_DirUtils.hxx
//  Autor  : Alexander A. BORODIN
//  Module : SALOME
//
#ifndef _Basics_UTILS_HXX_
#define _Basics_UTILS_HXX_

#include "SALOME_Basics.hxx"

#include <string>

namespace Kernel_Utils
{
  BASICS_EXPORT std::string GetHostname();

  class BASICS_EXPORT Localizer
  {
  public:
    Localizer();
    ~Localizer();
  private:
    std::string myCurLocale;
  };
  
  //! GUID type
  enum GUIDtype {
    DefUserID = 1,  //!< Default user attribute ID
    ObjectdID       //!< Global usage object identifier ID
  };

  //! Get predefined GUID
  BASICS_EXPORT std::string GetGUID( GUIDtype );
}

#endif //_Basics_UTILS_HXX_
