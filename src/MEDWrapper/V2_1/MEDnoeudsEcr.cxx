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
MEDnoeudsEcr(med_idt fid,char *maa,med_int mdim,med_float *coord,
	     med_mode_switch mode_coo,
	     med_repere repere,char *nomcoo, char *unicoo,char *nom,
	     med_booleen inom,med_int *num,med_booleen inum,med_int *fam,
	     med_int nnoeuds,med_mode_acces mode)
{
  med_err ret;

  /* ecriture des coordonnees */
  if ((ret = MEDcoordEcr(fid,maa,mdim,coord,mode_coo,
			 nnoeuds,mode,repere,nomcoo,
			 unicoo)) < 0)
    return -1;

  /* ecriture des noms (facultatifs) */
  if (inom == MED_VRAI)
    if ((ret = MEDnomEcr(fid,maa,nom,nnoeuds,mode,MED_NOEUD,MED_POINT1)) < 0)
      return -1;

  /* ecriture des numeros (facultatifs) */
  if (inum == MED_VRAI)
    if ((ret = MEDnumEcr(fid,maa,num,nnoeuds,mode,MED_NOEUD,MED_POINT1)) < 0)
      return -1;

  /* ecriture des numeros de familles */
  if ((ret = MEDfamEcr(fid,maa,fam,nnoeuds,mode,MED_NOEUD,MED_POINT1)) < 0)
    return -1;

  return 0;
}

}
