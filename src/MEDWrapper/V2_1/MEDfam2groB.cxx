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
 * FONCTION MEDfam2groB
 * 
 * - DESCRIPTION : 2e etape dans la conversion des familles MED en 
 *      groupes de noeuds et d'elements. Les tables allouees a partir
 *      des parametres calcules par MEDfam2groA() sont passees en argument
 *      a MEDfam2groB et remplies par cette derniere fonction.
 *      Il s'agit de :
 *      1 - la table des noms de groupes de noeuds, chaque nom ayant
 *          une taille de MED_TAILLE_LNOM
 *      2 - la table des noms des groupes d'elements
 *      3 - la table des groupes de noeuds -i.e. pour chaque groupe
 *          la liste des numeros des noeuds qui le composent
 *      4 - la table d'indexation de la table des groupes de noeuds
 *      5 - la table des groupes d'elements
 *      6 - la table d'indexation de la table des groupes d'elements
 *
 * - PARAMETRES :
 *   NOM            .E/S. TYPE    . DESCRIPTION
 *   ------------------------------------------------------------------- 
 *   nfam           .E  . med_int . nombre de familles
 *   numfam         .E  . med_int*. table des numeros de familles
 *   numfamnoe      .E  . med_int*. table des numeros de familles
 *                  .   .         . des noeuds
 *   nnoeuds        .E  . med_int . nombre de noeuds
 *   numfamele      .E  . med_int*. table des numeros de familles
 *                  .   .         . des elements
 *   nelememts      .E  . med_int . nombre total d'elements
 *   grofam         .E  . char*   . liste des groupes de familles
 *   indfamgro      .E  . int*    . liste des indices des groupes
 *                  .   .         . de familles dans indfamgro
 *   numnoeuds      .E  . med_int*. numeros des noeuds
 *   numele         .E  . med_int*. numeros des elements
 *   ngn            .E  . med_int . nombre de groupes de noeuds
 *   nge            .E  . med_int . nombre de groupes d'elements
 *   nindn          .E  . med_int . nombre d'indices dans la table
 *                  .             . des groupes de noeuds a creer
 *   ninde          .E  . med_int . nombre d'indices dans la table
 *                  .             . des groupes d'elements
 *   nomgronoe      .  S. char*   . noms des groupes de noeuds
 *   nomgroele      .  S. char*   . noms des groupes d'elements
 *   indgronoe      .  S. int*    . indices des groupes de noeuds
 *   indgroele      .  S. int*    . indices des groupes d'elements
 *   tabgronoe      .  S. med_int*. table des groupes de noeuds
 *   tabgroele      .  S. med_int*. table des groupes d'elements
 *
 * - RESULTAT : 0
 * 
 ***********************************************************************/

namespace med_2_1{

med_err 
MEDfam2groB(med_int nfam,med_int *numfam,med_int *numfamnoe,
	    med_int nnoeuds,med_int *numfamele,med_int nelements, 
	    char *grofam,int *indfamgro,med_int *numnoeuds,
	    med_int *numele,med_int ngn,med_int nge,med_int nindn, 
	    med_int ninde,char *nomgronoe,char *nomgroele,
	    int *indgronoe,int *indgroele,
	    med_int *tabgronoe,med_int *tabgroele)
{
  int i,j,k;
  char groupe[MED_TAILLE_LNOM];
  med_int numc;
  int nnoe = 0, nele = 0;
  int flag = 0;
  int nn = 0, ne = 0;
  int pos, cpt;

  /* initialisations */
  for (i=0;i<=ngn;i++)
    *(indgronoe+i) = 0;
  for (i=0;i<=nge;i++)
    *(indgroele+i) = 0;

  /* 1ere passe : on passe en revue toutes les familles :
     1 - on etablit dans (nomgronoe) et dans (nomgroele) les listes 
     des noms de groupes de noeuds et d'elements
     2 - on place dans les tables d'index (indgronoe) et (indgroele)
     le nombre de noeuds ou d'elements que chaque groupe se verra 
     attribuer */
  for (i=1;i<=nfam;i++)
      {
        numc = *(numfam+i-1);
	nnoe = 0;
	nele = 0;
        if (numc > 0)
	    for (j=0;j<nnoeuds;j++)
	      if (*(numfamnoe+j) == numc)
		nnoe++;
	if (numc < 0)
	  for (j=0;j<nelements;j++)
	    if (*(numfamele+j) == numc)
	      nele++; 	  
        for (j=0;j<(*(indfamgro+i)-*(indfamgro+i-1))/MED_TAILLE_LNOM; j++)
	  {
	    strncpy(groupe, grofam+*(indfamgro+i-1)+j*MED_TAILLE_LNOM,
		    MED_TAILLE_LNOM);
	    if (numc > 0)
	      {
                if (nn == 0)
		  {
		    strncpy(nomgronoe,groupe,MED_TAILLE_LNOM);
		    nn = 1;
                    pos = 1;
		  }
		else
		  {
		    flag = 0;
		    for (k=0; k<nn;k++)
		      if (strncmp(groupe,nomgronoe+k*MED_TAILLE_LNOM,
				  MED_TAILLE_LNOM) == 0)
			{
			  flag = 1;
			  pos = k+1;
			}
		    if (flag == 0)
		      {
			strncpy(nomgronoe+nn*MED_TAILLE_LNOM,groupe, 
				MED_TAILLE_LNOM);
			pos = nn + 1;
			nn = nn + 1;
		      }
		  }
		*(indgronoe+pos) = *(indgronoe+pos) + nnoe;
	      } 
	    if (numc < 0)
	      {
                if (ne == 0)
		  {
		    strncpy(nomgroele,groupe,MED_TAILLE_LNOM);
		    ne = 1;
		    pos = 1;
		  }
		else
		  {
		    flag = 0;
		    for (k=0; k<ne;k++)
		      if (strncmp(groupe,nomgroele+k*MED_TAILLE_LNOM,
				  MED_TAILLE_LNOM) == 0)
			{
			  flag = 1;
			  pos = k + 1;
			}
		    if (flag == 0)
		      {
			strncpy(nomgroele+ne*MED_TAILLE_LNOM,groupe, 
				MED_TAILLE_LNOM);
			pos = ne + 1;
			ne = ne + 1;
		      }
		  }
		*(indgroele+pos) = *(indgroele+pos) + nele;
	      }
	  } 
      }
  *(nomgronoe+ngn*MED_TAILLE_LNOM) = '\0';
  *(nomgroele+nge*MED_TAILLE_LNOM) = '\0';

  /* 2e passe : on construit les listes des index ainsi que les
     les tables des groupes */
  for (i=1;i<=ngn;i++)
    {
      cpt = 0;
      *(indgronoe+i) = *(indgronoe+i-1) + *(indgronoe+i);
      strncpy(groupe,nomgronoe+(i-1)*MED_TAILLE_LNOM,MED_TAILLE_LNOM);
      for (j=1;j<=nfam;j++)
	  {
	    numc = *(numfam+j-1);
	    if (numc > 0)
	      {
		flag = 0;
		for (k=0;k<(*(indfamgro+j)-*(indfamgro+j-1))/MED_TAILLE_LNOM;
		     k++)
		  if (! strncmp(groupe,
				grofam+*(indfamgro+j-1)+k*MED_TAILLE_LNOM,
				MED_TAILLE_LNOM))
		    flag = 1;
		if (flag == 1)
		  for (k=0;k<nnoeuds;k++)
		    if (*(numfamnoe+k) == numc)
		      {
			*(tabgronoe+*(indgronoe+i-1)+cpt) = *(numnoeuds+k);
			cpt++;
		      }
	      }
	  }
    }	      
  
  for (i=1;i<=nge;i++)
    {
      cpt = 0;
      *(indgroele+i) = *(indgroele+i-1) + *(indgroele+i);  
      strncpy(groupe,nomgroele+(i-1)*MED_TAILLE_LNOM,MED_TAILLE_LNOM);
      for (j=1;j<=nfam;j++)
	  {
	    numc = *(numfam+j-1);
	    if (numc < 0)
	      {
		flag = 0;
		for (k=0;k<(*(indfamgro+j)-*(indfamgro+j-1))/MED_TAILLE_LNOM;
		     k++)
		  if (! strncmp(groupe,
				grofam+*(indfamgro+j-1)+k*MED_TAILLE_LNOM,
				MED_TAILLE_LNOM))
		    flag = 1;
		if (flag == 1)
		  for (k=0;k<nelements;k++)
		    if (*(numfamele+k) == numc)
		      {
			*(tabgroele+*(indgroele+i-1)+cpt) = *(numele+k);
			cpt++;
		      }
	      }
	  }
    }
  
  return 0;
}

}
