//  
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : 
//  Author : 
//  Module : 
//  $Header$

#include "MED_Factory.hxx"
#include "MED_Utilities.hxx"
#include "MED_V2_2_Wrapper.hxx"
#include "MED_V2_1_Wrapper.hxx"

#include <stdio.h>

extern "C"{
#include <med.h>
}

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif

namespace MED{
  
  EVersion GetVersionId(const std::string& theFileName)
  {
    EVersion aVersion = eVUnknown;
    char* aFileName = const_cast<char*>(theFileName.c_str());
    med_idt aFid = MEDouvrir(aFileName,MED_LECTURE);
    MSG(MYDEBUG,"GetVersionId - theFileName = '"<<theFileName<<"'; aFid = "<<aFid<<endl);
    if(aFid >= 0){
      med_int aMajor, aMinor, aRelease;
      med_err aRet = MEDversionLire(aFid,&aMajor,&aMinor,&aRelease);
      INITMSG(MYDEBUG,"GetVersionId - theFileName = '"<<theFileName<<"'; aRet = "<<aRet<<endl);
      if(aRet >= 0){
	if(aMajor >= 2 && aMinor >= 2)
	  aVersion = eV2_2;
	else
	  aVersion = eV2_1;
      }
    }
    INITMSG(MYDEBUG,"GetVersionId - theFileName = '"<<theFileName<<"'; aVersion = "<<aVersion<<endl);
    return aVersion;
  }

  PWrapper CrWrapper(const std::string& theFileName)
  {
    PWrapper aWrapper;
    EVersion aVersion = GetVersionId(theFileName);
    switch(aVersion){
    case eV2_2:
      aWrapper.reset(new MED::V2_2::TVWrapper(theFileName));
      break;
    case eV2_1:
    default:
      aWrapper.reset(new MED::V2_1::TVWrapper(theFileName));
    }
    return aWrapper;
  }

  PWrapper CrWrapper(const std::string& theFileName, EVersion theId)
  {
    EVersion aVersion = GetVersionId(theFileName);

    if(aVersion != theId)
      remove(theFileName.c_str());
    
    PWrapper aWrapper;
    switch(theId){
    case eV2_2:
      aWrapper.reset(new MED::V2_2::TVWrapper(theFileName));
      break;
    case eV2_1:
      aWrapper.reset(new MED::V2_1::TVWrapper(theFileName));
      break;
    }
    return aWrapper;
  }

}
