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

#include <string.h>
#include <stdlib.h>

extern int mode_interlace;

namespace med_2_1{

med_err 
MEDcoordLire(med_idt fid, char *maa, med_int mdim, med_float *coo,
	     med_mode_switch mode_coo,med_int numco,
	     med_int * pfltabtmp, med_size psize, med_repere *type_rep, char *nom, char *unit)
{
  med_idt   maaid, noeid, dataset;
  med_err   ret;
  char      chemin[MED_TAILLE_MAA+MED_TAILLE_NOM+1];
  int       i,j;
  med_float *new_coo;
  med_int   type_rep_int;
  med_ssize * pfltab;

  /*
   * On inhibe le gestionnaire d'erreur
   */
  _MEDmodeErreurVerrouiller();

  /*
   * Si le maillage n'existe pas => erreur
   * Sinon on recupere sa dimension au passage
   */
  strcpy(chemin,MED_MAA);
  strcat(chemin,maa);
  if ((maaid = _MEDdatagroupOuvrir(fid,chemin)) < 0)
      return -1;

  /*
   * Si le Data Group "NOE" n'existe pas => erreur
   */
  if ((noeid = _MEDdatagroupOuvrir(maaid,MED_NOM_NOE)) < 0)
      return -1;

  /*
   * Convertion de med_int en med_ssize
   */
  if ( psize != MED_NOPF ) {  
    pfltab = (med_ssize *) malloc (sizeof(med_ssize)*psize);
    for (i=0;i<psize;i++)
      pfltab[i] = (med_ssize) pfltabtmp[i];
  }

  /*
   * Lecture du Data Set "COO"
   */
  if ((ret = _MEDdatasetNumLire(noeid,MED_NOM_COO,MED_REEL64,
				mode_coo,mdim,numco,
				psize,pfltab,MED_NOPG,
				(unsigned char*) coo)) < 0)
    return -1;

  

  /*
   * On re-ouvre le Data Set "COO" pour y lire des attributs
   */
  if ((dataset = _MEDdatasetOuvrir(noeid,MED_NOM_COO)) < 0)
    return -1;

  /*
   * L'attribut "REP"
   */
  if ((ret = _MEDattrEntierLire(dataset,MED_NOM_REP,&type_rep_int)) < 0)
    return -1;
  else
    *type_rep = (med_repere) type_rep_int;

  /*
   * Attribut "NOM"
   */
  if ((ret = _MEDattrStringLire(dataset,MED_NOM_NOM,mdim*MED_TAILLE_PNOM,
				nom)) < 0)
    return -1;

  /*
   * Attribut "UNI"
   */
  if ((ret = _MEDattrStringLire(dataset,MED_NOM_UNI,mdim*MED_TAILLE_PNOM,
				unit)) < 0)
    return -1;

  /*
   * On ferme tout
   */
  if ( psize != MED_NOPF ) free(pfltab); 
  
  if ((ret = _MEDdatasetFermer(dataset)) < 0)
    return -1;
  if ((ret = _MEDdatagroupFermer(noeid)) < 0)
    return -1;
  if ((ret = _MEDdatagroupFermer(maaid)) < 0)
    return -1;

  return 0; 
}

}
