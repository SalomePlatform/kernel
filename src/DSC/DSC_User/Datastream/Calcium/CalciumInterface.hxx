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

//  File   : CalciumInterface.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-03-01 13:27:58 +0100 (jeu, 01 mar 2007) $
// Id          : $Id$
//
#ifndef _CALCIUM_INTERFACE_HXX_
#define _CALCIUM_INTERFACE_HXX_

#if defined(__CONST_H) || defined(__CALCIUM_H)
#error "The header CalciumInterface.hxx must be included before calcium.h"
#endif

//Interface CALCIUM des utilisateurs en C++ 
#include "CalciumCxxInterface.hxx"

/* Déclaration de l'Interface entre l'API C et l'API C++
   L'utilisateur CALCIUM n'a normalement pas a utliser cette interface
   En C/C++ il utilisera celle définie dans Calcium.c (calcium.h)
   En C++/CORBA directement celle de CalciumCxxInterface.hxx
*/
#include "CalciumCInterface.hxx"




#endif
