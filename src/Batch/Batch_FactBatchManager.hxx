/*
 * FactBatchManager.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2004
 * Projet : SALOME 2
 *
 */

#ifndef _FACTBATCHMANAGER_H_
#define _FACTBATCHMANAGER_H_

#include <string>
#include <map>

namespace Batch {
  
  class BatchManager;

  class FactBatchManager
  {
  public:
    // Constructeur et destructeur
    FactBatchManager(const std::string & type);
    virtual ~FactBatchManager();

    virtual Batch::BatchManager * operator() (const char * hostname) const = 0;
    std::string getType() const;
    std::string __repr__() const;

  protected:
    std::string type;

  private:

  };

}

#endif
