/*
 * _exception.i : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

%exception {
    try {
      $action
    }
    catch (GenericException & ex) {
      string msg = ex.type + " : " + ex.message;
      PyErr_SetString(PyExc_RuntimeWarning, msg.c_str());
      return NULL;
    }
    catch (...) {
      PyErr_SetString(PyExc_RuntimeWarning, "unknown exception");
      return NULL;
    }
}

