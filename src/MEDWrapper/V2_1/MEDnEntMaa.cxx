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

namespace med_2_1{

med_int
MEDnEntMaa(med_idt fid, char *maa, med_table quoi, med_entite_maillage type_ent, 
	   med_geometrie_element type_geo, med_connectivite type_conn)
{
  med_idt root, maaid, entid,geoid, dataset=0;
  med_err ret;
  char chemin[MED_TAILLE_MAA+MED_TAILLE_NOM+1];
  char nom_ent[MED_TAILLE_NOM_ENTITE+1];
  char nom_geo[MED_TAILLE_NOM_ENTITE+1];
  char nom_dataset[MED_TAILLE_NOM_ENTITE+1];
  med_int res = 0;

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
    * Si le Data Group des entites n'existe pas => res = 0
    */
  entid = _MEDdatagroupOuvrir(maaid,nom_ent);

   /*
    * Pour les mailles, les faces et le aretes
    * si le Data Group du type geometrique n'existe pas => res = 0
    */
  if ((type_ent==MED_MAILLE)||(type_ent==MED_FACE)||(type_ent==MED_ARETE))
    {
      if ((ret = _MEDnomGeometrie(nom_geo,type_geo)) < 0)
	return -1;
      geoid = _MEDdatagroupOuvrir(entid,nom_geo);
    }
  else
    geoid = -1;
  
   /*
    * Ouverture du Data Set renvoye par _MEDnomDataset()
    * S'il n'existe pas => erreur
    * Sinon lecture de l'attribut NBR
    */
   if (geoid == -1)
     root = entid;
   else
     root = geoid;
   if ((ret = _MEDnomDataset(nom_dataset,quoi,type_conn)) < 0)
     return -1;
   dataset = _MEDdatasetOuvrir(root,nom_dataset);
   if (dataset > 0)
     if ((ret = _MEDattrEntierLire(dataset,MED_NOM_NBR,&res)) < 0)
       return -1;

   /*
    * On ferme tout
    */
   if (dataset > 0)
     if ((ret = _MEDdatasetFermer(dataset)) < 0)
       return -1;
   if (geoid > 0)
     if ((ret = _MEDdatagroupFermer(geoid)) < 0)
       return -1;
   if (entid > 0)
     if ((ret = _MEDdatagroupFermer(entid)) < 0)
       return -1;
   if ((ret = _MEDdatagroupFermer(maaid)) < 0)
     return -1; 

  return res;
}
     
}
