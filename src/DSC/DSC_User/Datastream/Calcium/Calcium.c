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
//  File   : Calcium.c
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Id          : $Id$

#include "calcium.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Interface C de SalomeCalcium 

typedef int InfoType;
typedef int bool;

// INTERFACES DE LECTURE


// Definition des méthodes calcium étendues 
// permettant le 0 copy.
//const char * nomvar
#define CALCIUM_EXT_LECT_INTERFACE_C_(_name,_timeType,_type,_typeName,_qual) \
  InfoType ecp_##_name (void * component, int mode,			\
			_timeType * ti, _timeType * tf, int * i,	\
		        char * nomvar, int bufferLength,		\
			int * nRead, _type _qual ** data ) {		\
    size_t _nRead;							\
    long   _i=*i;							\
    									\
    /*    std::cerr << "-------- CalciumInterface(C Part) MARK 1 ------------------" << std::endl; */ \
    InfoType info =  ecp_lecture_##_typeName (component, mode, ti, tf, &_i, \
					      nomvar, bufferLength, &_nRead, \
					      data );			\
    /*    std::cout << "-------- CalciumInterface(C Part), Valeur de data : " << std::endl; */ \
    /*    std::cout << "Ptr :" << *data << std::endl;				*/ \
    /*    for (int j=0; j<_nRead;++j)						*/ \
    /*      printf("--- Valeur de data[%d] : %d \n",j,(*data)[j]);		*/ \
    /*    std::cout << "Ptr :" << *data << std::endl;				*/ \
    /*									*/ \
    /*    std::cerr << "-------- CalciumInterface(C Part) MARK 2 ------------------" << std::endl; */ \
    if(mode == CP_SEQUENTIEL)   \
      *i = _i;								\
    *nRead=_nRead;							\
    /*    std::cerr << "-------- CalciumInterface(C Part) MARK 3 ------------------" << std::endl; */ \
									\
    return info;							\
  };									\
  void ecp_##_name##_free ( _type _qual * data) {			\
    ecp_lecture_##_typeName##_free(data);					\
  };

CALCIUM_EXT_LECT_INTERFACE_C_(len,float,int,int,);
CALCIUM_EXT_LECT_INTERFACE_C_(lre,float,float,float,);
CALCIUM_EXT_LECT_INTERFACE_C_(ldb,double,double,double,);
CALCIUM_EXT_LECT_INTERFACE_C_(llo,float,bool,bool,);
CALCIUM_EXT_LECT_INTERFACE_C_(lcp,float,float,cplx,);
#define STAR *
//  CALCIUM_EXT_LECT_INTERFACE_C_(lch,float,char,STAR[]); 
// La signature n'est pas la même pour les chaines de caractères il y a aussi 
// la taille des chaines

//const char * nomvar

#define CALCIUM_LECT_INTERFACE_C_(_name,_timeType,_type,_typeName,_qual)		\
  InfoType cp_##_name (void * component, int mode,			\
		       _timeType * ti, _timeType * tf, int * i,		\
		       char * nomvar, int bufferLength,			\
		       int * nRead, _type _qual * data ) {		\
    size_t _nRead;							\
    long   _i=*i;							\
    fflush(stdout);           \
    fflush(stderr);           \
    fprintf(stderr,"Beginning of CPLxx: %s %d %f\n",nomvar,*i,*ti); \
    									\
    if ( (data == NULL) || (bufferLength < 1) ) return CPNTNULL;	\
    									\
    InfoType info =  ecp_lecture_##_typeName (component, mode, ti, tf, &_i,	\
					 nomvar, bufferLength, &_nRead, \
					 &data );			\
    if(mode == CP_SEQUENTIEL)   \
      *i = _i;							\
    *nRead=_nRead;							\
    fprintf(stderr,"End of CPLxx: %s %d \n",nomvar,*i); \
    fflush(stdout);           \
    fflush(stderr);           \
									\
    return info;							\
  };									\
  void cp_##_name##_free ( _type _qual * data) {			\
    ecp_lecture_##_typeName##_free(data);					\
  };


// Definition des méthodes calcium standard 

CALCIUM_LECT_INTERFACE_C_(len,float,int,int,);
CALCIUM_LECT_INTERFACE_C_(lre,float,float,float,);
CALCIUM_LECT_INTERFACE_C_(ldb,double,double,double,);
CALCIUM_LECT_INTERFACE_C_(llo,float,bool,bool,);
CALCIUM_LECT_INTERFACE_C_(lcp,float,float,cplx,);
#define STAR *
//  CALCIUM_LECT_INTERFACE_C_(lch,float,char,STAR);


// INTERFACES DE DÉBUT ET DE FIN DE COUPLAGE

InfoType cp_cd (void * component, char * instanceName) {
  /* TODO : Trouver le nom de l'instance SALOME*/
  if (instanceName) strcpy(instanceName,"UNDEFINED");
  return CPOK;
}
InfoType cp_fin (void * component, int code) {
  /* TODO : gérer avec les callbacks des ports DSC */
  
  InfoType info =  ecp_fin_(component,code);
  
  return info;
}



#define CALCIUM_ECR_INTERFACE_C_(_name,_timeType,_type,_typeName,_qual)	\
  InfoType cp_##_name (void * component, int mode,			\
		       _timeType t, int i,				\
		       char * nomvar, int nbelem,			\
		       _type _qual * data ) {				\
									\
    /*long   _i=i;*/							\
    fflush(stdout);           \
    fflush(stderr);           \
    fprintf(stderr,"Beginning of CPExx: %s %d %f\n",nomvar,i,t); \
    if ( (data == NULL) || (nbelem < 1) ) return CPNTNULL;		\
									\
    InfoType info =  ecp_ecriture_##_typeName (component, mode, &t, i,	\
					       nomvar, nbelem,		\
					       data );			\
    fprintf(stderr,"End of CPExx: %s %d \n",nomvar,i); \
    fflush(stdout);           \
    fflush(stderr);           \
									\
    return info;							\
  };									\


// Definition des méthodes calcium standard 

CALCIUM_ECR_INTERFACE_C_(een,float,int,int,);
CALCIUM_ECR_INTERFACE_C_(ere,float,float,float,);
CALCIUM_ECR_INTERFACE_C_(edb,double,double,double,);
CALCIUM_ECR_INTERFACE_C_(elo,float,bool,bool,);
CALCIUM_ECR_INTERFACE_C_(ecp,float,float,cplx,);

