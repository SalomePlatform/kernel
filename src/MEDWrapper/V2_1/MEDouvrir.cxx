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

#ifdef PPRO_NT
#define F_OK 0
#else
#include <unistd.h>
#endif

namespace med_2_1{

med_idt
MEDouvrir(char *nom, med_mode_acces mode_acces)
{
  med_idt fid; 

  /*
   * On inhibe le gestionnaire d'erreur HDF
   */
  _MEDmodeErreurVerrouiller();

  /*
   * On ouvre le fichier MED sous HDF
   */
  switch(mode_acces)
    {
    case MED_LECT :
      if (access(nom,F_OK))
	return -1;
      else 
	if ((fid = _MEDfichierOuvrir(nom,mode_acces)) < 0)
	  return -1;
      break;

    case MED_ECRI :
      if (access(nom,F_OK))
	{
	  if ((fid = _MEDfichierCreer(nom)) < 0)
	    return -1;
	}
      else
	if ((fid = _MEDfichierOuvrir(nom,mode_acces)) < 0)
	  return -1;
      break;

    case MED_REMP :
      if ((fid = _MEDfichierCreer(nom)) < 0)
	return -1;
      break;

    default :
      return -1;
    }

  return fid;
}

}
