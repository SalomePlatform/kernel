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

namespace med_2_1{

med_err 
MEDchampInfo(med_idt fid,int indice,char *champ,
	     med_type_champ *type,char *comp,char *unit, 
	     med_int ncomp)
{
  med_err ret=0;
  med_idt gid;
  char chemin[MED_TAILLE_CHA+MED_TAILLE_NOM+1];
  int num;

  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();

  /*
   * On recupere le nom du champ
   */
  num = indice - 1;
  strcpy(chemin,MED_CHA);
  if ((ret = _MEDobjetIdentifier(fid,chemin,num,champ)) < 0)
    return -1;

  /* 
   * Si le Data Group cha n'existe pas => erreur
   */
  strcat(chemin,champ);
  if ((gid = _MEDdatagroupOuvrir(fid,chemin)) < 0)
    return -1;


  /*
   * La liste des attributs
   */
  if ((ret = _MEDattrEntierLire(gid,MED_NOM_TYP,(med_int*) type)) < 0)
    return -1;
  if ((ret = _MEDattrStringLire(gid,MED_NOM_NOM,ncomp*MED_TAILLE_PNOM,
				comp)) < 0)
    return -1;
  if ((ret = _MEDattrStringLire(gid,MED_NOM_UNI,ncomp*MED_TAILLE_PNOM,
				unit)) < 0)
    return -1;

  /*
   * On ferme tout
   */
  if ((ret = _MEDdatagroupFermer(gid)) < 0)
    return -1; 

  return 0;
}

}
