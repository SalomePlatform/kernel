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

namespace med_2_1{

med_err
MEDelementsEcr(med_idt fid,char *maa,med_int mdim,med_int *connectivite,med_mode_switch mode_switch,
	       char *nom,med_booleen inom,med_int *num,med_booleen inum,
	       med_int *fam,med_int nele,med_entite_maillage typ_ent, 
	       med_geometrie_element typ_geo,med_connectivite typ_conn,
	       med_mode_acces mode)
{
  med_err ret;

  /* Ecriture de la connectivite */
  if ((ret = MEDconnEcr(fid,maa,mdim,connectivite,mode_switch,nele,mode,typ_ent,typ_geo,
			typ_conn)) < 0)
    return -1;

  /* Ecriture des noms */
  if (inom == MED_VRAI)
    if ((ret = MEDnomEcr(fid,maa,nom,nele,mode,typ_ent,typ_geo)) < 0)
      return -1;

  /* Ecriture des numeros */
  if (inum == MED_VRAI)
    if ((ret = MEDnumEcr(fid,maa,num,nele,mode,typ_ent,typ_geo)) < 0)
      return -1;

  /* Ecriture des numeros de familles */
  if ((ret = MEDfamEcr(fid,maa,fam,nele,mode,typ_ent,typ_geo)) < 0)
    return -1;

  return 0;
}

}
