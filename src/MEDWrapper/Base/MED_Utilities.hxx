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

#ifndef MED_Utilities_HeaderFile
#define MED_Utilities_HeaderFile

#include <iostream>	
#include <sstream>	
#include <string>
#include <stdexcept>


namespace MED{
  using namespace std;
  class PrefixPrinter{
    static int myCounter;
    bool myIsActive;
  public:
    PrefixPrinter(bool theIsActive = true);
    ~PrefixPrinter();

    static string GetPrefix();
  };
};

//#define _DEBUG_
#ifdef _DEBUG_
  #define MSG(deb,msg) if(deb) std::cout<<MED::PrefixPrinter::GetPrefix()<<msg<<" ("<<__FILE__<<" ["<<__LINE__<<"])\n"
  #define BEGMSG(deb,msg) if(deb) std::cout<<MED::PrefixPrinter::GetPrefix()<<msg
  #define INITMSG(deb,msg) MED::PrefixPrinter aPrefixPrinter(deb); BEGMSG(deb,msg)
  #define ADDMSG(deb,msg) if(deb) std::cout<<msg
#else
  #define MSG(deb,msg)
  #define BEGMSG(deb,msg)
  #define INITMSG(deb,msg)
  #define ADDMSG(deb,msg)
#endif


#ifndef EXCEPTION

#define EXCEPTION(TYPE, MSG) {\
  std::ostringstream aStream;\
  aStream<<__FILE__<<"["<<__LINE__<<"]::"<<MSG;\
  throw TYPE(aStream.str());\
}

#endif

#endif
