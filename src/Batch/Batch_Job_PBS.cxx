/*
 * Job_PBS.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Fri Nov 14 11:00:39 2003
 * Projet : Salome 2
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Batch_Job_PBS.hxx"
using namespace std;

namespace Batch {

  // Ajoute un element (name,resource,value) a la liste chainee d'attributs + operateur
  void Job_PBS::setResourceAttributeOP(struct attropl ** attr_list, const char * attr_name, const char * attr_resource, const char * attr_value)
  {
    // L'element (name,resource,value) existe-t-il deja ? si oui ptr != 0
    struct attropl * ptr = findResourceAttributeOP(*attr_list, attr_name, attr_resource);

    if (!ptr) { // L'element n'existe pas, il faut le creer
      if (ptr = lastAttributeOP(*attr_list)) { // la liste n'est pas vide
	ptr->next = new struct attropl;
	ptr = ptr->next;
	ptr->next = 0;

      } else { // la liste est completement vide
	ptr = *attr_list = new struct attropl;
	ptr->next = 0;
      }
      
      // On remplit les champs (name,resource,value)
      ptr->name = new char[strlen(attr_name) + 1];
      strncpy(ptr->name, attr_name, strlen(attr_name));
      ptr->name[strlen(attr_name)] = 0;

      ptr->resource = new char[strlen(attr_resource) + 1];
      strncpy(ptr->resource, attr_resource, strlen(attr_resource));
      ptr->resource[strlen(attr_resource)] = 0;

      ptr->value = new char[strlen(attr_value) + 1];
      strncpy(ptr->value, attr_value, strlen(attr_value));
      ptr->value[strlen(attr_value)] = 0;

    } else { // L'attribut existe, on change sa valeur
      delete[] ptr->value; // On efface la valeur precedente
      ptr->value = new char[strlen(attr_value) + 1];
      strncpy(ptr->value, attr_value, strlen(attr_value));
      ptr->value[strlen(attr_value)] = 0;

    }
  }


  // Recherche un element (name,resource,value) dans la liste chainee d'attributs + operateur
  struct attropl * Job_PBS::findResourceAttributeOP(struct attropl * attr_list, const char * attr_name, const char * attr_resource)
  {
    // On parcoure la liste chainee pour trouver l'element dont les champs name et resource coincident
    struct attropl * ptr = attr_list;
    while (ptr) {
      if (!strcmp(ptr->name, attr_name) && !strcmp(ptr->resource, attr_resource)) break;
      ptr = ptr->next;
    }
    return ptr;
  }


  // Recherche le dernier element de la liste chainee d'attributs + operateur
  struct attropl * Job_PBS::lastAttributeOP(struct attropl * attr_list)
  {
    struct attropl * ptr = attr_list;
    while (ptr && ptr->next) {
      ptr = ptr->next;
    }
    return ptr;
  }
  

  // Convertit un objet Parametre en liste chainee d'attributs + operateur
  struct attropl * Job_PBS::ParametreToAttributeOPList(struct attropl ** _p_attr_list, Parametre & P)
  {
    Parametre::iterator it;
    string st_second;
    for(it=P.begin(); it!=P.end(); it++) {
      if ( (*it).first == ACCOUNT ) {
	st_second = (*it).second.str();
	setResourceAttributeOP(_p_attr_list, ATTR_A, "", st_second.c_str());


      } else if ( (*it).first == CHECKPOINT ) {
	setResourceAttributeOP(_p_attr_list, ATTR_c, "", "u");


      } else if ( (*it).first == CKPTINTERVAL ) {
	// Not significant

      } else if ( (*it).first == EXECUTABLE ) {
	// Already done

      } else if ( (*it).first == HOLD ) {
	if (static_cast< long >((*it).second)) 
	  setResourceAttributeOP(_p_attr_list, ATTR_h, "", USER_HOLD);
	else 
	  setResourceAttributeOP(_p_attr_list, ATTR_h, "", NO_HOLD);

      } else if ( (*it).first == INFILE ) {
	Versatile V = (*it).second;
	Versatile::iterator Vit;

	string sep = "";
	string stagein;

	for(Vit=V.begin(); Vit!=V.end(); Vit++, sep=",") {
	  CoupleType cpt  = *static_cast< CoupleType * >(*Vit);
	  Couple cp       = cpt;
	  string local    = cp.getLocal();
	  string remote   = cp.getRemote();
					
	  // ATTENTION : les notions de fichier "local" ou "remote" sont inverses de celle de PBS qui a un point de vue serveur et non pas utilisateur
	  stagein        += sep + remote + "@" + local;
	}

	if (stagein.size())
	  setResourceAttributeOP(_p_attr_list, ATTR_stagein, "", stagein.c_str());


      } else if ( (*it).first == MAIL ) {
	st_second = (*it).second.str();
	setResourceAttributeOP(_p_attr_list, ATTR_M, "", st_second.c_str());

      } else if ( (*it).first == MAXCPUTIME ) {
	char attr_value[32];
	long secondes = (*it).second;
	long heures = secondes / 3600L;
	long minutes = (secondes - 3600L * heures) / 60L;
	secondes = secondes % 60L;
	sprintf(attr_value, "%02ld:%02ld:%02ld", heures, minutes, secondes);

	setResourceAttributeOP(_p_attr_list, ATTR_l, "cput", attr_value);


      } else if ( (*it).first == MAXDISKSIZE ) {

      } else if ( (*it).first == MAXRAMSIZE ) {

      } else if ( (*it).first == MAXWALLTIME ) {
	char attr_value[32];
	long secondes = (*it).second;
	long heures = secondes / 3600L;
	long minutes = (secondes - 3600L * heures) / 60L;
	secondes = secondes % 60L;
	sprintf(attr_value, "%02ld:%02ld:%02ld", heures, minutes, secondes);

	setResourceAttributeOP(_p_attr_list, ATTR_l, "walltime", attr_value);


      } else if ( (*it).first == NAME ) {
	st_second = (*it).second.str();
	setResourceAttributeOP(_p_attr_list, ATTR_N, "", st_second.c_str());
	

      } else if ( (*it).first == OUTFILE ) {
	Versatile V = (*it).second;
	Versatile::iterator Vit;

	string sep = "";
	string stageout;

	for(Vit=V.begin(); Vit!=V.end(); Vit++, sep=",") {
	  CoupleType cpt  = *static_cast< CoupleType * >(*Vit);
	  Couple cp       = cpt;
	  string local    = cp.getLocal();
	  string remote   = cp.getRemote();

	  if (remote == "stdout")
	    setResourceAttributeOP(_p_attr_list, ATTR_o, "", local.c_str());
 
	  else if (remote == "stderr")
	    setResourceAttributeOP(_p_attr_list, ATTR_e, "", local.c_str());

	  else 
	    // ATTENTION : les notions de fichier "local" ou "remote" sont inverses de celle de PBS qui a un point de vue serveur et non pas utilisateur
	    stageout       += sep + remote + "@" + local;
	}
				
	if (stageout.size())
	  setResourceAttributeOP(_p_attr_list, ATTR_stageout, "", stageout.c_str());

      } else if ( (*it).first == QUEUE ) {
	// Already done

      } else if ( (*it).first == STARTDATE ) {

      } else if ( (*it).first == TMPDIR ) {

      } else if ( (*it).first == USER ) {
	st_second = (*it).second.str();
	setResourceAttributeOP(_p_attr_list, ATTR_u, "", st_second.c_str());

      }
    }
    return *_p_attr_list; 
  }


  // Convertit un objet Environnement en liste chainee d'attributs + operateur
  struct attropl * Job_PBS::EnvironnementToAttributeOPList(struct attropl ** _p_attr_list, Environnement & E)
  {
    Environnement::iterator it;
    for(it=E.begin(); it!=E.end(); it++) {
      setResourceAttributeOP(_p_attr_list, ATTR_v, (*it).first.c_str(), ( (*it).first + "=" + (*it).second ).c_str());
    }
    return *_p_attr_list;
  }


  // Ajoute les variables d'environnement presentes dans tout job PBS
  void Job_PBS::addPBSEnvironnement(Environnement & E)
  {
    char * c;

    c = getenv("HOME");
    if (c) E["PBS_O_HOME"] = c;

    c = getenv("LANG");
    if (c) E["PBS_O_LANG"] = c;

    c = getenv("LOGNAME");
    if (c) E["PBS_O_LOGNAME"] = c;

    c = getenv("PATH");
    if (c) E["PBS_O_PATH"] = c;

    c = getenv("LD_LIBRARY_PATH");
    if (c) E["PBS_O_LD_LIBRARY_PATH"] = c;

    c = getenv("MAIL");
    if (c) E["PBS_O_MAIL"] = c;

    c = getenv("SHELL");
    if (c) E["PBS_O_SHELL"] = c;

    c = getenv("TZ");
    if (c) E["PBS_O_TZ"] = c;

    /* Recuperation du working directory */
    size_t size = 256;
    char * buf = 0;
    char * rc = 0;
    do {
      if (buf) delete[] buf;
      buf = new char[size];
      rc = getcwd(buf, size);
      size += size;
    } while (!rc);
    E["PBS_O_WORKDIR"] = buf;
    delete[] buf;
  }


  // Ajoute un element (name,resource,value) a la liste chainee d'attributs
  void Job_PBS::setResourceAttribute(struct attrl ** attr_list, const char * attr_name, const char * attr_resource, const char * attr_value)
  {
    // L'element (name,resource,value) existe-t-il deja ? si oui ptr != 0
    struct attrl * ptr = findResourceAttribute(*attr_list, attr_name, attr_resource);

    if (!ptr) { // L'attribut n'existe pas, il faut le creer
      if (ptr = lastAttribute(*attr_list)) { // la liste n'est pas vide
	ptr->next = new struct attrl;
	ptr = ptr->next;
	ptr->next = 0;

      } else { // la liste est completement vide
	ptr = *attr_list = new struct attrl;
	ptr->next = 0;
      }
      
      // On remplit les champs (name,resource,value)
      ptr->name = new char[strlen(attr_name) + 1];
      strncpy(ptr->name, attr_name, strlen(attr_name));
      ptr->name[strlen(attr_name)] = 0;

      ptr->resource = new char[strlen(attr_resource) + 1];
      strncpy(ptr->resource, attr_resource, strlen(attr_resource));
      ptr->resource[strlen(attr_resource)] = 0;

      ptr->value = new char[strlen(attr_value) + 1];
      strncpy(ptr->value, attr_value, strlen(attr_value));
      ptr->value[strlen(attr_value)] = 0;

    } else { // L'attribut existe, on change sa valeur
      delete[] ptr->value; // On efface la valeur precedente
      ptr->value = new char[strlen(attr_value) + 1];
      strncpy(ptr->value, attr_value, strlen(attr_value));
      ptr->value[strlen(attr_value)] = 0;

    }
  }

  // Recherche un element (name,resource,value) dans la liste chainee d'attributs
  struct attrl * Job_PBS::findResourceAttribute(struct attrl * attr_list, const char * attr_name, const char * attr_resource)
  {
    // On parcoure la liste chainee pour trouver l'element dont les champs name et resource coincident
    struct attrl * ptr = attr_list;
    while (ptr) {
      if (!strcmp(ptr->name, attr_name) && !strcmp(ptr->resource, attr_resource)) break;
      ptr = ptr->next;
    }
    return ptr;
  }

  
  // Recherche le dernier element de la liste chainee d'attributs
  struct attrl * Job_PBS::lastAttribute(struct attrl * attr_list)
  {
    struct attrl * ptr = attr_list;
    while (ptr && ptr->next) {
      ptr = ptr->next;
    }
    return ptr;
  }
  

  // Constructeur
  Job_PBS::Job_PBS(const Job & job) : _p_attropl(0), _p_attrl(0), _script(0), _destination(0)
  {
    Parametre P = job.getParametre();
    Parametre::iterator it;

    // On extrait de l'objet Parametre le champ EXECUTABLE qui deviendra le script PBS
    if ( (it=P.find(EXECUTABLE)) != P.end()) {
      Versatile V = (*it).second;
      string st_exe = V.str();
      const char * exe = st_exe.c_str();
      int lg = strlen(exe);
      _script = new char[lg + 1];
      for (int ii=0; ii<lg+1; ii++) _script[ii] = 0;
      strcpy(_script, exe);
      _script[lg] = 0;

    } else {
      _script = new char[1];
      _script[0] = 0;
    }

    // On extrait de l'objet Parametre le champ QUEUE qui deviendra la destination
    if ( (it=P.find(QUEUE)) != P.end()) {
      Versatile V = (*it).second;
      string st_dest = V.str();
      const char * dest = st_dest.c_str();
      _destination = new char[strlen(dest) + 2 + 1];
      _destination[0] = 0;
      strcat(_destination, dest);
      _destination[strlen(dest)] = 0;
    } else {
      _destination = new char[1];
      _destination[0] = 0;
    }

    // On convertit les objets Parametre et Environnement en liste chainee d'attributs + operateur
    Environnement E = job.getEnvironnement();
    addPBSEnvironnement(E);

    _p_attropl = ParametreToAttributeOPList(&_p_attropl, P);
    _p_attropl = EnvironnementToAttributeOPList(&_p_attropl, E);

  }


  // Destructeur
  Job_PBS::~Job_PBS()
  {
    // On detruit la liste chainee d'attributs + operateur
    struct attropl * current_p_attropl = _p_attropl;
    while (current_p_attropl) {
      struct attropl * next = current_p_attropl->next;
      delete[] current_p_attropl->name;
      delete[] current_p_attropl->resource;
      delete[] current_p_attropl->value;
      delete current_p_attropl;
      current_p_attropl = next;
    }

    // On detruit la liste chainee d'attributs
    struct attrl * current_p_attrl = _p_attrl;
    while (current_p_attrl) {
      struct attrl * next = current_p_attrl->next;
      delete[] current_p_attrl->name;
      delete[] current_p_attrl->resource;
      delete[] current_p_attrl->value;
      delete current_p_attrl;
      current_p_attrl = next;
    }

    // On detruit les champs alloues
    delete[] _script;
    delete[] _destination;
  }


  // Accesseur
  struct attropl * Job_PBS::getAttributesOP()
  {
    return _p_attropl;
  }

  // Accesseur
  // Cette methode sert pour les pbs_alter de PBS 
  // Pbs_alter est buggé par rapport a la specification ers_all.ps car les 
  // variables d'environnement ne sont pas supportees (ATTR_v)
  struct attrl * Job_PBS::getAttributes()
  {
    if (_p_attrl == 0) {

      // On remplit la structure attrl a partir de la strucuture attropl
      // (elles ne different que par le parametre op, mais elles ne sont pas interchangeables
      // dans les appels)

      struct attropl * current_p_attropl = _p_attropl;
      while (current_p_attropl) {
	if (strcmp(current_p_attropl->name, ATTR_v))  // Bug fix for ATTR_v
	  setResourceAttribute(&_p_attrl,
			       current_p_attropl->name,
			       current_p_attropl->resource,
			       current_p_attropl->value);
				
	current_p_attropl = current_p_attropl->next;
      }
			
    }

    return _p_attrl;
  }

  // Accesseur
  char * Job_PBS::getScript()
  {
    return _script;
  }

  // Accesseur
  char * Job_PBS::getDestination()
  {
    return _destination;
  }

}
