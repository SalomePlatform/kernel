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
MEDfamMaaLire(med_idt fid,char *maa,med_int *numfam,med_int *attide,
	      med_int *attval,char *attdes,int *indatt,char *gro,int *indgro,
	      med_int nfamilles)
{
  med_err ret;
  med_int natt,ngro;
  med_int i;
  char nom[MED_TAILLE_NOM+1];

  *indatt = 0;
  *indgro = 0;
  for (i=0;i<nfamilles;i++)
    {
      if ((ret = MEDfamInfo(fid,maa,i+1,nom,numfam+i,attide+*(indatt+i),
			    attval+*(indatt+i),
			    attdes+*(indatt+i)*MED_TAILLE_DESC,
			    &natt,gro+*(indgro+i),&ngro)) < 0)
	return -1;
      *(indatt+i+1) = *(indatt+i)+natt;
      *(indgro+i+1) = *(indgro+i)+ngro*MED_TAILLE_LNOM;
    }
    
  return 0;
}

}
