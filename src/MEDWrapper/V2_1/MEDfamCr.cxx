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
MEDfamCr(med_idt fid,char* maa,char *famille,med_int numero, 
	 med_int *attr_ident, med_int *attr_val, char *attr_desc, 
	 med_int n_attr,char *groupe, med_int n_groupe)
{
  med_idt root, datagroup, famid;
  med_err ret;
  med_size dimd[1];
  char chemin[MED_TAILLE_MAA+MED_TAILLE_FAS+MED_TAILLE_NOM+1];
  char tmp[MED_TAILLE_FAS+1];

  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();

  /* 
   * Si le Data Group FAS n'existe pas, on le cree
   */
  strcpy(chemin,MED_MAA);
  strcat(chemin,maa);
  strncpy(tmp,MED_FAS,MED_TAILLE_FAS-1);
  tmp[MED_TAILLE_FAS-1] = '\0';
  strcat(chemin,tmp);
  if ((root = _MEDdatagroupOuvrir(fid,chemin)) < 0)
    if ((root = _MEDdatagroupCreer(fid,chemin)) < 0)
      return -1;
    
  /*
   * Si le Data Group de meme nom que famille existe => erreur
   * Sinon on le cree
   */
  if ((famid = _MEDdatagroupOuvrir(root,famille)) >= 0)
    return -1;
  if ((famid = _MEDdatagroupCreer(root,famille)) < 0)
    return -1;

  /*
   * L'attribut NUM
   */
  if ((ret = _MEDattrEntierEcrire(famid,MED_NOM_NUM,&numero,MED_REMP)) < 0)
    return -1;

  /*
   * Le Data Group "GRO"
   */
  if (n_groupe > 0)
    {
      /*
       * On cree le Data Group 
       */
      if ((datagroup = _MEDdatagroupCreer(famid,MED_NOM_GRO)) < 0)
	return -1;

      /*
       * L'attribut "NBR"
       */
      if ((ret = _MEDattrEntierEcrire(datagroup,MED_NOM_NBR,&n_groupe,MED_REMP)) < 0)
	return -1;
      
      /* 
       * Data Set des noms des groupes "NOM"
       */
      dimd[0] = n_groupe*MED_TAILLE_LNOM+1;
      if ((ret = _MEDdatasetStringEcrire(datagroup,MED_NOM_NOM,dimd,groupe,
				      MED_REMP))<0)
	return -1;

      /* 
       * On ferme le Data Group
       */
      if ((ret = _MEDdatagroupFermer(datagroup)) < 0)
	return -1;
    }

  /*
   * Le Data Group "ATT"
   */
  
  if (n_attr > 0)
    {
      if ((datagroup = _MEDdatagroupCreer(famid,MED_NOM_ATT)) < 0)
	return -1;

      /*
       * L'attribut "NBR"
       */
      if ((ret = _MEDattrEntierEcrire(datagroup,MED_NOM_NBR,&n_attr,MED_REMP)) < 0)
	return -1;
      
      /*
       * Le Data Set "IDE"
       */
      dimd[0] = n_attr;
#if defined(IRIX64)||defined(OSF1)
      if ((ret = _MEDdatasetNumEcrire(datagroup,MED_NOM_IDE,MED_INT64,MED_NO_INTERLACE,MED_DIM1,MED_ALL,MED_NOPF,0,MED_NOPG,dimd,
				   (unsigned char *)attr_ident,MED_REMP)) < 0)
	return -1;    
#else
      if ((ret = _MEDdatasetNumEcrire(datagroup,MED_NOM_IDE,MED_INT32,MED_NO_INTERLACE,MED_DIM1,MED_ALL,MED_NOPF,0,MED_NOPG,dimd,
				   (unsigned char *)attr_ident,MED_REMP)) < 0)
	return -1;      
#endif

      /*
       * Le Data Set "VAL"
       */
      dimd[0] = n_attr;
#if defined(IRIX64)||defined(OSF1)
      if ((ret = _MEDdatasetNumEcrire(datagroup,MED_NOM_VAL,MED_INT64,MED_NO_INTERLACE,MED_DIM1,MED_ALL,MED_NOPF,0,MED_NOPG,dimd,
				   (unsigned char*)attr_val,MED_REMP)) < 0)
	return -1;
#else
      if ((ret = _MEDdatasetNumEcrire(datagroup,MED_NOM_VAL,MED_INT32,MED_NO_INTERLACE,MED_DIM1,MED_ALL,MED_NOPF,0,MED_NOPG,dimd,
				   (unsigned char*)attr_val,MED_REMP)) < 0)
	return -1;
#endif

      /*
       * Le Data Set "DES"
       */
      dimd[0] = n_attr*MED_TAILLE_DESC+1;
      if ((ret = _MEDdatasetStringEcrire(datagroup,MED_NOM_DES,dimd,attr_desc,
				      MED_REMP)) < 0)
	return -1;

      /* 
       * On ferme le Data Group
       */
      if ((ret = _MEDdatagroupFermer(datagroup)) < 0)
	return -1;
    }

  /* 
   * On ferme tout
   */ 
  if ((ret = _MEDdatagroupFermer(famid)) < 0)
    return -1;
  if ((ret = _MEDdatagroupFermer(root)) < 0)
    return -1;

  return 0; 
}

}
