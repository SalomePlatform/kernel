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

extern int mode_interlace; 

namespace med_2_1{

med_err 
MEDequivLire(med_idt fid, char *maa, char *eq, med_int *corr, med_int n,
	     med_entite_maillage typ_ent,med_geometrie_element typ_geo)
{
  med_idt eqid, datagroup;
  med_err ret;
  char chemin[MED_TAILLE_MAA+MED_TAILLE_EQS+2*MED_TAILLE_NOM+1]; 
  char nomdatagroup[MED_TAILLE_NOM+1];
  char tmp[MED_TAILLE_NOM_ENTITE+1];

  if (typ_geo == MED_TETRA4 || typ_geo == MED_TETRA10 ||
      typ_geo == MED_HEXA8  || typ_geo == MED_HEXA20  ||
      typ_geo == MED_PENTA6 || typ_geo == MED_PENTA15 ||
      typ_geo == MED_PYRA5  || typ_geo == MED_PYRA13)
    return -1;

  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();

  /* 
   * Si le Data Group de "eq" n'existe pas => erreur
   */
  strcpy(chemin,MED_MAA);
  strcat(chemin,maa);
  strcat(chemin,MED_EQS);
  strcat(chemin,eq);
  if ((eqid = _MEDdatagroupOuvrir(fid,chemin)) < 0)
    return -1;  

  /*
   * Lecture de l'equivalence
   */
  if ((ret = _MEDnomEntite(nomdatagroup,typ_ent)) < 0)
    return -1;
  if ((typ_ent != MED_NOEUD))
    {
      if ((ret = _MEDnomGeometrie(tmp,typ_geo)) < 0)
	return -1;
      strcat(nomdatagroup,".");
      strcat(nomdatagroup,tmp);
    }
  if ((datagroup = _MEDdatagroupOuvrir(eqid,nomdatagroup)) < 0)
    return -1;
#if defined(IRIX64)||defined(OSF1)
  if ((ret =  _MEDdatasetNumLire(datagroup,MED_NOM_COR,MED_INT64,
				 MED_NO_INTERLACE,1,MED_ALL,
				 MED_NOPF,0,MED_NOPG,
				 (unsigned char *) corr)) < 0)
    return -1;
#else
  if ((ret =  _MEDdatasetNumLire(datagroup,MED_NOM_COR,MED_INT32,
				 MED_NO_INTERLACE,1,MED_ALL,
				 MED_NOPF,0,MED_NOPG,
				 (unsigned char *) corr)) < 0)
    return -1;
#endif

  /*
   * On ferme tout 
   */
  if ((ret = _MEDdatagroupFermer(datagroup)) < 0)
    return -1;
  if ((ret = _MEDdatagroupFermer(eqid)) < 0)
    return -1;

  return 0;  
}

}
