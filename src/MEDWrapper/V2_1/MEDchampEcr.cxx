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

/*
 * - Nom de la fonction : MEDchampEcr
 * - Description : ecriture d'un Champ Résultat
 * - Parametres :
 *     - fid      (IN)  : ID du fichier HDF courant
 *     - maa      (IN)  : le nom du maillage sur lequel s'applique le champ 
 *     - cha      (IN)  : le nom du champ 
 *     - val      (IN)  : valeurs du champ à stocker
 *     - interlace(IN)  : entrelacement utilisé en mémoire {MED_FULL_INTERLACE,MED_NO_INTERLACE} 
 *     - nbelem   (IN)  : nombre d'éléments (prend en compte le nbre 
 *                         de points de Gauss (c'est demandé à l'utilisateur ds la doc) mais pas le nbre de composantes)
 *     - ngauss   (IN)  : nbre de point de gauss utilisé (MED_NOPG si aucun)
 *     - numco    (IN)  : n° de la composante à stocker (MED_ALL si toutes)
 *     - profil   (IN)  : nom du profil utilisé (MED_NOPFL si inutilisé)
 *     - mode     (IN)       : mode d'ecriture MED (MED_ECRI | MED_REMP)
 *     - type_ent (IN)  : entité concerné par le champ {MED_NOEUD,MED_ARETE,MED_FACE,MED_MAILLE}
 *     - type_geo (IN)  : type géométrique de l'entité concerné {MED_POINT,MED_SEG2 ......}
 *     - numdt    (IN)  : n° du pas de temps (MED_NOPDT si aucun)
 *     - dt_unit  (IN)  : chaine de taille MED_NOMP indiquant l'unité du champ
 *     - dt       (IN)  : valeur du pas de temps 
 *     - numo     (IN)  : n° d'ordre utilisé MED_NONOR si inutile
 * - Resultat : 0 en cas de succes, -1 sinon
 */ 

namespace med_2_1{

med_err 
MEDchampEcr(med_idt fid, char *maa, char *cha,unsigned char *val,med_mode_switch interlace,med_int nbelem,med_int ngauss, 
	    med_int numco, char * profil, med_mode_acces mode, med_entite_maillage type_ent, 
	    med_geometrie_element type_geo, med_int numdt,char * dt_unit, med_float dt, med_int numo)
{
  med_err ret;
  med_idt chid,datagroup1,datagroup2;
  med_int ncomp, chtype, i, locnumdt,pfluse;
  char pflname [MED_TAILLE_NOM+1];
  char maillage[MED_TAILLE_NOM+1];
  char nomdatagroup1[2*MED_TAILLE_NOM_ENTITE+2],nomdatagroup2[2*MED_MAX_PARA+1];
  char tmp1[MED_TAILLE_NOM_ENTITE+1];
  med_size dimd[1],psize;
  med_int   *pfltabtmp=0;
  med_ssize *pfltab=0;
  char chemin[MED_TAILLE_CHA+MED_TAILLE_NOM+1];
  
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
   *  Creation du datagroup de niveau 1 <type_ent>[.<type_geo>] 
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
  if ( (datagroup1 = _MEDdatagroupOuvrir(chid,nomdatagroup1)) < 0) 
    if ((datagroup1 = _MEDdatagroupCreer(chid,nomdatagroup1)) < 0)
      return -1;
  
  /* Creation du datagroup de niveau 2 <numdt>.<numoo> */
  if ( numdt == MED_NOPDT) locnumdt = MED_NOPDT; else locnumdt = numdt;
  sprintf(nomdatagroup2,"%*li%*li",MED_MAX_PARA,(long ) locnumdt,MED_MAX_PARA,(long ) numo);

  
  /*Cree ou ouvre (en mode MED_REMP) le datagroup nomdatagroup2 */
  datagroup2 = 0;   
  if (((datagroup2 = _MEDdatagroupOuvrir(datagroup1,nomdatagroup2)) > 0)    
      && (mode != MED_REMP))   
    return -1;   
  else   
    if (datagroup2 < 0)
      if ((datagroup2 = _MEDdatagroupCreer(datagroup1,nomdatagroup2)) < 0)   
	return -1;   
  
  /*Cree ou ouvre (en mode MED_REMP) l'attribut MED_NOM_NDT */
  if ((ret = _MEDattrEntierEcrire(datagroup2,MED_NOM_NDT,&numdt,mode)) < 0)
    return -1;
  
  /*Cree ou ouvre (en mode MED_REMP) l'attribut MED_NOM_PDT */
  if ((ret = _MEDattrFloatEcrire(datagroup2,MED_NOM_PDT,&dt,mode)) < 0)
    return -1;
    
  /*Cree ou ouvre (en mode MED_REMP) l'attribut MED_NOM_NOR */
  if ((ret = _MEDattrEntierEcrire(datagroup2,MED_NOM_NOR,&numo,mode)) < 0)
    return -1;
  
  /*Cree ou ouvre (en mode MED_REMP) l'attribut MED_NOM_NBR */
  if ((ret = _MEDattrEntierEcrire(datagroup2,MED_NOM_NBR,&nbelem,mode)) < 0)
    return -1;

  /*Cree ou ouvre (en mode MED_REMP) l'attribut MED_MAA   */
  /* rem : Aucune verification de l'existence du maillage */
  strncpy(maillage,maa,MED_TAILLE_NOM);
  maillage[MED_TAILLE_NOM]='\0';
  if ((ret = _MEDattrStringEcrire(datagroup2,MED_NOM_MAI,MED_TAILLE_NOM,maillage,mode)) < 0)
    return -1;
  
  /*Cree ou ouvre (en mode MED_REMP) l'attribut MED_NOM_PFL   */ 
  pfluse = 0;
  if ( strlen(profil) == 0)   /* idem MED_NOPFL*/
    strncpy(pflname,MED_NOPFLi,MED_TAILLE_NOM+1);
  else {
    strncpy(pflname,profil,MED_TAILLE_NOM);
    pflname[MED_TAILLE_NOM]='\0';
    pfluse = 1;
  }
  if ((ret = _MEDattrStringEcrire(datagroup2,MED_NOM_PFL,MED_TAILLE_NOM,pflname,mode)) < 0)
    return -1; 

  /*Cree ou ouvre (en mode MED_REMP) l'attribut  MED_NOM_UNI */ 
  if ( strlen(dt_unit) == 0) {
    if ((ret = _MEDattrStringEcrire(datagroup2,MED_NOM_UNI,MED_TAILLE_PNOM,"        ",mode)) < 0)
      return -1;
  } else
    if ((ret = _MEDattrStringEcrire(datagroup2,MED_NOM_UNI,MED_TAILLE_PNOM,dt_unit,mode)) < 0)
      return -1;
  

  /* Cree ou ouvre (en mode MED_REMP) l'attribut MED_NOM_NGAU             */ 
  /* Ecriture du nombre de pts de gauss propre au <type_ent>[.<type_geo>] */
  /* On n'utilise pas ngauss=MED_NOPG mais ngauss=1 si aucun pt de gauss  */
  if ((ret = _MEDattrEntierEcrire(datagroup2,MED_NOM_NGA,&ngauss,mode)) < 0)
    return -1;

  /*Lecture de l'attribut MED_NOM_NCO */
  if ((ret = _MEDattrEntierLire(chid,MED_NOM_NCO,&ncomp)) < 0)
    return -1;
 
  /*Determination de la taille dimd[0] du dataset à stocker*/
  dimd[0] = nbelem*ncomp;

  /* Gestion des profils*/
  if ( pfluse ) {
   
    if ( ( i = MEDnValProfil(fid,pflname) ) < 0 )
      return -1;
    else
      psize = i;
    
    pfltabtmp = (med_int *)   malloc (sizeof(med_int)*psize);
    pfltab    = (med_ssize *) malloc (sizeof(med_ssize)*psize);
    if ((ret = MEDprofilLire(fid,pfltabtmp,pflname)) < 0)
      return -1;
    for (i=0;i<psize;i++)
      pfltab[i] = (med_ssize) pfltabtmp[i];
    
  }
  else
    psize = MED_NOPF;
  
  
  /*
   * Ecriture du champ
   */
  if ((ret = _MEDattrEntierLire(chid,MED_NOM_TYP,&chtype)) < 0)
    return -1;
  switch(chtype)
    {
    case MED_REEL64 :
      if ((ret =  _MEDdatasetNumEcrire(datagroup2,MED_NOM_CO,MED_REEL64,interlace,ncomp,numco,psize,pfltab,ngauss,dimd,val,
				       mode)) < 0)
	return -1;
      break;

    case MED_INT32 :
#if defined(IRIX64)||defined(OSF1) 
     if ((ret =  _MEDdatasetNumEcrire(datagroup2,MED_NOM_CO,MED_INT64,interlace,ncomp,numco,psize,pfltab,ngauss,dimd,val,
				       mode)) < 0)
	return -1;
#else
      if ((ret =  _MEDdatasetNumEcrire(datagroup2,MED_NOM_CO,MED_INT32,interlace,ncomp,numco,psize,pfltab,ngauss,dimd,val,
				       mode)) < 0)
	return -1;
#endif
      break;

    case MED_INT64 :
#if defined(IRIX64)||defined(OSF1) 
     if ((ret =  _MEDdatasetNumEcrire(datagroup2,MED_NOM_CO,MED_INT64,interlace,ncomp,numco,psize,pfltab,ngauss,dimd,val,
				       mode)) < 0)
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
