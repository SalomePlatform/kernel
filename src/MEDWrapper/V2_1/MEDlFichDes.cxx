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

med_int
MEDlFichDes(med_idt fid)
{
  med_idt attr, root;
  med_err ret=0;
  char des[MED_TAILLE_DESC+1];
  med_int longueur=0;
  char nom[MED_TAILLE_NOM+1];
  char chemin[MED_TAILLE_MAA+1];

  /*
   * On inhibe le gestionnaire d'erreur HDF
   */
  _MEDmodeErreurVerrouiller();

  /*
   * On ouvre le Data Group racine
   */
  strncpy(chemin,MED_MAA,MED_TAILLE_MAA-1);
  chemin[MED_TAILLE_MAA-1] = '\0';
  if ((root = _MEDdatagroupOuvrir(fid,chemin)) < 0)
    return -1;

  /*
   * On regarde si l'attribut existe
   * Si non => erreur
   * Si oui => on retourne sa longueur
   */
  strcpy(nom,MED_NOM_DESCRIPTEUR);
 
  if ((attr = _MEDattrOuvrir(root,nom)) < 0) {
    _MEDdatagroupFermer(root);
    longueur=0;
    return 0;
  }
 
  if ((ret = _MEDattrFermer(attr)) < 0) {
    _MEDdatagroupFermer(root);
    return -1;
  }
  
  if ((ret = _MEDattrStringLire(root,nom,MED_TAILLE_DESC,des)) < 0) {
    _MEDdatagroupFermer(root);
    return -1;
  }
  
  longueur = strlen(des);

  /*
   * fermetures 
   */
  if ((ret == _MEDdatagroupFermer(root)) < 0)
    return -1;

  return longueur;
}

}
