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
MEDelementsLire(med_idt fid,char *maa,med_int mdim,med_int *connectivite,med_mode_switch mode_switch,
	       char *nom,med_booleen *inom,med_int *num,med_booleen *inum,
	       med_int *fam,med_int nele,med_entite_maillage typ_ent, 
	       med_geometrie_element typ_geo,med_connectivite typ_conn)
{
  med_err ret;

  /* Lecure de la connectivite */
  if ((ret = MEDconnLire(fid,maa,mdim,connectivite,mode_switch,0,MED_NOPF,
			 typ_ent,typ_geo,typ_conn)) < 0)
    return -1;

  /* Lecture des noms */
  if ((ret = MEDnomLire(fid,maa,nom,nele,typ_ent,typ_geo)) < 0)
    *inom = MED_FAUX;
  else
    *inom = MED_VRAI;

  /* Lecture des numeros */
  if ((ret = MEDnumLire(fid,maa,num,nele,typ_ent,typ_geo)) < 0)
    *inum = MED_FAUX;
  else
    *inum = MED_VRAI;

  /* Lecture des numeros de familles */
  if ((ret = MEDfamLire(fid,maa,fam,nele,typ_ent,typ_geo)) < 0)
    return -1;

  return 0;
}

}
