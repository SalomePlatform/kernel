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
MEDnbnosoEcr(med_idt fid, char *nom_maillage,med_int n)
{
  med_idt maaid;
  char chemin[MED_TAILLE_MAA+MED_TAILLE_NOM+1];
  med_err ret;

  /*
   * On inhibe le gestionnaire d'erreur HDF
   */
  _MEDmodeErreurVerrouiller();

  /*
   * Si le maillage n'existe pas => erreur
   */
  strcpy(chemin,MED_MAA);
  strcat(chemin,nom_maillage);
  if ((maaid = _MEDdatagroupOuvrir(fid,chemin)) < 0)
      return -1;

  /*
   * Creation de l'attribut "Nombre de Noeuds Sommets"
   */
  if ((ret = _MEDattrEntierEcrire(maaid,MED_NOM_NNS,&n,MED_REMP)) < 0)
    return -1;

  /* 
   * Fermetures
   */
  if ((ret = _MEDdatagroupFermer(maaid)) < 0)
    return -1;

  return 0;
}
  
}
