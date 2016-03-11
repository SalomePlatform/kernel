// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

/*  
   File   : CalciumInterface.hxx
   Author : Eric Fayolle (EDF)
   Module : KERNEL
   Modified by : $LastChangedBy$
   Date        : $LastChangedDate: 2007-03-01 13:27:58 +0100 (jeu, 01 mar 2007) $
   Id          : $Id$
*/
#ifndef _CALCIUM_C_INTERFACE_H_
#define _CALCIUM_C_INTERFACE_H_

#include "CalciumMacroCInterface.hxx"
#include "CalciumTypes.hxx"
#include "CalciumFortranInt.h"
#include <cstdlib>

/* Déclaration de l'Interface entre l'API C et l'API C++
   L'utilisateur CALCIUM n'a normalement pas a utliser cette interface
   En C/C++ il utilisera celle définie dans Calcium.c (calcium.h)
2   En C++/CORBA directement celle de CalciumCxxInterface.hxx
*/

/* En CALCIUM l'utilisation de données de type double
   implique des dates de type double, pour les autres
   types de données les dates sont de type float
*/
template <class T> struct CalTimeType {
  typedef float TimeType;
};

template <> struct CalTimeType<double> {
  typedef double TimeType;
};

/* Déclaration de ecp_lecture_... , ecp_ecriture_..., ecp_free_... */

/*  Le premier argument est utilisé :
    - comme suffixe dans la définition des noms ecp_lecture_ , ecp_ecriture_ et ecp_free_
    Le second argument est utilisé :
    - comme argument template à l'appel de la méthode C++ correspondante
        ( le type CORBA de port correspondant est alors obtenu par un trait)
   Le troisième argument est utilisée :
   - pour typer le paramètre data de la procédure générée 
   - pour déduire le type des paramètres t, ti tf via un trait
   - comme premier paramètre template à l'appel de la méthode C++ correspondante
         (pour typer les données passées en paramètre )
   Notons que dans le cas CALCIUM_C2CPP_INTERFACE_(int,int,), le type int n'existe pas
   en CORBA, le port CALCIUM correspondant utilise une séquence de long. La méthode
   C++ CALCIUM de lecture repère cette différence de type et charge 
   le manipulateur de données d'effectuer  une recopie (qui fonctionne si les types sont compatibles). 
   Notons qu'en CORBA CORBA:Long est mappé sur long uniquement si celui-ci est 32bits sinon
   il sera mappé sur le type int (si il est 32bits). Le type CORBA:LongLong est mappé sur le type long
   s'il est 64 bits sinon celà peut être un long long (s'il existe).
*/
CALCIUM_C2CPP_INTERFACE_HXX_(intc,int,int,);
CALCIUM_C2CPP_INTERFACE_HXX_(long,long,long,);

CALCIUM_C2CPP_INTERFACE_HXX_(integer,integer,cal_int,);
CALCIUM_C2CPP_INTERFACE_HXX_(int2integer,integer,int,);
CALCIUM_C2CPP_INTERFACE_HXX_(long2integer,integer, long,);

CALCIUM_C2CPP_INTERFACE_HXX_(float,float,float, );
CALCIUM_C2CPP_INTERFACE_HXX_(double,double,double,);

CALCIUM_C2CPP_INTERFACE_HXX_(float2double,double,float, );

/*  Fonctionne mais essai suivant pour simplification de Calcium.c CALCIUM_C2CPP_INTERFACE_(bool,bool,);*/
CALCIUM_C2CPP_INTERFACE_HXX_(bool,bool,int,);
CALCIUM_C2CPP_INTERFACE_HXX_(cplx,cplx,float,);
CALCIUM_C2CPP_INTERFACE_HXX_(str,str,char*,);

/* Déclaration de ecp_fin */
extern "C" CalciumTypes::InfoType ecp_fin_ (void * component, int code);
extern "C" CalciumTypes::InfoType ecp_cd_ (void * component, char* instanceName);
extern "C" CalciumTypes::InfoType ecp_fini_ (void * component, char* nomVar, int i);
extern "C" CalciumTypes::InfoType ecp_fint_ (void * component, char* nomVar, float t);
extern "C" CalciumTypes::InfoType ecp_effi_ (void * component, char* nomVar, int i);
extern "C" CalciumTypes::InfoType ecp_efft_ (void * component, char* nomVar, float t);

#endif
