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
MEDfamGridEcr(med_idt fid, char *maa, med_int *fam, med_int n, med_mode_acces mode, med_entite_maillage type_ent) {
    /* Ecrire des numeros de familles pour les grilles cartesiennes ou polaires :
       - pour les noeuds
       - pour les aretes
       - pour les faces
       - pour les mailles */

    med_geometrie_element type_geo;

    switch(type_ent) {
        case MED_NOEUD : {
            type_geo = MED_POINT1;
            break;
        };
        case MED_ARETE : {
            type_geo = MED_SEG2;
            break;
        };
        case MED_FACE : {
            type_geo = MED_QUAD4;
            break;
        };
        case MED_MAILLE : {
            type_geo = MED_HEXA8;
            break;
        };
        default : {
            return(-1);
        };
    };

    return(MEDfamEcr(fid, maa, fam, n, mode, type_ent, type_geo));
}

}
