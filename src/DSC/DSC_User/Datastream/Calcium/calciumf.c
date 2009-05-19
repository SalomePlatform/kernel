//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
#include <string.h>
#include <stdlib.h>
#include "fortoc.h"
#include "calcium.h"
#include "calciumf.h"
#include "CalciumFortranInt.h"
#include <stdio.h>

static void* COMPO=0;

#ifdef __cplusplus
extern "C" {
#endif


static void fstrtocstr(char *cstr, char *fstr,cal_int fstr_len) 
{
  cal_int i,iend;
  for (iend = fstr_len-1; iend >= 0; iend--) 
    if (fstr[iend] != ' ') break;
  for (i = 0; i <= iend; i++)
    cstr[i] = fstr[i];
  cstr[i] = '\0';
}

static void cstrtofstr(char *cstr, char *fstr,cal_int fstr_len) 
{
  cal_int i, len;
  len = strlen(cstr);
  if (len > fstr_len) len = fstr_len;
  for (i = 0; i < len; i++)
    fstr[i] = cstr[i];
  while (i < fstr_len)
    fstr[i++] = ' ';
}

static char * fstr1(char *nom,cal_int nnom)
{
  char * cnom=(char*)malloc((nnom+1)*sizeof(char));
  fstrtocstr(cnom,nom,nnom);
  return cnom;
}

static char * free_str1(char *nom)
{
  free(nom);
}

/**********************************************/
/*  INTERFACES DE DÉBUT ET DE FIN DE COUPLAGE */
/**********************************************/

void F_FUNC(cpcd,CPCD)(long *compo,STR_PSTR(nom),cal_int *info STR_PLEN(nom))
{
  /* nom is OUT argument */
  cp_cd((void *)*compo,STR_PTR(nom));
  /* replace in place ??? */
  cstrtofstr(STR_PTR(nom),STR_PTR(nom),STR_LEN(nom));
}

void F_FUNC(cpfin,CPFIN)(long *compo,cal_int *dep,cal_int *err)
{
  fprintf(stderr,"CPFIN: \n");
  *err=cp_fin((void *)*compo,(int)*dep);
}

/**************************************/
/* INTERFACES DE LECTURE              */
/**************************************/



void F_FUNC(cplin,CPLIN)(long *compo,cal_int *dep,float *ti,float *tf,cal_int *iter,STR_PSTR(nom),
			 cal_int *max,cal_int *n, int *tab,cal_int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPLIN: %s %f %f\n",cnom,*ti,*tf);

#if   !SIZEOF_INT
#error "The macro SIZEOF_INT must be defined."    
#elif SIZEOF_INT == 4
  *err=cp_lin_fort_((void *)*compo,*dep,ti,tf,iter,cnom,*max,n,tab);
#else
  fprintf(stderr,"End of CPLIN: %s : Can't use fortran INTEGER*4 because int C is not 32bits long on this machine.\n",
	  cnom);
#endif
  fprintf(stderr,"End of CPLIN: %s \n",cnom);
  free_str1(cnom);
}

void F_FUNC(cpllg,CPLLG)(long *compo,cal_int *dep,float *ti,float *tf,cal_int *iter,STR_PSTR(nom),
            cal_int *max,cal_int *n, long *tab,cal_int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPLLG: %s %f %f\n",cnom,*ti,*tf);
#if   !SIZEOF_LONG
#error "The macro SIZEOF_LONG must be defined."    
#elif SIZEOF_LONG == 8
  *err=cp_llg_fort_((void *)*compo,*dep,ti,tf,iter,cnom,*max,n,tab);
#else
  fprintf(stderr,"End of CPLLG: %s : Can't use fortran INTEGER*8 because long C is not 64bits long on this machine.\n",
	  cnom);
#endif
  fprintf(stderr,"End of CPLLG: %s \n",cnom);
  free_str1(cnom);
}

void F_FUNC(cplen,CPLEN)(long *compo,cal_int *dep,float *ti,float *tf,cal_int *iter,STR_PSTR(nom),
            cal_int *max,cal_int *n, cal_int *tab,cal_int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPLEN: %s %f %f\n",cnom,*ti,*tf);
   *err=cp_len_fort_((void *)*compo,*dep,ti,tf,iter,cnom,*max,n,tab);
  fprintf(stderr,"End of CPLEN: %s \n",cnom);
  free_str1(cnom);
}


void F_FUNC(cpllo,CPLLO)(long *compo,cal_int *dep,float *ti,float *tf,cal_int *iter,STR_PSTR(nom),
            cal_int *max,cal_int *n, int *tab,cal_int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPLLO: %s %f %f\n",cnom,*ti,*tf);
  *err=cp_llo_fort_((void *)*compo,*dep,ti,tf,iter,cnom,*max,n,tab);
  fprintf(stderr,"End of CPLLO: %s \n",cnom);
  free_str1(cnom);
}

void F_FUNC(cpldb,CPLDB)(long *compo,cal_int *dep,double *ti,double *tf,cal_int *iter,STR_PSTR(nom),
            cal_int *max,cal_int *n, double *tab,cal_int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPLDB: %s %f %f \n",cnom, *ti,*tf);
  *err=cp_ldb_fort_((void *)*compo,*dep,ti,tf,iter,cnom,*max,n,tab);
  fprintf(stderr,"End of CPLDB: %s %f %f \n",cnom,*ti,*tf);
  free_str1(cnom);
}

void F_FUNC(cplre,CPLRE)(long *compo,cal_int *dep,float *ti,float *tf,cal_int *iter,STR_PSTR(nom),
            cal_int *max,cal_int *n, float *tab,cal_int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPLRE: %s %f %f \n",cnom, *ti,*tf);
  *err=cp_lre_fort_((void *)*compo,*dep,ti,tf,iter,cnom,*max,n,tab);
  fprintf(stderr,"End of CPLRE: %s %f %f \n",cnom,*ti,*tf);
  free_str1(cnom);
}

void F_FUNC(cplcp,CPLCP)(long *compo,cal_int *dep,float *ti,float *tf,cal_int *iter,STR_PSTR(nom),
            cal_int *max,cal_int *n, float *tab,cal_int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPLCP: %s %f %f \n",cnom, *ti,*tf);
  *err=cp_lcp_fort_((void *)*compo,*dep,ti,tf,iter,cnom,*max,n,tab);
  fprintf(stderr,"End of CPLCP: %s %f %f \n",cnom,*ti,*tf);
  free_str1(cnom);
}

void F_FUNC(cplch,CPLCH)(long *compo,cal_int *dep,float *ti,float *tf,cal_int *iter,STR_PSTR(nom),
            cal_int *max,cal_int *n, char *tab,cal_int *err STR_PLEN(nom) STR_PLEN(tab) )
{
  char **tabChaine = NULL;
  cal_int    index = 0;
  char*      cnom = fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPLCH: %s %f %f \n",cnom, *ti,*tf);

  tabChaine = (char **) malloc(sizeof(char *) * (*max));
  for (index = 0; index < *max; index++)
    tabChaine[index] = (char *) malloc(sizeof(char) * (STR_LEN(tab)+1));

  *err=cp_lch_fort_((void *)*compo,*dep,ti,tf,iter,cnom,*max,n,tabChaine,STR_LEN(tab));

  for (index = 0; index < *n; index++)
    strncpy(&tab[index * STR_LEN(tab)], tabChaine[index], strlen(tabChaine[index]));

  fprintf(stderr,"End of CPLCH: %s %f %f \n",cnom,*ti,*tf);
  if (tabChaine != (char **) NULL)  {
    for (index = 0; index < *n; index++)
      free(tabChaine[index]);
    free(tabChaine);
  }
  free_str1(cnom);
}


/***************************/
/*  INTERFACES D'ECRITURE  */
/***************************/

void F_FUNC(cpech,CPECH)(long *compo,cal_int *dep,float *ti,cal_int *iter,STR_PSTR(nom),cal_int *n, char *tab,cal_int *err 
			 STR_PLEN(nom) STR_PLEN(tab))
{
  char ** tabChaine=NULL;
  cal_int     index=0,index2=0;
  char*   cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPECH: %s %f \n",cnom, *ti);

  tabChaine = (char **) malloc(sizeof(char *) * *n);
  for (index = 0; index < *n; index++) {
    
    tabChaine[index] = (char *) malloc(sizeof(char) * (STR_LEN(tab) + 1));
    strncpy(tabChaine[index],&tab[STR_LEN(tab) * index],STR_LEN(tab));
    tabChaine[index][STR_LEN(tab)]='\0';
    for (index2 = STR_LEN(tab) - 1; index2 >= 0; index2--) {
      if ( tabChaine[index][index2] == ' '  ||
	   tabChaine[index][index2] == '\0'   )
	tabChaine[index][index2]='\0';
    }
  }

  *err=cp_ech_fort_((void *)*compo,*dep,*ti,*iter,cnom,*n,tabChaine,STR_LEN(tab) );

  fprintf(stderr,"End of CPECH: %s %f \n",cnom, *ti);
  if (tabChaine != (char **) NULL) {
    for (index = 0; index < *n; index++)
      free(tabChaine[index]);

    free(tabChaine);
  }
  free_str1(cnom);
}

void F_FUNC(cpedb,CPEDB)(long *compo,cal_int *dep,double *ti,cal_int *iter,STR_PSTR(nom),cal_int *n, double *tab,cal_int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPEDB: %s %f \n",cnom, *ti);
  *err=cp_edb_fort_((void *)*compo,*dep,*ti,*iter,cnom,*n,tab);
  fprintf(stderr,"End of CPEDB: %s %f \n",cnom, *ti);
  free_str1(cnom);
}

void F_FUNC(cpere,CPERE)(long *compo,cal_int *dep,float *ti,cal_int *iter,STR_PSTR(nom),cal_int *n, float *tab,cal_int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPERE: %s %f \n",cnom, *ti);
  *err=cp_ere_fort_((void *)*compo,*dep,*ti,*iter,cnom,*n,tab);
  fprintf(stderr,"End of CPERE: %s %f \n",cnom, *ti);
  free_str1(cnom);
}

void F_FUNC(cpecp,CPECP)(long *compo,cal_int *dep,float *ti,cal_int *iter,STR_PSTR(nom),cal_int *n, float *tab,cal_int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPECP: %s %f \n",cnom, *ti);
  *err=cp_ecp_fort_((void *)*compo,*dep,*ti,*iter,cnom,*n,tab);
  fprintf(stderr,"End of CPECP: %s %f \n",cnom, *ti);
  free_str1(cnom);
}


void F_FUNC(cpein,CPEIN)(long *compo,cal_int *dep,float *ti,cal_int *iter,STR_PSTR(nom),cal_int *n, int *tab,cal_int *err STR_PLEN(nom))
{

  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPEIN: %s %f %d\n",cnom, *ti,*iter);
#if   !SIZEOF_INT
#error "The macro SIZEOF_INT must be defined."    
#elif SIZEOF_INT == 4
  *err=cp_ein_fort_((void *)*compo,*dep,*ti,*iter,cnom,*n,tab);
#else
  fprintf(stderr,"CPEIN: %s %f %d : Can't use fortran INTEGER*4 because int C is not 32bits long on this machine.\n",
	  cnom, *ti,*iter);
#endif
  fprintf(stderr,"End of CPIN: %s %f \n",cnom,*ti);
  free_str1(cnom);
}

void F_FUNC(cpelg,CPELG)(long *compo,cal_int *dep,float *ti,cal_int *iter,STR_PSTR(nom),cal_int *n, long *tab,cal_int *err STR_PLEN(nom))
{

  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPELG: %s %f %d\n",cnom, *ti,*iter);
#if   !SIZEOF_LONG
#error "The macro SIZEOF_LONG must be defined."    
#elif SIZEOF_LONG == 8
  *err=cp_elg_fort_((void *)*compo,*dep,*ti,*iter,cnom,*n,tab);
#else
  fprintf(stderr,"CPELG: %s %f %d : Can't use fortran INTEGER*8 because long C is not 64bits long on this machine.\n",
	  cnom, *ti,*iter);
#endif
  fprintf(stderr,"End of CPELG: %s %f \n",cnom,*ti);
  free_str1(cnom);
}

void F_FUNC(cpeen,CPEEN)(long *compo,cal_int *dep,float *ti,cal_int *iter,STR_PSTR(nom),cal_int *n, cal_int *tab,cal_int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPEEN: %s %f %d\n",cnom, *ti,*iter);
    *err=cp_een_fort_((void *)*compo,*dep,*ti,*iter,cnom,*n,tab);
  fprintf(stderr,"End of CPEEN: %s %f \n",cnom,*ti);
  free_str1(cnom);
}

void F_FUNC(cpelo,CPELO)(long *compo,cal_int *dep,float *ti,cal_int *iter,STR_PSTR(nom),cal_int *n, int *tab,cal_int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPELO: %s %f %d\n",cnom, *ti,*iter);
  *err=cp_elo_fort_((void *)*compo,*dep,*ti,*iter,cnom,*n,tab);
  fprintf(stderr,"End of CPELO: %s %f \n",cnom,*ti);
  free_str1(cnom);
}


#ifdef __cplusplus
}
#endif

