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
