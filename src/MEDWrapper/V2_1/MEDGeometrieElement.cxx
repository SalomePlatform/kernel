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

namespace med_2_1{

med_err 
_MEDGeometrieElement(med_geometrie_element typ_geo[],med_entite_maillage typ_ent)
{
  int i;
  med_geometrie_element typ_mai[MED_NBR_GEOMETRIE_MAILLE] = {MED_POINT1,MED_SEG2, 
							 MED_SEG3,MED_TRIA3,
							 MED_TRIA6,MED_QUAD4,
							 MED_QUAD8,MED_TETRA4,
							 MED_TETRA10,MED_HEXA8,
							 MED_HEXA20,MED_PENTA6,
							 MED_PENTA15,MED_PYRA5,
							 MED_PYRA13};
  med_geometrie_element typ_fac[MED_NBR_GEOMETRIE_FACE] = {MED_TRIA3,MED_TRIA6,
						       MED_QUAD4,MED_QUAD8};
  med_geometrie_element typ_are[MED_NBR_GEOMETRIE_ARETE] = {MED_SEG2,MED_SEG3};  

  switch(typ_ent)
    {
    case MED_MAILLE :
      for (i=0;i<MED_NBR_GEOMETRIE_MAILLE;i++)
	typ_geo[i] = typ_mai[i];
      break;

    case MED_FACE :
      for (i=0;i<MED_NBR_GEOMETRIE_FACE;i++)
	typ_geo[i] = typ_fac[i];
      break;
      
    case MED_ARETE :
      for (i=0;i<MED_NBR_GEOMETRIE_ARETE;i++)
	typ_geo[i] = typ_are[i];
      break;

    default :
      return -1;
    }
  return 0;
}

}
