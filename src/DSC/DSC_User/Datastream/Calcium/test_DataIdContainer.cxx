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

//  File   : test_DataIdContainer.cxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-03-01 15:07:46 +0100 (jeu, 01 mar 2007) $
// Id          : $Id$
//
#include "CalciumCouplingPolicy.hxx"

#include "CalciumProvidesPort.hxx"
#include "CalciumException.hxx"

using namespace CalciumTypes;

class TEST1 : public CalciumCouplingPolicy {
public:
  int ret;
  TEST1() {

    ret=0;
  
    CORBA::Long time=1,tag=1;
    typedef CalciumCouplingPolicy::DataIdContainer DataIdContainer;  
    typedef CalciumCouplingPolicy::DataId          DataId;
    
    DataId          dataId(time,tag);   //potentiellement avec un troisième paramètre
    try {
      DataIdContainer dataIds(dataId,*this);   

      DataIdContainer::iterator dataIdIt = dataIds.begin();
      
      if (!dataIds.empty())
        for (;dataIdIt != dataIds.end();++dataIdIt) {
          std::cout << "(*dataIdIt) must be equal to given dataId parameter : " << *dataIdIt;
          std::cout << " == " << dataId << " : " << (ret = (*dataIdIt == dataId)) << std::endl;
        }
    } catch(const CalciumException & ex) {
      ret=1;
      std::cout << ex.what() << std::endl;
    }

  }
};

class TEST2 : public CalciumCouplingPolicy {
    
public:
  int ret;
  TEST2() {

    //  Doit filtrer le mode de dépendance temporel car le mode est 
    //  défini itératif
    ret=0;

    CORBA::Long time=1,tag=1;
    typedef CalciumCouplingPolicy::DataIdContainer DataIdContainer;  
    typedef CalciumCouplingPolicy::DataId          DataId;
    
    DataId          dataId(time,tag);   //potentiellement avec un troisième paramètre
    setDependencyType(ITERATION_DEPENDENCY);
    DataIdContainer dataIds(dataId,*this);   


    DataIdContainer::iterator dataIdIt = dataIds.begin();

    if (!dataIds.empty())
      for (;dataIdIt != dataIds.end();++dataIdIt) {
        std::cout << "(*dataIdIt) must be equal to given dataId parameter : " << *dataIdIt ;
        std::cout << " == " << DataId(0,tag) << " : " << (ret = (*dataIdIt == DataId(0,tag))) << std::endl;

      }
  }
};
 

class TEST3 : public CalciumCouplingPolicy {
public:
  int ret;

  TEST3() {

    //  Doit filtrer le mode de dépendance temporel car le mode est 
    //  défini itératif
    ret=0;

    CORBA::Long time=1,tag=1;
    typedef CalciumCouplingPolicy::DataIdContainer DataIdContainer;  
    typedef CalciumCouplingPolicy::DataId          DataId;
    
    DataId          dataId(time,tag);   //potentiellement avec un troisième paramètre
    setDependencyType(TIME_DEPENDENCY);
    DataIdContainer dataIds(dataId,*this);   


    DataIdContainer::iterator dataIdIt = dataIds.begin();

    if (!dataIds.empty())
      for (;dataIdIt != dataIds.end();++dataIdIt) {
        std::cout << "(*dataIdIt) must be equal to given dataId parameter : " << *dataIdIt ;
        std::cout << " == " << DataId(time,0) << " : " << (ret = (*dataIdIt == DataId(time,0))) << std::endl;

      }
  }
};
 
int main() {
  TEST1 test1;
  TEST2 test2;
  TEST3 test3;
  return !test1.ret+!test2.ret+!test3.ret;
};

