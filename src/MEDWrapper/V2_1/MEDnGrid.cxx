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

namespace med_2_1{

med_int 
MEDnGrid(med_idt fid, char *maa, med_grid n) 
{
    med_idt maaid, entid, geoid, dataset;
    char    chemin[MED_TAILLE_MAA+MED_TAILLE_NOM+1];
    char    nom_ent[MED_TAILLE_NOM_ENTITE+1];
    char    *nom_dataset;
    med_int  res = (-1);

    /* On inhibe le gestionnaire d'erreur HDF 5 */
    _MEDmodeErreurVerrouiller();

    /* Si le maillage n'existe pas => erreur */
    strcpy(chemin, MED_MAA);
    strcat(chemin, maa);
    maaid = _MEDdatagroupOuvrir(fid, chemin);
    if (maaid < 0) return(-1);

    switch (n) {
        case MED_FAM_NOEUD : {
            nom_dataset = MED_NOM_FAM;
            if (_MEDnomEntite(nom_ent, MED_NOEUD) < 0) return(-1);
            entid = _MEDdatagroupOuvrir(maaid, nom_ent);
            break;
        };
        case MED_FAM_ARETE : {
            nom_dataset = MED_NOM_FAM;
            if (_MEDnomEntite(nom_ent, MED_ARETE) < 0) return(-1);
            geoid = _MEDdatagroupOuvrir(maaid, nom_ent);
            if (geoid < 0) return(-1);
            if (_MEDnomGeometrie(nom_ent, MED_SEG2) < 0) return(-1);
            entid = _MEDdatagroupOuvrir(geoid, nom_ent);
            break;
        };
        case MED_FAM_FACE : {
            nom_dataset = MED_NOM_FAM;
            if (_MEDnomEntite(nom_ent, MED_FACE) < 0) return(-1);
            geoid = _MEDdatagroupOuvrir(maaid, nom_ent);
            if (geoid < 0) return(-1);
            if (_MEDnomGeometrie(nom_ent, MED_QUAD4) < 0) return(-1);
            entid = _MEDdatagroupOuvrir(geoid, nom_ent);
            break;
        };
        case MED_FAM_MAILLE : {
            nom_dataset = MED_NOM_FAM;
            if (_MEDnomEntite(nom_ent, MED_MAILLE) < 0) return(-1);
            geoid = _MEDdatagroupOuvrir(maaid, nom_ent);
            if (geoid < 0) return(-1);
            if (_MEDnomGeometrie(nom_ent, MED_HEXA8) < 0) return(-1);
            entid = _MEDdatagroupOuvrir(geoid, nom_ent);
            break;
        };
        case MED_GRID_NOEUD : {
            nom_dataset = MED_NOM_BOF;
            entid = _MEDdatagroupOuvrir(maaid, MED_NOM_NOE);
            break;
        };
        case MED_GRID_D1 : {
            nom_dataset = MED_NOM_IN1;
            entid = _MEDdatagroupOuvrir(maaid, MED_NOM_NOE);
            break;
        };
        case MED_GRID_D2 : {
            nom_dataset = MED_NOM_IN2;
            entid = _MEDdatagroupOuvrir(maaid, MED_NOM_NOE);
            break;
        };
        case MED_GRID_D3 : {
            nom_dataset = MED_NOM_IN3;
            entid = _MEDdatagroupOuvrir(maaid, MED_NOM_NOE);
            break;
        };
        default : {
            return(-1);
        };
    };

    if (entid < 0) return(-1);
    dataset = _MEDdatasetOuvrir(entid, nom_dataset);
    if (dataset < 0) return(-1);
    if (_MEDattrEntierLire(dataset, MED_NOM_NBR, &res) < 0) return(-1);

    /* On ferme tout */
    if (_MEDdatasetFermer(dataset) < 0) return(-1);
    if (_MEDdatagroupFermer(entid) < 0) return(-1);
    if (_MEDdatagroupFermer(maaid) < 0) return(-1);

    return(res);
}

}
