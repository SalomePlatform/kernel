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

//  File   : Calcium.c
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
//
#include "calcium.h"
#include "calciumf.h"
#include "CalciumFortranInt.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

// Interface C de SalomeCalcium

typedef int InfoType;
typedef char bool;

//TODO: ajouter les prototypes pour eviter les pbs de passage par valeur
InfoType ecp_fint_ (void * component, char* nomVar, float t);
InfoType ecp_fini_ (void * component, char* nomVar, int i);
InfoType ecp_efft_ (void * component, char* nomVar, float t);
InfoType ecp_effi_ (void * component, char* nomVar, int i);

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
  InfoType ecp_##_name (void * component, int mode,                     \
                        _timeType * ti, _timeType * tf, int * i,        \
                        char * nomvar, int bufferLength,                \
                        int * nRead, _type _qual ** data ) {            \
    size_t _nRead;                                                      \
    long   _i=*i;                                                       \
    fflush(stdout);                                                     \
    fflush(stderr);                                                     \
                                                                        \
    InfoType info =  ecp_lecture_##_typeName (component, mode, ti, tf, &_i, \
                                              nomvar, bufferLength, &_nRead, \
                                              data );                   \
    /*    std::cout << "-------- CalciumInterface(C Part), Valeur de data : " << std::endl; */ \
    /*    std::cout << "Ptr :" << *data << std::endl;                           */ \
    /*    for (int j=0; j<_nRead;++j)                                           */ \
    /*      printf("--- Valeur de data[%d] : %d \n",j,(*data)[j]);              */ \
    /*    std::cout << "Ptr :" << *data << std::endl;                           */ \
    /*                                                                  */ \
    /*    std::cerr << "-------- CalciumInterface(C Part) MARK 2 ------------------" << std::endl; */ \
    if(mode == CP_SEQUENTIEL)   \
      *i = _i;                                                          \
    *nRead=_nRead;                                                      \
    /*    std::cerr << "-------- CalciumInterface(C Part) MARK 3 ------------------" << std::endl; */ \
                                                                        \
    return info;                                                        \
  };                                                                    \
  void ecp_##_name##_free ( _type _qual * data) {                       \
    ecp_lecture_##_typeName##_free(data);                               \
  };


/* L'interface de cette routine diffère de celle obtenue par la macro :
   CALCIUM_LECT_INTERFACE_C_.
   Le paramètre supplémentaire strSize indique la taille fixe et identique
   des chaînes stockées dans data (les ports CALCIUM n'en n'ont pas besoin)
*/
InfoType ecp_lch(void * component, int mode, float * ti,        float * tf, int * i,
                char  * nomvar, int bufferLength, int * nRead,
                char *** data, int strSize) {

  size_t _nRead;
  long   _i=*i;
  fflush(stdout);fflush(stderr);

  InfoType info =  ecp_lecture_str (component, mode, ti, tf, &_i,
                                    nomvar, bufferLength, &_nRead,
                                    data);/*, strSize );
                                             strSize est  inutile pour les ports CALCIUM
                                             qui gèrent des tailles quelconques de chaines. */
  if(mode == CP_SEQUENTIEL)
    *i = _i;
  *nRead=_nRead;
  fflush(stdout);fflush(stderr);
  return info;
};

void ecp_lch_free (char* * data) {                              \
  ecp_lecture_str_free(data);                                   \
};


#define STAR *
/*REVERIFIER MAINTENANT 0 COPY avec int2integer*/
CALCIUM_EXT_LECT_INTERFACE_C_(len,float,int,int2integer,);
CALCIUM_EXT_LECT_INTERFACE_C_(lre,float,float,float,);
/*Permet d'envoyer/recevoir des réels sur un port Double  */
CALCIUM_EXT_LECT_INTERFACE_C_(lrd,float ,float  ,float2double,);
CALCIUM_EXT_LECT_INTERFACE_C_(ldb,double,double,double,);

CALCIUM_EXT_LECT_INTERFACE_C_(llo,float,int,bool,);
CALCIUM_EXT_LECT_INTERFACE_C_(lcp,float,float,cplx,);
/* CALCIUM_EXT_LECT_INTERFACE_C_(lch,float,char,STAR[]);  */



/**************************************/
/* INTERFACES DE LECTURE AVEC RECOPIE */
/**************************************/

#define CALCIUM_LECT_INTERFACE_C_(_name,_timeType,_calInt,_type,_typeName,_qual,lastarg) \
  _calInt cp_##_name (void * component, _calInt mode,                   \
                       _timeType * ti, _timeType * tf, _calInt * i,     \
                       char * nomvar, _calInt bufferLength,             \
                       _calInt * nRead, _type _qual * data              \
                       lastarg ) {                                      \
                                                                        \
    int    _mode         = (int) mode;                                  \
    size_t _bufferLength = bufferLength;                                \
    size_t _nRead;                                                      \
    long   _i            =*i;                                           \
    fflush(stdout);                                                     \
    fflush(stderr);                                                     \
                                                                        \
    if ( (data == NULL) || (_bufferLength < 1) ) return CPNTNULL;       \
                                                                        \
    _calInt info =  ecp_lecture_##_typeName (component, _mode, ti, tf, &_i, \
                                              nomvar, _bufferLength, &_nRead, \
                                              &data );                  \
    if(mode == CP_SEQUENTIEL)                                           \
      *i = _i;                                                          \
    *nRead=_nRead;                                                      \
    fflush(stdout);                                                     \
    fflush(stderr);                                                     \
                                                                        \
    return info;                                                        \
  };                                                                    \
  void cp_##_name##_free ( _type _qual * data) {                        \
    ecp_lecture_##_typeName##_free(data);                               \
  };


/* L'interface de cette routine diffère de celle obtenue par la macro :
   CALCIUM_LECT_INTERFACE_C_.
   Le paramètre supplémentaire strSize indique la taille fixe et identique
   des chaînes stockées dans data (les ports CALCIUM n'en n'ont pas besoin)
*/

/* InfoType cp_lch(void * component, int mode, float * ti,      float * tf, int * i, */
/*              char  * nomvar, int bufferLength, int * nRead, */
/*              char ** data, int strSize) { */

/*   size_t _nRead;                                                      */
/*   long   _i=*i;                                                       */
/*   fflush(stdout);fflush(stderr);                                                      */
/*   fprintf(stderr,"Beginning of cp_lch: %s %d %f\n",nomvar,*i,*ti);    */
/*   if ( (data == NULL) || (bufferLength < 1) ) return CPNTNULL;        */
/*   InfoType info =  ecp_lecture_str (component, mode, ti, tf, &_i,  */
/*                                  nomvar, bufferLength, &_nRead,  */
/*                                  &data);*/
/*, strSize );  */
/*                                           strSize est  inutile pour les ports CALCIUM */
/*                                           qui gèrent des tailles quelconques de chaines.  */
/*   if(mode == CP_SEQUENTIEL)                                           */
/*     *i = _i;                                                          */
/*   *nRead=_nRead;                                                      */
/*   fprintf(stderr,"End of cp_lch: %s %d \n",nomvar,*i);                        */
/*   fflush(stdout);fflush(stderr);                                                      */
/*   return info;                                                        */
/* };                                                                    */


/* Definition des méthodes calcium standard  */
/* CALCIUM_LECT_INTERFACE_C_( <suffixe du nom de l'interface à générer>, <type du paramètre temporel>, <type d'entier à utiliser pour les paramètres de type entier>,
                              <type de données>, <nom de l'interface C2CPP à utiliser>, <qualificateur de type optionnel des données>,<paramètres supplémentaire ignoré>)*/

CALCIUM_LECT_INTERFACE_C_(len,float ,int,int    ,int2integer,,);
/*llg ne fonctionnera pas toujours correctement (port calcium_integer) si sizeof(long) == 64 bits && cal_int==int (32 bits)
  sinon problème de conversion de 64bits vers 32bits */
CALCIUM_LECT_INTERFACE_C_(llg,float ,int,long   ,long2integer,,);

CALCIUM_LECT_INTERFACE_C_(lln,float ,int,long   ,long,,);

CALCIUM_LECT_INTERFACE_C_(lre,float ,int,float  ,float,,);
/*Permet d'envoyer/recevoir des réels sur un port Double  */
CALCIUM_LECT_INTERFACE_C_(lrd,float ,int,float  ,float2double,,);
CALCIUM_LECT_INTERFACE_C_(ldb,double,int,double ,double,,);

CALCIUM_LECT_INTERFACE_C_(llo,float ,int,int    ,bool,,);
CALCIUM_LECT_INTERFACE_C_(lcp,float ,int,float  ,cplx,,);
#define STAR *
#define LCH_LAST_PARAM ,int strsize
CALCIUM_LECT_INTERFACE_C_(lch,float ,int,char   ,str,STAR, LCH_LAST_PARAM );


/* Definition des méthodes calcium destinées à l'interfaçage fortran
   avec une taille des INTEGER fortran paramétrés à la configuration du KERNEL  */

CALCIUM_LECT_INTERFACE_C_(len_fort_,float ,cal_int,cal_int ,integer,,);
CALCIUM_LECT_INTERFACE_C_(lin_fort_,float ,cal_int,int     ,int2integer,,);
/*llg_fort_ ne fonctionnera pas toujours correctement (port calcium_integer) si sizeof(long) == 64 bits && cal_int==int (32 bits)
  sinon problème de conversion de 64bits vers 32bits */
CALCIUM_LECT_INTERFACE_C_(llg_fort_,float ,cal_int,long    ,long2integer,,);

CALCIUM_LECT_INTERFACE_C_(lre_fort_,float ,cal_int,float   ,float,,);
CALCIUM_LECT_INTERFACE_C_(lrd_fort_,float ,cal_int,float   ,float2double,,);
CALCIUM_LECT_INTERFACE_C_(ldb_fort_,double,cal_int,double  ,double,,);
CALCIUM_LECT_INTERFACE_C_(llo_fort_,float ,cal_int,int     ,bool,,);   /*int pour bool ou cal_int */
CALCIUM_LECT_INTERFACE_C_(lcp_fort_,float ,cal_int,float   ,cplx,,);
CALCIUM_LECT_INTERFACE_C_(lch_fort_,float ,cal_int,char    ,str,STAR, LCH_LAST_PARAM );

CALCIUM_LECT_INTERFACE_C_(lln_fort_,float ,cal_int,long    ,long,,);

/**********************************************/
/*  INTERFACES DE DÉBUT ET DE FIN DE COUPLAGE */
/**********************************************/

InfoType cp_cd (void * component, char * instanceName) {
  InfoType info =  ecp_cd_(component,instanceName);
  return info;
}

InfoType cp_fin (void * component, int code) {
  /* TODO : gérer avec les callbacks des ports DSC */

  InfoType info =  ecp_fin_(component,code);

  return info;
}


/***************************/
/*  INTERFACES D'ECRITURE  */
/***************************/

#define CALCIUM_ECR_INTERFACE_C_(_name,_timeType,_calInt,_type,_typeName,_qual,lastarg) \
  _calInt cp_##_name (void * component, _calInt mode,                   \
                       _timeType t, _calInt i,                          \
                       char * nomvar, _calInt nbelem,                   \
                       _type _qual * data                               \
                       lastarg ) {                                      \
                                                                        \
    int     _mode         =  mode;                                      \
    long    _i            =  i;                                         \
    size_t  _nbelem       =  nbelem;                                    \
    _timeType _t          =  t;                                         \
    fflush(stdout);                                                     \
    fflush(stderr);                                                     \
    if ( (data == NULL) || (nbelem < 1) ) return CPNTNULL;              \
                                                                        \
    _calInt info =  ecp_ecriture_##_typeName (component, _mode, &_t, _i, \
                                               nomvar, _nbelem,         \
                                               data );                  \
    fflush(stdout);                                                     \
    fflush(stderr);                                                     \
                                                                        \
    return info;                                                        \
  };                                                                    \




/* InfoType cp_ech(void * component, int mode, float t, int i, */
/*              char  * nomvar,  int nbelem, */
/*              char ** data, int strSize) { */

/*long   _i=i;*/
/*   fflush(stdout);fflush(stderr);                                                      */
/*   fprintf(stderr,"Beginning of cp_ech: %s %d %f\n",nomvar,i,t);       */
/*   if ( (data == NULL) || (nbelem < 1) ) return CPNTNULL;              */

/*   InfoType info =  ecp_ecriture_str (component, mode, &t, i,  */
/*                                   nomvar, nbelem,             */
/*                                   data); */
/*, strSize );*/
/*   fprintf(stderr,"End of cp_ech: %s %d \n",nomvar,i);                         */
/*   fflush(stdout);                                                     */
/*   fflush(stderr);                                                     */

/*   return info;                                                        */
/* };                                                                    */

/*  Definition des méthodes calcium standard  */
/*  CALCIUM_ECR_INTERFACE_C_(_name,_timeType,_calInt,type,_typeName,_qual) */
CALCIUM_ECR_INTERFACE_C_(een,float ,int,int   ,int2integer,,);
/*elg ne fonctionnera pas toujours correctement (port calcium_integer) si sizeof(long) == 64 bits && cal_int==int (32 bits)
  sinon problème de conversion de 64bits vers 32bits */
CALCIUM_ECR_INTERFACE_C_(elg,float ,int,long  ,long2integer,,);
CALCIUM_ECR_INTERFACE_C_(ere,float ,int,float ,float,,);
/*Permet d'envoyer/recevoir des réels sur un port Double  */
CALCIUM_ECR_INTERFACE_C_(erd,float ,int,float ,float2double,,);
CALCIUM_ECR_INTERFACE_C_(edb,double,int,double,double,,);

CALCIUM_ECR_INTERFACE_C_(elo,float ,int,int   ,bool,,);
CALCIUM_ECR_INTERFACE_C_(ecp,float ,int,float ,cplx,,);
CALCIUM_ECR_INTERFACE_C_(ech,float ,int,char  ,str,STAR,LCH_LAST_PARAM );

CALCIUM_ECR_INTERFACE_C_(eln,float ,int,long  ,long,,);

/* Definition des méthodes calcium destinées à l'interfaçage fortran
   avec une taille des INTEGER fortran paramétrés à la configuration du KERNEL  */

CALCIUM_ECR_INTERFACE_C_(een_fort_,float ,cal_int,cal_int,integer,,);
/*elg_fort_ ne fonctionnera pas toujours correctement (port calcium_integer) si sizeof(long) == 64 bits && cal_int==int (32 bits)
  sinon problème de conversion de 64bits vers 32bits */
CALCIUM_ECR_INTERFACE_C_(elg_fort_,float ,cal_int,long   ,long2integer,,);
CALCIUM_ECR_INTERFACE_C_(ein_fort_,float ,cal_int,int    ,int2integer,,);
CALCIUM_ECR_INTERFACE_C_(ere_fort_,float ,cal_int,float ,float,,);
CALCIUM_ECR_INTERFACE_C_(erd_fort_,float ,cal_int,float ,float2double,,);
CALCIUM_ECR_INTERFACE_C_(edb_fort_,double,cal_int,double,double,,);
CALCIUM_ECR_INTERFACE_C_(elo_fort_,float ,cal_int,int   ,bool,,);
CALCIUM_ECR_INTERFACE_C_(ecp_fort_,float ,cal_int,float ,cplx,,);
CALCIUM_ECR_INTERFACE_C_(ech_fort_,float ,cal_int,char  ,str,STAR,LCH_LAST_PARAM );

CALCIUM_ECR_INTERFACE_C_(eln_fort_,float ,cal_int,long   ,long,,);

/***************************/
/*  Interface for cleaning */
/***************************/

InfoType cp_fini (void * component, char * nomvar, int i)
{
  InfoType info =  ecp_fini_(component,nomvar,i);
  return info;
}

InfoType cp_fint (void * component, char * nomvar, float t)
{
  InfoType info =  ecp_fint_(component,nomvar,t);
  return info;
}

InfoType cp_effi (void * component, char * nomvar, int i)
{
  InfoType info =  ecp_effi_(component,nomvar,i);
  return info;
}

InfoType cp_efft (void * component, char * nomvar, float t)
{
  InfoType info =  ecp_efft_(component,nomvar,t);
  return info;
}
