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

//  SALOME ModuleCatalog : implementation of ModuleCatalog server which parsers xml description of modules
//  File   : SALOME_ModuleCatalog_Parser_IO.cxx
//  Author : Estelle Deville
//  Module : SALOME
//  $Header$
//
#include "SALOME_ModuleCatalog_Parser_IO.hxx"
#include <string>
#include <iostream>
#include "utilities.h"

std::ostream & operator<< (std::ostream & f, const ParserParameter & P)
{
  f << "          name :       " << P.name << std::endl;
  f << "          type :       " << P.type << std::endl;
  return f;
}

std::ostream & operator<< (std::ostream & f, 
                           const ParserDataStreamParameter & P)
{
  f << "          name :       " << P.name << std::endl;
  f << "          type :       " << P.type << std::endl;
  f << "          dependency : " << P.dependency << std::endl;
  return f;
}

std::ostream & operator<< (std::ostream & f, 
                           const ParserService & S)
{
  int i, n;
  f << "      name :       " << S.name << std::endl;
  f << "      default :    " << (S.byDefault ? "yes" : "no") 
    << std::endl;

  n = S.inParameters.size();
  f << "      in parameters : " << n << std::endl;
  for (i=0; i<n; i++)
    f << S.inParameters[i] << std::endl;
  if (n == 0) f << std::endl;

  n = S.inDataStreamParameters.size();
  f << "      in DataStream parameters : " << n << std::endl;
  for (i=0; i<n; i++)
    f << S.inDataStreamParameters[i] << std::endl;
  if (n == 0) f << std::endl;

  n = S.outParameters.size();
  f << "      out parameters : " << n << std::endl;
  for (i=0; i<n; i++)
    f << S.outParameters[i] << std::endl;
  if (n == 0) f << std::endl;

  n = S.outDataStreamParameters.size();
  f << "      out DataStream parameters : " << n << std::endl;
  for (i=0; i<n; i++)
    f << S.outDataStreamParameters[i] << std::endl;
  if (n == 0) f << std::endl;

  return f;
}

std::ostream & operator<< (std::ostream & f, 
                           const ParserInterface & I)
{
  int j, n;
  f << "    name :       " << I.name << std::endl;

  n = I.services.size();
  f << "    services : " << n << std::endl;
  for (j=0; j<n; j++) {
    MESSAGE(I.services[j].name);
    f << I.services[j] << std::endl;
  }

  return f;
}

std::ostream & operator<< (std::ostream & f, 
                           const  ParserComponentType & T)
{
  std::string s;
  switch (T) {
  case GEOM :   s = "GEOM"; break;
  case MESH :   s = "MESH"; break;
  case Med  :   s = "Med"; break;
  case SOLVER : s = "SOLVER"; break;
  case DATA :   s = "DATA"; break;
  case VISU :   s = "VISU"; break;
  case SUPERV : s = "SUPERV"; break;
  default :     s = "OTHER"; break;
  }
  f << s << std::endl;
  return f;
}

std::ostream & operator<< (std::ostream & f, 
                           const ParserComponent & C)
{
  int j, n;
  f << std::endl
    << "    name :       " << C.name << std::endl;
  f << "    user name :  " << C.username << std::endl;
  f << "    type :       " << C.type << std::endl;
  f << "    multistudy : " << (C.multistudy ? "yes" : "no")
    << std::endl;
  f << "    icon :       " << C.icon << std::endl;
  f << "    constraint : " << C.constraint << std::endl;

  n = C.interfaces.size();
  f << "    interfaces : " << n << std::endl;
  for (j=0; j<n; j++)
    f << C.interfaces[j] << std::endl;
  if (n == 0) f << std::endl;

  return f;
}


