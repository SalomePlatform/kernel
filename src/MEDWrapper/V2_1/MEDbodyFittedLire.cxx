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

#include "med_outils.hxx"
#include "med.hxx"

namespace med_2_1{

med_err 
MEDbodyFittedLire(med_idt fid, char *maa, med_int mdim, med_float *coo, med_mode_switch mode_coo,
		  med_repere *repere, char *nomcoo, char *unicoo, med_int *fam, med_int nnoeuds )
{
    med_idt maaid, noeid, dataset;
    char    chemin[MED_TAILLE_MAA+MED_TAILLE_NOM+1];
    med_int type_rep_int;

    /* On inhibe le gestionnaire d'erreur */
    _MEDmodeErreurVerrouiller();

    /* Si le maillage n'existe pas => erreur */
    strcpy(chemin, MED_MAA);
    strcat(chemin, maa);
    maaid = _MEDdatagroupOuvrir(fid, chemin);
    if (maaid < 0) return(-1);

    /* Si le Data Group "NOE" n'existe pas => erreur */
    noeid = _MEDdatagroupOuvrir(maaid, MED_NOM_NOE);
    if (noeid < 0) return(-1);

    /* Lecture du Data Set "BOF" */
    if (_MEDdatasetNumLire(noeid, MED_NOM_BOF, MED_REEL64, mode_coo, mdim, MED_ALL, MED_NOPF, 0, 1, (unsigned char*)coo) < 0) {
        return(-1);
    };

    /* On re-ouvre le Data Set "BOF" pour y lire des attributs */
    dataset = _MEDdatasetOuvrir(noeid, MED_NOM_BOF);
    if (dataset < 0) return(-1);

    /* L'attribut "REP" */
    if (_MEDattrEntierLire(dataset, MED_NOM_REP, &type_rep_int) < 0) {
        return(-1);
    } else {
        *repere = (med_repere)type_rep_int;
    };

    /* Attribut "NOM" */
    if (_MEDattrStringLire(dataset, MED_NOM_NOM, mdim*MED_TAILLE_PNOM, nomcoo) < 0) {
        return(-1);
    };

    /* Attribut "UNI" */
    if (_MEDattrStringLire(dataset, MED_NOM_UNI, mdim*MED_TAILLE_PNOM, unicoo) < 0) {
        return(-1);
    };

    /* lecture des numeros de familles */
    if (MEDfamLire(fid, maa, fam, nnoeuds, MED_NOEUD, MED_POINT1) < 0) {
        return(-1);
    };

    /* On ferme tout */
    if (_MEDdatasetFermer(dataset) < 0) {
        return(-1);
    };
    if (_MEDdatagroupFermer(noeid) < 0) {
        return(-1);
    };
    if (_MEDdatagroupFermer(maaid) < 0) {
        return(-1);
    };
    return(0);
}

}
