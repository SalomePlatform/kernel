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

#include "med_outils.hxx"
#include "med.hxx"

#include <stdlib.h>
#include <string.h>

namespace med_2_1{

med_err 
MEDcoordEcr(med_idt fid, char *maa, med_int mdim, med_float *coo, 
	    med_mode_switch mode_coo,med_int n,
	    med_mode_acces mode, med_repere type_rep, char *nom, char *unit)
{
  med_idt maaid, noeid, dataset;
  med_err ret;
  med_size dimd[1];
  char chemin[MED_TAILLE_MAA+MED_TAILLE_NOM+1];
  med_int type_rep_int; 
  /*
   * On inhibe le gestionnaire d'erreur HDF
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
   * Si le Data Group "NOE" n'existe pas
   * on le cree
   */
  /* EF : A faire : gerer le mode MED_REMP*/
  if ((noeid = _MEDdatagroupOuvrir(maaid,MED_NOM_NOE)) < 0)
    if ((noeid = _MEDdatagroupCreer(maaid,MED_NOM_NOE)) < 0)
      return -1;

  /*
   * Creation du Data Set "COO"
   */
  dimd[0] = n*mdim;
  if ((ret = _MEDdatasetNumEcrire(noeid,MED_NOM_COO,MED_REEL64,mode_coo,mdim,MED_ALL,MED_NOPF,0,MED_NOPG,dimd,
				  (unsigned char*) coo,mode)) < 0)
    return -1;
  
  /*
   * On re-ouvre le Data Set "COO" pour y placer des attributs
   */
  if ((dataset = _MEDdatasetOuvrir(noeid,MED_NOM_COO)) < 0)
    return -1;

  /*
   * Attribut NBR (nombre de noeuds)
   */
  if ((ret = _MEDattrEntierEcrire(dataset,MED_NOM_NBR,&n,mode)) < 0)
    return -1;

  /*
   * L'attribut "REP"
   */
  type_rep_int = (med_int) type_rep; 
  if ((ret = _MEDattrEntierEcrire(dataset,MED_NOM_REP,&type_rep_int,mode)) < 0)
    return -1;

  /*
   * Attribut "NOM"
   */
  if ((ret = _MEDattrStringEcrire(dataset,MED_NOM_NOM,mdim*MED_TAILLE_PNOM,nom,mode)) < 0)
    return -1;

  /*
   * Attribut "UNI"
   */
  if ((ret = _MEDattrStringEcrire(dataset,MED_NOM_UNI,mdim*MED_TAILLE_PNOM,unit,mode)) < 0)
    return -1;

  /*
   * On ferme tout
   */
  if ((ret = _MEDdatasetFermer(dataset)) < 0)
    return -1;
  if ((ret = _MEDdatagroupFermer(noeid)) < 0)
    return -1;
  if ((ret = _MEDdatagroupFermer(maaid)) < 0)
    return -1;

  return 0; 
}

}
