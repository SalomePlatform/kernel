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
 * FONCTION MEDfam2groA
 * 
 * - DESCRIPTION : 1ere etape dans la conversion des familles de groupes
 *      MED en goupes de noeuds et d'elements.
 *      Calcul des tailles des tableaux a allouer pour stocker les
 *      groupes que l'on veut creer.
 *      Les parametres renvoyes sont :
 *      1 - le nombre de groupes de noeuds a creer (ngn)
 *      2 - le nombre de groupes d'elements (nge)
 *      3 - le nombre total de noeuds composant l'ensemble des groupes
 *          de noeuds (nindn)
 *      4 - le nombre total d'elements composant l'ensemble des groupes
 *          d'elements (ninde)
 *      Ces parametres doivent permettre de creer les tables suivantes :
 *      1 - une table de taille (nindn) contenant pour chaque groupe
 *          de noeuds la liste des noeuds le composant. Cette table
 *          sera indexee par une table de taille (ngn) qui contiendra
 *          pour chaque noeud un numero d'indice. Une table de taille
 *          (ngn) qui contiendra la liste des noms des differents
 *          groupes de noeuds.
 *      2 - idem pour les elements
 *      Le remplissage de ces tables est realise par la fonction 
 *      MEDfam2groB().
 *
 * - PARAMETRES :
 *   NOM            .E/S. TYPE    .  DESCRIPTION
 *   ------------------------------------------------------------------- 
 *   nfam           .E  . med_int . nombre de familles
 *   numfam         .E  . med_int*. table des numeros de familles
 *   numfamnoe      .E  . med_int*. table des numeros de familles
 *                  .   .         . des noeuds
 *   nnoeuds        .E  . med_int . nombre de noeuds
 *   numfamele      .E  . med_int*. table des numeros de familles
 *                  .   .         . des elements
 *   nelememts      .E  .         . nombre total d'elements
 *   grofam         .E  . char*   . liste des groupes de familles
 *   indfamgro      .E  . int*    . liste des indices des groupes
 *                  .   .         . de familles dans grofam
 *   ngn            .  S. med_int*. nombre de groupes de noeuds a
 *                  .             . creer
 *   nge            .  S. med_int*. nombre de groupes d'elements a
 *                  .             . creer
 *   nindn          .  S. med_int*. taille de la table
 *                  .             . des groupes de noeuds a creer
 *   ninde          .  S. med_int*. taille de la table
 *                  .             . des groupes d'elements
 *
 * - RESULTAT : 0 si succes et -1 sinon
 * 
 ***********************************************************************/

namespace med_2_1{

med_err 
MEDfam2groA (med_int nfam,med_int *numfam,med_int *numfamnoe, 
	     med_int nnoeuds,med_int *numfamele,med_int nelements, 
	     char *grofam,int *indfamgro, 
	     med_int *ngn,med_int *nge,med_int *nindn,med_int *ninde)
{
  int i,j,k;
  char groupe[MED_TAILLE_LNOM];
  char *nomgronoe,*nomgroele,*tmp;
  med_int numc;
  int nnoe = 0,nele = 0;
  int flag = 0;

  *ngn = 0;
  *nge = 0;
  *nindn = 0;
  *ninde = 0;
  
  tmp = NULL;
  nomgronoe = NULL;
  nomgroele = NULL;

  /* Pour chaque famille, on regarde s'il y a de nouveaux groupes
     de noeuds ou d'elements a creer. Pour chaque nouveau groupe,
     on compte le nombre de noeuds ou d'elements qui devront lui etre
     rataches */
  for (i=1;i<=nfam;i++)
    if ((*(indfamgro+i)-*(indfamgro+i-1))/MED_TAILLE_LNOM > 0) 
      {
	/* on releve le numero de la famille courante */
        numc = *(numfam+i-1);
	nnoe = 0;
	nele = 0;
        /* si c'est une famille de noeuds, on compte le nombre de
           noeuds qui y sont rattaches */
        if (numc > 0)
	  for (j=0;j<nnoeuds;j++)
	    if (*(numfamnoe+j) == numc)
	      nnoe++;
        /* si c'est une famille d'elements, on compte le nombre d'elements
           qui y sont rattaches */
	if (numc < 0)
	  for (j=0;j<nelements;j++)
	    if (*(numfamele+j) == numc)
	      nele++; 	  
        /* on parcourt la liste des groupes de la famille et pour chaque
           groupe :
	   1 - on met a jour les compteurs nindn et ninde ;
           2 - on verifie s'il s'agit d'un groupe deja repertorie.
               Si c'est le cas on ne fait rien, sinon on met a jour les
               compteurs ngn ou nge */
        for (j=0;j<(*(indfamgro+i)-*(indfamgro+i-1))/MED_TAILLE_LNOM;j++)
	  {
	    strncpy(groupe,grofam+*(indfamgro+i-1)+j*MED_TAILLE_LNOM,
		    MED_TAILLE_LNOM);
	    if (numc > 0)
	      {
                *nindn = *nindn+nnoe;
		if (*ngn == 0)
		  {
		    *ngn = 1;
		    if ((nomgronoe=(char*)malloc(sizeof(char)*MED_TAILLE_LNOM))
			 == NULL)
		      return -1;
		    strncpy(nomgronoe,groupe,MED_TAILLE_LNOM);
		  }
		else
		  { 
                    flag = 0;
		    for (k=0;k<(*ngn);k++)
		      if (strncmp(groupe,nomgronoe+k*MED_TAILLE_LNOM,
				  MED_TAILLE_LNOM) == 0)
			flag = 1;
		    if (flag == 0)
		      { 
			*ngn = *ngn + 1;
			if ((tmp=(char*)malloc(sizeof(char)*
					       MED_TAILLE_LNOM**ngn)) == NULL)
			  return -1;
			strncpy(tmp,nomgronoe,MED_TAILLE_LNOM*(*ngn-1));
			strncpy(tmp+MED_TAILLE_LNOM*(*ngn-1),groupe, 
				MED_TAILLE_LNOM);
			free(nomgronoe); 
			nomgronoe = tmp;
		      }
		  } 
	      } 
	    if (numc < 0)
	      {
                *ninde = *ninde+nele;
		if (*nge == 0)
		  {
		    *nge = 1;
		    if ((nomgroele=(char *)malloc(sizeof(char)*
						  MED_TAILLE_LNOM)) == NULL)
		      return -1;
		    strncpy(nomgroele,groupe,MED_TAILLE_LNOM);
		  }
		else
		  { 
		    flag = 0;
		    for (k=0;k<(*nge);k++)
		      if (strncmp(groupe,nomgroele+k*MED_TAILLE_LNOM, 
				  MED_TAILLE_LNOM) == 0)
			flag = 1;
		    if (flag == 0)
		      {
			*nge = *nge + 1;
			if ((tmp = (char*) malloc(sizeof(char)*MED_TAILLE_LNOM*
						  *nge)) == NULL)
			  return -1;
			strncpy(tmp,nomgroele,MED_TAILLE_LNOM*(*nge-1));
			strncpy(tmp+MED_TAILLE_LNOM*(*nge-1), groupe, 
				MED_TAILLE_LNOM);
			free(nomgroele);
			nomgroele = tmp;
		      }
		  }
	      } 
	  } 
      }

  /* nettoyage memoire */
  free(nomgronoe);
  free(nomgroele);
  
  return 0;
}

}
