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
MEDnoeudsLire(med_idt fid,char *maa,med_int mdim, med_float *coord,
	      med_mode_switch mode_coo,
	      med_repere *repere,char *nomcoo, char *unicoo,char *nom,
	      med_booleen *inom,med_int *num,med_booleen *inum,med_int *fam,
	      med_int nnoeuds)
{
  med_err ret;

  /* lecture des coordonnees */
  if ((ret = MEDcoordLire(fid,maa,mdim,coord,mode_coo,MED_ALL,0,MED_NOPF,repere,nomcoo,
			  unicoo)) < 0)
    return -1;

  /* lecture des noms (facultatifs) */
  if ((ret = MEDnomLire(fid,maa,nom,nnoeuds,MED_NOEUD,MED_POINT1)) < 0)
    *inom = MED_FAUX;
  else
    *inom = MED_VRAI;

  /* lecture des numeros (facultatifs) */
  if ((ret = MEDnumLire(fid,maa,num,nnoeuds,MED_NOEUD,MED_POINT1)) < 0)
    *inum = MED_FAUX;
  else
    *inum = MED_VRAI;  

  /* lecture des numeros de familles */
  if ((ret = MEDfamLire(fid,maa,fam,nnoeuds,MED_NOEUD,MED_POINT1)) < 0)
    return -1;

  return 0;
}

}
