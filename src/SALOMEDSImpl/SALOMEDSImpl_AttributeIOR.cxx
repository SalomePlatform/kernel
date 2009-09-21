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
//  File   : SALOMEDSImpl_AttributeIOR.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeIOR.hxx"
#include "SALOMEDSImpl_Study.hxx"

using namespace std;

//to disable automatic genericobj management comment the following line
#define WITHGENERICOBJ

#ifdef WITHGENERICOBJ
#include "SALOME_GenericObj_i.hh"

static CORBA::ORB_var getORB()
{
  int argc=0;
  return CORBA::ORB_init(argc,0);
}

void IORGenericObjDecref(const std::string& anIOR)
{
  CORBA::Object_var obj;
  SALOME::GenericObj_var gobj;
  try
    {
      obj = getORB()->string_to_object(anIOR.c_str());
      if(obj->_non_existent())return;
      gobj = SALOME::GenericObj::_narrow(obj);
      if(! CORBA::is_nil(gobj) )
        {
          gobj->Destroy();
        }
    }
  catch(const CORBA::Exception& e)
    {
    }
}

void IORGenericObjIncref(const std::string& anIOR)
{
  CORBA::Object_var obj;
  SALOME::GenericObj_var gobj;
  try
    {
      obj = getORB()->string_to_object(anIOR.c_str());
      if(obj->_non_existent())return;
      gobj = SALOME::GenericObj::_narrow(obj);
      if(! CORBA::is_nil(gobj) )
        {
          gobj->Register();
        }
    }
  catch(const CORBA::Exception& e)
    {
    }
}
#else
void IORGenericObjDecref(const std::string& anIOR)
{}
void IORGenericObjIncref(const std::string& anIOR)
{}
#endif

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeIOR::GetID () 
{
  static std::string SALOMEDSImpl_AttributeIORID ("92888E01-7074-11d5-A690-0800369C8A03");
  return SALOMEDSImpl_AttributeIORID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

SALOMEDSImpl_AttributeIOR* SALOMEDSImpl_AttributeIOR::Set (const DF_Label& L,
                                                           const std::string& S) 
{
  SALOMEDSImpl_AttributeIOR* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeIOR*)L.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID()))) {
    A = new  SALOMEDSImpl_AttributeIOR(); 
    L.AddAttribute(A);
  }

  A->SetValue(S); 
  return A;
}

//=======================================================================
//function : SetValue
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeIOR::SetValue(const std::string& theValue)
{
  CheckLocked();

  SALOMEDSImpl_Study* study=SALOMEDSImpl_Study::GetStudy(Label());

  Backup();
  //remove IOR entry in study
  if(theValue != myString)
    {
      IORGenericObjIncref(theValue);
      IORGenericObjDecref(myString);
      study->DeleteIORLabelMapItem(myString);
    }

  myString = theValue;

  //add IOR entry in study
  SALOMEDSImpl_Study::IORUpdated(this);
}

//=======================================================================
//function : Value
//purpose  : 
//=======================================================================
std::string SALOMEDSImpl_AttributeIOR::Value() const
{
  return myString;
}

//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeIOR::SALOMEDSImpl_AttributeIOR()
:SALOMEDSImpl_GenericAttribute("AttributeIOR")
{
}

SALOMEDSImpl_AttributeIOR::~SALOMEDSImpl_AttributeIOR()
{
  IORGenericObjDecref(myString);
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeIOR::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

DF_Attribute* SALOMEDSImpl_AttributeIOR::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeIOR(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeIOR::Restore( DF_Attribute* with) 
{
  myString = dynamic_cast<SALOMEDSImpl_AttributeIOR*>(with)->Value();
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeIOR::Paste (DF_Attribute* into)
{
  dynamic_cast<SALOMEDSImpl_AttributeIOR*>(into)->SetValue(myString);
}

