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
MEDprofilInfo(med_idt fid, int indice, char *profil, med_int *n)
{
  int numero;
  med_idt proid;
  med_err ret;
  char chemin[MED_TAILLE_PROFILS+MED_TAILLE_NOM+1];

  /*
   * On inhibe le gestionnaire d'erreur
   */
  _MEDmodeErreurVerrouiller();

  /*
   * On recupere le nom du groupe de rang "indice"
   */ 
  numero = indice-1;
  if ((ret = _MEDobjetIdentifier(fid,MED_PROFILS,numero,profil)) < 0)
    return -1;

  /*
   * On va chercher l'attribut taille du profil 
   */
  strcpy(chemin,MED_PROFILS);
  strcat(chemin,profil);
  if ((proid = _MEDdatagroupOuvrir(fid,chemin)) < 0)
    return -1;   
  if ((ret = _MEDattrEntierLire(proid,MED_NOM_N,n)) < 0)
    return -1;
  if ((ret = _MEDdatagroupFermer(proid)) < 0)
    return -1;

  return 0;
}

}
