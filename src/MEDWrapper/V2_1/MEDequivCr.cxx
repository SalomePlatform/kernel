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
MEDequivCr(med_idt fid,char *maa, char *eq, char *desc)
{
  med_idt root,eqid;
  med_err ret;
  char chemin[MED_TAILLE_MAA+MED_TAILLE_EQS+MED_TAILLE_NOM+1];
  char tmp[MED_TAILLE_EQS+1];

  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();

  /* 
   * Si le Data Group "EQS" n'existe pas, on le cree
   */
  strcpy(chemin,MED_MAA);
  strcat(chemin,maa);
  strncpy(tmp,MED_EQS,MED_TAILLE_EQS-1);
  tmp[MED_TAILLE_EQS-1] = '\0';
  strcat(chemin,tmp);
  if ((root = _MEDdatagroupOuvrir(fid,chemin)) < 0)
    if ((root = _MEDdatagroupCreer(fid,chemin)) < 0)
      return -1;

  /*
   * Si une equivalence du meme nom existe => erreur
   * Sinon on la cree
   */
  if ((eqid = _MEDdatagroupOuvrir(root,eq)) >= 0)
    return -1;
  if ((eqid = _MEDdatagroupCreer(root,eq)) < 0)
    return -1;

  /*
   * L'attribut "DES"
   */
  if ((ret = _MEDattrStringEcrire(eqid,MED_NOM_DES,MED_TAILLE_DESC,desc,MED_REMP)) < 0)
    return -1;

  /*
   * On ferme tout 
   */
  if ((ret = _MEDdatagroupFermer(eqid)) < 0)
    return -1;
  if ((ret = _MEDdatagroupFermer(root)) < 0)
    return -1;

  return 0 ; 
}

}
