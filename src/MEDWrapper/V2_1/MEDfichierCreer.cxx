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

/*
 * - Nom de la fonction : _MEDfichierCreer
 * - Description : creation d'un fichier HDF
 * - Parametres :
 *     - nom (IN) : le nom du fichier
 * - Resultat : ID du fichier en cas de succes, -1 sinon
 */ 

namespace med_2_1{

med_idt
_MEDfichierCreer(char *nom)
{
  med_idt fid,gid;
  med_err ret;
  med_int majeur = MED_NUM_MAJEUR;
  med_int mineur = MED_NUM_MINEUR; 
  med_int release = MED_NUM_RELEASE;

  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();

  if ((fid = H5Fcreate(nom,H5F_ACC_TRUNC,
			       H5P_DEFAULT,H5P_DEFAULT)) < 0)
    return -1;

  if ((gid = _MEDdatagroupCreer(fid,MED_NOM_INFOS)) < 0)
    return -1;

  /* Numero de versions de MED */
  if ((ret = _MEDattrEntierEcrire(gid,MED_NOM_MAJEUR,&majeur,MED_REMP)) < 0)
    return -1;

  if ((ret = _MEDattrEntierEcrire(gid,MED_NOM_MINEUR,&mineur,MED_REMP)) < 0)
    return -1;

  if ((ret = _MEDattrEntierEcrire(gid,MED_NOM_RELEASE,&release,MED_REMP)) < 0)
    return -1;

  /* On ferme tout */
  if ((ret = _MEDdatagroupFermer(gid)) < 0)
    return -1;

  return fid;
}

}
