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
 * - Nom de la fonction : _MEDattrNumLire
 * - Description : lecture d'un attribut entier
 * - Parametres :
 *     - pere (IN)  : l'ID de l'objet HDF pere ou placer l'attribut
 *     - type (IN)  : le type du champ {MED_REEL64,MED_INT}
 *     - nom  (IN)  : le nom de l'attribut 
 *     - val  (OUT) : la valeur de l'attribut
 * - Resultat : 0 en cas de succes, -1 sinon
 */ 

namespace med_2_1{

med_err 
_MEDattrNumLire(med_idt pere,med_type_champ type,char *nom,unsigned char *val)
{
  med_idt attid;
  med_err ret;
  int type_hdf;

  if ((attid = H5Aopen_name(pere,nom)) < 0)
    return -1;

  switch(type) 
    {
    case MED_REEL64 :
#if defined(PCLINUX) || defined(OSF1)
      type_hdf = H5T_IEEE_F64BE;
#else 
      type_hdf = H5T_IEEE_F64LE;
#endif
      break;
      
    case MED_INT :
#if defined(IRIX64) || defined(OSF1)
      type_hdf = H5T_NATIVE_LONG; 
#else
      type_hdf = H5T_NATIVE_INT;
#endif
      break;
      
    default :
      return -1;
    }

  if ((ret = H5Aread(attid,type_hdf,val)) < 0)
    return -1;

  if ((ret = H5Aclose(attid)) < 0)
    return -1;

  return 0;
}

}
