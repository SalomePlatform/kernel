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

namespace med_2_1{

med_err 
MEDfichEntete(med_idt fid, med_fich_info quoi, char str[])
{
  med_idt atid, root;
  med_err ret;
  char locale[MED_TAILLE_DESC+1];
  char chemin[MED_TAILLE_MAA+1];

  switch (quoi)
    {
    case MED_HDF_VERSION : 
      strcpy(str,HDF_VERSION_ACTUELLE);
      break;

    case MED_VERSION :
      strcpy(str,MED_VERSION_ACTUELLE); 
      break;

    case MED_FICH_DES :
      /*
       * On inhibe le gestionnaire d'erreur HDF
       */
      _MEDmodeErreurVerrouiller();
      
      /*
       * On ouvre le Data Group racine
       */
      strncpy(chemin,MED_MAA,strlen(MED_MAA)-1);
      chemin[MED_TAILLE_MAA-1] = '\0';
      if ((root = _MEDdatagroupOuvrir(fid,chemin)) < 0)
	return -1;

      /*
       * On regarde si l'attribut existe
       * Si non => erreur
       * Si oui => on le copie dans str
       */
      if ((ret = _MEDattrStringLire(root,MED_NOM_DESCRIPTEUR,
				    MED_TAILLE_DESC,locale)) < 0)
	return -1;
      strcpy(str,locale);
      
      if ((ret == _MEDdatagroupFermer(root)) < 0)
	return -1;

      break;
      
    default :
      return -1;
    }
  return 0;
}

}
