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

med_int 
MEDnFam(med_idt fid,char *maa, int indice, med_dim_famille quoi)
{
  med_idt datagroup,famid;
  med_err ret;
  char chemin[MED_TAILLE_MAA+MED_TAILLE_FAS+2*MED_TAILLE_NOM+1];
  med_int n;
  int n_tmp;
  int num;
  char famille[MED_TAILLE_NOM+1];


  /* 
   * On inhibe le gestionnaire d'erreur HDF 
   */
  _MEDmodeErreurVerrouiller();

  strcpy(chemin,MED_MAA);
  strcat(chemin,maa);
  strcat(chemin,MED_FAS);
  if (indice == 0)
    {
      n_tmp = 0;
      _MEDnObjets(fid,chemin,&n_tmp);
      n = (med_int ) n_tmp;
    }
  else
    {
      /*
       * On recupere le nom de la famille
       */
      num = indice - 1;
      if ((ret = _MEDobjetIdentifier(fid,chemin,num,
			    famille)) < 0)
	return -1;
      
  /* 
   * Si le Data Group de la famille n'existe pas => erreur
   */
      strcat(chemin,famille);
      if ((famid = _MEDdatagroupOuvrir(fid,chemin)) < 0)
	return -1;

      switch (quoi)
	{
	case MED_GROUPE :
	  if ((datagroup = _MEDdatagroupOuvrir(famid,MED_NOM_GRO)) < 0)
	    n = 0;
	  else
	    {
	      if ((ret = _MEDattrEntierLire(datagroup,MED_NOM_NBR,&n)) < 0)
		return -1;
	      if ((ret = _MEDdatagroupFermer(datagroup)) < 0)
		return -1;
	    }
	  break;

	case MED_ATTR :
	  if ((datagroup = _MEDdatagroupOuvrir(famid,MED_NOM_ATT)) < 0)
	    n = 0;
	  else
	    {
	      if ((ret = _MEDattrEntierLire(datagroup,MED_NOM_NBR,&n)) < 0)
		return -1;
	      if ((ret = _MEDdatagroupFermer(datagroup)) < 0)
		return -1;
	    }
	  break;

	default :
	  return -1;
	}

      if ((ret = _MEDdatagroupFermer(famid)) < 0)
        return -1;

    }

  return (med_int) n;
}

}
