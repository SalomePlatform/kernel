// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
/*
 * libBatch_Swig.i : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

/* ATTENTION:
	 ==========
	 Certaines classes ont des methodes surchargees et SWIG ne gere pas bien
	 ces surcharges, d'ou un probleme d'utilisation en Python de celles-ci.
	 En bref, ça ne marche pas et il faudra corriger le probleme...

	 TODO : corriger le probleme de surcharge des methodes en Python

	 IDM.
*/


/* Le nom du module Python tel qu'il est importe */
%module libBatch_Swig

/* Inclusion des conversions de type */
%include libBatch_Swig_typemap.i

/* Inclusion de la gestion des exceptions */
%include libBatch_Swig_exception.i

%{
#include "Batch_Job.hxx"
#include "Batch_JobId.hxx"
#include "Batch_JobInfo.hxx"

#include "Batch_BatchManager.hxx"
#include "Batch_BatchManagerCatalog.hxx"
#include "Batch_FactBatchManager.hxx"
%}

/* Les classes exportees en Python */
%include Batch_Job.hxx
%include Batch_JobId.hxx
%include Batch_JobInfo.hxx

%include Batch_BatchManager.hxx
%include Batch_BatchManagerCatalog.hxx
%include Batch_FactBatchManager.hxx



/* Les methodes alterJob (surchargees et mal gerees en Python) sont
	 remplacees par des methodes setParametre et setEnvironnement.
	 cf. remarque ci-dessus.
*/
%ignore JobId::alterJob(const Parametre & param, const Environnement & env) const;
%ignore JobId::alterJob(const Parametre & param) const;
%ignore JobId::alterJob(const Environnement & env) const;
