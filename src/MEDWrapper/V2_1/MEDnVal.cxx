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
#include <string.h>
#include <stdlib.h>
#include "med_outils.hxx"

namespace med_2_1{

med_int 
MEDnVal(med_idt fid, char *champ, med_entite_maillage type_ent, 
	med_geometrie_element type_geo,med_int numdt, med_int numo)
{
  med_int n;
  med_idt datagroup;
  med_err ret;
  char nomdatagroup1[2*MED_TAILLE_NOM_ENTITE+2],nomdatagroup2[2*MED_MAX_PARA+1];
  char tmp1   [MED_TAILLE_NOM_ENTITE+1];
  char chemin [MED_TAILLE_CHA+(MED_TAILLE_NOM+1)+(2*MED_TAILLE_NOM_ENTITE+2)+(2*MED_MAX_PARA)+1+100];

  /*
   * On inhibe le gestionnaire d'erreur HDF 
   */
  _MEDmodeErreurVerrouiller();

  /*
   * On cree le chemin d'accès
   */
  strcpy(chemin,MED_CHA);
  strcat(chemin,champ);
  strcat(chemin,"/");

  /* On cree le nom du datagroup de niveau 1 */
  if ((ret = _MEDnomEntite(nomdatagroup1,type_ent)) < 0)
    return -1;
  if ((type_ent != MED_NOEUD))
    {
      if ((ret = _MEDnomGeometrie(tmp1,type_geo)) < 0)
	return -1;
      strcat(nomdatagroup1,".");
      strcat(nomdatagroup1,tmp1);
    }
  strcat(chemin,nomdatagroup1);
  strcat(chemin,"/");

  /* Creation du datagroup de niveau 2 <numdt>.<numoo> */
  sprintf(nomdatagroup2,"%*li%*li",MED_MAX_PARA,(long ) numdt,MED_MAX_PARA,(long ) numo);
  strcat(chemin,nomdatagroup2);
 
 /*
   * Acces au champ
  */
  if ((datagroup = _MEDdatagroupOuvrir(fid,chemin)) < 0) 
    return 0;
  if ((ret = _MEDattrEntierLire(datagroup,MED_NOM_NBR,&n)) < 0)
    return -1;

  /*
   * fermetures 
   */
  if ((ret = _MEDdatagroupFermer(datagroup)) < 0)
    return -1;
 
  return n;
}

}
