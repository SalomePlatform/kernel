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

extern int mode_interlace; 

namespace med_2_1{

med_err 
MEDconnEcr(med_idt fid,char *maa, med_int mdim, med_int *connectivite,med_mode_switch mode_switch,
	   med_int nbre,med_mode_acces mode,med_entite_maillage type_ent,
	   med_geometrie_element type_geo,med_connectivite type_conn)
{
  med_idt maaid, entid, geoid, dataset;
  med_err ret;
  med_size dimd[1];
  char chemin[MED_TAILLE_MAA+MED_TAILLE_NOM+1];
  char nom_ent[MED_TAILLE_NOM_ENTITE+1];
  char nom_geo[MED_TAILLE_NOM_ENTITE+1];
  char nom_dataset[MED_TAILLE_NOM_ENTITE+1];
  int dim, nnoe, ndes;
  int nsup = 0;
  int taille;
  
  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();

  /*
   * Si le maillage n'existe pas => erreur
   */
  strcpy(chemin,MED_MAA);
  strcat(chemin,maa);
  if ((maaid = _MEDdatagroupOuvrir(fid,chemin)) < 0)
      return -1;

  /*
   * On met a jour le nom du Data Group representant
   * le type des entites
   */
   if ((ret = _MEDnomEntite(nom_ent,type_ent)) < 0)
     return -1;
   /*
    * Si le Data Group des entites n'existe pas on le cree
    */
   /*EF Gerer le mode */
   if ((entid = _MEDdatagroupOuvrir(maaid,nom_ent)) < 0)
     if ((entid = _MEDdatagroupCreer(maaid,nom_ent)) < 0)
       return -1;

   /*
    * On cree s'il n'existe pas le Data Group du type geometrique
    */
   /*EF Gerer le mode */
  if ((ret = _MEDnomGeometrie(nom_geo,type_geo)) < 0)
     return -1;
   if ((geoid = _MEDdatagroupOuvrir(entid,nom_geo)) < 0)
     if ((geoid = _MEDdatagroupCreer(entid,nom_geo)) < 0)
       return -1;

   /*
    * On regarde si le Data Set existe et on le cree sinon
    */
   if ((ret=_MEDparametresGeometrie(type_ent,type_geo,&dim,&nnoe,&ndes))<0)
     return -1;
   if (mdim == 2 || mdim == 3)
     if (type_ent == MED_MAILLE && dim == 1)
       nsup = 1;
   if (mdim == 3)
     if (type_ent == MED_MAILLE && dim == 2)
       nsup = 1;
   switch(type_conn)
     {
     case MED_NOD :
       strcpy(nom_dataset,MED_NOM_NOD);
       taille = nsup + nnoe;
       break;

     case MED_DESC :
       strcpy(nom_dataset,MED_NOM_DES);
       taille = nsup + ndes;
       break;
       
     default :
       return -1;
     }
   dimd[0] = nbre*taille;
#if defined(IRIX64)||defined(OSF1)
   if ((ret = _MEDdatasetNumEcrire(geoid,nom_dataset,MED_INT64,mode_switch,(med_size)taille,MED_ALL,MED_NOPF,0,MED_NOPG,dimd,
				    (unsigned char*) connectivite,mode)) < 0)
     return -1;
#else
   if ((ret = _MEDdatasetNumEcrire(geoid,nom_dataset,MED_INT32,mode_switch,(med_size)taille,MED_ALL,MED_NOPF,0,MED_NOPG,dimd,
				    (unsigned char*) connectivite,mode)) < 0)
     return -1;
#endif

  /*
   * Attribut NBR (nombre de noeuds ou d'elements)
   */
   if ((dataset = _MEDdatasetOuvrir(geoid,nom_dataset)) < 0)
     return -1;
   if ((ret = _MEDattrEntierEcrire(dataset,MED_NOM_NBR,&nbre,mode)) < 0)
     return -1;

   /*
    * On ferme tout 
    */
   if ((ret = _MEDdatasetFermer(dataset)) < 0)
     return -1;
   if ((ret = _MEDdatagroupFermer(geoid)) < 0)
     return -1;
   if ((ret = _MEDdatagroupFermer(entid)) < 0)
     return -1;
   if ((ret = _MEDdatagroupFermer(maaid)) < 0)
     return -1;

  return 0; 
}

}
