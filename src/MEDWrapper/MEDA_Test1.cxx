//  VISU OBJECT : interactive object for VISU entities implementation
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
//  File:    VISUConvertor.cxx
//  Author:  Alexey PETROV
//  Module : VISU

#include "MEDA_Algorithm.hxx"
#include "MED_Utilities.hxx"

using namespace std;
using namespace MEDA;

#ifdef DEBUG
static int MYDEBUG = 1;
#else
static int MYDEBUG = 0;
#endif


namespace MEDA{
  using namespace std;
  using namespace boost;
  
  
};

void ReadMed(const char* theFileName){
  TWrapper aMed(theFileName);
  string aFileName = theFileName;
  aFileName += "+";
  TWrapper aMedW(aFileName);

  int aNbMeshes = aMed.GetNbMeshes();
  cout<<"GetNbMeshes() = "<<aNbMeshes<<endl;

  string aName;
  for(int iMesh = 0; iMesh < aNbMeshes; iMesh++){
    PMeshInfo aMeshInfo = aMed.GetMeshInfo(iMesh);
  
    PNodeInfo aNodeInfo = aMed.GetNodeInfo(aMeshInfo);

    MED::TEntityInfo aEntityInfo = aMed.GetEntityInfo(aMeshInfo);

    TCellGroup aCellGroup = GetCellsByEntity(aMed,aMeshInfo,aEntityInfo);

    TFamilyGroup aFamilyGroup = GetFamilies(aMed,aMeshInfo);

    TFamilyByEntity aFamilyByEntity = GetFamiliesByEntity(aMed,aNodeInfo,aCellGroup,aFamilyGroup);

    TGroupInfo aGroupInfo = GetFamiliesByGroup(aFamilyGroup);

    TTimeStampGroup aTimeStampGroup = GetFieldsByEntity(aMed,aMeshInfo,aEntityInfo);

    TFieldGroup aFieldGroup = GetFieldsByEntity(aTimeStampGroup);

  }
  cout<<"OK"<<endl;
}


int main(int argc, char** argv){ 
  try{
    if(argc > 1)
      ReadMed(argv[1]);
    return 0;
  }catch(std::exception& exc){
    cout<<"Follow exception was accured :\n"<<exc.what()<<endl;
  }catch(...){
    cout<<"Unknown exception was accured in VISU_Convertor_impl"<<endl;
  } 
  return 1;
}
