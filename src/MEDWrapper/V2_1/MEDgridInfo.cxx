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

#include <string.h>
#include <stdlib.h>

namespace med_2_1{

med_err 
MEDgridInfo(med_idt fid, int indice, med_int *isAGrid, med_grid_type *typ) 
{
    int     numero;
    med_idt maaid;
    char    maillage[MED_TAILLE_NOM+1];
    char    chemin[MED_TAILLE_MAA+MED_TAILLE_NOM+1];

    /* On inhibe le gestionnaire d'erreur */
    _MEDmodeErreurVerrouiller();

    /* On recupere le nom du groupe de rang "indice" */
    numero = indice-1;
    if (_MEDobjetIdentifier(fid, MED_MAA, numero, maillage) < 0) {
        return(-1);
    };

    /* On va chercher l'attribut dimension */
    strcpy(chemin, MED_MAA);
    strcat(chemin, maillage);
    maaid = _MEDdatagroupOuvrir(fid, chemin);
    if (maaid < 0) return(-1);

    if (_MEDattrEntierLire(maaid, MED_NOM_GRD, typ) < 0) {
        *isAGrid = 0;
    } else {
        *isAGrid = 1;
    };

    if (_MEDdatagroupFermer(maaid) < 0) return(-1);
    return(0);
}

}
