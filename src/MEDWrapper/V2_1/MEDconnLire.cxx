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
MEDconnLire(med_idt fid,char *maa,med_int mdim,med_int *connectivite,med_mode_switch mode_switch,
	    med_int * pfltabtmp, med_size psizetmp,
	    med_entite_maillage type_ent, med_geometrie_element type_geo,med_connectivite type_conn)
{
  med_idt maaid,entid,geoid;
  med_err ret;
  char chemin[MED_TAILLE_MAA+MED_TAILLE_NOM+1];
  char nom_ent[MED_TAILLE_NOM_ENTITE+1];
  char nom_geo[MED_TAILLE_NOM_ENTITE+1];
  char nom_dataset[MED_TAILLE_NOM_ENTITE+1];
  med_ssize * pfltab;
  med_size    psize;
  int dim,nnoe,ndes;
  int nsup = 0;
  int taille;  
  int i,j;
  
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
    * Si le Data Group des entites n'existe pas => erreur
    */
   if ((entid = _MEDdatagroupOuvrir(maaid,nom_ent)) < 0)
     return -1;

   /*
    * si le Data Group du type geometrique n'existe pas => erreur
    */
   if ((ret = _MEDnomGeometrie(nom_geo,type_geo)) < 0)
     return -1;
   if ((geoid = _MEDdatagroupOuvrir(entid,nom_geo)) < 0)
     return -1;

   /*
    * Si le Data Set de la connectivite n'existe pas => erreur
    * Si oui => on le lit
    */
   if ((ret=_MEDparametresGeometrie(type_ent,type_geo,&dim,&nnoe,&ndes))<0)
     return -1;
   if (mdim == 2 || mdim == 3)
     if (type_ent == MED_MAILLE && dim == 1)
       nsup = 1;
   if (mdim == 3)
     if (type_ent == MED_MAILLE && dim == 2)
       nsup = 1;


   psize = psizetmp;
   switch(type_conn)
     {
     case MED_NOD :
       strcpy(nom_dataset,MED_NOM_NOD);
       taille = nsup + nnoe;
       break;

     case MED_DESC :
       strcpy(nom_dataset,MED_NOM_DES);
       taille = nsup + ndes;
       if ( psizetmp != MED_NOPF ) {  
	 psize = psizetmp;
	 pfltab = (med_ssize *) malloc (sizeof(med_ssize)*psize);
	 for (i=0;i<psizetmp;i++)
	   pfltab[i] = (med_ssize) (pfltabtmp[i]);
       };
       
       break;
       
     default :
       return -1;
     }


#if defined(IRIX64)||defined(OSF1)
   if ((ret = _MEDdatasetNumLire(geoid,nom_dataset,MED_INT64,
				 mode_switch,(med_size)taille,MED_ALL,
				 psize,pfltab,MED_NOPG,
				 (unsigned char*) connectivite)) < 0)
     return -1;
#else
   if ((ret = _MEDdatasetNumLire(geoid,nom_dataset,MED_INT32,
				 mode_switch,(med_size) taille,MED_ALL,
				 psize,pfltab,MED_NOPG,
				 (unsigned char*) connectivite)) < 0)
     return -1;
#endif 

   /*
    * On ferme tout 
    */

   if ( (psize != MED_NOPF) && (type_conn == MED_DESC) ) free(pfltab);
  
   if ((ret = _MEDdatagroupFermer(geoid)) < 0)
     return -1;
   if ((ret = _MEDdatagroupFermer(entid)) < 0)
     return -1;
   if ((ret = _MEDdatagroupFermer(maaid)) < 0)
     return -1;

   return 0; 
}

}
