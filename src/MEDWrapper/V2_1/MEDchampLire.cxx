/*************************************************************************
* COPYRIGHT (C) 1999 - 2002  EDF R&D
* THIS LIBRARY IS FREE SOFTWARE; YOU CAN REDISTRIBUTE IT AND/OR MODIFY
* IT UNDER THE TERMS OF THE GNU LESSER GENERAL PUBLIC LICENSE 
* AS PUBLISHED BY THE FREE SOFTWARE FOUNDATION; 
* EITHER VERSION 2.1 OF THE LICENSE, OR (AT YOUR OPTION) ANY LATER VERSION.
*  
* THIS LIBRARY IS DISTRIBUTED IN THE HOPE THAT IT WILL BE USEFUL, BUT
* WITHOUT ANY WARRANTY; WITHOUT EVEN THE IMPLIED WARRANTY OF
* MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. SEE THE GNU
* LESSER GENERAL PUBLIC LICENSE FOR MORE DETAILS.
*
* YOU SHOULD HAVE RECEIVED A COPY OF THE GNU LESSER GENERAL PUBLIC LICENSE
* ALONG WITH THIS LIBRARY; IF NOT, WRITE TO THE FREE SOFTWARE FOUNDATION,
* INC., 59 TEMPLE PLACE, SUITE 330, BOSTON, MA 02111-1307 USA
*
*************************************************************************/

#include "med.hxx"
#include "med_outils.hxx"

#include <string.h>
#include <stdlib.h>

 /*La taille de val allouée par l'utilisateur doit prendre en compte le nbre de points de gauss et le nbre de composantes*/

namespace med_2_1{

med_err 
MEDchampLire(med_idt fid,char *maa, char *cha, unsigned char *val,med_mode_switch interlace,med_int numco,
	     char *profil,med_entite_maillage type_ent, med_geometrie_element type_geo,
	     med_int numdt, med_int numo)
     /* VERIFIER LA POSSIBILITE DE RELIRE L'UNITE DE PAS DE TEMPS (DS CHAMPINFO) */
{
  med_err ret;
  med_idt chid, datagroup1, datagroup2;
  med_int ncomp, chtype, ngauss, i, locnumdt, pfluse;
  char nomdatagroup1[2*MED_TAILLE_NOM_ENTITE+2],nomdatagroup2[2*MED_MAX_PARA+1];
  char tmp1[MED_TAILLE_NOM_ENTITE+1], pfltmp[MED_TAILLE_NOM+1];
  char chemin[MED_TAILLE_CHA+MED_TAILLE_NOM+1];
  med_size   psize;
  med_int   *pfltabtmp=0;
  med_ssize *pfltab=0;
  char maatmp[MED_TAILLE_NOM+1];
  
  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();

  /* 
   * Si le Data Group cha n'existe pas => erreur
   */
  strcpy(chemin,MED_CHA);
  strcat(chemin,cha);
  if ((chid = _MEDdatagroupOuvrir(fid,chemin)) < 0)
    return -1;

 /* 
   * Si le Data Group  de niveau 1 <type_ent>[.<type_geo>] n'existe pas => erreur
   */
  if ((ret = _MEDnomEntite(nomdatagroup1,type_ent)) < 0)
    return -1;
  if ((type_ent != MED_NOEUD))
    {
      if ((ret = _MEDnomGeometrie(tmp1,type_geo)) < 0)
	return -1;
      strcat(nomdatagroup1,".");
      strcat(nomdatagroup1,tmp1);
    }
  datagroup1 = 0;
  if ( (datagroup1 = _MEDdatagroupOuvrir(chid,nomdatagroup1)) < 0 )
    return -1;

  /*
   * Si le Data Group de niveau 2 <numdtt>.<numoo> n'existe pas => erreur
   */
  if ( numdt == MED_NOPDT) locnumdt = MED_NOPDT; else locnumdt = numdt;
  sprintf(nomdatagroup2,"%*li%*li",MED_MAX_PARA,(long ) locnumdt,MED_MAX_PARA,(long ) numo);
  
  datagroup2 = 0;   
  if ( (datagroup2 = _MEDdatagroupOuvrir(datagroup1,nomdatagroup2)) < 0)    
    return -1;   

  /* Lecture du nbre de composantes du champ */

  if ((ret = _MEDattrEntierLire(chid,MED_NOM_NCO,&ncomp)) < 0)
    return -1;
  
  /* Gestion des profils*/

  /*
   * Lire le profil demandé
   */

  if ((ret = _MEDattrStringLire(datagroup2,MED_NOM_PFL,MED_TAILLE_NOM,pfltmp)) < 0)
    return -1;
  
  if ( pfluse = (strcmp(pfltmp,MED_NOPFLi) && strcmp(pfltmp,"")) ) /* le test "" pour des raisons de compatibilité */
    {
      strcpy(profil,pfltmp);
      if ( (i = MEDnValProfil(fid,profil)) < 0 )
	return -1;
      else
	psize = i;

      pfltabtmp = (med_int *)   malloc (sizeof(med_int)*psize);
      pfltab = (med_ssize *) malloc (sizeof(med_ssize)*psize);
      if ((ret = MEDprofilLire(fid,pfltabtmp,profil)) < 0)
	return -1;
      for (i=0;i<psize;i++)
	pfltab[i] = (med_ssize) pfltabtmp[i];
       
    }
  else {
    psize = MED_NOPF;
    strcpy(profil,MED_NOPFL);
  }
  
  /*
   * Lire le nom de maillage associé au champ
   */
  if ((ret = _MEDattrStringLire(datagroup2,MED_NOM_MAI,MED_TAILLE_NOM,maatmp)) < 0)
    return -1;

  if (strcmp(maa,maatmp)) 
    return -1;
  
  /* Lit le nbre de points de Gauss  */ 
  /* (attribut MED_NOM_NGAU) propre au <type_ent>[.<type_geo>] pour simplifier la relecture */
  if ( (ret = _MEDattrEntierLire(datagroup2,MED_NOM_NGA,&ngauss)) < 0 )
    return -1;

  /*
   * Lecture du champ
   */

  if ((ret = _MEDattrEntierLire(chid,MED_NOM_TYP,&chtype)) < 0)
    return -1;

  switch(chtype)
    {
    case MED_REEL64 :
      if ((ret =  _MEDdatasetNumLire(datagroup2,MED_NOM_CO,MED_REEL64,
				     interlace,ncomp,numco,
				     psize,pfltab,ngauss,val))< 0)
	return -1;
      break;

    case MED_INT32 :
#if defined(IRIX64)||defined(OSF1) 
     if ((ret =  _MEDdatasetNumLire(datagroup2,MED_NOM_CO,MED_INT64,
				     interlace,ncomp,numco,
				     psize,pfltab,ngauss,val))< 0)
	return -1;
#else
     if ((ret =  _MEDdatasetNumLire(datagroup2,MED_NOM_CO,MED_INT32,
				     interlace,ncomp,numco,
				     psize, pfltab,ngauss,val))< 0)
	return -1;
#endif
     break;

    case MED_INT64 :
#if defined(IRIX64)||defined(OSF1) 
     if ((ret =  _MEDdatasetNumLire(datagroup2,MED_NOM_CO,MED_INT64,
				     interlace,ncomp,numco,
				     psize,pfltab,ngauss,val))< 0)
	return -1;
#else
     return -1;
#endif
      break;      

    default :
      return -1;
    }

  /*
   * On ferme tout 
   */
  if ( pfluse ) { free(pfltab); free(pfltabtmp);}

  if ((ret = _MEDdatagroupFermer(datagroup2)) < 0)
    return -1;
  if ((ret = _MEDdatagroupFermer(datagroup1)) < 0)
    return -1;
  if ((ret = _MEDdatagroupFermer(chid)) < 0)
    return -1;
    
  return 0;     
}

}
