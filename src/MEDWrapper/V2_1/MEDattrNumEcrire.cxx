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
 * - Nom de la fonction : _MEDattrNumEcrire
 * - Description : ecriture d'un attribut entier
 * - Parametres :
 *     - pere (IN) : l'ID de l'objet HDF pere ou placer l'attribut
 *     - type (IN) : le type du champ {MED_REEL64,MED_INT}
 *     - nom  (IN) : le nom de l'attribut
 *     - val  (IN) : la valeur de l'attribut
 * - Resultat : 0 en cas de succes, -1 sinon
 */

namespace med_2_1{

med_err 
_MEDattrNumEcrire(med_idt pere,med_type_champ type,char *nom,unsigned char *val, 
		  med_mode_acces mode)
{
  med_idt aid,attr;
  med_err ret;
  int type_hdf;

  switch(type)
    {
    case MED_REEL64 :
      /* 1) IA32 is LE but due to an (?HDF convertion BUG?) when using H5T_NATIVE_DOUBLE/MED_REEL64? under PCLINUX
	 the file read under SGI is incorrect
	 2) Compaq OSF/1 is LE, since we force SGI64,SUN4SOL2,HP to write double in LE even if they are BE, mips OSF/1 must be BE
	 REM  : Be careful of compatibility between MED files when changing this (med2.2)                    */
#if defined(PCLINUX) || defined(OSF1)
      type_hdf = H5T_IEEE_F64BE;
#else 
      type_hdf = H5T_IEEE_F64LE;
#endif
      break;
      
    case MED_INT :
#if defined(IRIX64) || defined(OSF1)
      type_hdf = H5T_NATIVE_LONG; 
#elif defined(PCLINUX)
      /* This explicit convertion avoid a core dump between in HDF&ASTER when reading on SGI
	 a file written under a PCLINUX system (in founction H5Tconvert),
	 we don't know yet if it is an HDF bug or an ASTER one */
      /* The problem seems to be in convertion process between INT32LE->INT32BE ? */
      type_hdf = H5T_STD_I32BE;
      if ((H5Tconvert(H5T_NATIVE_INT,H5T_STD_I32BE,1,(void *)val,NULL,NULL)) < 0) 
	  return -1;
#else
      type_hdf = H5T_NATIVE_INT;
#endif
      break;

    default :
      return -1;
    }

  if ((aid = H5Screate(H5S_SCALAR)) < 0)
    return -1;

  if ( ((attr = H5Aopen_name(pere,nom)) > 0) && (mode != MED_REMP) )
    return -1;
  else
    if ( attr < 0)
      if ((attr = H5Acreate(pere,nom,type_hdf,aid,H5P_DEFAULT)) < 0) return -1;  

  if ((ret = H5Awrite(attr,type_hdf,val)) < 0)
    return -1;


  if ((ret = H5Sclose(aid)) < 0)
    return -1;
  if ((ret = H5Aclose(attr)) < 0)
    return -1;

#if defined(PCLINUX)
  /* This explicit convertion cancel the previous on which avoid a mysterious bug between HDF&ASTER when reading
     a file written under a PCLINUX system, we don't know yet if it is an HDF bug or an ASTER one */  
  if (type == MED_INT) 
    if ((H5Tconvert(H5T_STD_I32BE,H5T_NATIVE_INT,1,(void *)val,NULL,NULL)) < 0) 
      return -1;
#endif

  return 0;
}

}
