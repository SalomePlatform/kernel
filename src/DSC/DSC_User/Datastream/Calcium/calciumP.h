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

//  File   : calciumP.h
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
//
/* Outils d'Aide au Couplage de Code de Calcul : $Id$ */
#ifndef __CONST_H
#define __CONST_H

#ifndef        TrueOrFalse
#define        TrueOrFalse        int
#define        TRUE        1
#define        FALSE        0
#endif

/* Definition d'un type de donnes pour le stockage du        */
/* des pas de temps                                        */
#ifdef CRAY
#define        Temps                float
#define        FORMAT_TEMPS        "%lf"
#else
#define Temps                double
#define        FORMAT_TEMPS        "%lg"
#endif

/* Precision relative pour tests d'egalite sur les temps */
#define EPSILON  1.e-6

/* Tailles utilisees lors des transmissions des noms        */
#define                VERSION_LEN        144
#define                CODE_LEN        72
#define                VARIABLE_LEN        144
#define                INSTANCE_LEN        72
#define                DRIVER_LEN        72


/* Tailles maximums                                         */
#define                ARCHI_LEN                64
#define                ENV_VAR_LEN                256
#define                ACCC_LEN                256
#define                PVM_LEN                        256
#define                USER_LEN                256
#define                MACHINE_LEN                256
#define                COMMAND_LEN                256
#define                MAX_LEN                        256
#define                FICHIER_LEN                256

/* Nombre de lignes dans une page du fichier erreur         *
 * de trace                                                */
#define                NB_LIGNE_ERREUR                45
#define                NB_LIGNE_TRACE                45



/* Type d'arret d'une instance                                */
#define                CP_NORMALE                10
#define                CP_ANORMALE                11

/* Directive de continuation d'une instance                */
#define                CP_CONT                        20
#define                CP_ARRET                21

/* Etat d'une instance                                        */
#define                CP_INITIAL                22
#define                CP_NON_CONNECTE                23
#define                CP_ATTENTE                24
#define                CP_EXECUTION                25
#define                CP_DECONNECTE                26
#define                CP_TERMINE                27

/* Type de variables                                        */
#define                CP_ENTIER                30
#define                CP_REEL                        31
#define                CP_DREEL                32
#define                CP_COMPLEXE                33
#define                CP_LOGIQUE                34
#define                CP_CHAINE                35

/* Type de dependance des variables                        */
#define                CP_TEMPS                40
#define                CP_ITERATION                41
#define                CP_SEQUENTIEL                42

/* Send des variables                                        */
#define                CP_IN                        50
#define                CP_OUT                        51

/* Type des instances                                        */
#define                CP_NORMAL                60
#define                CP_ESPION                62

/* Niveaux                                                */
#define                CP_ILLIMITE                -70
#define                CP_AUTESP                -71

/* Mode de trace                                        */
#define                CP_SANS                        80
#define                CP_SUCCINCT                81
#define                CP_DETAILLE                82


/* Mode d'execution                                        */
/* La valeur CP_NORMAL definie pour les types                */
/* est aussi utilisee                                        */
#define                CP_PAUSE                91


/* Type d'interpolation                                        */
#define                CP_LINEAIRE                100
#define                CP_ESCALIER                101


/* Repere dans cycle de temps                                */
#define                TI                        110
#define                TF                        111

/* Mode de nettoyage des donnees d'un lien                */
#define                CP_TOUTES                120
#define                CP_AUCUNE                121
#define                CP_PAS                   122

/* Options configurables                                */
#define                CP_ROUTE                131
#define                CP_ERREURS                132

/* Valeurs pour l'option CP_ROUTE                        */
#define                CP_ROUTE_NORMAL                133
#define                CP_ROUTE_DIRECT                134

/* Valeurs pour l'option Comportement en cas d'erreur        */
#define                CP_AUTO                        135
#define                CP_BLOCAGE                136
#define                CP_MANUEL                137

/* Type de requete de lecture                                */
/* La valeur CP_ATTENTE define pour l'etat d'une        */
/* instance est aussi utilisee                                */
#define                CP_IMMEDIATE                141

/* Definition des types de lien                                */
#define CPLVAV                                151
#define CPLVAF                                152
#define CPLVAS                                153
#define CPLCAV                                154
#define CPLFAV                                155

/* Codes des entetes des messages (Evenemet *)                */
#define                ERREUR                                1000
#define                CONNEXION                        1001
#define                EMISSION                        1002
#define                DECONNEXION                        1003
#define                LECTURE_VARIABLE                1004
#define                ECRITURE_VARIABLE                1005
#define                FIN_DE_PAS                        1006
#define                AUIN_FIN_DE_PAS                        1007
#define                EFFACEMENT                        1008
#define                MODE_EXEC                        1009
#define                RUN_N_PAS                        1010
#define                DEF_CODE                        1011
#define                DEF_INSTANCE                        1012
#define                DEF_LIEN                        1013
#define                DEM_VERSION                        1014
#define                SET_OPTION                        1015
#define                DEM_OPTION                        1016
#define                DEM_CODES                        1017
#define                DEM_INSTS_DE_CODE                1018
#define                DEM_VARIABLES                        1019
#define                DEM_VARS_DE_CODE                1020
#define                DEM_VARS_DE_INST                1021
#define                DEM_CARS_DE_VARIABLE                1022
#define                DEM_CARS_DE_LIEN                1023
#define                ENV_VALEURS_VARIABLE                1024
#define                ENV_OPTION                        1025
#define                ENV_CODES                        1026
#define                ENV_INSTS_DE_CODE                1027
#define                ENV_VARIABLES                        1028
#define                ENV_VARS_DE_CODE                1029
#define                ENV_VARS_DE_INST                1030
#define                ENV_CARS_DE_VARIABLE                1031
#define                ENV_CARS_DE_LIEN                1032
#define         ENV_TOPOLOGY                    1033
#define         R_ENV_TOPOLOGY                  1034
#define         ENV_MACHINE                     1035
#define         R_ENV_MACHINE                   1036
#define         ENV_CODE                        1037
#define         R_ENV_CODE                      1038
#define         ENV_INSTANCE                    1039
#define         R_ENV_INSTANCE                  1040
#define         ENV_VAR                         1041
#define         R_ENV_VAR                       1042
#define         ENV_LIEN                        1043
#define         R_ENV_LIEN                      1044
#define         ENV_ATTRIBUTS                   1045
#define         R_ENV_ATTRIBUTS                 1046
#define         ENV_VDATA                       1047
#define         R_ENV_VDATA                     1048









/* Message PVM                                                */
#define                P_HOST_DELETE                        2000
#define                P_TASK_EXIT                        2001





/* Codes d'erreur                                        */

/* Pas d'erreur                                */
#define                CPOK                        0

/* Emetteur inconnu                        */
#define                CPERIU                        1

/* Nom de variable inconnu                */
#define                CPNMVR                        2

/* Type entree/sortie incompatible        */
#define                CPIOVR                        3

/* Type inconnu                                */
#define                CPTP                        4

/* Type de variable incompatible        */
#define                CPTPVR                        5

/* Mode de dependance inconnu                */
#define                CPIT                        6

/* Mode dependance incompatible                */
#define                CPITVR                        7

/* Requete non autorisee                */
#define                CPRENA                        8

/* Type de deconnexion incorrect        */
#define                CPDNTP                        9

/* Directive de deconnexion incorrecte        */
#define                CPDNDI                        10

/* Nom de code inconnu                        */
#define                CPNMCD                        11

/* Nom d'instance inconnu                */
#define                CPNMIN                        12

/* Attente                                */
#define                CPATTENTE                13

/* Blocage                                */
#define                CPBLOC                        14

/* Nombre de valeurs transmises egal a zero        */
#define                CPNTNULL                15

/* Longueur de variable insuffisante        */
#define                CPLGVR                        16

/* L'instance doit s'arreter                */
#define                CPSTOP                        17

/* Arret anormal                        */
#define                CPATAL                        18

/* Coupleur abscent                        */
#define                CPNOCP                        19

/* Variable sortante non connectee        */
#define                CPCTVR                        20

/* Nombre de pas a executer egal a zero        */
#define                CPPASNULL                21

/* Machine inconnue                        */
#define                CPMACHINE                22

/* COUPLAGE_GROUPE non positionnee        */
#define                CPGRNU                        23

/* Groupe d'instances incorrect                */
#define                CPGRIN                        24

/* Fin du fichier d'entree                */
#define                CPFINFICH                25

/* Erreur de format dans un fichier        */
#define                CPERRFICH                26

/* Requete d'avance de n pas annullee        */
/* par passage en mode NORMAL                */
#define                CPNORERR                27

/* Coupleur en mode NORMAL pour une        */
/* requete RUN_N_PAS ou DEF_*                */
#define                CPRUNERR                28

/* Option inconnue                        */
#define                CPOPT                        29

/* Valeur d'option inconnue                */
#define                CPVALOPT                30

/* Ecriture impossible par effacement        */
#define                CPECREFF                31

/* Lecture d'une variable non connectee        */
/* ou n'appartenant pas a un lien VAS        */
/* ou VAV s'il s'agit d'une sortante        */
#define                CPLIEN                        32

/* Lecture d'une variable d'une instance*/
/* deconnectee avec directive CP_ARRET        */
#define                CPINARRET                33

/* Les lectures sequentielles ne pourront plus                */
/* etre satisfaites : instance productrice arretee        */
#define                CPSTOPSEQ                34

/* Erreur dans la chaine de declaration        */
#define                CPDECL                        35

/* Erreur dans l'execution de l'instance ajoutee */
#define                CPINEXEC                36

/* Erreur PVM                                */
#define                CPPVM                        37

/* Erreur detectee au niveau de l'interface de couplage        */
#define                CPERRINST                38


/* IDM : les deux define suivants ont ete rajoutes a la main*/

/* Erreur de mode d'execution non defini */
#define                CPMODE                        39

/* Erreur d'instance deconnectee */
#define         CPINSTDEC                40

/* Codes des evenements pour le fichier de trace        */

/* Debut du couplage                                        */
#define                DEBUT_COUPLAGE                0

/* Fin du couplage                                        */
#define                FIN_COUPLAGE                1

/* Execution d'une instance par Oacc                        */
#define                EXEC                        2

/* Connexion d'une instance                                */
#define                CPCD                        3

/* Permission d'emettre accordee a l'instance                */
#define                CPALLOW                        4

/* Requete d'ecriture                                        */
#define                CPECRI                        5

/* Requete de lecture                                        */
#define                DEB_LECT                6

/* Envoi des donnees suite a une requete de lecture        */
#define                FIN_LECT                7

/* Deconnexion d'une instance                                */
#define                CPFIN                        8

/* Requete de fin de pas                                */
#define                CPFINP                        9

/* Requete d'autorisation de fin de pas                        */
#define                CPAUFP                        10

/* Requete d'interdiction de fin de pas                        */
#define                CPINFP                        11

/* Requete d'effacement                                        */
#define                CPEFF                        12

/* Signal d'arret d'une instance                        */
#define                STOP                        13

/* Avis de blocage suite a une requete de lecture        */
#define                BLOCAGE                        14

/* Requete de passage en mode pause                        */
#define                CPPAUSE                        15

/* Requete de passage en mode normal                        */
#define                CPNORMAL                16

/* Requete d'execution de n pas                                */
#define                CPNPAS                        17

/* Requete de definition d'un code                        */
#define                CPADCD                        18

/* Requete de definition d'une instance                        */
#define                CPADINCD                19

/* Requete de definition d'un lien                        */
#define                CPADLN                        20

/* Requete d'identification de version                        */
#define                CPIVERS                        21

/* Requete de demande de la liste des codes                */
#define                CPICD                        22

/* Requete de demande des instances d'un code                */
#define                CPIINCD                        23

/* Requete de demande de la liste des variables globales*/
#define                CPIVR                        24

/* Requete de demande des variables d'un code                */
#define                CPIVRCD                        25

/* Requete de demande des variables d'une instance        */
#define                CPIVRIN                        26

/* Requete de demande d'info sur une variable globale        */
#define                CPICAVR                        27

/* Requete de demande des caracteristiques d'un lien        */
#define                CPIILIEN                28

/* Requete de modification d'une option                        */
#define                CPSETOPT                29

/* Requete de consultation d'une option                        */
#define                CPGETOPT                30

/* Terminaison d'une tache PVM                                */
#define                TASK_EXIT                31

/* Deconnexion d'une machine                                */
#define                HOST_DELETE                32


#ifdef PRG_MAIN
/*        Fichier principal de la bibliotheque de couplage */
  const char *  CPMESSAGE[] = {
    "",
    "Emitter unknown",
    "Variable name unknown",
    "Different input/output codes in code and supervisor",
    "Variable type unknown",
    "Different variable types in code and supervisor",
    "Dependency mode unknown",
    "Different dependency modes in code and supervisor",
    "Unauthorized request",
    "Unauthorized disconnection request type",
    "Unauthorized disconnection directive",
    "Code name unknown",
    "Instance name unknown",
    "Waiting request",
    "Blocking",
    "Zero value number",
    "Insufficient variable length",
    "Instance is going to stop",
    "Unexpected instance stop",
    "Manuel execution",
    "Output variable not connected",
    "Number of steps to execute is nul",
    "Non declared computer",
    "Environment variable COUPLAGE_GROUPE is not set",
    "Instance group given by COUPLAGE_GROUPE is wrong",
    "End of input file",
    "Format error in input file",
    "Request ignored because of switching to NORMAL mode",
    "Supervisor is in normal execution mode",
    "Unknown option",
    "Option value is wrong",
    "Impossible to write because of an erasing request",
    "Reading of a variable wrongly connected",
    "Reading of a variable of an instance disconnected with CP_ARRET",
    "Sequential reading no more possible",
    "Error in declaration",
    "Error in instance launching",
    "Communication error",
    "Error in the instance",
    "Environnement variable CAL_MODE is not set",
    "Disconnected instance",
  };

#else

extern const char * CPMESSAGE[];

#endif


/* Type de variables        */
#define        ENTIER        30
#define        REEL          31
#define        DREEL         32
#define        COMPLEXE      33
#define        LOGIQUE       34
#define        CHAINE        35

/* Macro minuscule majuscule */
#define                TOUPPER(string)        \
{\
  int i, number = strlen(string);\
  for (i = 0; i < number; i++) string[i] = toupper(string[i]); \
}

/* Macro inferieur */
#define                INF(a,b) (a <= b ? a : b)

/* Macro superieur */
#define                SUP(a,b) (a >= b ? a : b)


#endif

