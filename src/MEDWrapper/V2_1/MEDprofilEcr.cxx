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
MEDprofilEcr(med_idt fid,med_int *pflval,med_int n,char *nom)
{
  med_idt root, pid, did;
  med_size dimd[1];
  med_err ret;
  char chemin[MED_TAILLE_PROFILS+1];

  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();

  /* 
   * Si le groupe "PROFILS" n'existe pas, on le cree
   */
  strncpy(chemin,MED_PROFILS,MED_TAILLE_PROFILS-1);
  chemin[MED_TAILLE_PROFILS-1] = '\0';
  if ((root = _MEDdatagroupOuvrir(fid,chemin)) < 0)
    if ((root = _MEDdatagroupCreer(fid,chemin)) < 0)
      return -1;

  /* 
   * Si le groupe "nom" n'existe pas, on le cree
   * Sinon => erreur
   */
  if ((pid = _MEDdatagroupOuvrir(root,nom)) >= 0)
    return -1;
  if ((pid = _MEDdatagroupCreer(root,nom)) < 0)
    return -1;

  /*
   * On stocke "n" sous forme d'attribut
   */
  if ((ret = _MEDattrEntierEcrire(pid,MED_NOM_N,&n,MED_REMP)) < 0)
    return -1;

  /*
   * On stocke le profil dans un dataset
   */
  dimd[0] = n;
#if defined(IRIX64)||defined(OSF1)
  if ((ret =  _MEDdatasetNumEcrire(pid,MED_NOM_PFL,MED_INT64,MED_NO_INTERLACE,MED_DIM1,MED_ALL,MED_NOPF,0,MED_NOPG,dimd,
				(unsigned char*) pflval,MED_REMP)) < 0)
    return -1;
#else
  if ((ret =  _MEDdatasetNumEcrire(pid,MED_NOM_PFL,MED_INT32,MED_NO_INTERLACE,MED_DIM1,MED_ALL,MED_NOPF,0,MED_NOPG,dimd,
				(unsigned char*) pflval,MED_REMP)) < 0)
    return -1;
#endif

  /*
   * On ferme tout
   */
  if ((ret = _MEDdatagroupFermer(pid)) < 0)
    return -1;
  if ((ret = _MEDdatagroupFermer(root)) < 0)
    return -1;

  return 0; 
}

}
