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
 * - Nom de la fonction : _MEDparametresGeometrie
 * - Description : fournit les parametres geometriques des differents
 *                 entites et elements MED
 * - Parametres :
 *     - typ_ent (IN)  : type d'entite de l'element
 *     - type_geo (IN) : le type geometrique de l'element
 *     - dim (OUT)     : dimension de l'element
 *     - nnoe (OUT)    : nombre de noeuds composant l'element (connectivite 
 *                       nodale)
 *     - ndes (OUT)    : nombre de composants dans l'elements (connectivite
 *                       descendante)
 * - Resultat : 0 en cas de succes, -1 sinon
 */

namespace med_2_1{

med_err 
_MEDparametresGeometrie(med_entite_maillage type_ent,
			med_geometrie_element type_geo, int *dim, 
			int *nnoe,int *ndes)
{
  *nnoe = type_geo % 100;
  *dim = type_geo / 100;

  switch(type_ent)
    {
    case MED_MAILLE :
      switch (type_geo)
	{
	case MED_POINT1 :
	  *ndes = 0;
	  break;
	  
	case MED_SEG2 :
	  *ndes = 2;
	  break;
	  
	case MED_SEG3 :
	  *ndes = 3;
	  break;
	  
	case MED_TRIA3 :
	  *ndes = 3;
	  break;
	  
	case MED_TRIA6 :
	  *ndes = 3;
	  break;
	  
	case MED_QUAD4 :
	  *ndes = 4;
	  break;
	  
	case MED_QUAD8 :
	  *ndes = 4;
	  break;
	  
	case MED_TETRA4 :
	  *ndes = 4;
	  break;
	  
	case MED_TETRA10 :
	  *ndes = 4;
	  break;
	  
	case MED_HEXA8 :
	  *ndes = 6;
	  break;
	  
	case MED_HEXA20 :
	  *ndes = 6;
	  break;
	  
	case MED_PENTA6 :
	  *ndes = 5;
	  break;
	  
	case MED_PENTA15 :
	  *ndes = 5;
	  break;
	  
	case MED_PYRA5 :
	  *ndes = 5;
	  break;
	  
	case MED_PYRA13 :
	  *ndes = 5;
	  break;
	  
	default :
	  return -1;
	}
      break;
      
    case MED_FACE :
      switch(type_geo)
	{
	case MED_TRIA3 :
	  *ndes = 3;
	  break;
	  
	case MED_TRIA6 :
	  *ndes = 3;
	  break;
	  
	case MED_QUAD4 :
	  *ndes = 4;
	  break;
	  
	case MED_QUAD8 :
	  *ndes = 4;
	  break;
	  
	default :
	  return -1;
	}
      break;
      
    case MED_ARETE :
      switch(type_geo)
	{
	case MED_SEG2 :
	  *ndes = 2;
	  break;
	  
	case MED_SEG3 :
	  *ndes = 3;
	  break;
	  
	default :
	  return -1;
	}
      break;
      
    default :
      return -1;
    }
  
  return 0;
}

}
