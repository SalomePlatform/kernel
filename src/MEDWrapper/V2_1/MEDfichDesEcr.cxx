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

namespace med_2_1{

med_err 
MEDfichDesEcr(med_idt fid, char *des, med_mode_acces mode)
{ 
  med_idt attr; 
  med_idt root;
  med_err ret;
  char nom[] = MED_NOM_DESCRIPTEUR;
  char chemin[MED_TAILLE_MAA+1];

  /*
   * On inhibe le gestionnaire d'erreur
   */
  _MEDmodeErreurVerrouiller();

  /*
   * On ouvre le Data Group racine
   * s'il n'existe pas on le cree
   */
  strncpy(chemin,MED_MAA,MED_TAILLE_MAA-1);
  chemin[MED_TAILLE_MAA-1] = '\0';
  if ((root = _MEDdatagroupOuvrir(fid,chemin)) < 0)
    if ((root = _MEDdatagroupCreer(fid,chemin)) < 0)
      return -1;

  /*
   * On regarde si l'attribut existe
   * Si oui on le met a jour en fonction
   * du mode d'ouverture, sinon on le cree
   */

  if ((ret = _MEDattrStringEcrire(root,nom,MED_TAILLE_DESC,des,mode)) < 0)
    return -1;

  /*
   * Fermetures 
   */

  if ((ret = _MEDdatagroupFermer(root)) < 0)
    return -1;
	  
  return 0; 
}

}
