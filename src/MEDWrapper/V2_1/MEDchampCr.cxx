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
MEDchampCr(med_idt fid, char *champ, med_type_champ type, char *comp,
	   char *unit,med_int ncomp)
{
  med_err ret = 0;
  med_idt root,gid;
  char chemin[MED_TAILLE_CHA+1];
  med_size dimd[1];
  med_int _type = (med_int) type;
  
  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();

  /* 
   * Si le Data Group "/CHA/" n'existe pas, on le cree
   */
  strncpy(chemin,MED_CHA,MED_TAILLE_CHA-1);
  chemin[MED_TAILLE_CHA-1] = '\0';
  if ((root = _MEDdatagroupOuvrir(fid,chemin)) < 0)
    if ((root = _MEDdatagroupCreer(fid,chemin)) < 0)
      return -1;  

  /* 
   * Si le Data Group cha n'existe pas, on le cree
   * Sinon => erreur
   */
  if ((gid = _MEDdatagroupOuvrir(root,champ)) >= 0)
    return -1;
  if ((gid = _MEDdatagroupCreer(root,champ)) < 0)
    return -1;

  /*
   * Les infos sur les composants du champ
   */
  if ((ret = _MEDattrEntierEcrire(gid,MED_NOM_NCO,&ncomp,MED_REMP)) < 0)
    return -1;
  if ((ret = _MEDattrEntierEcrire(gid,MED_NOM_TYP,&_type,MED_REMP)) < 0)
    return -1;
  if ((ret = _MEDattrStringEcrire(gid,MED_NOM_NOM,MED_TAILLE_PNOM*ncomp,comp,MED_REMP)) < 0)
    return -1;
  if ((ret = _MEDattrStringEcrire(gid,MED_NOM_UNI,MED_TAILLE_PNOM*ncomp,unit,MED_REMP)) < 0)
    return -1;

  /*
   * On ferme tout
   */
  if ((ret = _MEDdatagroupFermer(gid)) < 0)
    return -1;
  if ((ret = _MEDdatagroupFermer(root)) < 0)
    return -1;
  
  return ret;
}

}
