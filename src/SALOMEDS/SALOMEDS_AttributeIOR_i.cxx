//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_AttributeIOR_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeIOR_i.hxx"
#include "SALOMEDS.hxx"

using namespace std;

char* SALOMEDS_AttributeIOR_i::Value()
{
 SALOMEDS::Locker lock;
  CORBA::String_var c_s = 
    CORBA::string_dup(dynamic_cast<SALOMEDSImpl_AttributeIOR*>(_impl)->Value().c_str());
  return c_s._retn();
}

//To disable automatic management of GenericObj, comment the following line
#define WITHGENERICOBJ

void SALOMEDS_AttributeIOR_i::SetValue(const char* value) 
{
#ifdef WITHGENERICOBJ
  CORBA::Object_var obj;
  SALOME::GenericObj_var gobj;
  try
    {
      obj = _orb->string_to_object(value);
      gobj = SALOME::GenericObj::_narrow(obj);
      if(! CORBA::is_nil(gobj) )
        gobj->Register();
    }
  catch(const CORBA::Exception& e)
    {
    }
  //unregister value
  try
    {
      std::string value=dynamic_cast<SALOMEDSImpl_AttributeIOR*>(_impl)->Value();
      if(value != "")
        {
          obj = _orb->string_to_object(value.c_str());
          gobj = SALOME::GenericObj::_narrow(obj);
          if(! CORBA::is_nil(gobj) )
            gobj->Destroy();
        }
    }
  catch(const CORBA::Exception& e)
    {
    }
#endif

  SALOMEDS::Locker lock;
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  string anExtStr((char *)Str.in());
  dynamic_cast<SALOMEDSImpl_AttributeIOR*>(_impl)->SetValue(anExtStr);
}

SALOMEDS_AttributeIOR_i::~SALOMEDS_AttributeIOR_i()
{}
