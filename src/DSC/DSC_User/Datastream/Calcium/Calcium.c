/*  Copyright (C) 2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN, */
/*  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS  */

/*  This library is free software; you can redistribute it and/or  */
/*  modify it under the terms of the GNU Lesser General Public  */
/*  License as published by the Free Software Foundation; either  */
/*  version 2.1 of the License.  */

/*  This library is distributed in the hope that it will be useful,  */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of  */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  */
/*  Lesser General Public License for more details.  */

/*  You should have received a copy of the GNU Lesser General Public  */
/*  License along with this library; if not, write to the Free Software  */
/*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA  */

/* See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com */



/*  File   : Calcium.c */
/*  Author : Eric Fayolle (EDF) */
/*  Module : KERNEL */
/* Id          : $Id$ */

#include "calcium.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Interface C de SalomeCalcium 

typedef int InfoType;
typedef char bool;

/************************************/
/* INTERFACES DE LECTURE EN 0 COPIE */
/************************************/

/* Definition des méthodes calcium étendues en 0 copie */
/* Le buffer est alloué par le port pas par l'utilisateur */
/* Remarquer le type ** de data */
/* L'utilisateur devra appeler ecp_..._free pour désallouer le buffer interne */
/* Attention en cas de lectures multiples : le buffer retourné est le même */
/* Attention si les niveaux sont actifs le buffer peut être supprimé automatiquement par calcium. */

#define CALCIUM_EXT_LECT_INTERFACE_C_(_name,_timeType,_type,_typeName,_qual) \
  InfoType ecp_##_name (void * component, int mode,			\
			_timeType * ti, _timeType * tf, int * i,	\
		        char * nomvar, int bufferLength,		\
			int * nRead, _type _qual ** data ) {		\
    size_t _nRead;							\
    long   _i=*i;							\
    fflush(stdout);							\
    fflush(stderr);							\
    fprintf(stderr,"Beginning of ecp_" #_name " : %s %d %f\n",nomvar,*i,*ti); \
    									\
    									\
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
    ecp_lecture_##_typeName##_free(data);				\
  };

#define STAR *
CALCIUM_EXT_LECT_INTERFACE_C_(len,float,int,int,);
CALCIUM_EXT_LECT_INTERFACE_C_(lre,float,float,float,);
CALCIUM_EXT_LECT_INTERFACE_C_(ldb,double,double,double,);
CALCIUM_EXT_LECT_INTERFACE_C_(llo,float,int,bool,);
CALCIUM_EXT_LECT_INTERFACE_C_(lcp,float,float,cplx,);
/* CALCIUM_EXT_LECT_INTERFACE_C_(lch,float,char,STAR[]);  */

/* L'interface de cette routine diffère de celle obtenue par la macro :
   CALCIUM_LECT_INTERFACE_C_.
   Le paramètre supplémentaire strSize indique la taille fixe et identique
   des chaînes stockées dans data (les ports CALCIUM n'en n'ont pas besoin)
*/
InfoType ecp_lch(void * component, int mode, float * ti,	float *	tf, int * i,
		char  *	nomvar, int bufferLength, int * nRead,
		char *** data, int strSize) {

  size_t _nRead;							
  long   _i=*i;							
  fflush(stdout);fflush(stderr);							
  fprintf(stderr,"Beginning of cp_lch: %s %d %f\n",nomvar,*i,*ti);	
    									
 
  InfoType info =  ecp_lecture_str (component, mode, ti, tf, &_i, 
				    nomvar, bufferLength, &_nRead, 
				    data);/*, strSize ); 
					     strSize est  inutile pour les ports CALCIUM
					     qui gèrent des tailles quelconques de chaines. */
  if(mode == CP_SEQUENTIEL)						
    *i = _i;								
  *nRead=_nRead;							
  fprintf(stderr,"End of cp_lch: %s %d \n",nomvar,*i);			
  fflush(stdout);fflush(stderr);							
									
  return info;							
};									

void ecp_lch_free (char* * data) {				\
  ecp_lecture_str_free(data);					\
};


/**************************************/
/* INTERFACES DE LECTURE AVEC RECOPIE */
/**************************************/

#define CALCIUM_LECT_INTERFACE_C_(_name,_timeType,_type,_typeName,_qual) \
  InfoType cp_##_name (void * component, int mode,			\
		       _timeType * ti, _timeType * tf, int * i,		\
		       char * nomvar, int bufferLength,			\
		       int * nRead, _type _qual * data ) {		\
    size_t _nRead;							\
    long   _i=*i;							\
    fflush(stdout);							\
    fflush(stderr);							\
    fprintf(stderr,"Beginning of cp_" #_name " : %s %d %f\n",nomvar,*i,*ti); \
    									\
    if ( (data == NULL) || (bufferLength < 1) ) return CPNTNULL;	\
    									\
    InfoType info =  ecp_lecture_##_typeName (component, mode, ti, tf, &_i, \
					      nomvar, bufferLength, &_nRead, \
					      &data );			\
    if(mode == CP_SEQUENTIEL)						\
      *i = _i;								\
    *nRead=_nRead;							\
    fprintf(stderr,"End of cp_" #_name " : %s %d \n",nomvar,*i);	\
    fflush(stdout);							\
    fflush(stderr);							\
									\
    return info;							\
  };									\
  void cp_##_name##_free ( _type _qual * data) {			\
    ecp_lecture_##_typeName##_free(data);				\
  };


/* L'interface de cette routine diffère de celle obtenue par la macro :
   CALCIUM_LECT_INTERFACE_C_.
   Le paramètre supplémentaire strSize indique la taille fixe et identique
   des chaînes stockées dans data (les ports CALCIUM n'en n'ont pas besoin)
*/
InfoType cp_lch(void * component, int mode, float * ti,	float *	tf, int * i,
		char  *	nomvar, int bufferLength, int * nRead,
		char ** data, int strSize) {

  size_t _nRead;							
  long   _i=*i;							
  fflush(stdout);fflush(stderr);							
  fprintf(stderr,"Beginning of cp_lch: %s %d %f\n",nomvar,*i,*ti);	
    									
  if ( (data == NULL) || (bufferLength < 1) ) return CPNTNULL;	
  
  InfoType info =  ecp_lecture_str (component, mode, ti, tf, &_i, 
				    nomvar, bufferLength, &_nRead, 
				    &data);/*, strSize ); 
					     strSize est  inutile pour les ports CALCIUM
					     qui gèrent des tailles quelconques de chaines. */
  if(mode == CP_SEQUENTIEL)						
    *i = _i;								
  *nRead=_nRead;							
  fprintf(stderr,"End of cp_lch: %s %d \n",nomvar,*i);			
  fflush(stdout);fflush(stderr);							
									
  return info;							
};									


/* Definition des méthodes calcium standard  */

CALCIUM_LECT_INTERFACE_C_(len,float,int,int,);
CALCIUM_LECT_INTERFACE_C_(lre,float,float,float,);
CALCIUM_LECT_INTERFACE_C_(ldb,double,double,double,);
CALCIUM_LECT_INTERFACE_C_(llo,float,int,bool,); 
CALCIUM_LECT_INTERFACE_C_(lcp,float,float,cplx,);
#define STAR *
/*   CALCIUM_LECT_INTERFACE_C_(lch,float,char,STAR); */



/**********************************************/
/*  INTERFACES DE DÉBUT ET DE FIN DE COUPLAGE */
/**********************************************/

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



/***************************/
/*  INTERFACES D'ECRITURE  */
/***************************/

#define CALCIUM_ECR_INTERFACE_C_(_name,_timeType,_type,_typeName,_qual)	\
  InfoType cp_##_name (void * component, int mode,			\
		       _timeType t, int i,				\
		       char * nomvar, int nbelem,			\
		       _type _qual * data ) {				\
									\
    /*long   _i=i;*/							\
    fflush(stdout);							\
    fflush(stderr);							\
    fprintf(stderr,"Beginning of cp_" #_name " : %s %d %f\n",nomvar,i,t);	\
    if ( (data == NULL) || (nbelem < 1) ) return CPNTNULL;		\
									\
    InfoType info =  ecp_ecriture_##_typeName (component, mode, &t, i,	\
					       nomvar, nbelem,		\
					       data );			\
    fprintf(stderr,"End of cp_" #_name " : %s %d \n",nomvar,i);		\
    fflush(stdout);							\
    fflush(stderr);							\
									\
    return info;							\
  };									\


/*  Definition des méthodes calcium standard  */

CALCIUM_ECR_INTERFACE_C_(een,float,int,int,);
CALCIUM_ECR_INTERFACE_C_(ere,float,float,float,);
CALCIUM_ECR_INTERFACE_C_(edb,double,double,double,);
/*CALCIUM_ECR_INTERFACE_C_(elo,float,bool,bool,);*/
CALCIUM_ECR_INTERFACE_C_(elo,float,int,bool,);
CALCIUM_ECR_INTERFACE_C_(ecp,float,float,cplx,);


InfoType cp_ech(void * component, int mode, float t, int i,
		char  *	nomvar,  int nbelem,
		char ** data, int strSize) {
									
  /*long   _i=i;*/							
  fflush(stdout);fflush(stderr);							
  fprintf(stderr,"Beginning of cp_ech: %s %d %f\n",nomvar,i,t);	
  if ( (data == NULL) || (nbelem < 1) ) return CPNTNULL;		
    
  InfoType info =  ecp_ecriture_str (component, mode, &t, i,	
				     nomvar, nbelem,		
				     data);/*, strSize );*/
  fprintf(stderr,"End of cp_ech: %s %d \n",nomvar,i);			
  fflush(stdout);							
  fflush(stderr);							
									
  return info;							
};									


