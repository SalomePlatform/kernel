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

#ifndef MED_HDFI_H
#define MED_HDFI_H
#include "med.hxx"

/* FONCTIONS INTERFACE MED/HDF */
namespace med_2_1{

/* Gestion des fichiers HDF */
extern
med_idt _MEDfichierCreer(char *nom);

extern
med_idt _MEDfichierOuvrir(char *nom,med_mode_acces mode);

extern
med_err _MEDfichierFermer(med_idt fid);


/* Gestion des datagroups HDF */
extern 
med_idt _MEDdatagroupCreer(med_idt pid, char *nom);

extern 
med_idt _MEDdatagroupOuvrir(med_idt pid, char *nom);

extern
med_err _MEDdatagroupFermer(med_idt id);


/* Gestion des datasets HDF */

extern 
med_idt _MEDdatasetOuvrir(med_idt pid,char *nom);

extern
med_err _MEDdatasetFermer(med_idt id);

extern
med_err _MEDdatasetNumEcrire (med_idt pere,char *nom, med_type_champ type,
			     med_mode_switch interlace, med_size nbdim, med_size fixdim, 
			     med_size psize, med_ssize * profil, med_int ngauss,
			      med_size *size,  unsigned char *val, med_mode_acces mode);


extern
med_err _MEDdatasetNumLire(med_idt pere,char *nom,med_type_champ type,
			   med_mode_switch interlace, med_size nbdim, med_size fixdim, 
			   med_size psize, med_ssize * pfltab, med_int ngauss,
			   unsigned char *val);

extern
med_err _MEDdatasetStringEcrire(med_idt pere,char *nom,med_size *dimd,
				char *val, med_mode_acces mode);

extern
med_err _MEDdatasetStringLire(med_idt pere,char *nom,char *val);

/* Gestion des attributs HDF */
extern 
med_idt _MEDattrOuvrir(med_idt pid,char * nom);

extern
med_err _MEDattrFermer(med_idt id);

extern
med_err _MEDattrNumEcrire(med_idt pere,med_type_champ type,char *nom,unsigned char *val,med_mode_acces mode);

#define _MEDattrEntierEcrire(w,x,y,z)  _MEDattrNumEcrire(w,MED_INT   ,x,(unsigned char *) y,z)
#define _MEDattrFloatEcrire(w,x,y,z)   _MEDattrNumEcrire(w,MED_REEL64,x,(unsigned char *) y,z)

extern
med_err _MEDattrNumLire(med_idt pere,med_type_champ type,char *nom,unsigned char *val);

#define _MEDattrEntierLire(x,y,z) _MEDattrNumLire(x,MED_INT   ,y,(unsigned char*)z)
#define _MEDattrFloatLire(x,y,z)  _MEDattrNumLire(x,MED_REEL64,y,(unsigned char*)z)

extern
med_err _MEDattrStringEcrire(med_idt pere,char *nom,int longueur,char *val,med_mode_acces mode);

extern
med_err _MEDattrStringLire(med_idt pere,char *nom,int longueur,char *val);


/* Divers */
extern
med_err _MEDindiceInfo(med_idt id, const char *nom, void *donnees);

extern
med_err _MEDindiceNum(med_idt id,const char *nom, void *donnees);

extern  
med_err _MEDobjetIdentifier(med_idt fid,char *chemin,int indice,void *nom);

extern 
med_err _MEDnObjets(med_idt fid,char *chemin,int *n);

extern 
void _MEDmodeErreurVerrouiller(); 

}

#endif /* MED_HDFI_H */
