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
 * BatchManager.hxx : 
 *
 * Auteur : Bernard SECHER - CEA/DEN
 * Date   : Juillet 2007
 * Projet : SALOME
 *
 */

#ifndef _BL_BATCHMANAGER_PBS_H_
#define _BL_BATCHMANAGER_PBS_H_

#include <string>
#include "Utils_SALOME_Exception.hxx"
#include "BatchLight_BatchManager.hxx"

namespace BatchLight {

  class Job;

  class BatchManager_PBS : public BatchManager
  {
  public:
    // Constructeur et destructeur
    BatchManager_PBS(const batchParams& p) throw(SALOME_Exception); // connexion a la machine host
    virtual ~BatchManager_PBS();

    // Methodes pour le controle des jobs : virtuelles pures
    void deleteJob(const int & jobid); // retire un job du gestionnaire
    std::string queryJob(const int & jobid); // renvoie l'etat du job

  private:
    void buildSalomeCouplingScript(BatchLight::Job* job) throw(SALOME_Exception);
    void buildSalomeBatchScript(BatchLight::Job* job) throw(SALOME_Exception);
    int submit(BatchLight::Job* job) throw(SALOME_Exception);

    // Permet d'avoir la chaîne complête pour demander
    // le statut du job
    typedef std::map<int, std::string> _pbs_job_name_t;
    _pbs_job_name_t _pbs_job_name;
  };

}

#endif
