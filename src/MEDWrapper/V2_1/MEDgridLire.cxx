#include "med_outils.hxx"
#include "med.hxx"

namespace med_2_1{

med_err 
MEDgridLire(med_idt fid, char *maa, med_int mdim, med_float *coo, med_int dim, med_mode_switch mode_coo,
	    med_repere *repere, char *nomcoo, char *unicoo )
{
    med_idt   maaid, noeid, ds;
    char      chemin[MED_TAILLE_MAA+MED_TAILLE_NOM+1];
    char      *dataset;
    med_int   type_rep_int;

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

    /* Lecture du Data Set "IN1" ou "IN2" ou "IN3" */
    if (_MEDdatasetNumLire(noeid, dataset, MED_REEL64, mode_coo, 1, MED_ALL, MED_NOPF, 0, 1, (unsigned char*)coo) < 0) {
        return(-1);
    };

    /* On re-ouvre le Data Set precedant pour y lire des attributs */
    ds = _MEDdatasetOuvrir(noeid, dataset);
    if (ds < 0) return(-1);

    /* L'attribut "REP" */
    if (_MEDattrEntierLire(ds, MED_NOM_REP, &type_rep_int) < 0) {
        return(-1);
    } else {
        *repere = (med_repere)type_rep_int;
    };

    /* Attribut "NOM" */
    if (_MEDattrStringLire(ds, MED_NOM_NOM, mdim*MED_TAILLE_PNOM, nomcoo) < 0) {
        return(-1);
    };

    /* Attribut "UNI" */
    if (_MEDattrStringLire(ds, MED_NOM_UNI, mdim*MED_TAILLE_PNOM, unicoo) < 0) {
        return(-1);
    };

    /* On ferme tout */
    if (_MEDdatasetFermer(ds) < 0) {
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
