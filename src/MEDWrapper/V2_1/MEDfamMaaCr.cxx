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

#include <stdio.h>

namespace med_2_1{

med_err
MEDfamMaaCr(med_idt fid,char *maa,
	    med_int *numfam,med_int *attide,
	    med_int *attval,char *attdes,int *indatt,char *gro,int *indgro,
	    med_int nfamilles)
{
  med_err ret;
  med_int i;
  med_int natt,ngro;
  med_int numf;
  char nomfam[MED_TAILLE_NOM+1];

  /* La famille de numero 0 n'a aucun attribut, ni aucun groupe
     Les familles de numero > 0 sont des familles de noeuds
     Les familles de numero < 0 sont des familles d'elements */
  for (i=0;i<nfamilles;i++)
    {
      numf = *(numfam+i);
      if (numf == 0)
	strcpy(nomfam,"FAMILLE_0");
      if (numf > 0)
	{
	  strcpy(nomfam,"FAMILLE_NOEUD_");
	  sprintf(nomfam,"%s%d",nomfam,numf);
          nomfam[MED_TAILLE_NOM] = '\0';
	}
      if (numf < 0)
	{
	  strcpy(nomfam,"FAMILLE_ELEMENT_");
	  sprintf(nomfam,"%s%d",nomfam,-numf);
	  nomfam[MED_TAILLE_NOM] = '\0';
	}	
      natt = *(indatt+i+1) - *(indatt+i);
      ngro = (*(indgro+i+1) - *(indgro+i))/MED_TAILLE_LNOM;
      if ((ret = MEDfamCr(fid,maa,nomfam,numf,
			  attide+*(indatt+i),
			  attval+*(indatt+i),
			  attdes+*(indatt+i)*MED_TAILLE_DESC,natt,
			  gro+*(indgro+i),ngro)) < 0)
	return -1;
    }
    
  return 0;
}

}
