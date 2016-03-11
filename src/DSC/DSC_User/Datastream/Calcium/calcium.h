// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  File   : calcium.h
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
//
/* Outils d'Aide au Couplage de Code de Calcul : $Id$ */

#ifndef __CALCIUM_H
#define __CALCIUM_H

#include "calciumE.h"
#include "version.h"
#include "calciumP.h"

#if defined(__STDC__) || defined(__cplusplus) || defined(c_plusplus)
#define CPNeedPrototype 1
#else
#define CPNeedPrototype 0
#endif


#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*                                              */
/*                                              */
/* Fonctions de connexion                       */
/*                                              */
/*                                              */
extern int      cp_cd(
/*              -----                           */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        char  * /* S   Nom de l instance appelante*/
#endif
);


/*                                              */
/*                                              */
/* Fonction de deconnexion                      */
/*                                              */
/*                                              */
extern int      cp_fin(
/*              ------                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E Directive de continuation  */
                /* CP_CONT ou CP_ARRET          */
#endif
);



/*                                              */
/*                                              */
/* Fonctions de lecture bloquante               */
/*                                              */
/*                                              */
extern int      cp_len(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        float * /* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        float * /* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration a lire                   */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        int   * /* S   Tableau d'entiers pour stocker les       */
                /*     valeurs lues                             */
#endif
);

extern int      cp_llg(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        float * /* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        float * /* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration a lire                   */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        long  * /* S   Tableau d'entiers pour stocker les       */
                /*     valeurs lues                             */
#endif
);

extern int      cp_lln(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        float * /* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        float * /* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration a lire                   */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        long  * /* S   Tableau d'entiers pour stocker les       */
                /*     valeurs lues                             */
#endif
);

extern int      cp_lre(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        float * /* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        float * /* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration a lire                   */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        float * /* S   Tableau de flottants pour stocker les    */
                /*     valeurs lues                             */
#endif
);

extern int      cp_lrd(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        float * /* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        float * /* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration a lire                   */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        float * /* S   Tableau de flottants pour stocker les    */
                /*     valeurs lues                             */
#endif
);


extern int      cp_ldb(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        double* /* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        double* /* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration a lire                   */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        double* /* S   Tableau de doubles pour stocker les      */
                /*     valeurs lues                             */
#endif
);

extern int      cp_lcp(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        float * /* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        float * /* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration lire                     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        float * /* S   Tableau de flottants pour stocker les    */
                /*     valeurs lues (dimension = 2 * le nombre  */
                /*     de valeurs lues)                         */
#endif
);

extern int      cp_llo(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        float * /* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        float * /* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration a lire                   */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        int   * /* S   Tableau d 'entier pour stocker les       */
                /*     valeurs lues (remplace le logiques)      */
#endif
);

extern int      cp_lch(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        float * /* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        float * /* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration a lire                   */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        char *[]/* S   Tableau de chaines pour stocker les      */
                /*     valeurs lues (remplace le logiques)      */,
        int     /* E   Taille des chaines du tablaeu            */
#endif
);



/*                                              */
/*                                              */
/* Fonctions de lecture non bloquantes          */
/*                                              */
/*                                              */
extern int      cp_nlen(
/*              -------                                         */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        float * /* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        float * /* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration a lire                   */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        int   * /* S   Tableau d'entiers pour stocker les       */
                /*     valeurs lues                             */
#endif
);

extern int      cp_nlre(
/*              -------                                         */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        float * /* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        float * /* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration a lire                   */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        float * /* S   Tableau de flottants pour stocker les    */
                /*     valeurs lues                             */
#endif
);

extern int      cp_nldb(
/*              -------                                         */

#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        double */* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        double */* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration a lire                   */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        double* /* S   Tableau de doubles pour stocker les      */
                /*     valeurs lues                             */
#endif
);

extern int      cp_nlcp(
/*              -------                                         */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        float * /* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        float * /* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration lire                     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        float * /* S   Tableau de flottants pour stocker les    */
                /*     valeurs lues (dimension = 2 * le nombre  */
                /*     de valeurs lues)                         */
#endif
);

extern int      cp_nllo(
/*              -------                                         */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        float * /* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        float * /* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration a lire                   */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        int   * /* S   Tableau d 'entier pour stocker les       */
                /*     valeurs lues (remplace le logiques)      */
#endif
);

extern int      cp_nlch(
/*              -------                                         */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance ou de lecture         */
                /*     CP_TEMPS, CP_ITERATION, CP_SEQUENTIEL    */,
        float * /* E/S Borne inf de l'intervalle de lecture     */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        float * /* E   Borne Sup de l'intervalle de lecture     */,
        int   * /* E/S Pas d'iteration a lire                   */
                /*     Retourne le pas lu dans le cas de        */
                /*     lecture sequentielle                     */,
        char  * /* E   Nom de la variable a lire                */,
        int     /* E   Nombre max de valeurs a lire             */,
        int   * /* S   Nombre de valeurs rellement lues         */,
        char *[]/* S   Tableau de chaines pour stocker les      */
                /*     valeurs lues (remplace le logiques)      */,
        int     /* E   Taille des chaines du tablaeu            */
#endif
);




/*                                              */
/*                                              */
/* Fonctions d'ecriture                         */
/*                                              */
/*                                              */

extern int      cp_een(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance                       */
                /*     CP_TEMPS, CP_ITERATION                   */,
        float   /* E   Pas de temps a ecrire                    */,
        int     /* E   Pas d'iteration a ecrire                 */,
        char  * /* E   Nom de la variable a ecrire              */,
        int     /* E   Nombre de valeurs a ecrire               */,
        int   * /* E   Tableau d'entiers a ecrire               */
#endif
);

extern int      cp_elg(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance                       */
                /*     CP_TEMPS, CP_ITERATION                   */,
        float   /* E   Pas de temps a ecrire                    */,
        int     /* E   Pas d'iteration a ecrire                 */,
        char  * /* E   Nom de la variable a ecrire              */,
        int     /* E   Nombre de valeurs a ecrire               */,
        long  * /* E   Tableau d'entiers a ecrire               */
#endif
);

extern int      cp_eln(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance                       */
                /*     CP_TEMPS, CP_ITERATION                   */,
        float   /* E   Pas de temps a ecrire                    */,
        int     /* E   Pas d'iteration a ecrire                 */,
        char  * /* E   Nom de la variable a ecrire              */,
        int     /* E   Nombre de valeurs a ecrire               */,
        long  * /* E   Tableau d'entiers a ecrire               */
#endif
);

extern int      cp_ere(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance                       */
                /*     CP_TEMPS, CP_ITERATION                   */,
        float   /* E   Pas de temps a ecrire                    */,
        int     /* E   Pas d'iteration a ecrire                 */,
        char  * /* E   Nom de la variable a ecrire              */,
        int     /* E   Nombre de valeurs a ecrire               */,
        float * /* E   Tableau de flottants a ecrire            */
#endif
);

extern int      cp_erd(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance                       */
                /*     CP_TEMPS, CP_ITERATION                   */,
        float   /* E   Pas de temps a ecrire                    */,
        int     /* E   Pas d'iteration a ecrire                 */,
        char  * /* E   Nom de la variable a ecrire              */,
        int     /* E   Nombre de valeurs a ecrire               */,
        float * /* E   Tableau de flottants a ecrire            */
#endif
);


extern int      cp_edb(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance                       */
                /*     CP_TEMPS, CP_ITERATION                   */,
        double  /* E   Pas de temps a ecrire                    */,
        int     /* E   Pas d'iteration a ecrire                 */,
        char  * /* E   Nom de la variable a ecrire              */,
        int     /* E   Nombre de valeurs a ecrire               */,
        double* /* E   Tableau de doubles a ecrire              */
#endif
);

extern int      cp_ecp(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance                       */
                /*     CP_TEMPS, CP_ITERATION                   */,
        float   /* E   Pas de temps a ecrire                    */,
        int     /* E   Pas d'iteration a ecrire                 */,
        char  * /* E   Nom de la variable a ecrire              */,
        int     /* E   Nombre de valeurs a ecrire               */,
        float * /* E   Tableau de flottants a ecrire            */
                /*     (dimension = 2 * le nombre de valeurs    */
                /*      a ecrire                                */
#endif
);

extern int      cp_elo(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance                       */
                /*     CP_TEMPS, CP_ITERATION                   */,
        float   /* E   Pas de temps a ecrire                    */,
        int     /* E   Pas d'iteration a ecrire                 */,
        char  * /* E   Nom de la variable a ecrire              */,
        int     /* E   Nombre de valeurs a ecrire               */,
        int   * /* E   Tableau d'entiers a ecrire               */
#endif
);

extern int      cp_ech(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E   Type de dependance                       */
                /*     CP_TEMPS, CP_ITERATION                   */,
        float   /* E   Pas de temps a ecrire                    */,
        int     /* E   Pas d'iteration a ecrire                 */,
        char  * /* E   Nom de la variable a ecrire              */,
        int     /* E   Nombre de valeurs a ecrire               */,
        char *[]/* E   Tableau de chaines a ecrire              */,
        int     /* E   Taille des chaines a ecrire              */
#endif
);




/*                                              */
/*                                              */
/* Fonctions de fin de pas                      */
/*                                              */
/*                                              */
extern int      cp_aufp(
/*              -------                                 */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */
#endif
);

extern int      cp_infp(
/*              -------                                 */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */
#endif
);

extern int      cp_fini(
/*              -------                                 */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        char  * /* E   Nom de la variable (not in original CALCIUM API)     */,
        int     /* E    Tous les pas <= a cette valeur  */
                /*      seront oublies par le coupleur  */
#endif
);

extern int      cp_fint(
/*              -------                                 */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        char  * /* E   Nom de la variable (not in original CALCIUM API)     */,
        float   /* E    Tous les pas <= a cette valeur  */
                /*      seront oublies par le coupleur  */
#endif
);



/*                                              */
/*                                              */
/* Fonctions d'effacement                       */
/*                                              */
/*                                              */
extern int      cp_effi(
/*              -------                                 */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        char *  /* E    Nom de la variable a effacer    */,
        int     /* E    Tous les pas >= a cette valeurt */
                /*      seront effaces par le coupleur  */
#endif
);


extern int      cp_efft(
/*              -------                                 */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        char *  /* E    Nom de la variable a effacer    */,
        float   /* E    Tous les pas >= a cette valeur  */
                /*      seront effaces par le coupleur  */
#endif
);



/*                                              */
/*                                              */
/* Fonctions de mode d'execution                */
/*                                              */
/*                                              */
extern int      cp_pause(
/*              --------                        */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */
#endif
);

extern int      cp_run(
/*              ------                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */
#endif
);

extern int      cp_npas(
/*              -------                         */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E  Nombre de pas a executer  */
#endif
);



/*                                              */
/*                                              */
/* Fonctions de configuration dynamique         */
/*                                              */
/*                                              */
extern int      cp_dfcode(
/*              --------                        */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        char  * /* E  Declaration du code       */
#endif
);

extern int      cp_dfinst(
/*              --------                        */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        char  * /* E  Declaration de l'instance */
#endif
);

extern int      cp_dflien(
/*              --------                        */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        char  * /* E  Declaration du lien       */
#endif
);



/*                                              */
/*                                              */
/* Fonctions d'interrogation                    */
/*                                              */
/*                                              */
extern int      cp_ivers(
/*              --------                        */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        char *  /* S  Repertoire ACCC           */,
        char *  /* S  Repertoire ACCC           */
#endif
);


extern int      cp_icd(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E  Longueur du tableau passe en parametre    */,
        int *   /* S  Nombre de codes retournes                 */,
        char *[]/* S  Tableau contenant les noms des codes      */
#endif
);

extern int      cp_iincd(
/*              --------                                        */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        char *  /* E  Nom du code dont on veut les instances    */,
        int     /* E  Longueur du tableau passe en parametre    */,
        int *   /* S  Nombre d'instances retournees             */,
        char *[]/* S  Tableau contenant les noms des instances  */
#endif
);

extern int      cp_ivr(
/*              ------                                          */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E  Longueur des tableaux passes en parametre */,
        int *   /* S  Nombre de variables retournees            */,
        char *[]/* S  Tableau contenant les noms des instances  */,
        char *[]/* S  Tableau contenant les noms des variables  */
#endif
);

extern int      cp_ivrcd(
/*              --------                                        */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        char *  /* E  Nom du code dont on veut les variables    */,
        int     /* E  Longueur des tableaux passes en parametre */,
        int *   /* S  Nombre de variables retournees            */,
        char *[]/* S  Tableau contenant les noms des variables  */
#endif
);

extern int      cp_ivrin(
/*              --------                                        */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        char *  /* E  Nom de l'instance dont on veut la liste   */
                /*    des variables                             */,
        char *  /* S  Nom du code pere de l'instance            */,
        int     /* E  Longueur du tableau pour les variables    */
                /*    entrantes                                 */,
        int     /* E  Longueur du tableau pour les variables    */
                /*    sortantes                                 */,
        int *   /* S  Nombre de variables entrantes retournees  */,
        int *   /* S  Nombre de variables sortantes retournees  */,
        char *[]/* S  Tableau contenant les noms des variables  */
                /*    entrantes                                 */,
        char *[]/* S  Tableau contenant les noms des variables  */
                /*    sortantes                                 */
#endif
);

extern int      cp_icavr(
/*              --------                                        */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        char *  /* E  Nom de l'instance contenant la variable   */,
        char *  /* E  Nom local de la variable                  */,
        int *   /* S  Sens de la variable                       */,
        int *   /* S  Dependance de la variable                 */,
        int *   /* S  Type de la variable                       */,
        int     /* E  Longueur des tableaux permettant de       */
                /*    recueillir les pas de temps et d'iteration*/,
        int *   /* S  Nombre de codes retournes                 */,
        float * /* S  Tableau contenant les pas de temps        */,
        int *   /* S  Tableau contenant les pas d'iterations    */
#endif
);

extern int      cp_ilien(
/*              --------                                        */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        char *  /* E  Nom global de la variable                 */,
        int     /* E  Longueur des tableaux pour les noms des   */
                /*    instances et des variables                */,
        int *   /* S  Nombre de variables participant au lien   */,
        char *[]/* S  Tableau contenant les noms des instances  */,
        char *[]/* S  Tableau contenant les noms des variables  */,
        int *   /* S  Type du lien                              */,
        int *   /* S  Niveau du lien                            */,
        int *   /* S  Tableau des parametres d'interpolation    */
                /*    des variables entrantes                   */,
        float * /* S  Tableau des valeurs de delta utilisees    */
                /*    pour les variables entrantes              */,
        int *   /* S  Tableau des parametres d'extrapolation    */
                /*    des variables entrantes                   */
#endif
);



/*                                              */
/*                                              */
/* Fonctions de manipulation d'options          */
/*                                              */
/*                                              */
extern int      cp_setopt(
/*              ---------                                       */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E  Numero identifiant l'option a modifier    */,
        int     /* E  Valeur a donner a l'option                */
#endif
);

extern int      cp_getopt(
/*              ---------                                       */
#if CPNeedPrototype
        void * component /* Pointeur de type Superv_Component_i* sur le */
                         /* composant SALOME Supervisable  */,
        int     /* E  Numero identifiant l'option a consulter   */,
        int *   /* S  Valeur recupereee de l'option             */
#endif
);



#if defined(__cplusplus) || defined(c_plusplus)
}
#endif


#endif
