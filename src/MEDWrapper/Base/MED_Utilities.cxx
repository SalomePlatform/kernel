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

#include "MED_Utilities.hxx"
#include "MED_Common.hxx"

using namespace std;

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif


int MED::PrefixPrinter::myCounter = 0;

MED::PrefixPrinter::PrefixPrinter(bool theIsActive):
  myIsActive(theIsActive)
{
  if(myIsActive)
    myCounter++;
  MSG(MYDEBUG,"MED::PrefixPrinter::PrefixPrinter(...)- "<<myCounter);
}

MED::PrefixPrinter::~PrefixPrinter()
{
  if(myIsActive)
    myCounter--;
}

string MED::PrefixPrinter::GetPrefix()
{
  if(myCounter)
    return string(myCounter*2,' ');
  return "";
}


static MED::TEntity2GeomSet Entity2GeomSet;

bool InitEntity2GeomSet()
{
  using namespace MED;

  TGeomSet& aGeomARETESet = Entity2GeomSet[eARETE];
  aGeomARETESet.insert(eSEG2);
  aGeomARETESet.insert(eSEG3);

  TGeomSet& aGeomFACESet = Entity2GeomSet[eFACE];
  aGeomFACESet.insert(eTRIA3);
  aGeomFACESet.insert(eQUAD4);
  aGeomFACESet.insert(eTRIA6);
  aGeomFACESet.insert(eQUAD8);
  aGeomFACESet.insert(ePOLYGONE);

  TGeomSet& aGeomMAILLESet = Entity2GeomSet[eMAILLE];
  aGeomMAILLESet.insert(ePOINT1);
  aGeomMAILLESet.insert(aGeomARETESet.begin(),aGeomARETESet.end());
  aGeomMAILLESet.insert(aGeomFACESet.begin(),aGeomFACESet.end());
  aGeomMAILLESet.insert(eTETRA4);
  aGeomMAILLESet.insert(ePYRA5);
  aGeomMAILLESet.insert(ePENTA6);
  aGeomMAILLESet.insert(eHEXA8);
  aGeomMAILLESet.insert(eTETRA10);
  aGeomMAILLESet.insert(ePYRA13);
  aGeomMAILLESet.insert(ePENTA15);
  aGeomMAILLESet.insert(eHEXA20);
  aGeomMAILLESet.insert(ePOLYEDRE);
  
  return true;
}

static bool anIsInited = InitEntity2GeomSet();

const MED::TEntity2GeomSet& MED::GetEntity2GeomSet()
{
  return Entity2GeomSet;
}


