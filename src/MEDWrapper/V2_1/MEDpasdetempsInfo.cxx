/*************************************************************************
* COPYRIGHT (C) 1999 - 2002  EDF R&D
* THIS LIBRARY IS FREE SOFTWARE; YOU CAN REDISTRIBUTE IT AND/OR MODIFY
* IT UNDER THE TERMS OF THE GNU LESSER GENERAL PUBLIC LICENSE 
* AS PUBLISHED BY THE FREE SOFTWARE FOUNDATION; 
* EITHER VERSION 2.1 OF THE LICENSE, OR (AT YOUR OPTION) ANY LATER VERSION.
*  
* THIS LIBRARY IS DISTRIBUTED IN THE HOPE THAT IT WILL BE USEFUL, BUT
* WITHOUT ANY WARRANTY; WITHOUT EVEN THE IMPLIED WARRANTY OF
* MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. SEE THE GNU
* LESSER GENERAL PUBLIC LICENSE FOR MORE DETAILS.
*
* YOU SHOULD HAVE RECEIVED A COPY OF THE GNU LESSER GENERAL PUBLIC LICENSE
* ALONG WITH THIS LIBRARY; IF NOT, WRITE TO THE FREE SOFTWARE FOUNDATION,
* INC., 59 TEMPLE PLACE, SUITE 330, BOSTON, MA 02111-1307 USA
*
*************************************************************************/

#include "med.hxx"
#include "med_outils.hxx"

/* Le nom de cette fonction n'est pas très bien choisie */

namespace med_2_1{

med_err 
MEDpasdetempsInfo(med_idt fid,char *champ
		  ,med_entite_maillage type_ent, med_geometrie_element type_geo,
		  int indice, char *maa, med_int * ngauss, med_int * numdt, char * dt_unit, med_float * dt, 
		  med_int * numo)
{

  med_err ret=0;
  med_int gauss_size;
  med_idt gid;
  char chemin[(MED_TAILLE_CHA+MED_TAILLE_NOM+1)+(2*MED_TAILLE_NOM_ENTITE+2)+2*MED_MAX_PARA+1];
  int num;
  char nomdatagroup1[2*MED_TAILLE_NOM_ENTITE+2],nomdatagroup2[2*MED_MAX_PARA+1];
  char tmp1         [MED_TAILLE_NOM_ENTITE+1];

  /*
   * On inhibe le gestionnaire d'erreur HDF 5
   */
  _MEDmodeErreurVerrouiller();

  /*
   * On recupere le nom du datagroup <numdtt>.<numoo>
   */
  strcpy(chemin,MED_CHA);
  strcat(chemin,champ);
  strcat(chemin,"/");

  if ((ret = _MEDnomEntite(nomdatagroup1,type_ent)) < 0)
    return -1;
  if ((type_ent != MED_NOEUD))
    {
      if ((ret = _MEDnomGeometrie(tmp1,type_geo)) < 0)
	return -1;
      strcat(nomdatagroup1,".");
      strcat(nomdatagroup1,tmp1);
    }
  strcat(chemin,nomdatagroup1);
  strcat(chemin,"/");

  num = indice - 1;
  if ((ret = _MEDobjetIdentifier(fid,chemin,num,nomdatagroup2)) < 0)
    return -1;
  strcat(chemin,nomdatagroup2);
  if ((gid = _MEDdatagroupOuvrir(fid,chemin)) < 0)
    return -1;

  /*
   * La liste des attributs
   */
 if ((ret = _MEDattrStringLire(gid,MED_NOM_MAI,MED_TAILLE_NOM,maa)) < 0)
    return -1;
  
 if ((ret = _MEDattrEntierLire(gid,MED_NOM_NDT,(med_int*) numdt)) < 0)
   return -1;

 if ((ret = _MEDattrFloatLire(gid,MED_NOM_PDT,(med_float*) dt)) < 0)
   return -1;

 if ((ret = _MEDattrStringLire(gid,MED_NOM_UNI,MED_TAILLE_PNOM,dt_unit)) < 0)
   return -1;
 
 if ((ret = _MEDattrEntierLire(gid,MED_NOM_NOR,(med_int*) numo)) < 0)
   return -1;

 if ( (ret = _MEDattrEntierLire(gid,MED_NOM_NGA,ngauss)) < 0 )
   return -1;


 if ((ret = _MEDdatagroupFermer(gid)) < 0)
   return -1;
 
  return 0; 
}

}
