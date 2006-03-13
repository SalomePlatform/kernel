/*
 * IOMutex.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Fri Nov 14 11:00:39 2003
 * Projet : Salome 2
 *
 */

#include "Batch_IOMutex.hxx"

namespace Batch {

  pthread_mutex_t IOMutex = PTHREAD_MUTEX_INITIALIZER;

}
