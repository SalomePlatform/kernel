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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "med.hxx"

/***********************************************************************
 * FONCTION MEDgro2famB
 * 
 * - DESCRIPTION : 2eme etape dans la conversion des groupes de noeuds
 *      et d'elements en familles MED
 *      Mise a jour des tables suivantes passees en parametres :
 *      1 - les nouveaux numeros des familles a creer
 *      2 - les nouveaux numeros des familles des elements
 *      3 - les nouveaux numeros des familles des noeuds
 *      4 - les noms des groupes composant ces familles
 *      5 - les index de la table des groupes 
 *
 * - PARAMETRES :
 *   NOM            .E/S. TYPE    .  DESCRIPTION
 *   ------------------------------------------------------------------- 
 *   nnoe           .E  . med_int . nombre de noeuds 
 *   nele           .E  . med_int . nombre d'elements 
 *   numnoe         .E  . med_int*. numeros des noeuds
 *   numele         .E  . med_int*. numeros des elements
 *   ngn            .E  . med_int . nombre de groupes de noeuds
 *   nge            .E  . med_int . nombre de groupes d'elements
 *   nindn          .E  . med_int . nombre d'indices dans la table
 *                  .   .         . des groupes de noeuds
 *   ninde          .E  . med_int . nombre d'indices dans la table
 *                  .   .         . de groupes d'elements
 *   nomgronoe      .E  . char*   . noms des groupes de noeuds
 *   nomgroele      .E  . char*   . noms des groupes d'elements
 *   indgronoe      .E  . int*    . table index de la table des groupes
 *                  .   .         . de noeuds
 *   indgroele      .E  . int*    . table index de la table des groupes
 *                  .   .         . d'elements
 *   tabgronoe      .E  . int*    . table des groupes de noeuds
 *   tabgroele      .E  . int*    . table des groupes d'elements
 *   nfamg          .E  . med_int . nombre de familles MED a creer
 *   nidnf          .E  . med_int . nombre de noms groupes associes a 
 *                  .   .         . l'ensemble des familles MED
 *   newnumfam      .  S. med_int*. nouveaux numeros de familles
 *   newnumfamele   .  S. med_int*. nouveaux numeros de familles des
 *                  .   .         . elements
 *   newnumfamnoe   .  S. med_int*. nouveaux numeros de familles des
 *                  .   .         . noeuds
 *   newindfamgro   .  S. int*    . table des index de la table des
 *                  .   .         . noms de groupes associes aux familles
 *   newfamgro      .   . char*   . table des noms des groupes des 
 *                  .   .         . familles 
 *
 * - RESULTAT : 0
 * 
 ***********************************************************************/

namespace med_2_1{

med_err 
MEDgro2famB (med_int nnoe,med_int nele,med_int *numnoe,med_int *numele,
	     med_int ngn,med_int nge,med_int nindn,
	     med_int ninde, char *nomgronoe,char *nomgroele, 
	     int *indgronoe,int *indgroele,med_int *tabgronoe, 
	     med_int *tabgroele,med_int nfamg,med_int nindf,
	     med_int *newnumfam,med_int *newnumfamele, 
	     med_int *newnumfamnoe,int *newindfamgro,
	     char *newfamgro)
{
  int i,j,k;

  med_int *famnoe, *famele, *tmp;
  med_int *p;
  med_int num;
  int flag,exist;
  int nfamn, nfame;
  int estfam0 = 1;
  int newnumnoe, newnumele;
  int tmp1;
  int existfam0 = 0;
  int ind = 0;

  famnoe = NULL;
  famele = NULL;

  nfamn = 0;
  nfame = 0;
  newnumnoe = 0;
  newnumele = 0;

  *newindfamgro = 0;

  if (nfamg > 1)
    {
      /* pour chaque noeud :
	 1 - on dresse la liste des groupes de noeuds auquel il appartient
	 2 - en la comparant avec les listes pre-existantes, on
         estime s'il est necessaire de creer une nouvelle famille de noeuds.
	 Si oui => - on cree le numero de famille que l'on reporte 
                     dans newnumfam
                   - on reporte ce numero dans newnumnoe
                   - on met a jour la table des noms des groupes des familles
                     ainsi que sa table d'index  
	 Si non => on ne fait rien 
         ATTENTION : pour la famille 0, on ne met a jour que les numeros */
      for (i=0;i<nnoe;i++)
	{
	  if ((tmp = (med_int*) malloc(sizeof(med_int)*ngn)) == NULL)
	    return -1;
	  num = *(numnoe+i);
	  for (j=0;j<ngn;j++)
	    {
	      flag = 0;
	      /* on regarde si le noeud appartient au groupe */
	      for (k=0;k<*(indgronoe+j+1)-*(indgronoe+j);k++)
		if (num == *(tabgronoe+*(indgronoe+j)+k))
		  flag = 1;
	      /* on met le flag a jour dans tmp */
	      *(tmp+j) = flag;
	    }
	  /* on regarde si le numero de famille est 0 */
	  estfam0 = 0;
	  flag = 1;
	  for (j=0;j<ngn;j++)
	    if (*(tmp+j) == 1)
	      flag = 0;
	  if (flag == 1)
	    {
	      estfam0 = 1;
	      *(newnumfamnoe+i) = 0;
	    }
	  if (flag == 1 && existfam0 == 0)
	    existfam0 = 1;
	  /* faut-il creer une nouvelle famille ? */
	  if (famnoe == NULL)
	    {
	      exist = 0;
	      if ((famnoe = (med_int *) malloc (sizeof(med_int)*ngn)) == NULL)
		return -1;
	      /* on met a jour la table d'indices */
	      nfamn = 1;
	      *(newindfamgro+nfamn) = *(newindfamgro+nfamn-1);
	      for (j=0;j<ngn;j++)
		{
		  tmp1 = *(tmp+j);
		  *(famnoe+j) = tmp1;
		  if (tmp1 == 1)
		    {
		      strncpy(newfamgro+*(newindfamgro+nfamn),
			      nomgronoe+j*MED_TAILLE_LNOM,MED_TAILLE_LNOM);
		      *(newindfamgro+nfamn) = *(newindfamgro+nfamn) + 
			MED_TAILLE_LNOM;
		    }
		}
	      if (estfam0 == 0)
		{
		  newnumnoe = 1;
		  *newnumfamnoe = newnumnoe;
		  *newnumfam = newnumnoe;
		}
	      else
		*newnumfam = 0;
	    }
	  else
	    {
	      for (j=0;j<nfamn;j++)
		{
		  p = famnoe + ngn*j;
		  for (k=0;k<ngn;k++)
		    {
		      if (*(p+k) != *(tmp+k))
			{
			  exist = 0;
			  break;
			}
		      else
			exist = 1;
		    }
		  if (exist == 1)
		    {
		      if (estfam0 == 0)
			*(newnumfamnoe+i) = *(newnumfam+j);
		      break;
		    }
		}
	      if (exist == 0)
		{
		  nfamn = nfamn + 1;
		  *(newindfamgro+nfamn) = *(newindfamgro+nfamn-1);
		  p = famnoe;
		  if ((famnoe = (med_int*) malloc(sizeof(med_int)*ngn*nfamn))
		      == NULL)
		    return -1;
		  for (j=0;j<nfamn-1;j++)
		    for (k=0;k<ngn;k++)
		      *(famnoe+j*ngn+k) = *(p+j*ngn+k);
		  free(p);
		  p = famnoe+(nfamn-1)*ngn;
		  for (j=0;j<ngn;j++)
		    {
		      tmp1 = *(tmp+j);
		      *(p+j) = tmp1;
		      if (tmp1 == 1)
			{
			  strncpy(newfamgro+*(newindfamgro+nfamn), 
				  nomgronoe+j*MED_TAILLE_LNOM,MED_TAILLE_LNOM);
		      *(newindfamgro+nfamn) = *(newindfamgro + nfamn) 
			+ MED_TAILLE_LNOM;
			}
		    }
		  if (estfam0 == 0)
		    {
		      newnumnoe = newnumnoe + 1;
		      *(newnumfamnoe+i) = newnumnoe;
		      *(newnumfam+nfamn-1) = newnumnoe;
		    }
		  else
		    *(newnumfam+nfamn-1) = 0;
		}
	    }
	  free(tmp);
	}
      
      /* pour chaque element :
	 1 - on dresse la liste des groupes de noeuds auquel il appartient
	 2 - en la comparant avec les listes pre-existantes, on
         estime s'il est necessaire de creer une nouvelle famille d'elements.
	 Si oui => - on cree le numero de famille que l'on reporte 
                     dans newnumfam
                   - on reporte ce numero dans newnumele
                   - on met a jour la table des noms des groupes des familles
                     ainsi que sa table d'index  
	 Si non => on ne fait rien 
         ATTENTION : pour la famille 0, on ne met a jour que les numeros */
      for (i=0;i<nele;i++)
	{
	  if ((tmp = (med_int*) malloc(sizeof(med_int)*nge)) == NULL)
	    return -1;
	  num = *(numele+i);
	  for (j=0;j<nge;j++)
	    {
	      flag = 0;
	      /* on regarde si l'element appartient au groupe */
	      for (k=0;k<*(indgroele+j+1)-*(indgroele+j);k++)
		if (num == *(tabgroele+*(indgroele+j)+k))
		  flag = 1;
	      /* on met le flag a jour dans tmp */
	      *(tmp+j) = flag;
	    }
	  /* on regarde si le numero de famille est 0 */
	  estfam0 = 0;
	  flag = 1;
	  for (j=0;j<nge;j++)
	    if (*(tmp+j) == 1)
	      flag = 0;
	  if (flag == 1)
	    {
	      estfam0 = 1;
	      *(newnumfamele+i) = 0;
	    }
	  /* faut-il creer une nouvelle famille ? */
	  if (famele == NULL)
	    {
	      if (!(estfam0&&existfam0))
		{
		  exist = 0;
		  if ((famele = (med_int *) malloc (sizeof(med_int)*nge))
		      == NULL)
		    return -1;
		  nfame = 1;
		  *(newindfamgro+nfamn+nfame) = *(newindfamgro+nfamn+nfame-1);
		  for (j=0;j<nge;j++)
		    {
		      tmp1 = *(tmp+j);
		      *(famele+j) = tmp1;
		      if (tmp1 == 1)
			{
			  strncpy(newfamgro+*(newindfamgro+nfamn+nfame),
				  nomgroele+j*MED_TAILLE_LNOM,MED_TAILLE_LNOM);
			  *(newindfamgro+nfamn+nfame) = *(newindfamgro+nfamn+nfame)
			    + MED_TAILLE_LNOM;
			}
		    }
		  if (estfam0 == 0)
		    {
		      newnumele = -1;
		      *(newnumfamele+i) = newnumele;
		      *(newnumfam+nfamn+nfame-1) = newnumele;
		    }
		  else
		    {
		      newnumele = 0;
		      *(newnumfam+nfamn+nfame-1) = newnumele;
		      existfam0 = 1;
		    }
		}
	    }
	  else
	    {
	      for (j=0;j<nfame;j++)
		{
		  p = famele + nge*j;
		  for (k=0;k<nge;k++)
		    {
		      if (*(p+k) != *(tmp+k))
			{
			  exist = 0;
			  break;
			}
		      else
			exist = 1;
		    }
		  if (exist == 1)
		    {
		      if (estfam0 == 0)
			*(newnumfamele+i) = *(newnumfam+nfamn+j);
		      break;
		    }
		}
	      if (exist == 0 && !(estfam0 && existfam0))
		/* on cree une nouvelle famille */
		{
		  nfame = nfame + 1;
		  *(newindfamgro+nfamn+nfame) = *(newindfamgro+nfamn+nfame-1);
		  p = famele;
		  if ((famele = (med_int*) malloc(sizeof(med_int)*nge*nfame))
		      == NULL)
		    return -1;
		  for (j=0;j<nfame-1;j++)
		    for (k=0;k<nge;k++)
		      *(famele+j*nge+k) = *(p+j*nge+k);
		  free(p);
		  p = famele+(nfame-1)*nge;
		  for (j=0;j<nge;j++)
		    {
		      tmp1 = *(tmp+j);
		      *(p+j) = tmp1;
		      if (tmp1 == 1)
			{
			  strncpy((newfamgro+*(newindfamgro+nfamn+nfame)), 
				  nomgroele+j*MED_TAILLE_LNOM,MED_TAILLE_LNOM);
			  *(newindfamgro+nfamn+nfame) =
			    *(newindfamgro+nfamn+nfame) + MED_TAILLE_LNOM;
			}
		    }
		  if (estfam0 == 0)
		    {
		      newnumele = newnumele - 1;
		      *(newnumfamele+i) = newnumele;
		      *(newnumfam+nfamn+nfame-1) = newnumele;
		    }
		  else
		    if (existfam0 == 0)
		      {
			*(newnumfam+nfamn+nfame-1) = 0;
			existfam0 =1;
		      }
		}
	    }
	  free(tmp);
	}
      
      *(newfamgro+MED_TAILLE_LNOM*nindf) = '\0';

      free(famnoe);
      free(famele);
    }
  else
    {
      *newnumfam = 0;
      for (i=0;i<nele;i++)
	*(newnumfamele+i) = 0;
      for (i=0;i<nnoe;i++)
	*(newnumfamnoe+i) = 0;
    }
  
  return 0;
}

}
