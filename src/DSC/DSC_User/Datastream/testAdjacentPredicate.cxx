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

//  File   : testAdjacentPredicate.cxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-01-08 19:01:14 +0100 (lun, 08 jan 2007) $
// Id          : $Id$
//
#include "AdjacentPredicate.hxx"
#include "DisplayPair.hxx"

#include <iostream>

#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

struct MyRand {
  static const double MAXVALUE = 150.0;
  MyRand() { srand(getpid()); }
  int operator()() const {
        return 1+(int) ( MAXVALUE *rand()/(RAND_MAX +1.0));
  }
};

typedef int TimeType;
typedef double TagType;
typedef std::pair< TimeType , TagType >     DataId;

template < typename DataType > 
  DataType processTimeInterval (DataId & dataId, 
                                typename std::map<DataId, DataType>::const_iterator const & it1) {
     return (*it1).second;
};

int main() {
  typedef int Type;
  const   int N=20;
  std::vector<Type> vect(N);
  MyRand   myRand;

  //TEST1
  std::generate(vect.begin(),vect.end(),myRand);
  std::cout << "Vecteur généré aléatoirement :" << std::endl;
  copy(vect.begin(),vect.end(),std::ostream_iterator<Type>(std::cout," "));
  std::cout<< std::endl;

  int valueToFind = myRand();

  std::sort(vect.begin(),vect.end(),std::less< Type > ());
  std::cout << "Vecteur trié par ordre croissant :" << std::endl;
  copy(vect.begin(),vect.end(),std::ostream_iterator<Type>(std::cout," "));
  std::cout<< std::endl;

  std::vector<Type>::iterator it;
  AdjacentPredicate<Type> ap(valueToFind);
  it = std::adjacent_find(vect.begin(),vect.end(),ap);
  if ( it == vect.end() )
    std::cout << "Je n'est pas trouvé d'intervalle pour encadrer " << valueToFind << std::endl;
  else
    std::cout << "La valeur à trouver : " << valueToFind <<" est entre * it :" << *it << " et valeur de *(it+1) :" << *(it+1) << std::endl;
  std::cout<< std::endl;
  
  //TEST2
  typedef std::map<int,double> MapIntDouble;
  MapIntDouble myMap;
  MapIntDouble::const_iterator itM1,itM2;
  for(it=vect.begin(); it!=vect.end(); ++it) myMap[*it] = myRand();
  
  std::cout << "Clés de la Map :" << std::endl;
  for(itM1=myMap.begin();itM1!=myMap.end();++itM1)
    //std::cout << &((*itM1).first) 
    std::cout << (*itM1).first << " ";
  std::cout<< std::endl;
 
  //AdjacentPredicate<std::pair<int,double> > apMap(valueToFind);
  AdjacentPredicate< MapIntDouble::value_type  > apMap(valueToFind);
  itM1 = std::adjacent_find(myMap.begin(),myMap.end(),apMap);
  itM2=itM1;itM2++;
  if ( itM1 == myMap.end() )
    std::cout << "Map : Je n'est pas trouvé d'intervalle pour encadrer " << valueToFind << std::endl;
  else {
    std::cout << "Map : La valeur à trouver : " << valueToFind <<" est entre (*itM1).first :" << (*itM1).first << " et valeur de *(itM1+1).first :" << (*(itM2)).first << std::endl;
  }
  std::cout<< std::endl;

  // TEST3
  typedef std::map<std::pair<int,double>, double> MapPIntDouble_Double;
  MapPIntDouble_Double myMapP;
  MapPIntDouble_Double::const_iterator itMP1,itMP2;
  //  for(it=vect.begin(); it!=vect.end(); ++it) myMapP[std::make_pair<int,double>(*it,myRand())] = myRand();
  myMapP[std::make_pair<int,double>(*(vect.begin()),myRand())] = myRand();
  
  std::cout << "Clés de la MapP :" << std::endl;
  for(itMP1=myMapP.begin();itMP1!=myMapP.end();++itMP1)
    std::cout << (*itMP1).first  << " " ;
  std::cout<< std::endl;
 
  //AdjacentPredicate<std::pair<int,double> > apMap(valueToFind);
  //  std::pair<int,double> valuePToFind=std::make_pair<int,double>(valueToFind,myRand());
  std::pair<int,double> valuePToFind=myMapP.begin()->first;
  AdjacentPredicate< MapPIntDouble_Double::value_type  > apMapP(valuePToFind);
  itMP1 = std::adjacent_find(myMapP.begin(),myMapP.end(),apMapP);
  itMP2=itMP1;itMP2++;
  if ( itMP1 == myMapP.end() )
    std::cout << "MapP : Je n'est pas trouvé d'intervalle pour encadrer " << valuePToFind << std::endl;
  else {
    std::cout << "MapP : La valeur à trouver : " << valuePToFind <<" est entre (*itMP1).first :" << (*itMP1).first << " et valeur de *(itMP1+1).first :" << (*(itMP2)).first << std::endl;
  }

  std::cout << processTimeInterval<double>(valuePToFind,itMP1);
  std::cout<< std::endl;

  std::cout<< std::endl;

};

