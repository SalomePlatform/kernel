/*************************************************************************
* COPYRIGHT (C) 1999 - 2003  EDF R&D
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

#ifndef MED_H
#define MED_H

extern "C"{
#include <hdf5.h>
}

namespace med_2_1{

#define MED_NULL       (void *) NULL
#define MED_MAX_PARA        20

#define MED_TAILLE_DESC 200
#define MED_TAILLE_IDENT  8
#define MED_TAILLE_NOM   32
#define MED_TAILLE_LNOM  80    
#define MED_TAILLE_PNOM   8 

/* Integration des developpements OCC */
typedef enum {MED_CARTESIAN, MED_POLAR, MED_BODY_FITTED} med_grid_type;

typedef enum {MED_GRID_D1=0, MED_GRID_D2=1, MED_GRID_D3=2,
              MED_GRID_NOEUD=3,
              MED_FAM_NOEUD=4, MED_FAM_ARETE=5, MED_FAM_FACE=6, MED_FAM_MAILLE=7 } med_grid;

/* Fin de l'integration*/

typedef enum {MED_FULL_INTERLACE,
	      MED_NO_INTERLACE}  med_mode_switch; 

typedef enum {MED_GLOBALE,
	      MED_COMPACT }  med_mode_profil; 

typedef enum {MED_LECT,MED_ECRI,MED_REMP} med_mode_acces; 

typedef enum {MED_MAILLE, MED_FACE, MED_ARETE, MED_NOEUD} med_entite_maillage; 

typedef enum {MED_COOR, MED_CONN, MED_NOM, MED_NUM, MED_FAM} med_table;

typedef enum {MED_REEL64=6, MED_INT32=24,MED_INT64=26, MED_INT} med_type_champ;

#define MED_NBR_GEOMETRIE_MAILLE 15
#define MED_NBR_GEOMETRIE_FACE 4
#define MED_NBR_GEOMETRIE_ARETE 2
typedef enum {MED_POINT1=1, MED_SEG2=102, MED_SEG3=103, MED_TRIA3=203,
	      MED_QUAD4=204, MED_TRIA6=206,MED_QUAD8=208, MED_TETRA4=304,
	      MED_PYRA5=305, MED_PENTA6=306, MED_HEXA8=308, MED_TETRA10=310, 
	      MED_PYRA13=313, MED_PENTA15=315, MED_HEXA20=320}
med_geometrie_element;

typedef enum {MED_NOD, MED_DESC} med_connectivite ; 

typedef enum {MED_CART, MED_CYL, MED_SPHER} med_repere; 

typedef enum {MED_FAUX, MED_VRAI} med_booleen ; 

typedef enum {MED_GROUPE, MED_ATTR, MED_FAMILLE} med_dim_famille; 

typedef enum {MED_COMP, MED_DTYPE} med_dim_champ; 

typedef enum {MED_HDF_VERSION, MED_VERSION, MED_FICH_DES} med_fich_info; 

#define MED_NOPG   1                   /* -> pas de point de Gauss                    */
#define MED_NOPFL  ""                  /* -> pas de profils utilisateur               */
#define MED_NOPFLi "                                "  /* Variable Interne                      */
#define MED_NOPF   0                   /* -> pas de profils pour _MEDdataseNnumEcrire */
#define MED_NOPDT -1                   /* rem: pas de pas de temps negatifs           */
#define MED_NONOR -1                   /* rem: pas de n°ordre negatif                 */
#define MED_DIM1   1                   /* PAS */
#define MED_ALL    0

#if defined(SUN4SOL2) || defined(PCLINUX) || defined(OSF1_32) || defined(IRIX64_32) || defined(RS6000)
/* interface C/FORTRAN */
/* this true only with g77 and gcc : we must change it to use directly NOMF_... and INT32 or INT64 -
 it will be more simple to understand and to use ! */
#define NOMF_POST_UNDERSCORE 

/* correspondance des types avec HDF 5 */
typedef hsize_t        med_size;
typedef hssize_t       med_ssize;
typedef hid_t          med_idt;
typedef herr_t         med_err;

/* types elementaires */
typedef int            med_int;
typedef double         med_float;
#endif

#if defined(HP9000)
/* correspondance des types avec HDF 5 */
typedef hsize_t        med_size;
typedef hssize_t       med_ssize;
typedef hid_t          med_idt;
typedef herr_t         med_err;

/* types elementaires */
typedef int            med_int;
typedef double         med_float;
#endif

#if defined(IRIX64) || defined(OSF1)
#define NOMF_POST_UNDERSCORE

/* correspondance des types avec HDF 5 */
typedef hsize_t        med_size;
typedef hssize_t       med_ssize;
typedef hid_t          med_idt;
typedef herr_t         med_err;

/* types elementaires */
typedef long           med_int;
typedef double         med_float;
#endif


#if defined(PPRO_NT) 
/* correspondance des types avec HDF 5 */
typedef hsize_t        med_size;
typedef hssize_t       med_ssize;
typedef hid_t          med_idt;
typedef herr_t         med_err;

/* types elementaires */
typedef int	       med_int;
typedef double         med_float;
#endif


#if defined(NOMF_PRE_UNDERSCORE) && defined(NOMF_POST_UNDERSCORE)
#   define NOMF(x)     _##x##_
#endif
#if defined(NOMF_PRE_UNDERSCORE) && !defined(NOMF_POST_UNDERSCORE)
#   define NOMF(x)     _##x
#endif
#if !defined(NOMF_PRE_UNDERSCORE) && defined(NOMF_POST_UNDERSCORE)
#   define NOMF(x)     x##_
#endif
#if !defined(NOMF_PRE_UNDERSCORE) && !defined(NOMF_POST_UNDERSCORE)
#   define NOMF(x)     x
#endif

}

#include "med_proto.hxx"

#endif  /* MED_H */
