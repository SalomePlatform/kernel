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
#include <string.h>
#include <stdlib.h>
#include "med_outils.hxx"

namespace med_2_1{

med_int 
MEDnChamp(med_idt fid, int indice)
{
  int n1;
  med_int n2;
  med_idt datagroup;
  med_err ret;
  char nomdatagroup[MED_TAILLE_NOM+1];
  int num;
  char chemin[MED_TAILLE_CHA+MED_TAILLE_NOM+1];

  if (indice < 0)
    return -1;

  /*
   * On inhibe le gestionnaire d'erreur HDF 
   */
  _MEDmodeErreurVerrouiller();
  
  /* 
   * Si le Data Group cha n'existe pas et indice == 0 => 0
   * sinon erreur => erreur
   */
  strcpy(chemin,MED_CHA);

  /*
   * Si indice == 0 => nombre de champs
   */
  if (indice == 0)
    {
      n1 = 0;
      _MEDnObjets(fid,chemin,&n1);
      n2 = n1;
    }

  /*
   * Si indice > 0 => nbre de composants
   */
  if (indice > 0)
    {
      /*
       * On recupere le nom du champ 
       */
      num = indice-1;
      if ((ret = _MEDobjetIdentifier(fid,chemin,num,nomdatagroup)) < 0)
	return -1;
      strcat(chemin,nomdatagroup);
      /*
       * On recupere le nombre de composants
       */
      if ((datagroup = _MEDdatagroupOuvrir(fid,chemin)) < 0) 
	return -1;
      if ((ret = _MEDattrEntierLire(datagroup,MED_NOM_NCO,&n2)) < 0)
	return -1;
      if ((ret = _MEDdatagroupFermer(datagroup)) < 0)
	return -1;
    }

  return n2;
}

}
