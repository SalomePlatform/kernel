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

//  File   : testAdjacentFunctor.cxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-01-08 19:01:14 +0100 (lun, 08 jan 2007) $
// Id          : $Id$
//
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <functional>
#include <utility>

#include "DisplayPair.hxx"
#include "AdjacentFunctor.hxx"

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
  std::cout << "La valeur recherchée est : " << valueToFind << std::endl;

  std::sort(vect.begin(),vect.end(),std::less< Type > ());
  std::cout << "Vecteur trié par ordre croissant :" << std::endl;
  copy(vect.begin(),vect.end(),std::ostream_iterator<Type>(std::cout," "));
  std::cout<< std::endl;

  std::vector<Type>::iterator it,it1,it2;
  AdjacentFunctor<Type> ap(valueToFind);
  AdjacentFunctor<Type> & vap(ap);
  // Ne peut pas marcher à cause de la recopie du prédicat !!
  it = std::find_if(vect.begin(),vect.end(),vap);
  // On peut garder la valeur prédente de l'itérateur
  // si l'itérateur n'est pas reverse
//   for (it = vect.begin(); it!=vect.end(); ++it) 
//      if ( ap(*it) ) break;

  if ( it == vect.end() ) {
    std::cout << " Je n'ai pas trouvé la valeur " << valueToFind << std::endl;
  } else {
    if ( (*it) == valueToFind ) std::cout << " La valeur est trouvée *it :" << (*it) << std::endl;
    else {
      //      Type min,max;
      //      if ( ap.getBounds(min,max) ) {
      it2=it;it2--;
      std::cout << "La valeur : " << valueToFind << " est encadrée par  (*it) :" << (*it) << " et valeur de *(it-1) :" << (*it2) << std::endl;
    }
  }
  
  

         
 //  if ( it == vect.end() ) {
//     std::cout << " Je n'ai pas trouvé la valeur " << valueToFind << std::endl;
//   } else {
//     if ( (*it) == valueToFind ) std::cout << " La valeur est trouvée *it :" << (*it) << std::endl;
//     else {
//       Type min,max;
//       if ( ap.getBounds(min,max) ) {
//         it2=it;it2--;
//      std::cout << "La valeur : " << valueToFind << " est encadrée par  (*it) :" << (*it) << " et valeur de *(it-1) :" << (*it2) << std::endl;
//       }
//     }
//   }

  //TEST2
//   typedef std::map<Type,double> MapIntDouble;
//   MapIntDouble myMap;
//   MapIntDouble::const_iterator itM1,itM2;
//   for(it=vect.begin(); it!=vect.end(); ++it) myMap[*it] = myRand();
  
//   std::cout << "Clés de la Map :" << std::endl;
//   for(itM1=myMap.begin();itM1!=myMap.end();++itM1)
//     //std::cout << &((*itM1).first) 
//     std::cout << (*itM1).first << " ";
//   std::cout<< std::endl;
 
//   AdjacentFunctor<MapIntDouble::value_type::first_type> apMap(valueToFind);
//   for (itM1 = myMap.begin(); itM1 != myMap.end(); ++itM1) 
//      if ( apMap(itM1->first) ) break;
         
//   if ( itM1 == myMap.end() ) {
//     std::cout << " Je n'est pas trouvé la valeur " << valueToFind << std::endl;
//   } else {
//     if ( (itM1->first) == valueToFind ) std::cout << " La valeur est trouvée itM1->first :" << (itM1->first) << std::endl;
//     else {
//       Type min,max;
//       if ( apMap.getBounds(min,max) ) {
//         itM2=itM1;itM2--;
//      std::cout << "La valeur " << valueToFind << " est encadrée par  (itM1->first) :" << (itM1->first) << " et valeur de (it-1)->first :" << (itM2->first) << std::endl;
//       }
//     }
//   }

  // TEST3
//   typedef std::map<std::pair<int,double>, double> MapIntDouble_Double;
//   MapIntDouble_Double myMapP;
//   MapIntDouble_Double::const_iterator itMP1,itMP2;
//   for(it=vect.begin(); it!=vect.end(); ++it) myMapP[std::make_pair<int,double>(*it,myRand())] = myRand();
//   //myMapP[std::make_pair<int,double>(*(vect.begin()),myRand())] = myRand();
  
//   std::cout << "Clés de la MapP :" << std::endl;
//   for(itMP1=myMapP.begin();itMP1!=myMapP.end();++itMP1)
//     std::cout << (*itMP1).first  << " " ;
//   std::cout<< std::endl;
 
//   //AdjacentPredicate<std::pair<int,double> > apMap(valueToFind);
//   std::pair<int,double> valuePToFind=std::make_pair<int,double>(valueToFind,myRand());
//   //std::pair<int,double> valuePToFind=myMapP.begin()->first;
//   AdjacentFunctor< MapIntDouble_Double::key_type  > apMapP(valuePToFind);
//   for (itMP1 = myMapP.begin(); itMP1 != myMapP.end(); ++itMP1) 
//      if ( apMapP(itMP1->first) ) break;

//   if ( itMP1 == myMapP.end() ) {
//     std::cout << " Je n'est pas trouvé la valeur " << valuePToFind << std::endl;
//   } else {
//     if ( (itMP1->first) == valuePToFind ) std::cout << " La valeur est trouvée itMP1->first :" << (itMP1->first) << std::endl;
//     else {
//       MapIntDouble_Double::key_type min,max;
//       if ( apMapP.getBounds(min,max) ) {
//         itMP2=itMP1;itMP2--;
//      std::cout << "La valeur " << valuePToFind << " est encadrée par  (itMP1->first) :" << (itMP1->first) << " et valeur de (it-1)->first :" << (itMP2->first) << std::endl;
//       }
//     }
//   }


  std::cout<< std::endl;

};

