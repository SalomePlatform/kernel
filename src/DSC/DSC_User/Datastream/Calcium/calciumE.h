// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

#ifndef __CALCIUM_E_H
#define __CALCIUM_E_H

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
extern int      ecp_cd(
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
extern int      ecp_fin(
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
/* Fonctions de libération du buffer 0 copy  */
/*                                              */

  extern void ecp_len_free(
#if CPNeedPrototype
                           int *
#endif
                           );
  extern void ecp_lre_free(
#if CPNeedPrototype
                           float *
#endif
                           );
  extern void ecp_ldb_free(
#if CPNeedPrototype
                           double *
#endif
                           );
  extern void ecp_llo_free(
#if CPNeedPrototype
                           int *
#endif
                           );
  extern void ecp_lcp_free(
#if CPNeedPrototype
                           float *
#endif
                           );
  extern void ecp_lch_free(
#if CPNeedPrototype
                           char **
#endif
                           );



/*                                              */
/*                                              */
/* Fonctions de lecture bloquante 0 copy        */
/*                                              */
/*                                              */
extern int      ecp_len(
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
        int   ** /* E/S   Tableau d'entiers pour stocker les    */
                /*     valeurs lues                             */
#endif
);

extern int      ecp_lre(
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
        float **/* E/S   Tableau de flottants pour stocker les  */
                /*     valeurs lues                             */
#endif
);

extern int      ecp_ldb(
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
        double**/* E/S   Tableau de doubles pour stocker les    */
                /*     valeurs lues                             */
#endif
);

extern int      ecp_lcp(
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
        float **/* E/S   Tableau de flottants pour stocker les  */
                /*     valeurs lues (dimension = 2 * le nombre  */
                /*     de valeurs lues)                         */
#endif
);

extern int      ecp_llo(
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
        int  ** /* E/S   Tableau d 'entier pour stocker les     */
                /*     valeurs lues (remplace le logiques)      */
#endif
);

extern int      ecp_lch(
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
        char **[]/*E/S   Tableau de chaines pour stocker les    */
                /*     valeurs lues (remplace le logiques)      */,
        int     /* E   Taille des chaines du tablaeu            */
#endif
);



/*                                              */
/*                                              */
/* Fonctions de lecture non bloquantes          */
/*                                              */
/*                                              */
extern int      ecp_nlen(
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
        int  ** /* E/S   Tableau d'entiers pour stocker les     */
                /*     valeurs lues                             */
#endif
);

extern int      ecp_nlre(
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
        float **/* E/S   Tableau de flottants pour stocker les  */
                /*     valeurs lues                             */
#endif
);

extern int      ecp_nldb(
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
        double**/* E/S   Tableau de doubles pour stocker les    */
                /*     valeurs lues                             */
#endif
);

extern int      ecp_nlcp(
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
        float **/* E/S   Tableau de flottants pour stocker les  */
                /*     valeurs lues (dimension = 2 * le nombre  */
                /*     de valeurs lues)                         */
#endif
);

extern int      ecp_nllo(
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
        int   **/* E/S   Tableau d 'entier pour stocker les     */
                /*     valeurs lues (remplace le logiques)      */
#endif
);

extern int      ecp_nlch(
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
        char **[]/* E/S   Tableau de chaines pour stocker les   */
                /*     valeurs lues (remplace le logiques)      */,
        int     /* E   Taille des chaines du tablaeu            */
#endif
);



#if defined(__cplusplus) || defined(c_plusplus)
}
#endif


#endif
