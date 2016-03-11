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

//  File   : data_short_port_provides.cxx
//  Author : André RIBES (EDF)
//  Module : KERNEL
//
#include "data_short_port_provides.hxx"

data_short_port_provides::data_short_port_provides() {
  _val = 0;
  short_termine = false;                    
  short_mutex = new pthread_mutex_t();
  pthread_mutex_init(short_mutex, NULL);
  short_condition = new pthread_cond_t();
  pthread_cond_init(short_condition, NULL);
  short_termine_cp = true;                  
  short_mutex_cp = new pthread_mutex_t();
  pthread_mutex_init(short_mutex_cp, NULL);
  short_condition_cp = new pthread_cond_t();
  pthread_cond_init(short_condition_cp, NULL);
}

data_short_port_provides::~data_short_port_provides() {
  pthread_mutex_destroy(short_mutex);
  delete short_mutex;
  pthread_cond_destroy(short_condition);
  delete short_condition;
  pthread_mutex_destroy(short_mutex_cp);
  delete short_mutex_cp;
  pthread_cond_destroy(short_condition_cp);
  delete short_condition_cp;
}

void
data_short_port_provides::put(CORBA::Short data) {
  // On attend que le get soit fait
  pthread_mutex_lock(short_mutex_cp);
  while (short_termine_cp == false)
  {
     pthread_cond_wait(short_condition_cp, short_mutex_cp);
  }
  short_termine_cp = false;
  pthread_mutex_unlock(short_mutex_cp);

  pthread_mutex_lock(short_mutex);
  _val = data;
  short_termine = true;
  pthread_cond_signal(short_condition);
  pthread_mutex_unlock(short_mutex);
}

CORBA::Short
data_short_port_provides::get() {
  CORBA::Short result;
  pthread_mutex_lock(short_mutex);
  while (short_termine == false)
  {
     pthread_cond_wait(short_condition, short_mutex);
  }
  result = _val;
  short_termine = false;
  pthread_mutex_unlock(short_mutex);

  // On indique que l'on a copie la valeur
  pthread_mutex_lock(short_mutex_cp);
  short_termine_cp = true;
  pthread_cond_signal(short_condition_cp);
  pthread_mutex_unlock(short_mutex_cp);
  return result;
}

Ports::Port_ptr
data_short_port_provides::get_port_ref() {
  return this->_this();
}
