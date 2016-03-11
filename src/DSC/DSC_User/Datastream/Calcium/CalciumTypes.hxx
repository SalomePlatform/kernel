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

//  File   : CalciumTypes.hxx
//  Author : Eric Fayolle (EDF)
//  Module : KERNEL
// Modified by : $LastChangedBy$
// Date        : $LastChangedDate: 2007-02-28 15:26:32 +0100 (mer, 28 fÃ©v 2007) $
// Id          : $Id$
//
#ifndef __CALCIUM_TYPES__ 
#define __CALCIUM_TYPES__

namespace CalciumTypes {


  const float EPSILON =  1.e-6;


  /* Type de dependance des variables */
  const int CP_TEMPS = 40;
  const int CP_ITERATION = 41;
  const int CP_SEQUENTIEL = 42;
  /* Mode de dependance inconnu  */
  const int     CPIT   = 6;

  /* Directive de continuation d'une instance  */
  const int     CP_CONT         =       20;
  const int     CP_ARRET        =       21;

  /* Type d'interpolation                      */
  const int     CP_LINEAIRE     =       100;
  const int     CP_ESCALIER     =       101;


  /* Repere dans cycle de temps                */
  const int     TI              =       110;
  const int     TF              =       111;


  /* Niveaux                                   */
  const int     CP_ILLIMITE     =       -70;
  const int     CP_AUTESP       =       -71;


    
  typedef int  InfoType ;
  const   int  UNLIMITED_STORAGE_LEVEL = CP_ILLIMITE;
  typedef enum {UNDEFINED_DEPENDENCY=CPIT,TIME_DEPENDENCY=CP_TEMPS,
                ITERATION_DEPENDENCY=CP_ITERATION,
                // TYPE uniquement utilisé ds CalciumInterface 
                SEQUENCE_DEPENDENCY =CP_SEQUENTIEL 
                                                   }                        DependencyType;
  typedef enum {TI_SCHEM=TI,TF_SCHEM=TF,ALPHA_SCHEM}                        DateCalSchem;
  typedef enum {L0_SCHEM=CP_ESCALIER,L1_SCHEM=CP_LINEAIRE}                  InterpolationSchem;
  typedef enum {UNDEFINED_EXTRA_SCHEM,E0_SCHEM,E1_SCHEM}                    ExtrapolationSchem;
  typedef enum {UNDEFINED_DIRECTIVE=0,CONTINUE=CP_CONT,STOP=CP_ARRET}       DisconnectDirective;

  /* Codes d'erreur   */

  /* Pas d'erreur          */
  const int CPOK   = 0;

  /* Emetteur inconnu   */
  const int CPERIU = 1;

  /* Nom de variable inconnu  */
  const int CPNMVR = 2;
  
  /* Type entree/sortie incompatible */
  const int CPIOVR = 3;
  
  /* Type inconnu    */
  const int CPTP   = 4;
  
  /* Type de variable incompatible */
  const int CPTPVR = 5;
  
  /* Mode de dependance inconnu  */
  // Déclaré au dessus 
  // const int CPIT   = 6;

  /* Mode dependance incompatible  */
  const int CPITVR = 7;

  /* Requete non autorisee  */
  const int CPRENA = 8;

  /* Type de deconnexion incorrect */
  const int CPDNTP = 9;

  /* Directive de deconnexion incorrecte */
  const int CPDNDI = 10;

  /* Nom de code inconnu   */
  const int CPNMCD = 11;

  /* Nom d'instance inconnu  */
  const int CPNMIN = 12;

  /* Attente    */
  const int CPATTENTE = 13;

  /* Blocage    */
  const int CPBLOC    = 14;

  /* Nombre de valeurs transmises egal a zero */
  const int CPNTNULL  = 15;

  /* Longueur de variable insuffisante */
  const int CPLGVR    = 16;

  /* L'instance doit s'arreter  */
  const int CPSTOP    = 17;

  /* Arret anormal   */
  const int CPATAL    = 18;

  /* Coupleur absent   */
  const int CPNOCP    = 19;

  /* Variable sortante non connectee */
  const int CPCTVR    = 20;

  /* Nombre de pas a executer egal a zero */
  const int CPPASNULL = 21;

  /* Machine inconnue   */
  const int CPMACHINE = 22;

  /* COUPLAGE_GROUPE non positionnee */
  const int CPGRNU    = 23;

  /* Groupe d'instances incorrect  */
  const int CPGRIN    = 24;

  /* Fin du fichier d'entree  */
  const int CPFINFICH = 25;

  /* Erreur de format dans un fichier */
  const int CPERRFICH = 26;

  /* Requete d'avance de n pas annulee */
  /* par passage en mode NORMAL  */
  const int CPNORERR  = 27;

  /* Coupleur en mode NORMAL pour une */
  /* requete RUN_N_PAS ou DEF_*  */
  const int CPRUNERR  = 28;

  /* Option inconnue   */
  const int CPOPT     = 29;

  /* Valeur d'option inconnue  */
  const int CPVALOPT  = 30;

  /* Ecriture impossible par effacement */
  const int CPECREFF  = 31;

  /* Lecture d'une variable non connectee */
  /* ou n'appartenant pas a un lien VAS */
  /* ou VAV s'il s'agit d'une sortante */
  const int CPLIEN    = 32;

  /* Lecture d'une variable d'une instance*/
  /* deconnectee avec directive CP_ARRET */
  const int CPINARRET = 33;

  /* Les lectures sequentielles ne pourront plus */
  /* etre satisfaites : instance productrice arretee */
  const int CPSTOPSEQ = 34;

  /* Erreur dans la chaine de declaration */
  const int CPDECL    = 35;

  /* Erreur dans l'execution de l'instance ajoutee */
  const int CPINEXEC  = 36;

  /* Erreur PVM    */
  //   const int CPPVM = 37;
  //   const int CPCOM = 37;
  /* Erreur detectee au niveau de l'int CPERRINST = 38;

  /* Mode d'execution non defini  */
  //   const int CPMODE    = 39;

  /* Instance deconnectee   */
  const int CPINSTDEC = 40;

}
#endif
