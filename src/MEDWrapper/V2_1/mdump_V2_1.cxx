/******************************************************************************
 * - Nom du fichier : mdump.c
 *
 * - Description : utilitaire de dump pour fichier MED V2.1
 *
 *****************************************************************************/
#define NBR_MAILLE_EQU 7

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "med.hxx"
using namespace med_2_1;

/*******************************************************************
 *  DUMP D'UN FICHIER MED STRUCTURE EN GRILLE :                    *
 *  NOEUDS, ELEMENTS, FAMILLES, EQUIVALENCES, CHAMPS DE RESULTATS  *
 *******************************************************************/

int grid_cartesian_or_polar(med_idt fid, int numero, med_mode_switch mode_coo) {
    med_int mdim, nnoe, nfam, i, j;
    char nommaa[MED_TAILLE_NOM+1];
    char nom_universel[MED_TAILLE_LNOM+1];
    med_float *coo;
    med_int *fam;
    char nomcoo[3*MED_TAILLE_PNOM+1];
    char unicoo[3*MED_TAILLE_PNOM+1];
    med_repere rep;
    char str[MED_TAILLE_PNOM+1];

    fprintf(stdout,"\n(****************************)\n");
    fprintf(stdout,"(* INFORMATIONS GENERALES : *)\n");
    fprintf(stdout,"(****************************)\n");

    /* lecture du nom et de la dimension de la grille */
    if (MEDmaaInfo(fid, numero, nommaa, &mdim) < 0) {
        fprintf(stderr, ">> ERREUR : lecture du nom de la grille\n");
        return(EXIT_FAILURE);
    };
    fprintf(stdout, "- Nom de la grille : <<%s>>\n", nommaa);
    fprintf(stdout, "- Dimension de la grille : %d\n", mdim);

    /* lecture du nom universel (presence optionnelle) */
    if (MEDunvLire(fid, nommaa, nom_universel) > 0) {
        fprintf(stdout, "- Nom universel de la grille : %s \n", nom_universel);
    } else {
        fprintf(stdout, "- Pas de nom universel \n");
    };

    /* nombre de familles */
    nfam = MEDnFam(fid, nommaa, 0, (med_dim_famille)0);
    if (nfam < 0) {
        fprintf(stderr, ">> ERREUR : lecture du nombre de familles\n");
        return(EXIT_FAILURE);
    };
    fprintf(stdout, "- Nombre de familles : %d \n", nfam);

    fprintf(stdout, "\n- Lecture des indices : \n");
    for (i=0; i<mdim; i++) {
        fprintf(stdout, "-- Lecture de l'indice : %d\n", i);
        nnoe = MEDnGrid(fid, nommaa, (med_grid)i);
        fprintf(stdout, "-- nombre d'indice : %d\n", nnoe);
        coo  = (med_float*)malloc(sizeof(med_float)*nnoe);

        if (MEDgridLire(fid, nommaa, mdim, coo, i, mode_coo, &rep, nomcoo, unicoo) < 0) {
            fprintf(stderr, ">> ERREUR : lecture des indices \n");
            return(EXIT_FAILURE);
        };

        fprintf(stdout, "- Type de repere des coordonnees : %d \n", rep);

        fprintf(stdout, "- Nom des coordonnees : \n");
        for (j=0; j<mdim; j++) {
            strncpy(str, nomcoo+j*MED_TAILLE_PNOM, MED_TAILLE_PNOM);
            str[MED_TAILLE_PNOM] = '\0';
            fprintf(stdout, " %s ", str);
        };

        fprintf(stdout, "\n- Unites des coordonnees : \n");
        for (j=0; j<mdim; j++) {
            strncpy(str, unicoo+j*MED_TAILLE_PNOM, MED_TAILLE_PNOM);
            str[MED_TAILLE_PNOM] = '\0';
            fprintf(stdout, " %s ", str);
        };

        fprintf(stdout, "\n-- Coordonnees des indices : \n");
        for (j=0; j<nnoe; j++) {
            fprintf(stdout, "   %f ", *(coo+j));
        };

        fprintf(stdout, "\n\n");
        free(coo);
    };

    nfam = MEDnGrid(fid, nommaa, (med_grid)-2);
    fprintf(stdout, "- Nombre de noeud pour les familles : %d\n", nfam);
    if (nfam > 0) {
        fam = (med_int*)malloc(sizeof(med_int)*nfam);
        if (MEDfamGridLire(fid, nommaa, fam, nfam,MED_NOEUD) < 0) {
            fprintf(stderr, ">> ERREUR : lecture des familles\n");
            return(EXIT_FAILURE);
        };

        fprintf(stdout, "\n- Numeros des familles des noeuds :\n");
        for (i=0; i<nfam; i++) {
            fprintf(stdout, " %d ", *(fam+i));
        };
        fprintf(stdout, "\n");
        free(fam);
    };

    return(0);
}

int grid_body_fitted(med_idt fid, int numero, med_mode_switch mode_coo) {
    med_int mdim, nnoe, nfam, i;
    char nommaa[MED_TAILLE_NOM+1];
    char nom_universel[MED_TAILLE_LNOM+1];
    char str[MED_TAILLE_PNOM+1];
    char nomcoo[3*MED_TAILLE_PNOM+1];
    char unicoo[3*MED_TAILLE_PNOM+1];
    med_float *coo;
    med_int *fam;
    med_repere rep;

    fprintf(stdout,"\n(****************************)\n");
    fprintf(stdout,"(* INFORMATIONS GENERALES : *)\n");
    fprintf(stdout,"(****************************)\n");

    /* lecture du nom et de la dimension du maillage */
    if (MEDmaaInfo(fid, numero, nommaa, &mdim) < 0) {
        fprintf(stderr, ">> ERREUR : lecture du nom du maillage body fitted\n");
        return(EXIT_FAILURE);
    };
    fprintf(stdout, "- Nom du maillage body fitted : <<%s>>\n", nommaa);
    fprintf(stdout, "- Dimension du maillage body fitted : %d\n", mdim);

    /* lecture du nom universel (presence optionnelle) */
    if (MEDunvLire(fid, nommaa, nom_universel) > 0) {
        fprintf(stdout, "- Nom universel du maillage body fitted : %s \n", nom_universel);
    } else {
        fprintf(stdout, "- Pas de nom universel \n");
    };

    /* Combien de noeuds ? */
    /*    nnoe = MEDnGrid(fid, nommaa, -1);*/
    nnoe = MEDnGrid(fid, nommaa, MED_GRID_NOEUD);
    if (nnoe < 0) {
        fprintf(stderr, ">> ERREUR : lecture du nombre de noeuds (via MEDnGrid) \n");
        return(EXIT_FAILURE);
    };
    fprintf(stdout, "- Nombre de noeuds : %d \n", nnoe);

    /* Combien de noeuds dans la dimension 1 ? */
    i = MEDnGrid(fid, nommaa, (med_grid)0);
    if (i < 0) {
        fprintf(stderr, ">> ERREUR : lecture du nombre de noeuds (via MEDnGrid) dans la dimension 1 \n");
        return(EXIT_FAILURE);
    };
    fprintf(stdout, "- Nombre de noeuds dans la dimension 1 : %d \n", i);

    /* Combien de noeuds dans la dimension 2 ? */
    i = MEDnGrid(fid, nommaa, (med_grid)1);
    if (i < 0) {
        fprintf(stderr, ">> ERREUR : lecture du nombre de noeuds (via MEDnGrid) dans la dimension 2 \n");
        return(EXIT_FAILURE);
    };
    fprintf(stdout, "- Nombre de noeuds dans la dimension 2 : %d \n", i);

    /* nombre de familles */
    nfam = MEDnFam(fid, nommaa, 0,(med_dim_famille)0);
    if (nfam < 0) {
        fprintf(stderr, ">> ERREUR : lecture du nombre de familles \n");
        return(EXIT_FAILURE);
    };
    fprintf(stdout, "- Nombre de familles : %d \n", nfam);

    coo = (med_float*)malloc(sizeof(med_float)*nnoe*mdim);
    fam = (med_int*)malloc(sizeof(med_int)*nnoe);

    if (MEDbodyFittedLire(fid, nommaa, mdim, coo, mode_coo, &rep, nomcoo, unicoo, fam, nnoe) < 0) {
        fprintf(stderr, ">> ERREUR : lecture des noeuds \n");
        return(EXIT_FAILURE);
    };

    fprintf(stdout, "- Type de repere des coordonnees : %d \n", rep);

    fprintf(stdout, "- Nom des coordonnees : \n");
    for (i=0; i<mdim; i++) {
        strncpy(str, nomcoo+i*MED_TAILLE_PNOM, MED_TAILLE_PNOM);
        str[MED_TAILLE_PNOM] = '\0';
        fprintf(stdout, " %s ", str);
    };

    fprintf(stdout, "\n- Unites des coordonnees : \n");
    for (i=0; i<mdim; i++) {
        strncpy(str, unicoo+i*MED_TAILLE_PNOM, MED_TAILLE_PNOM);
        str[MED_TAILLE_PNOM] = '\0';
        fprintf(stdout, " %s ", str);
    };

    fprintf(stdout, "\n- Coordonnees des noeuds : \n");
    for (i=0; i<nnoe*mdim; i++) {
        fprintf(stdout, " %f ", *(coo+i));
    };

    fprintf(stdout, "\n- Numeros des familles des noeuds : \n");
    for (i=0; i<nnoe; i++) {
        fprintf(stdout, " %d ", *(fam+i));
    };
    fprintf(stdout, "\n");

    if (nnoe) {
        free(coo);
        free(fam);
    };
    return(0);
}

int grid(med_idt fid, int numero, med_grid_type theType, med_mode_switch mode_coo) {
    switch (theType) {
        case MED_CARTESIAN : {
            fprintf(stdout, "- Type de la grille : MED_CARTESIAN\n");
            return(grid_cartesian_or_polar(fid, numero, mode_coo));
        };
        case MED_POLAR : {
            fprintf(stdout, "- Type de la grille : MED_POLAR\n");
            return(grid_cartesian_or_polar(fid, numero, mode_coo));
        };
        case MED_BODY_FITTED : {
            fprintf(stdout, "- Type de la grille : MED_BODY_FITTED\n");
            return(grid_body_fitted(fid, numero, mode_coo));
        };
        default : {
            fprintf(stderr, ">> ERREUR : type de maillage inconnu\n");
            return(EXIT_FAILURE);
        };
    };
}

/******************************************************************************
 * DUMP D'UN FICHIER MED  : NOEUDS,
 * ELEMENTS, FAMILLES, EQUIVALENCES, CHAMPS DE RESULTATS
 *****************************************************************************/

int main (int argc, char **argv)
{
  med_err ret = 0;
  med_idt fid;
  int i,j,k,l,kp;
  int numero;
  char message[200];
  /* nombre d'objets MED */
  char nom_universel[MED_TAILLE_LNOM+1];
  med_int long_fichier_en_tete; 
  char *fichier_en_tete;
  char version_hdf[10];
  char version_med[10];
  med_int nmaa,mdim,nnoe;
  med_int nmai[MED_NBR_GEOMETRIE_MAILLE],nfac[MED_NBR_GEOMETRIE_FACE];
  med_int nare[MED_NBR_GEOMETRIE_ARETE];
  /* nom du maillage */
  char nommaa[MED_TAILLE_NOM+1];
  /* noeuds */
  med_float *coo;
  char nomcoo[3*MED_TAILLE_PNOM+1];
  char unicoo[3*MED_TAILLE_PNOM+1];
  char *nomnoe;
  med_int *numnoe;
  med_int *nufano; 
  med_repere rep;
  med_booleen inonoe,inunoe;
  med_mode_switch mode_coo;
  char str[MED_TAILLE_PNOM+1];
  /* elements */
  med_int nsup;
  med_int edim;
  med_int taille;
  med_int *connectivite;
  char *nomele;
  med_int *numele;
  med_int *nufael;
  med_booleen inoele, inuele;
  med_connectivite typ_con;
  med_geometrie_element typgeo;
  med_geometrie_element typmai[MED_NBR_GEOMETRIE_MAILLE] = {MED_POINT1,MED_SEG2, 
						   MED_SEG3,MED_TRIA3,
						   MED_TRIA6,MED_QUAD4,
						   MED_QUAD8,MED_TETRA4,
						   MED_TETRA10,MED_HEXA8,
						   MED_HEXA20,MED_PENTA6,
						   MED_PENTA15,MED_PYRA5,
						   MED_PYRA13};
  med_int desmai[MED_NBR_GEOMETRIE_MAILLE] = {0,2,3,3,3,4,4,4,4,6,6,5,5,5,5};
  med_int nmailles[MED_NBR_GEOMETRIE_MAILLE];
  char nommai[MED_NBR_GEOMETRIE_MAILLE] [MED_TAILLE_NOM+1] = {"MED_POINT1",
							  "MED_SEG2", 
							  "MED_SEG3",
							  "MED_TRIA3",
							  "MED_TRIA6",
							  "MED_QUAD4",
							  "MED_QUAD8",
							  "MED_TETRA4",
							  "MED_TETRA10",
							  "MED_HEXA8",
							  "MED_HEXA20",
							  "MED_PENTA6",
							  "MED_PENTA15",
							  "MED_PYRA5",
							  "MED_PYRA13"};
  med_geometrie_element typfac[MED_NBR_GEOMETRIE_FACE] = {MED_TRIA3,MED_TRIA6,
						 MED_QUAD4,MED_QUAD8};
  med_int desfac[MED_NBR_GEOMETRIE_FACE] = {3,3,4,4};
  med_int nfaces[MED_NBR_GEOMETRIE_FACE];
  char nomfac[MED_NBR_GEOMETRIE_FACE][MED_TAILLE_NOM+1] = {"MED_TRIA3","MED_TRIA6",
						       "MED_QUAD4","MED_QUAD8"};
  med_geometrie_element typare[MED_NBR_GEOMETRIE_ARETE] = {MED_SEG2,MED_SEG3};
  med_int desare[MED_NBR_GEOMETRIE_ARETE] = {2,3};
  med_int naretes[MED_NBR_GEOMETRIE_ARETE];
  char nomare[MED_NBR_GEOMETRIE_ARETE] [MED_TAILLE_NOM+1] = {"MED_SEG2","MED_SEG3"};
  /* familles */
  med_int nfam;
  med_int natt,ngro;
  char *attdes,*gro;
  med_int *attval,*attide;
  char nomfam[MED_TAILLE_NOM+1];
  med_int numfam;
  char str1[MED_TAILLE_DESC+1];
  char str2[MED_TAILLE_LNOM+1];
  /* equivalences */
  med_int nequ,ncor;
  med_int *cor;
  char equ[MED_TAILLE_NOM+1];
  char des[MED_TAILLE_DESC+1];
  /* champs de resultats */
  char *comp;
  char *unit;
  char nomcha[MED_TAILLE_NOM+1];
  char maillage_champ[MED_TAILLE_NOM+1];
  med_int ncomp;
  med_float *valr;
  med_int *vale;
  med_type_champ typcha;
  med_int ncha;
  med_int nval;
  int reponse;
  int lecture_en_tete_seulement = 0;
  med_int npdt;
  med_int ngauss,numdt,numo;
  med_float dt;
  char dtunit[MED_TAILLE_PNOM+1];
  char pflnom[MED_TAILLE_NOM+1];
  med_int pflsize;
  med_int *pflval;
  med_int isGrid;
  med_grid_type theType;

  /****************************************************************************
  *                  TEST DU NOMBRE D'ARGUMENTS                               *
  *                  argument 1 = nom du fichier MED                          *
  ****************************************************************************/
  if (argc != 2 && argc != 5)
    {
      fprintf(stderr,">> ERREUR : nombre de parametres incorrects \n");
      exit(EXIT_FAILURE);
    }

  /****************************************************************************
  *                      OUVERTURE DU FICHIER EN LECTURE                      *
  ****************************************************************************/
  fid = MEDouvrir(argv[1],MED_LECT);
  if (fid < 0)
    {
      fprintf(stderr,">> ERREUR : ouverture du fichier %s \n",argv[1]);
      exit(EXIT_FAILURE);
    }


  /****************************************************************************
  *                     QUESTIONS PRELIMINAIRES                               *
  *    1. Mode d'affichage des coordonnees (entrelace ou non) ?               *
  *    2. Connectivite des elements (nodale ou descendante)                   *
  ****************************************************************************/
  fprintf(stdout,"\n >>>>>> DUMP DU FICHIER %s >>>>>>\n",argv[1]);

  /* en-tete du fichier (presence optionnelle) */
  long_fichier_en_tete = MEDlFichDes(fid);
  if (long_fichier_en_tete > 0)
    {
      fichier_en_tete = (char *) malloc(sizeof(char)*(long_fichier_en_tete+1));
      ret = MEDfichEntete(fid,MED_FICH_DES,fichier_en_tete);
      if (ret < 0)
	{
	  fprintf(stderr,">> ERREUR : lecture de l'en-tete du fichier \n");
	  exit(EXIT_FAILURE);
	}      
      fprintf(stdout,"- En-tete du fichier : %s \n",fichier_en_tete);
      free(fichier_en_tete);
    }
  /* versions hdf et med */
  ret = MEDfichEntete(fid,MED_HDF_VERSION,version_hdf);
  if (ret < 0)
    {
      fprintf(stderr,">> ERREUR : lecture du numero de version de HDF \n");
      exit(EXIT_FAILURE);
    }      
  ret = MEDfichEntete(fid,MED_VERSION,version_med);
  if (ret < 0)
    {
      fprintf(stderr,">> ERREUR : lecture du numero de version de MED \n");
      exit(EXIT_FAILURE);
    }     
  fprintf(stdout,"- Version de HDF utilisee : %s \n",version_hdf);
  fprintf(stdout,"- Version de MED utilisee : %s \n",version_med);

  if (argc == 2)
    {
      fprintf(stdout,"(*****************)\n");
      fprintf(stdout,"(* PARAMETRAGE : *)\n");
      fprintf(stdout,"(*****************)\n");
      fprintf(stdout,"- Mode d'affichage des coordonnées des noeuds ? \n");
      fprintf(stdout,"  1. Mode entrelacé : taper 1 \n"); 
      fprintf(stdout,"  2. Mode non entrelacé : taper 2 \n");
      reponse = 0;
      do
	{
	  fprintf(stdout,"  Reponse : ");
	  scanf("%d",&reponse);
	}
      while (reponse != 1 && reponse != 2);
      if (reponse == 1)
	mode_coo = MED_FULL_INTERLACE;
      else
	mode_coo = MED_NO_INTERLACE;
      
      fprintf(stdout,"- Connectivité des éléments ? \n");
      fprintf(stdout,"  1. Nodale : taper 1 \n"); 
      fprintf(stdout,"  2. Descendante : taper 2 \n");
      reponse = 0;
      do
	{
	  fprintf(stdout,"  Reponse : ");
	  scanf("%d",&reponse);
	}
      while (reponse != 1 && reponse != 2);
      if (reponse == 1)
	typ_con = MED_NOD;
      else
	typ_con = MED_DESC;
    }
  else
      {
	if (! strcmp(argv[3],"NODALE"))
	  typ_con = MED_NOD;
	if (! strcmp(argv[3],"DESCENDANTE"))	
	  typ_con = MED_DESC;
	
	if (!strcmp(argv[4],"NO_INTERLACE"))
	  mode_coo = MED_NO_INTERLACE;
	if (!strcmp(argv[4],"FULL_INTERLACE"))
	  mode_coo = MED_FULL_INTERLACE;
        if (! strcmp(argv[4],"LECTURE_EN_TETE_SEULEMENT"))
	  lecture_en_tete_seulement = 1;

      }

  /****************************************************************************
  *                      QUEL MAILLAGE LIRE                                   *
  ****************************************************************************/
  nmaa = MEDnMaa(fid);
  if (nmaa < 0)
    {
      fprintf(stderr,">> ERREUR : lecture du nombre de maillages \n");
      exit(EXIT_FAILURE);
    }
  
  /* Quel maillage lire ? */
  if (argc == 2)
    {
      fprintf(stdout,"- Il y a %d maillages dans ce fichier \n",nmaa);
      fprintf(stdout,"  Lequel voulez-vous lire (1|2|3|...|n) ?\n");
      do
	{
	  fprintf(stdout,"  Reponse : ");
	  scanf("%d",&numero);
	}
      while (numero > nmaa || numero <= 0);
    }
  else
    {
      numero = atoi(argv[2]);
      if (numero > nmaa || numero  <= 0)
	{
	  fprintf(stderr,">> ERREUR : il y a %d maillages dans ce fichier  \n",
		  nmaa);
	  exit(EXIT_FAILURE);
	}
    }

/*****************************************************************************
 *       QUELLE SORTE DE MAILLAGE : GRILLE OU PAS                            *
 *****************************************************************************/

    fprintf(stdout,"\n(**************************************************)\n");
    fprintf(stdout,"(* MAILLAGE STRUCTURE (GRILLE) OU NON STRUCTURE : *)\n");
    fprintf(stdout,"(**************************************************)\n");

    /* lecture de la sorte de maillage : structure ou pas */
    ret = MEDgridInfo(fid, numero, &isGrid, &theType);
    if (ret < 0) {
        fprintf(stderr, ">> ERREUR : lecture de la sorte de maillage (structure ou pas)\n");
        exit(EXIT_FAILURE);
    };
    fprintf(stdout, "- Sorte de maillage : %s\n", isGrid? "structure (grille)": "non structure");
    if (isGrid) {
        ret = grid(fid, numero, theType, mode_coo);
        if (ret == 0) {
            ret = MEDfermer(fid);
            if (ret == 0) {
                fprintf(stdout, "\n >>>>>> FIN DU DUMP DU FICHIER %s >>>>>>\n", argv[1]);
            } else {
                fprintf(stderr, ">> ERREUR : erreur a la fermeture du fichier %s\n", argv[1]);
            };
        };
        if (ret == 0) {
            return(0);
        } else {
            exit(EXIT_FAILURE);
        };
    };

  /****************************************************************************
  *                       NOMBRES D'OBJETS MED                                *
  ****************************************************************************/
  fprintf(stdout,"\n(****************************)\n");
  fprintf(stdout,"(* INFORMATIONS GENERALES : *)\n");
  fprintf(stdout,"(****************************)\n");

  /* lecture du nom et de la dimension du maillage */
  ret = MEDmaaInfo(fid,numero,nommaa,&mdim);
  if (ret < 0)
    {
      fprintf(stderr,">> ERREUR : lecture du nom du maillage \n");
      exit(EXIT_FAILURE);
    }
  fprintf(stdout,"- Nom du maillage : <<%s>>\n",nommaa);
  fprintf(stdout,"- Dimension du maillage : %d\n",mdim);

  /* lecture du nom universel (presence optionnelle) */
 ret = MEDunvLire(fid,nommaa,nom_universel);
 if (ret > 0)
   fprintf(stdout,"- Nom universel du maillage : %s \n",nom_universel);
 else
   fprintf(stdout,"- Pas de nom universel \n");
  
      
  /* Combien de noeuds ? */
  nnoe = MEDnEntMaa(fid,nommaa,MED_COOR,MED_NOEUD,(med_geometrie_element)0,(med_connectivite)0);
  if (nnoe < 0)
    {
      fprintf(stderr,">> ERREUR : lecture du nombre de noeuds (via MEDnEntMaa) \n");
      exit(EXIT_FAILURE);
    }
  fprintf(stdout,"- Nombre de noeuds : %d \n",nnoe);

  /* Combien de mailles, faces ou aretes ? */
  for (i=0;i<MED_NBR_GEOMETRIE_MAILLE;i++)
    {
      nmailles[i] = MEDnEntMaa(fid,nommaa,MED_CONN,MED_MAILLE,typmai[i],
			       typ_con);
      if (nmailles[i] < 0)
	{
	  fprintf(stderr,">> ERREUR : lecture du nombre de mailles \n");
	  exit(EXIT_FAILURE);
	}
      fprintf (stdout,"- Nombre de mailles de type %s : %d \n",nommai[i],nmailles[i]);
    }

  for (i=0;i<MED_NBR_GEOMETRIE_FACE;i++)
    {
      nfaces[i] = MEDnEntMaa(fid,nommaa,MED_CONN,MED_FACE,typfac[i],
			     typ_con);
      if (nfaces[i] < 0)
	{
	  fprintf(stderr,">> ERREUR : lecture du nombre de faces \n");
	  exit(EXIT_FAILURE);
	}
      fprintf (stdout,"- Nombre de faces de type %s : %d \n",nomfac[i],nfaces[i]);
    }    

  for (i=0;i<MED_NBR_GEOMETRIE_ARETE;i++)
    {
      naretes[i] = MEDnEntMaa(fid,nommaa,MED_CONN,MED_ARETE,typare[i],
			      typ_con); 
      if (naretes[i] < 0)
	{
	  fprintf(stderr,">> ERREUR : lecture du nombre d'aretes \n");
	  exit(EXIT_FAILURE);
	}
      fprintf (stdout,"- Nombre d'aretes de type %s : %d \n",nomare[i],naretes[i]);
    }

  /* nombre de familles */
  nfam = MEDnFam(fid,nommaa,0,(med_dim_famille)0);
  if (nfam < 0)
    {
      fprintf(stderr,">> ERREUR : lecture du nombre de familles \n");
      exit(EXIT_FAILURE);
    }	
  fprintf(stdout,"- Nombre de familles : %d \n",nfam);

  /* combien d'equivalences dans le fichier */
  nequ = MEDnEquiv(fid,nommaa);
  if (nequ < 0)
    {
      fprintf(stderr,">> ERREUR : lecture du nombre d'equivalences \n");
      exit(EXIT_FAILURE);
    }	
    fprintf(stdout,"- Nombre d'equivalences : %d \n",nequ);

  /* combien de champs dans le fichier */
  ncha = MEDnChamp(fid,0);
  if (ncha < 0)
    {
      fprintf(stderr,">> ERREUR : lecture du nombre de champs \n");
      exit(EXIT_FAILURE);
    }	
  fprintf(stdout,"- Nombre de champs : %d \n",ncha);

  /* Doit-on s'arreter ? */
  if (lecture_en_tete_seulement)
    {
      ret = MEDfermer(fid);
      if (ret == 0)
	{
	  fprintf(stdout,"\n >>>>>> FIN DU DUMP DU FICHIER %s >>>>>>\n",argv[1]);
	  return 0;
	}
      else
	{
	  fprintf(stderr,">> ERREUR : fermeture du fichier %s  \n",argv[1]);
	  exit(EXIT_FAILURE);
	}
    }

  /****************************************************************************
  *                       LECTURE DES NOEUDS                                  *
  ****************************************************************************/
  fprintf(stdout,"\n(************************)\n");
  fprintf(stdout,"(* NOEUDS DU MAILLAGE : *)\n");
  fprintf(stdout,"(************************)\n");

  /* Allocations memoires */
  /* table des coordonnees 
     profil : (dimension * nombre de noeuds ) */
  coo = (med_float*) malloc(sizeof(med_float)*nnoe*mdim);
  /* table  des numeros, des numeros de familles des noeuds
     profil : (nombre de noeuds) */
  numnoe = (med_int*) malloc(sizeof(med_int)*nnoe);
  nufano = (med_int*) malloc(sizeof(med_int)*nnoe);
  /* table des noms des noeuds 
     profil : (nnoe*MED_TAILLE_PNOM+1) */
  nomnoe = (char*) malloc(MED_TAILLE_PNOM*nnoe+1);

  /* lecture des noeuds : 
     - coordonnees
     - noms (optionnel dans un fichier MED) 
     - numeros (optionnel dans un fichier MED) 
     - numeros des familles */
  ret = MEDnoeudsLire(fid,nommaa,mdim,coo,mode_coo,&rep,
		      nomcoo,unicoo,nomnoe,&inonoe,numnoe,&inunoe,
		      nufano,nnoe);
  if (ret < 0)
    strcpy(message,">> ERREUR : lecture des noeuds \n");

  /* affichage des resultats */
  if (ret == 0)
    { 
      fprintf(stdout,"- Type de repere des coordonnees : %d \n",rep);
      fprintf(stdout,"- Nom des coordonnees : \n");
      for (i=0;i<mdim;i++)
	{
	  strncpy(str,nomcoo+i*MED_TAILLE_PNOM,MED_TAILLE_PNOM);
          str[MED_TAILLE_PNOM] = '\0';
          fprintf(stdout," %s ",str);
	}
      fprintf(stdout,"\n- Unites des coordonnees : \n");
      for (i=0;i<mdim;i++)
	{
	  strncpy(str,unicoo+i*MED_TAILLE_PNOM,MED_TAILLE_PNOM);
          str[MED_TAILLE_PNOM] = '\0';
          fprintf(stdout," %s ",str);
	}     
      fprintf(stdout,"\n- Coordonnees des noeuds : \n");
      for (i=0;i<nnoe*mdim;i++)
	fprintf(stdout," %f ",*(coo+i));
      if (inonoe)
	{
	  fprintf(stdout,"\n- Noms des noeuds : \n");
	  for (i=0;i<nnoe;i++)
	    {
	      strncpy(str,nomnoe+i*MED_TAILLE_PNOM,MED_TAILLE_PNOM);
              str[MED_TAILLE_PNOM] = '\0';
	      fprintf(stdout," %s ",str);
	    }
	}
      if (inunoe)
	{
	  fprintf(stdout,"\n- Numeros des noeuds : \n");
	  for (i=0;i<nnoe;i++)
	      fprintf(stdout," %d ",*(numnoe+i));
	}
      fprintf(stdout,"\n- Numeros des familles des noeuds : \n");
      for (i=0;i<nnoe;i++)
	fprintf(stdout," %d ",*(nufano+i));
      fprintf(stdout,"\n");
    }

  /* liberation memoire */
  free(coo);
  free(nomnoe);
  free(numnoe);
  free(nufano);

  /****************************************************************************
  *                       LECTURE DES ELEMENTS                                *
  ****************************************************************************/
  fprintf(stdout,"\n(**************************)\n");
  fprintf(stdout,"(* ELEMENTS DU MAILLAGE : *)\n");
  fprintf(stdout,"(**************************)");
  /* Lecture des connectivites, noms, numeros des mailles */
  if (ret == 0)
    for (i=0;i<MED_NBR_GEOMETRIE_MAILLE;i++)
      {
	if (nmailles[i] > 0 && ret == 0)
	  {
	    /* dimension de la maille */
	    edim = typmai[i] / 100;
	    nsup = 0;
	    if (mdim  == 2 || mdim == 3)
	      if (edim == 1)
		nsup = 1;
	    if (mdim == 3)
	      if (edim == 2)
		nsup = 1;
	    switch(typ_con)
	      {
	      case MED_NOD :
		taille = nsup+typmai[i]%100;
		break;
		
	      case MED_DESC :
		taille = nsup+desmai[i];
		break;
		
	      default :
		ret = -1;
	      }
	    
	    /* allocation memoire */
	    connectivite = (med_int*)malloc(sizeof(med_int)*
					    taille*nmailles[i]);
	    nomele = (char*)malloc(sizeof(char)*MED_TAILLE_PNOM*
				   nmailles[i]+1);
	    numele = (med_int*)malloc(sizeof(med_int)*
				      nmailles[i]);
	    nufael = (med_int*)malloc(sizeof(med_int)*
				      nmailles[i]);
	    
	    /* lecture des données */
	    ret = MEDelementsLire(fid,nommaa,mdim,connectivite,mode_coo,
				  nomele,&inoele,numele,&inuele,nufael,
				  nmailles[i],MED_MAILLE,typmai[i],
				  typ_con);
	    if (ret < 0)
	      strcpy(message,">> ERREUR : lecture des mailles \n");
	    
	    /* affichage des resultats */
	    if (ret == 0)
	      {
		fprintf(stdout,"\n\n- Mailles de type %s : ", nommai[i]);
		fprintf(stdout,"\n  - Connectivité : \n");
		for (j=0;j<nmailles[i]*taille;j++)
		  fprintf(stdout," %d ",*(connectivite+j));
		if (inoele)
		  {
		    fprintf(stdout,"\n  - Noms : \n");
		    for (j=0;j<nmailles[i];j++)
		      {
			fprintf(stdout," %d ",*(connectivite+j));
			strncpy(str,nomele+j*MED_TAILLE_PNOM,MED_TAILLE_PNOM);
			str[MED_TAILLE_PNOM] = '\0';
			fprintf(stdout," %s ",str);
		      }
		  }
		if (inuele)
		  {
		    fprintf(stdout,"\n  - Numeros :\n");
		    for (j=0;j<nmailles[i];j++)
		      fprintf(stdout," %d ",*(numele+j));
		  }
		fprintf(stdout,"\n  - Numéros de familles : \n");
		for (j=0;j<nmailles[i];j++)
		  fprintf(stdout," %d ",*(nufael+j));
	      }
		
	    /* liberation memoire */
	    free(connectivite);
	    free(nomele);
	    free(numele);
	    free(nufael);
	  }
      }

  if (ret == 0)
    for (i=0;i<MED_NBR_GEOMETRIE_FACE;i++)
      {
	if (nfaces[i] > 0 && ret == 0)
	  {
	    /* dimension de la face */
	    edim = typfac[i] / 100;
	    nsup = 0;
	    if (mdim  == 2 || mdim == 3)
	      if (edim == 1)
		nsup = 1;
	    if (mdim == 3)
	      if (edim == 2)
		nsup = 1;
	    switch(typ_con)
	      {
	      case MED_NOD :
		taille = nsup+typfac[i]%100;
		break;
		
	      case MED_DESC :
		taille = nsup+desfac[i];
		break;
		
	      default :
		ret = -1;
	      }
	    
	    /* allocation memoire */
	    connectivite = (med_int*)malloc(sizeof(med_int)*
					    taille*nfaces[i]);
	    nomele = (char*)malloc(sizeof(char)*MED_TAILLE_PNOM*
				   nfaces[i]+1);
	    numele = (med_int*)malloc(sizeof(med_int)*
				      nfaces[i]);
	    nufael = (med_int*)malloc(sizeof(med_int)*
				      nfaces[i]);
	    
	    /* lecture des données */
	    ret = MEDelementsLire(fid,nommaa,mdim,connectivite,mode_coo,
				  nomele,&inoele,numele,&inuele,nufael,
				  nfaces[i],MED_FACE,typfac[i],
				  typ_con);
	    if (ret < 0)
	      strcpy(message,">> ERREUR : lecture des faces \n");
	    
	    /* affichage des resultats */
	    if (ret == 0)
	      {
		fprintf(stdout,"\n- Faces de type %s : ", nomfac[i]);
		fprintf(stdout,"\n  - Connectivité : \n");
		for (j=0;j<nfaces[i]*taille;j++)
		  fprintf(stdout," %d ",*(connectivite+j));
		if (inoele)
		  {
		    fprintf(stdout,"\n  - Noms : \n");
		    for (j=0;j<nfaces[i];j++)
		      {
			fprintf(stdout," %d ",*(connectivite+j));
			strncpy(str,nomele+j*MED_TAILLE_PNOM,MED_TAILLE_PNOM);
			str[MED_TAILLE_PNOM] = '\0';
			fprintf(stdout," %s ",str);
		      }
		  }
		if (inuele)
		  {
		    fprintf(stdout,"\n  - Numeros :\n");
		    for (j=0;j<nfaces[i];j++)
		      fprintf(stdout," %d ",*(numele+j));
		  }
		fprintf(stdout,"\n  - Numéros de familles : \n");
		for (j=0;j<nfaces[i];j++)
		  fprintf(stdout," %d ",*(nufael+j));
	      }
		
	    /* liberation memoire */
	    free(connectivite);
	    free(nomele);
	    free(numele);
	    free(nufael);
	  }
    }    

  if (ret == 0)
    for (i=0;i<MED_NBR_GEOMETRIE_ARETE;i++)
      {
	if (naretes[i] > 0 && ret == 0)
	  {
	    /* dimension de l'arete  */
	    edim = typare[i] / 100;
	    nsup = 0;
	    if (mdim  == 2 || mdim == 3)
	      if (edim == 1)
		nsup = 1;
	    if (mdim == 3)
	      if (edim == 2)
		nsup = 1;
	    switch(typ_con)
	      {
	      case MED_NOD :
		taille = nsup+typare[i]%100;
		break;
		
	      case MED_DESC :
		taille = nsup+desare[i];
		break;
		
	      default :
		ret = -1;
	      }
	    
	    /* allocation memoire */
	    connectivite = (med_int*)malloc(sizeof(med_int)*
					    taille*naretes[i]);
	    nomele = (char*)malloc(sizeof(char)*MED_TAILLE_PNOM*
				   naretes[i]+1);
	    numele = (med_int*)malloc(sizeof(med_int)*
				      naretes[i]);
	    nufael = (med_int*)malloc(sizeof(med_int)*
				      naretes[i]);
	    
	    /* lecture des données */
	    ret = MEDelementsLire(fid,nommaa,mdim,connectivite,mode_coo,
				  nomele,&inoele,numele,&inuele,nufael,
				  naretes[i],MED_ARETE,typare[i],
				  typ_con);
	    if (ret < 0)
	      strcpy(message,">> ERREUR : lecture des aretes \n");
	    
	    /* affichage des resultats */
	    if (ret == 0)
	      {
		fprintf(stdout,"\n- Aretes de type %d : ", nomare[i]);
		fprintf(stdout,"\n  - Connectivité : \n");
		for (j=0;j<naretes[i]*taille;j++)
		  fprintf(stdout," %d ",*(connectivite+j));
		if (inoele)
		  {
		    fprintf(stdout,"\n  - Noms : \n");
		    for (j=0;j<naretes[i];j++)
		      {
			fprintf(stdout," %d ",*(connectivite+j));
			strncpy(str,nomele+j*MED_TAILLE_PNOM,MED_TAILLE_PNOM);
			str[MED_TAILLE_PNOM] = '\0';
			fprintf(stdout," %s ",str);
		      }
		  }
		if (inuele)
		  {
		    fprintf(stdout,"\n  - Numeros :\n");
		    for (j=0;j<naretes[i];j++)
		      fprintf(stdout," %d ",*(numele+j));
		  }
		fprintf(stdout,"\n  - Numéros de familles : \n");
		for (j=0;j<naretes[i];j++)
		  fprintf(stdout," %d ",*(nufael+j));
	      }
		
	    /* liberation memoire */
	    free(connectivite);
	    free(nomele);
	    free(numele);
	    free(nufael);
	  }
      }
  
  /****************************************************************************
  *                       LECTURE DES FAMILLES                                *
  ****************************************************************************/
  printf("\n(*************************)\n");
  printf("(* FAMILLES DU MAILLAGE : *)\n");
  printf("(*************************)\n");
  if (ret == 0)
    for (i=0;i<nfam;i++)
      {
	
	/* nombre de groupes */
	ngro = MEDnFam(fid,nommaa,i+1,MED_GROUPE);
	if (ngro < 0)  
	  {
	    ret = -1;
	    strcpy(message,
		   ">> ERREUR : lecture du nombre de groupes d'une famille \n");
	  }
	
	/* nombre d'attributs */
	if (ret == 0)
	  {
	    natt = MEDnFam(fid,nommaa,i+1,MED_ATTR);
	    if (natt < 0)
	      {
		ret = -1;
		strcpy(message,
		   ">> ERREUR : lecture du nombre d'attributs d'une famille\n");
	      }
	  }

	if (ret == 0)
	  fprintf(stdout,"- Famille %d a %d attributs et %d groupes \n",i+1,natt,ngro); 

	/* nom,numero,attributs,groupes */
	if (ret == 0)
	  {
	    attide = (med_int*) malloc(sizeof(med_int)*natt);
	    attval = (med_int*) malloc(sizeof(med_int)*natt);	    
	    attdes = (char *) malloc(MED_TAILLE_DESC*natt+1);
	    gro = (char*) malloc(MED_TAILLE_LNOM*ngro+1);
	    ret = MEDfamInfo(fid,nommaa,i+1,nomfam,&numfam,attide,attval,
			     attdes,&natt,gro,&ngro);
	    fprintf(stdout,"  - Famille de nom %s et de numero %d : \n",nomfam,numfam);
	    fprintf(stdout,"  - Attributs : \n");
	    for (j=0;j<natt;j++)
	      {
		strncpy(str1,attdes+j*MED_TAILLE_DESC,MED_TAILLE_DESC);
		str1[MED_TAILLE_DESC] = '\0';
		fprintf(stdout,"   ide = %d - val = %d - des = %s\n",*(attide+j),
		       *(attval+j),str1);
	      }
            free(attide);
	    free(attval);
            free(attdes);
            fprintf(stdout,"  - Groupes :\n");
	    for (j=0;j<ngro;j++)
	      {
		strncpy(str2,gro+j*MED_TAILLE_LNOM,MED_TAILLE_LNOM);
		str2[MED_TAILLE_LNOM] = '\0';
		fprintf(stdout,"   gro = %s\n",str2);
	      }
            free(gro);
	  }
      }

  /****************************************************************************
  *                       LECTURE DES EQUIVALENCES                            *
  ****************************************************************************/
  fprintf(stdout,"\n(******************************)\n");
  fprintf(stdout,"(* EQUIVALENCES DU MAILLAGE : *)\n");
  fprintf(stdout,"(******************************)\n");

  /* lecture de toutes les equivalences associes a nommaa */
  if (ret == 0)
    for (i = 0;i<nequ;i++)
      {
	fprintf(stdout,"- Equivalence numero : %d ",i+1);

	/* lecture des infos sur l'equivalence */
	ret = MEDequivInfo(fid,nommaa,i+1,equ,des);
        if (ret == 0)
	  {
	    fprintf(stdout,"\n  - Nom de l'equivalence: %s \n",equ);
            fprintf(stdout,"\n  - Description de l'equivalence : %s \n",des);
	  }
	else
	  strcpy(message,">> ERREUR : lecture informations sur equivalence\n");

	/* lecture des correspondances sur les differents types d'entites */
	if (ret == 0)
	  {
	    /* les noeuds */
	    if ((ncor = MEDnCorres(fid,nommaa,equ,MED_NOEUD,(med_geometrie_element)0)) < 0)
	      {
		ret = -1;
		strcpy(message,">> ERREUR : lecture nombre de correspondances\n");
	      }
	    else
	      fprintf(stdout,"\n  - Il y a %d correspondances sur les noeuds \n",ncor);
	    if (ncor > 0)
	      {
		cor = (med_int*) malloc(sizeof(med_int)*ncor*2);
                ret = MEDequivLire(fid,nommaa,equ,cor,ncor,MED_NOEUD,(med_geometrie_element)0);
		if (ret == 0)
		  for (j=0;j<ncor;j++)
		    fprintf(stdout,"\n  - Correspondance %d : %d et %d \n",j+1,*(cor+2*j),
			   *(cor+2*j+1));
		else
		   strcpy(message,">> ERREUR : lecture des correspondances\n");
                free(cor);
	      }
	    
	    /* sur les mailles : on ne prend pas en compte les mailles 3D */
	    if (ret ==0)
	      for (j=0;j<NBR_MAILLE_EQU;j++)
		{
		  if ((ncor = MEDnCorres(fid,nommaa,equ,MED_MAILLE,typmai[j])) < 0)
		    {
		      ret = -1;
		       strcpy(message,
			      ">> ERREUR : lecture informations sur nombre de correspondances \n");
		    }
		  else
		    fprintf(stdout,"\n  - Il y a %d correspondances sur les mailles %s \n",ncor,
			   nommai[j]);
		  if (ncor > 0)
		    {
		      cor = (med_int*) malloc(sizeof(med_int)*ncor*2);
		      ret = MEDequivLire(fid,nommaa,equ,cor,ncor,MED_MAILLE,
					 typmai[j]);
		      if (ret == 0)
			for (k=0;k<ncor;k++)
			  fprintf(stdout,"\n  - Correspondance %d : %d et %d \n",k+1,*(cor+2*k),
				 *(cor+2*k+1));
		      else
			 strcpy(message,">> ERREUR : correspondances\n");
		      free(cor);
		    }
		}

	    /* sur les faces */
	    if (ret == 0)
	      for (j=0;j<MED_NBR_GEOMETRIE_FACE;j++)
		{
		  if ((ncor = MEDnCorres(fid,nommaa,equ,MED_FACE,typfac[j])) < 0)
		    {
		      ret = -1;
		      strcpy(message,">> ERREUR : informations sur correspondances \n");
		    }
		  else
		    fprintf(stdout,"\n  - Il y a %d correspondances sur les faces %s\n",ncor,
			   nomfac[j]);
		  if (ncor > 0)
		    {
		      cor = (med_int*) malloc(sizeof(med_int)*ncor*2);
		      ret = MEDequivLire(fid,nommaa,equ,cor,ncor,MED_FACE,
					 typfac[j]);
		      if (ret < 0)
			strcpy(message,"ERREUR : lecture des equivalences \n");
		      else
			for (k=0;k<ncor;k++)
			  fprintf(stdout,"\n  - Correspondance %d : %d et %d \n",k+1,*(cor+2*k),
				 *(cor+2*k+1));
		      free(cor);
		    }
		}
	    
	    /*  sur les aretes */
	    for (j=0;j<MED_NBR_GEOMETRIE_ARETE;j++)
	      {
		if ((ncor = MEDnCorres(fid,nommaa,equ,MED_ARETE,typare[j])) < 0)
		  {
		    ret = -1;
		    strcpy(message,">> ERREUR : nombre de correspondances \n");
		  }
		else
		  fprintf(stdout,"\n  - Il y a %d correspondances sur les aretes %s \n",
			 ncor,nomare[j]);
		if (ncor > 0)
		  {
		    cor = (med_int*) malloc(sizeof(med_int)*ncor*2);
		    ret = MEDequivLire(fid,nommaa,equ,cor,ncor,MED_ARETE,
				       typare[j]);
		    if (ret < 0)
		      strcpy(message,">> ERREUR : equivalences \n");
		    else
		      for (k=0;k<ncor;k++)
			fprintf(stdout,"\n  Correspondance %d : %d et %d \n",k+1,*(cor+2*k),
			       *(cor+2*k+1));
		    free(cor);
		  }
	      }
	  }			    
      }

  /****************************************************************************
  *                       LECTURE DES CHAMPS                                  *
  ****************************************************************************/
  fprintf(stdout,"\n(************************)\n");
  fprintf(stdout,"(* CHAMPS DU MAILLAGE : *)\n");
  fprintf(stdout,"(************************)\n");

  if (ret == 0)
    for (i=0;i<ncha;i++)
      {
	fprintf(stdout,"- Champ numero : %d \n",i+1);
	
	/* combien de composantes */
	if ((ncomp = MEDnChamp(fid,i+1)) < 0)
	  {
	    ret = -1;
	    strcpy(message,">> ERREUR : nombre de composants d'un champ\n");
	  }
	
	/* allocation memoire de comp et unit*/
        if (ret == 0)
	  {
	    comp = (char*) malloc(ncomp*MED_TAILLE_PNOM+1);
	    unit = (char*) malloc(ncomp*MED_TAILLE_PNOM+1);
	  }

	/* infos sur les champs */
	if (ret == 0)
	  ret = MEDchampInfo(fid,i+1,nomcha,&typcha,comp,unit,ncomp);
	if (ret < 0)
	  strcpy(message,">> ERREUR : information sur les champs \n");

	if (ret == 0) {
	  fprintf(stdout,"  - Nom du champ : %s de type %d\n",nomcha,typcha);
	  fprintf(stdout,"  - Nom des composantes : %s\n",comp);
	  fprintf(stdout,"  - Unites des composantes : %s \n",unit);
	  free(comp);
	  free(unit);	
	}
	     
	      
	if (ret == 0)  /* Valeurs sur les noeuds */
	  {
	    /* Combien de pas de temps ? */
	    npdt = MEDnPasdetemps(fid,nomcha,MED_NOEUD,(med_geometrie_element)0);
	    if (npdt < 0)
	      ret = -1;
	    if (ret == -1)
	      strcpy(message,">> ERREUR : la lecture du nombe de pas de temps");
	    else
	      fprintf(stdout,"\n  - Il y a %d pas de temps sur les noeuds \n",npdt);

	    /* Lecture des valeurs pour tous les pas de temps */
	    if (ret == 0)
	      for (j=0;j<npdt;j++)
		{
		  /* Informations sur les pas de temps */
		  if (ret == 0)
		    ret = MEDpasdetempsInfo(fid,nomcha,MED_NOEUD,(med_geometrie_element)0,
					    j+1, maillage_champ, &ngauss, &numdt,  dtunit, &dt, &numo);

		  if (ret == 0 && (! strcmp(maillage_champ,nommaa)))
		    fprintf(stdout,"\n  -> \tPas de Temps n° %4i (%f), N°d'ordre %4i, avec %i pts de gauss\n",
			    numdt,dt,numo,ngauss);
		  else
		    strcpy(message,">> ERREUR : information sur les pas de temps \n");
		  
		  /* Combien de valeurs a lire ? */
		  if (ret == 0 && (! strcmp(maillage_champ,nommaa)))
		    {
		      if ((nval = MEDnVal(fid,nomcha,MED_NOEUD,(med_geometrie_element)0,numdt,numo)) < 0)
			{
			  ret = -1;
			  strcpy(message,">> ERREUR : nombre de valeurs d'un champ\n");
			}
		      else
			fprintf(stdout,"\n  - Il y a %d valeurs sur les noeuds \n",nval);
		    }

		  if (ret == 0 && (! strcmp(maillage_champ,nommaa)))
		    {
		      if (typcha == MED_REEL64)
			{
			  valr = (med_float*) malloc(sizeof(med_float)*ncomp*nval);
			  ret = MEDchampLire(fid,nommaa,nomcha,(unsigned char*)valr,mode_coo,MED_ALL,
					     pflnom,MED_NOEUD,(med_geometrie_element)0,numdt,numo);
					     
			  if (ret < 0)
			    strcpy(message,">> ERREUR : lecture des champs \n");
			  else
			    for (k=0;k<nval*ncomp;k++)
			      fprintf(stdout," %f ",*(valr+k));
			  free(valr);
			}
		      else
			{
			  vale = (med_int*) malloc(sizeof(med_int)*ncomp*nval);
			  ret = MEDchampLire(fid,nommaa,nomcha,(unsigned char*)vale,mode_coo,MED_ALL,
					     pflnom,MED_NOEUD,(med_geometrie_element)0,numdt,numo);
			  if (ret < 0)
			    strcpy(message,">> ERREUR : lecture des champs \n");
			  else
			    for (k=0;k<nval*ncomp;k++)
			      fprintf(stdout," %d ",*(vale+k));
			  free(vale);
			}

		      /* Lecture d'un profil eventuel */
		      if (strcmp(pflnom,MED_NOPFL) == 0 )
			fprintf(stdout,"\n \t- Pas de profil\n");
		      else 
			{
			  if ( (pflsize = MEDnValProfil(fid,pflnom)) <0)
			    {
			      ret = -1;
			      strcpy(message,">> ERREUR : lecture de la taille du profil \n");
			    }
			  else 
			    {
			      fprintf(stdout,"\n \t- Profil : %s de taille %i\n",pflnom,pflsize);
			      pflval = (med_int*) malloc(sizeof(med_int)*pflsize);
			      
			      if ( (ret = MEDprofilLire(fid,pflval,pflnom)) <0)
				strcpy(message,">> ERREUR : lecture du profil \n");
			      else
				for (l=0;l<pflsize;l++)
				  fprintf(stdout,"\t%i\n",*(pflval+l));
			      
			      free(pflval);
			    }
			}
		    }		  		  
		}
	  }


	if (ret == 0)  /* Valeurs sur les mailles */
	  {
	    for (k=0;k<MED_NBR_GEOMETRIE_MAILLE;k++)
	      {
		typgeo = typmai[k];

		/* Combien de pas de temps ? */
		npdt = MEDnPasdetemps(fid,nomcha,MED_MAILLE,typgeo);
		if (npdt < 0)
		  ret = -1;
		if (ret == -1)
		  strcpy(message,">> ERREUR : la lecture du nombe de pas de temps");
		else
		  fprintf(stdout,"\n  - Il y a %d pas de temps sur les mailles de type %d \n",npdt,typgeo);

		/* Lecture des valeurs pour tous les pas de temps */
		if (ret == 0)
		  for (j=0;j<npdt;j++)
		    {
		      /* Informations sur les pas de temps */
		      if (ret == 0)
			ret = MEDpasdetempsInfo(fid,nomcha,MED_MAILLE,typgeo,
						j+1, maillage_champ, &ngauss, &numdt,  dtunit, &dt, &numo);

		      if (ret == 0 && (! strcmp(maillage_champ,nommaa)))
			fprintf(stdout,"\n  -> \tPas de Temps n° %4i (%f), N°d'ordre %4i, avec %i pts de gauss\n",
				numdt,dt,numo,ngauss);
		      else
			strcpy(message,">> ERREUR : information sur les pas de temps \n");

		      /* Combien de valeurs a lire ? */
		      if (ret == 0 && (! strcmp(maillage_champ,nommaa)))
			{
			  if ((nval = MEDnVal(fid,nomcha,MED_MAILLE,typgeo,numdt,numo)) < 0)
			    {
			      ret = -1;
			      strcpy(message,">> ERREUR : nombre de valeurs d'un champ\n");
			    }
			  else
			    fprintf(stdout,"\n  - Il y a %d valeurs sur les noeuds \n",nval);
			}

		      if (ret == 0 && (! strcmp(maillage_champ,nommaa)))
			{
			  if (typcha == MED_REEL64)
			    {
			      valr = (med_float*) malloc(sizeof(med_float)*ncomp*nval);
			      ret = MEDchampLire(fid,nommaa,nomcha,(unsigned char*)valr,mode_coo,MED_ALL,
						 pflnom,MED_MAILLE,typgeo,numdt,numo);
			      if (ret < 0)
				strcpy(message,">> ERREUR : lecture des champs \n");
			      else
				for (kp=0;kp<nval*ncomp;kp++)
				  fprintf(stdout," %f ",*(valr+kp));
			      free(valr);
			    }
			  else
			    {
			      vale = (med_int*) malloc(sizeof(med_int)*ncomp*nval);
			      ret = MEDchampLire(fid,nommaa,nomcha,(unsigned char*)vale,mode_coo,MED_ALL,
						 pflnom,MED_MAILLE,typgeo,numdt,numo);
			      if (ret < 0)
				strcpy(message,">> ERREUR : lecture des champs \n");
			      else
				for (kp=0;kp<nval*ncomp;kp++)
				  fprintf(stdout," %d ",*(vale+kp));
			      free(vale);
			    }
			  
			  /* Lecture d'un profil eventuel */
			  if (strcmp(pflnom,MED_NOPFL) == 0 )
			    fprintf(stdout,"\n \t- Pas de profil\n");
			  else 
			    {
			      if ( (pflsize = MEDnValProfil(fid,pflnom)) <0)
				{
				  ret = -1;
				  strcpy(message,">> ERREUR : lecture de la taille du profil \n");
				}
			      else 
				{
				  fprintf(stdout,"\n \t- Profil : %s de taille %i\n",pflnom,pflsize);
				  pflval = (med_int*) malloc(sizeof(med_int)*pflsize);
				  
				  if ( (ret = MEDprofilLire(fid,pflval,pflnom)) <0)
				    strcpy(message,">> ERREUR : lecture du profil \n");
				  else
				    for (l=0;l<pflsize;l++)
				      fprintf(stdout,"\t%i\n",*(pflval+l));
				  
				  free(pflval);
				}
			    }
			}		  		  
		    }
	      }
	  }    
      

	if (ret == 0)  /* Valeurs sur les faces */
	  {
	    for (k=0;k<MED_NBR_GEOMETRIE_FACE;k++)
	      {
		typgeo = typfac[k];

		/* Combien de pas de temps ? */
		npdt = MEDnPasdetemps(fid,nomcha,MED_FACE,typgeo);
		if (npdt < 0)
		  ret = -1;
		if (ret == -1)
		  strcpy(message,">> ERREUR : la lecture du nombe de pas de temps");
		else
		  fprintf(stdout,"\n  - Il y a %d pas de temps sur les faces de type %d \n",npdt,typgeo);

		/* Lecture des valeurs pour tous les pas de temps */
		if (ret == 0)
		  for (j=0;j<npdt;j++)
		    {
		      /* Informations sur les pas de temps */
		      if (ret == 0)
			ret = MEDpasdetempsInfo(fid,nomcha,MED_FACE,typgeo,
						j+1, maillage_champ, &ngauss, &numdt,  dtunit, &dt, &numo);

		      if (ret == 0 && (! strcmp(maillage_champ,nommaa)))
			fprintf(stdout,"\n  -> \tPas de Temps n° %4i (%f), N°d'ordre %4i, avec %i pts de gauss\n",
				numdt,dt,numo,ngauss);
		      else
			strcpy(message,">> ERREUR : information sur les pas de temps \n");

		      /* Combien de valeurs a lire ? */
		      if (ret == 0 && (! strcmp(maillage_champ,nommaa)))
			{
			  if ((nval = MEDnVal(fid,nomcha,MED_FACE,typgeo,numdt,numo)) < 0)
			    {
			      ret = -1;
			      strcpy(message,">> ERREUR : nombre de valeurs d'un champ\n");
			    }
			  else
			    fprintf(stdout,"\n  - Il y a %d valeurs sur les noeuds \n",nval);
			}

		      if (ret == 0 && (! strcmp(maillage_champ,nommaa)))
			{
			  if (typcha == MED_REEL64)
			    {
			      valr = (med_float*) malloc(sizeof(med_float)*ncomp*nval);
			      ret = MEDchampLire(fid,nommaa,nomcha,(unsigned char*)valr,mode_coo,MED_ALL,
						 pflnom,MED_FACE,typgeo,numdt,numo);
			      if (ret < 0)
				strcpy(message,">> ERREUR : lecture des champs \n");
			      else
				for (kp=0;kp<nval*ncomp;kp++)
				  fprintf(stdout," %f ",*(valr+kp));
			      free(valr);
			    }
			  else
			    {
			      vale = (med_int*) malloc(sizeof(med_int)*ncomp*nval);
			      ret = MEDchampLire(fid,nommaa,nomcha,(unsigned char*)vale,mode_coo,MED_ALL,
						 pflnom,MED_FACE,typgeo,numdt,numo);
			      if (ret < 0)
				strcpy(message,">> ERREUR : lecture des champs \n");
			      else
				for (kp=0;kp<nval*ncomp;kp++)
				  fprintf(stdout," %d ",*(vale+kp));
			      free(vale);
			    }
			  
			  /* Lecture d'un profil eventuel */
			  if (strcmp(pflnom,MED_NOPFL) == 0 )
			    fprintf(stdout,"\n \t- Pas de profil\n");
			  else 
			    {
			      if ( (pflsize = MEDnValProfil(fid,pflnom)) <0)
				{
				  ret = -1;
				  strcpy(message,">> ERREUR : lecture de la taille du profil \n");
				}
			      else 
				{
				  fprintf(stdout,"\n \t- Profil : %s de taille %i\n",pflnom,pflsize);
				  pflval = (med_int*) malloc(sizeof(med_int)*pflsize);
				  
				  if ( (ret = MEDprofilLire(fid,pflval,pflnom)) <0)
				    strcpy(message,">> ERREUR : lecture du profil \n");
				  else
				    for (l=0;l<pflsize;l++)
				      fprintf(stdout,"\t%i\n",*(pflval+l));
				  
				  free(pflval);
				}
			    }
			}		  		  
		    }
	      }
	  }    


	if (ret == 0)  /* Valeurs sur les aretes */
	  {
	    for (k=0;k<MED_NBR_GEOMETRIE_ARETE;k++)
	      {
		typgeo = typare[k];

		/* Combien de pas de temps ? */
		npdt = MEDnPasdetemps(fid,nomcha,MED_ARETE,typgeo);
		if (npdt < 0)
		  ret = -1;
		if (ret == -1)
		  strcpy(message,">> ERREUR : la lecture du nombe de pas de temps");
		else
		  fprintf(stdout,"\n  - Il y a %d pas de temps sur les aretes de type %d \n",npdt,typgeo);

		/* Lecture des valeurs pour tous les pas de temps */
		if (ret == 0)
		  for (j=0;j<npdt;j++)
		    {
		      /* Informations sur les pas de temps */
		      if (ret == 0)
			ret = MEDpasdetempsInfo(fid,nomcha,MED_ARETE,typgeo,
						j+1, maillage_champ, &ngauss, &numdt,  dtunit, &dt, &numo);

		      if (ret == 0 && (! strcmp(maillage_champ,nommaa)))
			fprintf(stdout,"\n  -> \tPas de Temps n° %4i (%f), N°d'ordre %4i, avec %i pts de gauss\n",
				numdt,dt,numo,ngauss);
		      else
			strcpy(message,">> ERREUR : information sur les pas de temps \n");

		      /* Combien de valeurs a lire ? */
		      if (ret == 0 && (! strcmp(maillage_champ,nommaa)))
			{
			  if ((nval = MEDnVal(fid,nomcha,MED_ARETE,typgeo,numdt,numo)) < 0)
			    {
			      ret = -1;
			      strcpy(message,">> ERREUR : nombre de valeurs d'un champ\n");
			    }
			  else
			    fprintf(stdout,"\n  - Il y a %d valeurs sur les noeuds \n",nval);
			}

		      if (ret == 0 && (! strcmp(maillage_champ,nommaa)))
			{
			  if (typcha == MED_REEL64)
			    {
			      valr = (med_float*) malloc(sizeof(med_float)*ncomp*nval);
			      ret = MEDchampLire(fid,nommaa,nomcha,(unsigned char*)valr,mode_coo,MED_ALL,
						 pflnom,MED_ARETE,typgeo,numdt,numo);
			      if (ret < 0)
				strcpy(message,">> ERREUR : lecture des champs \n");
			      else
				for (kp=0;kp<nval*ncomp;kp++)
				  fprintf(stdout," %f ",*(valr+kp));
			      free(valr);
			    }
			  else
			    {
			      vale = (med_int*) malloc(sizeof(med_int)*ncomp*nval);
			      ret = MEDchampLire(fid,nommaa,nomcha,(unsigned char*)vale,mode_coo,MED_ALL,
						 pflnom,MED_ARETE,typgeo,numdt,numo);
			      if (ret < 0)
				strcpy(message,">> ERREUR : lecture des champs \n");
			      else
				for (kp=0;kp<nval*ncomp;kp++)
				  fprintf(stdout," %d ",*(vale+kp));
			      free(vale);
			    }
			  
			  /* Lecture d'un profil eventuel */
			  if (strcmp(pflnom,MED_NOPFL) == 0 )
			    fprintf(stdout,"\n \t- Pas de profil\n");
			  else 
			    {
			      if ( (pflsize = MEDnValProfil(fid,pflnom)) <0)
				{
				  ret = -1;
				  strcpy(message,">> ERREUR : lecture de la taille du profil \n");
				}
			      else 
				{
				  fprintf(stdout,"\n \t- Profil : %s de taille %i\n",pflnom,pflsize);
				  pflval = (med_int*) malloc(sizeof(med_int)*pflsize);
				  
				  if ( (ret = MEDprofilLire(fid,pflval,pflnom)) <0)
				    strcpy(message,">> ERREUR : lecture du profil \n");
				  else
				    for (l=0;l<pflsize;l++)
				      fprintf(stdout,"\t%i\n",*(pflval+l));
				  
				  free(pflval);
				}
			    }
			}		  		  
		    }
	      }
	  }    

      }

  if (ret < 0)
    fprintf(stderr,"%s\n",message);

  /****************************************************************************
  *                      FERMETURE DU FICHIER                                 *
  ****************************************************************************/
  ret = MEDfermer(fid);
  
  if (ret == 0)
    fprintf(stdout,"\n >>>>>> FIN DU DUMP DU FICHIER %s >>>>>>\n",argv[1]);
  else
   fprintf(stderr,">> ERREUR : erreur a la fermeture du fichier %s\n",argv[1]);

  return 0;
}
