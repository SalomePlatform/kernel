//  Copyright (C) 2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : testInterpolation.cxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-01-08 19:01:14 +0100 (lun, 08 jan 2007) $
// Id          : $Id$

#include "lambda.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

struct MyRand {
  static const double MAXVALUE = 150.0;
  MyRand() { srand(getpid()); }
  int operator()() const {
 	return 1+(int) ( MAXVALUE *rand()/(RAND_MAX +1.0));
  }
};

int main() {

 typedef double Type;
 typedef double TimeType;
 const   int dataSize1=20;
 const   int dataSize2=30;
 const   int dataSize3=std::min< size_t >(dataSize1,dataSize2);
 std::vector<Type> vect1(dataSize1),vect2(dataSize2),vect3(dataSize3);
 MyRand   myRand;

  //TEST1
  std::generate(vect1.begin(),vect1.end(),myRand);
  std::cout << "Vecteur1 généré aléatoirement :" << std::endl;
  copy(vect1.begin(),vect1.end(),std::ostream_iterator<Type>(std::cout," "));
  std::cout<< std::endl;

  std::generate(vect2.begin(),vect2.end(),myRand);
  std::cout << "Vecteur2 généré aléatoirement :" << std::endl;
  copy(vect2.begin(),vect2.end(),std::ostream_iterator<Type>(std::cout," "));
  std::cout<< std::endl;
  std::vector<Type>::iterator InIt1=vect1.begin(),InIt2=vect2.begin(),OutIt=vect3.begin();

  TimeType t = 2.4;
  TimeType t2 = 3.4;
  TimeType t1 = 1.1;
  TimeType deltaT = t2-t1;
  TimeType coeff = (t2-t)/deltaT;

  boost::lambda::placeholder1_type _1;
  boost::lambda::placeholder2_type _2;

  std::transform(InIt1,InIt1+dataSize3,InIt2,OutIt, ( _1 - _2 ) * coeff + _2 );

  std::cout << "Vecteur3 calculé :" << std::endl;
  copy(vect3.begin(),vect3.end(),std::ostream_iterator<Type>(std::cout," "));
  std::cout<< std::endl;
	 

};

