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

/*
 * - Nom de la fonction : _MEDnomGeometrie
 * - Description : fournit le nom de l'element geometrique associe
 *                 au type geometrique MED
 * - Parametres :
 *     - nom_geo (OUT) : le nom de l'element
 *     - type_geo (IN) : le type de l'element
 * - Resultat : 0 en cas de succes, -1 sinon
 */ 

namespace med_2_1{

med_err 
_MEDnomGeometrie(char *nom_geo,med_geometrie_element type_geo)
{
   switch (type_geo)
     {
     case MED_POINT1 :
       strcpy(nom_geo,MED_NOM_PO1);
       break;
       
     case MED_SEG2 :
       strcpy(nom_geo,MED_NOM_SE2);
       break;
	   
     case MED_SEG3 :
       strcpy(nom_geo,MED_NOM_SE3);
       break;
	   
     case MED_TRIA3 :
       strcpy(nom_geo,MED_NOM_TR3);
       break;
	   
     case MED_TRIA6 :
       strcpy(nom_geo,MED_NOM_TR6);
       break;
	   
     case MED_QUAD4 :
       strcpy(nom_geo,MED_NOM_QU4);
       break;
       
     case MED_QUAD8 :
       strcpy(nom_geo,MED_NOM_QU8);
       break;
       
     case MED_TETRA4 :
       strcpy(nom_geo,MED_NOM_TE4);
       break;
       
     case MED_TETRA10 :
       strcpy(nom_geo,MED_NOM_T10);
       break;
       
     case MED_HEXA8 :
       strcpy(nom_geo,MED_NOM_HE8);
       break;
       
     case MED_HEXA20 :
       strcpy(nom_geo,MED_NOM_H20);
       break;
       
     case MED_PENTA6 :
       strcpy(nom_geo,MED_NOM_PE6);
       break;
       
     case MED_PENTA15 :
       strcpy(nom_geo,MED_NOM_P15);
       break;
       
     case MED_PYRA5 :
       strcpy(nom_geo,MED_NOM_PY5);
       break;
       
     case MED_PYRA13 :
       strcpy(nom_geo,MED_NOM_P13);
       break;

     default :
       return -1;
	 }
   
   return 0;
} 

}
