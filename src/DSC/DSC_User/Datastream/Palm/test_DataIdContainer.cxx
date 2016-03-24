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
//
#include "PalmCouplingPolicy.hxx"

#include "palm_data_short_port_provides.hxx"

//using namespace PalmCouplingPolicy;

class TEST1 : public PalmCouplingPolicy {
public:
  TEST1() {

  std::cout << "------------  TEST1 --------------" << std::endl;

  // int ret=0;
  
  CORBA::Long time=1,tag=1;
  typedef PalmCouplingPolicy::DataIdContainer DataIdContainer;  
  typedef PalmCouplingPolicy::DataId          DataId;
    
  DataId          dataId(time,tag);   //potentiellement avec un troisième paramètre
  //std::vector<CORBA::Long> a(1,time);std::vector<CORBA::Long> b(1,tag);
  //DataIdContainer dataIds(a,b);   

  std::cout << "-------- MARK 1 --"<< dataId <<"----------------" << std::endl;
   
  filtre_conversion * f_time = get_filtre_convert_TIME();
  filtre_conversion * f_tag =  get_filtre_convert_TAG(); 

  // On accepte les temps de 1 à 100 sans conversion
  filtre_elementaire * f_time_el = new filtre_elementaire(1, 100);
  f_time->config_elementaire(*f_time_el);

  // On accepte que les tags 1 et ce sans conversion
  filtre_elementaire * f_time_tag = new filtre_elementaire(1);
  f_tag->config_elementaire(*f_time_tag);

  DataIdContainer dataIds(dataId,*this);   

  DataIdContainer::iterator dataIdIt = dataIds.begin();

  if (!dataIds.empty())
    for (;dataIdIt != dataIds.end();++dataIdIt) {
      std::cout << "-------- MARK 2 --"<< *dataIdIt <<"----------------" << std::endl;

    }
  }
};

class TEST2 : public PalmCouplingPolicy {
public:
  TEST2() {
 
    std::cout << "------------  TEST2 --------------" << std::endl;

    // int ret=0;
  
  CORBA::Long time=1,tag=1;
  typedef PalmCouplingPolicy::DataIdContainer DataIdContainer;  
  typedef PalmCouplingPolicy::DataId          DataId;
    
  DataId          dataId(time,tag);   //potentiellement avec un troisième paramètre
  //std::vector<CORBA::Long> a(1,time);std::vector<CORBA::Long> b(1,tag);
  //DataIdContainer dataIds(a,b);   

  std::cout << "-------- MARK 1 --"<< dataId <<"----------------" << std::endl;

  filtre_conversion * f_time = get_filtre_convert_TIME();
  filtre_conversion * f_tag =  get_filtre_convert_TAG(); 
  
  
  filtre_elementaire * f_time_2 = new filtre_elementaire(1);
  filtre_elementaire * g_time_2 = new filtre_elementaire(2);
  f_time->config_elementaire(*f_time_2, *g_time_2);
  filtre_elementaire * f_time_3 = new filtre_elementaire(1);
  f_time->config_elementaire(*f_time_3);

  // On accepte que les tags 1 et ce sans conversion
  filtre_elementaire * f_time_tag = new filtre_elementaire(1);
  f_time_tag = new filtre_elementaire(1);
  f_tag->config_elementaire(*f_time_tag);

   
  DataIdContainer dataIds(dataId,*this);   

  DataIdContainer::iterator dataIdIt = dataIds.begin();

  if (!dataIds.empty())
    for (;dataIdIt != dataIds.end();++dataIdIt) {
      std::cout << "-------- MARK 2 --"<< *dataIdIt <<"----------------" << std::endl;

    }
  }
};

int main() {
  TEST1 test1;
  TEST2 test2;
};

