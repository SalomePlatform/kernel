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

#include <stdlib.h>
#include <string.h>

namespace med_2_1{

med_err 
MEDfamInfo(med_idt fid,char *maa,int indice, char *famille, 
	   med_int *numero,
	   med_int *attr_ident, med_int *attr_val, char *attr_desc,
	   med_int *n_attr, char *groupe ,med_int *n_groupe)
{
  med_idt famid,datagroup;
  med_err ret;
  char chemin[MED_TAILLE_MAA+MED_TAILLE_FAS+2*MED_TAILLE_NOM+1];
  int num;

  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();

  /*
   * On recupere le nom de la famille
   */
  num = indice - 1;
  strcpy(chemin,MED_MAA);
  strcat(chemin,maa);
  strcat(chemin,MED_FAS); 
  if ((ret = _MEDobjetIdentifier(fid,chemin,num,famille)) < 0)
    return -1;

  /* 
   * Si le Data Group de la famille n'existe pas => erreur
   */
  strcat(chemin,famille);
  if ((famid = _MEDdatagroupOuvrir(fid,chemin)) < 0)
    return -1;

  /*
   * L'attribut NUM
   */
  if ((ret = _MEDattrEntierLire(famid,MED_NOM_NUM,numero)) < 0)
    return -1;

  /*
   * Le Data Group "GRO"
   */
  if ((datagroup = _MEDdatagroupOuvrir(famid,MED_NOM_GRO)) >= 0)
    {
      /*
       * L'attribut "NBR"
       */
      if ((ret = _MEDattrEntierLire(datagroup,MED_NOM_NBR,n_groupe)) < 0)
	return -1;
      
      /* 
       * Data Set des noms des groupes "NOM"
       */
      if ((ret = _MEDdatasetStringLire(datagroup,MED_NOM_NOM,groupe)) < 0)
	return -1;

      /* 
       * On ferme le Data Group
       */
      if ((ret = _MEDdatagroupFermer(datagroup)) < 0)
	return -1;
    }
  else
    *n_groupe = 0;

  /*
   * Le Data Group "ATT"
   */
  if ((datagroup = _MEDdatagroupOuvrir(famid,MED_NOM_ATT)) >= 0)
    {
      /*
       * L'attribut "NBR"
       */
      if ((ret = _MEDattrEntierLire(datagroup,MED_NOM_NBR,n_attr)) < 0)
	return -1;
      
      /*
       * Le Data Set "IDE"
       */
#if defined(IRIX64)||defined(OSF1)
      if ((ret = _MEDdatasetNumLire(datagroup,MED_NOM_IDE,MED_INT64,
				    MED_NO_INTERLACE,1,MED_ALL,
				    MED_NOPF,0,MED_NOPG,
				    (unsigned char*) attr_ident)) < 0)
	return -1;     
#else
      if ((ret = _MEDdatasetNumLire(datagroup,MED_NOM_IDE,MED_INT32,
				    MED_NO_INTERLACE,1,MED_ALL,
				    MED_NOPF,0,MED_NOPG,
				    (unsigned char*) attr_ident)) < 0)
	return -1;     
#endif

      /*
       * Le Data Set "VAL"
       */
#if defined(IRIX64)||defined(OSF1)
      if ((ret = _MEDdatasetNumLire(datagroup,MED_NOM_VAL,MED_INT64,
				    MED_NO_INTERLACE,1,MED_ALL,
				    MED_NOPF,0,MED_NOPG,
				    (unsigned char *) attr_val)) < 0)
	return -1;
#else
      if ((ret = _MEDdatasetNumLire(datagroup,MED_NOM_VAL,MED_INT32,
				    MED_NO_INTERLACE,1,MED_ALL,
				    MED_NOPF,0,MED_NOPG,
				    (unsigned char *) attr_val)) < 0)
	return -1;
#endif

      /*
       * Le Data Set "DES"
       */
      ret = _MEDdatasetStringLire(datagroup,MED_NOM_DES,attr_desc);

      /* 
       * On ferme le Data Group
       */
      if ((ret = _MEDdatagroupFermer(datagroup)) < 0)
	return -1;
    }
  else
    *n_attr = 0;

  /* 
   * On ferme tout
   */ 
  if ((ret = _MEDdatagroupFermer(famid)) < 0)
    return -1;

  return 0; 
}

}
