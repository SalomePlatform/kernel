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

med_err
MEDequivInfo(int fid, char *maa, int ind, char *eq, char *des)
{
  med_idt eqid;
  med_err ret;
  char chemin[MED_TAILLE_MAA+MED_TAILLE_EQS+2*MED_TAILLE_NOM+1];
  int num;
  int idx;

  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();

  /*
   * On recupere le nom de l'equivalence
   */
  num = ind - 1;
  strcpy(chemin,MED_MAA);
  strcat(chemin,maa);
  strcat(chemin,MED_EQS); 
  if ((idx = _MEDobjetIdentifier(fid,chemin,num,eq)) < 0)
    return -1;

  /* 
   * Si le Data Group eq n'existe pas => erreur
   */
  strcat(chemin,eq);
  if ((eqid = _MEDdatagroupOuvrir(fid,chemin)) < 0)
      return -1;

  /*
   * L'attribut "DES"
   */
  if ((ret = _MEDattrStringLire(eqid,MED_NOM_DES,MED_TAILLE_DESC,des)) < 0)
    return -1;

  /*
   * On ferme tout 
   */
  if ((ret = _MEDdatagroupFermer(eqid)) < 0)
    return -1;

  return 0;
}

}
