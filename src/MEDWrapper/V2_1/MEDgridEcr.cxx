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
MEDgridEcr(med_idt fid, char *maa, med_int mdim, med_float *coo, med_int nb, med_int dim, med_mode_switch mode_coo,
	   med_repere repere, char *nomcoo, char *unicoo, med_mode_acces mode )
{
    /* ecriture des indices */

    med_idt  maaid, noeid, ds;
    char     chemin[MED_TAILLE_MAA+MED_TAILLE_NOM+1];
    med_size dimd[1];
    char     *dataset;
    med_int  type_rep_int;

    /* On inhibe le gestionnaire d'erreur HDF */
    _MEDmodeErreurVerrouiller();

    /* Si le maillage n'existe pas => erreur */
    strcpy(chemin, MED_MAA);
    strcat(chemin, maa);
    maaid = _MEDdatagroupOuvrir(fid, chemin);
    if (maaid < 0) return(-1);

    /* Si le Data Group "NOE" n'existe pas on le cree */
    if ((noeid = _MEDdatagroupOuvrir(maaid, MED_NOM_NOE)) < 0) {
        if ((noeid = _MEDdatagroupCreer(maaid, MED_NOM_NOE)) < 0) {
            return(-1);
        };
    };

    switch (dim) {
        case 0 : {
            dataset = MED_NOM_IN1;
            break;
        };
        case 1 : {
            dataset = MED_NOM_IN2;
            break;
        };
        case 2 : {
            dataset = MED_NOM_IN3;
            break;
        };
        default : {
            return(-1);
        };
    };

    /* Creation du Data Set "IN1" ou "IN2" ou "IN3" */
    dimd[0] = nb;
    if (_MEDdatasetNumEcrire(noeid, dataset, MED_REEL64, mode_coo, 1, MED_ALL, MED_NOPF, 0, 0, dimd, (unsigned char*)coo, mode) < 0) {
        return(-1);
    };

    /* On re-ouvre le Data Set "IN1" ou "IN2" ou "IN3" pour y placer des attributs */
    if ((ds = _MEDdatasetOuvrir(noeid, dataset)) < 0) {
        return(-1);
    };

    /* Attribut NBR (nombre de noeuds) */
    if (_MEDattrEntierEcrire(ds, MED_NOM_NBR, &nb, mode) < 0) {
        return(-1);
    };

    /* L'attribut "REP" */
    type_rep_int = (med_int)repere;
    if (_MEDattrEntierEcrire(ds, MED_NOM_REP, &type_rep_int, mode) < 0) {
        return(-1);
    };

    /* Attribut "NOM" */
    if (_MEDattrStringEcrire(ds, MED_NOM_NOM, mdim*MED_TAILLE_PNOM, nomcoo, mode) < 0) {
        return(-1);
    };

    /* Attribut "UNI" */
    if (_MEDattrStringEcrire(ds, MED_NOM_UNI, mdim*MED_TAILLE_PNOM, unicoo, mode) < 0) {
        return(-1);
    };

    /* On ferme tout */
    if (_MEDdatasetFermer(ds)      < 0) return(-1);
    if (_MEDdatagroupFermer(noeid) < 0) return(-1);
    if (_MEDdatagroupFermer(maaid) < 0) return(-1);
    return(0);
}

}
