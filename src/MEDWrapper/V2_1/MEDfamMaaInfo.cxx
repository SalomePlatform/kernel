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
MEDfamMaaInfo(med_idt fid,char *maa,med_int *nfam,med_int *nattc,
	      med_int *ngroc)
{
  med_int ret;
  med_int i;

  /* Lecture du nombre de familles */
  if ((*nfam = MEDnFam(fid,maa,0,(med_dim_famille)0)) < 0)
    return -1;

  /* Lecture des nombres cumules de groupes et d'attributs dans toutes
     les familles du maillage */
  *nattc = 0;
  *ngroc = 0;
  for (i=0;i<*nfam;i++)
    {
      if ((ret = MEDnFam(fid,maa,i+1,MED_ATTR)) < 0)
	return -1;
      *nattc += ret;
      if ((ret = MEDnFam(fid,maa,i+1,MED_GROUPE)) < 0)
	return -1;
      *ngroc += ret;
    }

  return 0;
}

}
