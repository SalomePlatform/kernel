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

#if defined(IRIX64)
#define MED_INTEGER MED_INT64
#else
#define MED_INTEGER MED_INT32
#endif

namespace med_2_1{

med_err 
MEDbodyFittedEcr(med_idt fid, char *maa, med_int mdim, med_float *coo, med_int *nbr, med_mode_switch mode_coo,
		 med_repere repere, char *nomcoo, char *unicoo, med_int *fam, med_int nnoeuds, med_mode_acces mode )
{
    /* ecriture des coordonnees */
    med_idt   maaid, noeid, dataset;
    med_size dimd[1];
    char     chemin[MED_TAILLE_MAA+MED_TAILLE_NOM+1];
    med_int  type_rep_int;
    int      d;
    char     *ds;

    /* On inhibe le gestionnaire d'erreur HDF */
    _MEDmodeErreurVerrouiller();

    /* Si le maillage n'existe pas => erreur */
    strcpy(chemin, MED_MAA);
    strcat(chemin, maa);
    if ((maaid = _MEDdatagroupOuvrir(fid, chemin)) < 0) {
        return(-1);
    };

    /* Si le Data Group "NOE" n'existe pas on le cree */
    if ((noeid = _MEDdatagroupOuvrir(maaid, MED_NOM_NOE)) < 0) {
        if ((noeid = _MEDdatagroupCreer(maaid, MED_NOM_NOE)) < 0) {
            return(-1);
        };
    };

    /* Creation du Data Set "BOF" */
    dimd[0] = nnoeuds*mdim;
    if (_MEDdatasetNumEcrire(noeid, MED_NOM_BOF, MED_REEL64, mode_coo, mdim, MED_ALL, MED_NOPF, 0, 0, dimd, (unsigned char*)coo, mode) < 0) {
        return(-1);
    };
  
    /* On re-ouvre le Data Set "BOF" pour y placer des attributs */
    if ((dataset = _MEDdatasetOuvrir(noeid, MED_NOM_BOF)) < 0) {
        return(-1);
    };

    /* Attribut NBR (nombre de noeuds) */
    if (_MEDattrEntierEcrire(dataset, MED_NOM_NBR, &nnoeuds, mode) < 0) {
        return(-1);
    };

    /* L'attribut "REP" */
    type_rep_int = (med_int)repere;
    if (_MEDattrEntierEcrire(dataset, MED_NOM_REP, &type_rep_int, mode) < 0) {
        return(-1);
    };

    /* Attribut "NOM" */
    if (_MEDattrStringEcrire(dataset, MED_NOM_NOM, mdim*MED_TAILLE_PNOM, nomcoo, mode) < 0) {
        return(-1);
    };

    /* Attribut "UNI" */
    if (_MEDattrStringEcrire(dataset, MED_NOM_UNI, mdim*MED_TAILLE_PNOM, unicoo, mode) < 0) {
        return(-1);
    };

    if (_MEDdatasetFermer(dataset) < 0) return(-1);

    dimd[0] = 1;
    for (d=0; d<mdim; d++) {
        switch (d) {
            case 0 : {
                ds = MED_NOM_IN1;
                break;
            };
            case 1 : {
                ds = MED_NOM_IN2;
                break;
            };
            case 2 : {
                ds = MED_NOM_IN3;
                break;
            };
            default : {
                return(-1);
            };
        };

        /* Creation du Data Set "IN1", "IN2", "IN3" contenant la taille du bodyfitted sur cette dimension */
        if (_MEDdatasetNumEcrire(noeid, ds, MED_INTEGER, mode_coo, MED_DIM1, MED_ALL, MED_NOPF, 0, 0, dimd, (unsigned char*)&nbr[d], mode) < 0) {
            return(-1);
        };

        /* On re-ouvre ce Data Set pour y placer des attributs */
        if ((dataset = _MEDdatasetOuvrir(noeid, ds)) < 0) {
            return(-1);
        };

        /* Attribut NBR (nombre de noeuds dans l'une des dimensions) */
        if (_MEDattrEntierEcrire(dataset, MED_NOM_NBR, &nbr[d], mode) < 0) {
            return(-1);
        };

        if (_MEDdatasetFermer(dataset) < 0) return(-1);
    };

    /* Ecriture des numeros de familles */
    if (MEDfamEcr(fid, maa, fam, nnoeuds, mode, MED_NOEUD, MED_POINT1) < 0) {
        return(-1);
    };

    /* On ferme tout */
    if (_MEDdatagroupFermer(noeid) < 0) return(-1);
    if (_MEDdatagroupFermer(maaid) < 0) return(-1);
    return(0);
}

}
