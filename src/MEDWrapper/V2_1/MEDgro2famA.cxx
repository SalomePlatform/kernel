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

#include "med.hxx"

/***********************************************************************
 * FONCTION MEDgro2famA
 * 
 * - DESCRIPTION : 1ere etape dans la conversion des groupes de noeuds
 *      et d'elements en familles MED. 
 *      Calcul des tailles des tableaux que l'on devra allouer pour
 *      stocker les familles qui seront construites par MEDgro2famB().
 *      Les parametres renvoyes sont :
 *      1 - le nombre de familles MED a creer (nfamg)
 *      2 - le nombre de noms groupes associes a l'ensemble des familles
 *      MED (nindf)
 *      Ces parametres doivent permettre a l'appelant de creer les tables
 *      suivantes : 
 *      1 - une table des noms des groupes propres a chaque famille,
 *      de taille : nindf*MED_TAILLE_LNOM+1
 *      2 - une table d'index donnant pour chaque famille son numero d'indice
 *      dans la table des noms, de taille : nfamg+1
 *      3 - une table destinee a contenir la liste des numeros de familles
 *          a creer, de taille : nfamg
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
 *   indgronoe      .E  . int*    . table index de la table des groupes
 *                  .   .         . de noeuds
 *   indgroele      .E  . int*    . table index de la table des groupes
 *                  .   .         . d'elements
 *   tabgronoe      .E  . med_int*. table des groupes de noeuds
 *   tabgroele      .E  . med_int*. table des groupes d'elements
 *   nfamg          .  S. med_int*. nombre de familles MED a creer
 *   nidnf          .  S. med_int*. nombre de noms groupes associes a 
 *                  .   .         . l'ensemble des familles MED
 *
 * - RESULTAT : 0 si succes, -1 sinon
 * 
 ***********************************************************************/

namespace med_2_1{

med_err 
MEDgro2famA (med_int nnoe,med_int nele,med_int *numnoe,med_int *numele,
	     med_int ngn,med_int nge,med_int nindn, 
	     med_int ninde,int *indgronoe,int *indgroele,med_int *tabgronoe, 
	     med_int *tabgroele,med_int *nfamg,med_int *nindf)
{
  int i,j,k;
  int *famnoe,*famele,*tmp;
  int *p;
  int flag, num,exist;
  int nfamn, nfame;
  int fam01 = 0;
  int fam02 = 0;

  /* initialisations */
  famnoe = NULL;
  famele = NULL;

  *nfamg = 0;
  *nindf = 0;
  nfamn = 0;
  nfame = 0;

  if ((ngn > 0) || (nge > 0))
    {
      /* pour chaque noeud :
	 1 - on dresse la liste des groupes de noeuds auquel il appartient
	 2 - en la comparant avec les listes pre-existantes, on
         estime s'il est necessaire de creer une nouvelle famille de noeuds.
	 Si oui => on incremente le compteur local nfamn (nombre de familles
                   de noeuds)
                   on incremente le parametre nindf du nombre de groupes
                   que devra compter cette famille de noeuds
	 Si non => on ne fait rien */
      for (i=0;i<nnoe;i++)
	{
	  if ((tmp = (int*) malloc(sizeof(int)*ngn)) == NULL)
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
	  /* on note la creation de la famille 0 */
	  if (fam01 == 0)
	    {
	      flag = 1;
	      for (j=0;j<ngn;j++)
		if (*(tmp+j) == 1)
		  flag = 0;
	      if (flag == 1)
		fam01 = 1;
	    }
	  /* faut-il creer une nouvelle famille ? */
	  if (famnoe == NULL)
	    {
	      exist = 0;
	      if ((famnoe = (int *) malloc (sizeof(int)*ngn)) == NULL)
		return -1;
	      for (j=0;j<ngn;j++)
		{
		  *(famnoe+j) = *(tmp+j);
		  if (*(famnoe+j) == 1)
		    *nindf = *nindf + 1;
		}
	      nfamn = 1;
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
		    break;
		}
	      if (exist == 0)
		{
		  nfamn = nfamn + 1;
		  p = famnoe;
		  if ((famnoe = (int*) malloc(sizeof(int)*ngn*nfamn)) == NULL)
		    return -1;
		  for (j=0;j<nfamn-1;j++)
		    for (k=0;k<ngn;k++)
		      *(famnoe+j*ngn+k) = *(p+j*ngn+k);
		  free(p);
		  p = famnoe+(nfamn-1)*ngn;
		  for (j=0;j<ngn;j++)
		    {
		      *(p+j) = *(tmp+j);
		      if (*(p+j) == 1)
			*nindf = *nindf + 1;
		    }
		}
	    }
	  free(tmp);
	}
      
      /* pour chaque element : idem que pour les noeuds */
      for (i=0;i<nele;i++)
	{
	  if ((tmp = (int*) malloc(sizeof(int)*nge)) == NULL)
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
	  /* on note la creation de la famille 0 */
	  if (fam02 == 0)
	    {
	      flag = 1;
	      for (j=0;j<nge;j++)
		if (*(tmp+j) == 1)
		  flag = 0;
	      if (flag == 1)
		fam02 = 1;
	    }
	  /* faut-il creer une nouvelle famille ? */
	  if (famele == NULL)
	    {
	      exist = 0;
	      if ((famele = (int *) malloc (sizeof(int)*nge)) == NULL)
		return -1;
	      for (j=0;j<nge;j++)
		{
		  *(famele+j) = *(tmp+j);
		  if (*(famele+j) == 1)
		    *nindf = *nindf + 1;
		}
	      nfame = 1;
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
		    break;
		}
	      if (exist == 0)
		{
		  nfame = nfame + 1;
		  p = famele;
		  if ((famele = (int*) malloc(sizeof(int)*nge*nfame)) == NULL)
		    return -1;
		  for (j=0;j<nfame-1;j++)
		    for (k=0;k<nge;k++)
		      *(famele+j*nge+k) = *(p+j*nge+k);
		  free(p);
		  p = famele+(nfame-1)*nge;
		  for (j=0;j<nge;j++)
		    {
		      *(p+j) = *(tmp+j);
		      if (*(p+j) == 1)
			*nindf = *nindf + 1;
		    }
		}
	    }
	  free(tmp);
	}
      
      /* la famille 0 existe pour les noeuds et les elements, on 
	 ne la compte qu'une fois */
      if (fam01 && fam02)
	nfamn = nfamn - 1;
      
      /* le nombre de familles a creer est egal au nombre de familles
	 de noeuds + nombre de familles d'elements */
      *nfamg = nfamn + nfame;
      
      
      /* Nettoyage memoire */
      free(famnoe);
      free(famele);
    }  
  else
    {
      /* on a aucun groupes de noeuds ou d'elements */
      *nfamg = 1; /* on a au moins la famille 0 */
      *nindf = 0;
    }

  return 0;
}

}
