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
 * Nombre de Couple (PDT,NOR) pour le champ <cha>
 */

namespace med_2_1{

med_int 
MEDnPasdetemps(med_idt fid,char *cha,med_entite_maillage type_ent, 
	       med_geometrie_element type_geo)

{
  med_err ret;
  int n1;
  char nomdatagroup1[MED_TAILLE_NOM+1];
  char tmp1         [MED_TAILLE_NOM_ENTITE+1];
  char chemin       [MED_TAILLE_CHA+(MED_TAILLE_NOM+1)+MED_TAILLE_NOM+1];

  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();
  
  /* 
   * Creation du chemin d'accès aux différents (PDT,NOR) pour les différents <type_ent>[.<type_geo>]
   */
  strcpy(chemin,MED_CHA);
  strcat(chemin,cha);
  strcat(chemin,"/");
 
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

  n1 =0;
  _MEDnObjets(fid,chemin,&n1);
      
  return (med_int) n1;

}

}
