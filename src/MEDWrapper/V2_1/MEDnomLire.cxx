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

#include <stdlib.h>
#include <string.h>

namespace med_2_1{

med_err
MEDnomLire(med_idt fid,char *maa, char *nom, med_int n, 
	   med_entite_maillage type_ent,med_geometrie_element type_geo)
{
  med_idt root, maaid, entid, geoid;
  med_err ret;
  char chemin[MED_TAILLE_MAA+MED_TAILLE_NOM+1];
  char nom_ent[MED_TAILLE_NOM_ENTITE+1];
  char nom_geo[MED_TAILLE_NOM_ENTITE+1];

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
    * Pour les mailles, les faces et le aretes,
    * on ouvre le Data Group du type geometrique
    */
   if ((type_ent==MED_MAILLE)||(type_ent==MED_FACE)||(type_ent==MED_ARETE))
     {
       if ((ret = _MEDnomGeometrie(nom_geo,type_geo)) < 0)
	 return -1;
       if ((geoid = _MEDdatagroupOuvrir(entid,nom_geo)) < 0)
	   return -1;
     }
   else
     geoid = -1;

   /*
    * lecture du Data Set "NOM" 
    */
   if (geoid == -1)
     root = entid;
   else
     root = geoid;
   if ((ret = _MEDdatasetStringLire(root,MED_NOM_NOM,nom)) < 0)
     return -1;

   /*
    * On ferme tout
    */
   if (geoid > 0)
     if ((ret = _MEDdatagroupFermer(geoid)) < 0)
       return -1;
   if ((ret = _MEDdatagroupFermer(entid)) < 0)
     return -1;
   if ((ret = _MEDdatagroupFermer(maaid)) < 0)
     return -1;

  return 0; 
}

}
