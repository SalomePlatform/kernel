#include <string.h>
#include <stdlib.h>
#include <fortoc.h>
#include <calcium.h>
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

void F_FUNC(cpeen,CPEEN)(long *compo,int *dep,float *ti,int *iter,STR_PSTR(nom),int *n, int *tab,int *err STR_PLEN(nom))
{
  char* cnom=fstr1(STR_PTR(nom),STR_LEN(nom));
  fprintf(stderr,"CPEEN: %s %f %d\n",cnom, *ti,*iter);
  *err=cp_een((void *)*compo,*dep,*ti,*iter,cnom,*n,tab);
  fprintf(stderr,"End of CPEEN: %s %f \n",cnom,*ti);
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

