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

using namespace std;
#include <string>
#include <map>

namespace Batch {
  
  class BatchManager;

  class FactBatchManager
  {
  public:
    // Constructeur et destructeur
    FactBatchManager(const string & type);
    virtual ~FactBatchManager();

    virtual BatchManager * operator() (const char * hostname) const = 0;
    string getType() const;
    string __repr__() const;

  protected:
    string type;

  private:

  };

}

#endif
