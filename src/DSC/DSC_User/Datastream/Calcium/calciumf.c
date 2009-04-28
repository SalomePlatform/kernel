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
#include <stdio.h>

static void* COMPO=0;

#ifdef __cplusplus
extern "C" {
#endif

static void fstrtocstr(char *cstr, char *fstr,int fstr_len) 
{
  int i,iend;
  for (iend = fstr_len-1; iend >= 0; iend--) 
    if (fstr[iend] != ' ') break;
  for (i = 0; i <= iend; i++)
    cstr[i] = fstr[i];
  cstr[i] = '\0';
}

static void cstrtofstr(char *cstr, char *fstr,int fstr_len) 
{
  int i, len;
  len = strlen(cstr);
  if (len > fstr_len) len = fstr_len;
  for (i = 0; i < len; i++)
    fstr[i] = cstr[i];
  while (i < fstr_len)
    fstr[i++] = ' ';
}

static char * fstr1(char *nom,int nnom)
{
  char * cnom=(char*)malloc((nnom+1)*sizeof(char));
  fstrtocstr(cnom,nom,nnom);
  return cnom;
}

static char * free_str1(char *nom)
{
  free(nom);
}

void F_FUNC(cpcd,CPCD)(long *compo,STR_PSTR(nom),int *info STR_PLEN(nom))
{
  /* nom is OUT argument */
  cp_cd((void *)*compo,STR_PTR(nom));
  /* replace in place ??? */
  cstrtofstr(STR_PTR(nom),STR_PTR(nom),STR_LEN(nom));
}

void F_FUNC(cplen,CPLEN)(long *compo,int *dep,float *ti,float *tf,int *iter,STR_PSTR(nom),
            int *max,int *n, int *tab,int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPLEN: %s %f %f\n",cnom,*ti,*tf);
  *err=cp_len((void *)*compo,*dep,ti,tf,iter,cnom,*max,n,tab);
  fprintf(stderr,"End of CPLEN: %s \n",cnom);
  free_str1(cnom);
}

void F_FUNC(cpllo,CPLLO)(long *compo,int *dep,float *ti,float *tf,int *iter,STR_PSTR(nom),
            int *max,int *n, int *tab,int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPLLO: %s %f %f\n",cnom,*ti,*tf);
  *err=cp_llo((void *)*compo,*dep,ti,tf,iter,cnom,*max,n,tab);
  fprintf(stderr,"End of CPLLO: %s \n",cnom);
  free_str1(cnom);
}

void F_FUNC(cpldb,CPLDB)(long *compo,int *dep,double *ti,double *tf,int *iter,STR_PSTR(nom),
            int *max,int *n, double *tab,int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPLDB: %s %f %f \n",cnom, *ti,*tf);
  *err=cp_ldb((void *)*compo,*dep,ti,tf,iter,cnom,*max,n,tab);
  fprintf(stderr,"End of CPLDB: %s %f %f \n",cnom,*ti,*tf);
  free_str1(cnom);
}

void F_FUNC(cplre,CPLRE)(long *compo,int *dep,float *ti,float *tf,int *iter,STR_PSTR(nom),
            int *max,int *n, float *tab,int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPLRE: %s %f %f \n",cnom, *ti,*tf);
  *err=cp_lre((void *)*compo,*dep,ti,tf,iter,cnom,*max,n,tab);
  fprintf(stderr,"End of CPLRE: %s %f %f \n",cnom,*ti,*tf);
  free_str1(cnom);
}

void F_FUNC(cplcp,CPLCP)(long *compo,int *dep,float *ti,float *tf,int *iter,STR_PSTR(nom),
            int *max,int *n, float *tab,int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPLCP: %s %f %f \n",cnom, *ti,*tf);
  *err=cp_lcp((void *)*compo,*dep,ti,tf,iter,cnom,*max,n,tab);
  fprintf(stderr,"End of CPLCP: %s %f %f \n",cnom,*ti,*tf);
  free_str1(cnom);
}

void F_FUNC(cplch,CPLCH)(long *compo,int *dep,float *ti,float *tf,int *iter,STR_PSTR(nom),
            int *max,int *n, char *tab,int *err STR_PLEN(nom) STR_PLEN(tab) )
{
  char **tabChaine=NULL;
  int    index=0;
  char*  cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPLCH: %s %f %f \n",cnom, *ti,*tf);

  tabChaine = (char **) malloc(sizeof(char *) * (*max));
  for (index = 0; index < *max; index++)
    tabChaine[index] = (char *) malloc(sizeof(char) * (STR_LEN(tab)+1));

  *err=cp_lch((void *)*compo,*dep,ti,tf,iter,cnom,*max,n,tabChaine,STR_LEN(tab));

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

void F_FUNC(cpech,CPECH)(long *compo,int *dep,float *ti,int *iter,STR_PSTR(nom),int *n, char *tab,int *err 
			 STR_PLEN(nom) STR_PLEN(tab))
{
  char ** tabChaine=NULL;
  int     index=0,index2=0;
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

  *err=cp_ech((void *)*compo,*dep,*ti,*iter,cnom,*n,tabChaine,STR_LEN(tab) );

  fprintf(stderr,"End of CPECH: %s %f \n",cnom, *ti);
  if (tabChaine != (char **) NULL) {
    for (index = 0; index < *n; index++)
      free(tabChaine[index]);

    free(tabChaine);
  }
  free_str1(cnom);
}

void F_FUNC(cpedb,CPEDB)(long *compo,int *dep,double *ti,int *iter,STR_PSTR(nom),int *n, double *tab,int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPEDB: %s %f \n",cnom, *ti);
  *err=cp_edb((void *)*compo,*dep,*ti,*iter,cnom,*n,tab);
  fprintf(stderr,"End of CPEDB: %s %f \n",cnom, *ti);
  free_str1(cnom);
}

void F_FUNC(cpere,CPERE)(long *compo,int *dep,float *ti,int *iter,STR_PSTR(nom),int *n, float *tab,int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPERE: %s %f \n",cnom, *ti);
  *err=cp_ere((void *)*compo,*dep,*ti,*iter,cnom,*n,tab);
  fprintf(stderr,"End of CPERE: %s %f \n",cnom, *ti);
  free_str1(cnom);
}

void F_FUNC(cpecp,CPECP)(long *compo,int *dep,float *ti,int *iter,STR_PSTR(nom),int *n, float *tab,int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPECP: %s %f \n",cnom, *ti);
  *err=cp_ecp((void *)*compo,*dep,*ti,*iter,cnom,*n,tab);
  fprintf(stderr,"End of CPECP: %s %f \n",cnom, *ti);
  free_str1(cnom);
}

void F_FUNC(cpeen,CPEEN)(long *compo,int *dep,float *ti,int *iter,STR_PSTR(nom),int *n, int *tab,int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPEEN: %s %f %d\n",cnom, *ti,*iter);
  *err=cp_een((void *)*compo,*dep,*ti,*iter,cnom,*n,tab);
  fprintf(stderr,"End of CPEEN: %s %f \n",cnom,*ti);
  free_str1(cnom);
}

void F_FUNC(cpelo,CPELO)(long *compo,int *dep,float *ti,int *iter,STR_PSTR(nom),int *n, int *tab,int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPELO: %s %f %d\n",cnom, *ti,*iter);
  *err=cp_elo((void *)*compo,*dep,*ti,*iter,cnom,*n,tab);
  fprintf(stderr,"End of CPELO: %s %f \n",cnom,*ti);
  free_str1(cnom);
}

void F_FUNC(cpfin,CPFIN)(long *compo,int *dep,int *err)
{
  fprintf(stderr,"CPFIN: \n");
  *err=cp_fin((void *)*compo,*dep);
}

#ifdef __cplusplus
}
#endif

