//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
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
//  File   : SALOMEDS_AttributeReal_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include "SALOMEDS_AttributeReal_i.hxx"
#include "SALOMEDS.hxx"

#include <sstream>

using namespace std;

static const char* write_double(double value)
{
  std::ostringstream os;
  unsigned char* array = (unsigned char*)&value;
  for(int i = 0; i < sizeof(double); i++) {
    unsigned tmp = (unsigned short)array[i];
    os << " " << tmp;
  }
  return os.str().c_str();
}

static double read_double(const char* str)
{
  std::istringstream is(str);
  double value;
  unsigned char* array = (unsigned char*)(&value);
  for(int i = 0; i < sizeof(double); i++) {
    unsigned tmp;
    is >> tmp;
    array[i] = (unsigned char)tmp;
  }
  return value;
}

CORBA::Double SALOMEDS_AttributeReal_i::Value() {
  SALOMEDS::Locker lock;

  return Handle(TDataStd_Real)::DownCast(_myAttr)->Get();
}

void SALOMEDS_AttributeReal_i::SetValue(CORBA::Double value) {
  SALOMEDS::Locker lock;

  CheckLocked();
  Handle(TDataStd_Real)::DownCast(_myAttr)->Set(value);
}

char* SALOMEDS_AttributeReal_i::Store() {
  SALOMEDS::Locker lock;

  //  char* RealVal = new char[35];
  //  sprintf(RealVal, "%.20f", Value());
  //return RealVal;
  return (char*)write_double( (double)Value() );
}

void SALOMEDS_AttributeReal_i::Restore(const char* value) {
  SALOMEDS::Locker lock;

  //char *err = NULL;
  //CORBA::Double r =  strtod(value, &err);
  //if (err != value) SetValue(r);
  SetValue( read_double(value) );
}

