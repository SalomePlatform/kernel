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

# ifndef __UTILITES_H__
# define __UTILITES_H__


/* pour indiquer le statut des arguments des fonctions. */

#ifdef _IN
#error _IN already defined
#endif
#define _IN

#ifdef _OUT
#error _OUT already defined
#endif
#define _OUT

#ifdef _INOUT
#error _INOUT already defined
#endif
#define _INOUT

#ifdef _UNUSED
#error _UNUSED already defined
#endif
#define _UNUSED







/* --- Pour afficher le nom du fichier source courant et le numero de la ligne courante --- */
/* --- sur la stderr.                                                                   --- */

# define ICI			{\
					fflush(stdout);\
					fprintf(stderr, "%s [%d] : " , __FILE__ , __LINE__ ) ;\
					fflush(stderr) ;\
				}







/* --- Pour afficher la date et l'heure de la compilation du fichier source courant,    --- */
/* --- sur la stdout.                                                                   --- */

# ifdef INFOS_COMPILATION
# error INFOS_COMPILATION already defined
# endif
# define INFOS_COMPILATION	{\
					fflush(stderr);\
					fprintf(stdout, "%s [%d] : " , __FILE__ , __LINE__ ) ;\
					fprintf(stdout,"Compilation le %s" , __DATE__);\
					fprintf(stdout," a %s" , __TIME__ );\
					fprintf(stdout,"\n\n\n" );\
					fflush(stdout) ;\
				}







/* --- Pour attendre "secondes" secondes et afficher un message sur la stderr indiquant --- */
/* --- cette attente volontaire.                                                        --- */

# ifdef ATTENTE
# error ATTENTE already defined
# endif
# define ATTENTE(secondes)	{\
					ICI ;\
					fprintf( stderr, "ATTENTE de %d secondes" , secondes);\
					fflush(stderr) ;\
					sleep(secondes) ;\
					fprintf( stderr, "\n" );\
					fflush(stderr) ;\
				}







/* ----------    Les macros suivantes ne doivent pas deja exister !              ---------- */

# ifdef EXECUTION
# error EXECUTION already defined
# endif
# ifdef INTERRUPTION
# error INTERRUPTION already defined
# endif
# ifdef ISCRUTE
# error ISCRUTE already defined
# endif
# ifdef RSCRUTE
# error RSCRUTE already defined
# endif
# ifdef SSCRUTE
# error SSCRUTE already defined
# endif
# ifdef CSCRUTE
# error CSCRUTE already defined
# endif
# ifdef XSCRUTE
# error XSCRUTE already defined
# endif
# ifdef MESSAGE
# error MESSAGE already defined
# endif





# ifdef _DEBOG_


/* --- Pour tracer sur la stderr l'execution d"une instruction.                         --- */

# define EXECUTION(instruction)	{\
					ICI ;\
					fprintf( stderr,"INSTRUCTION %s" , #instruction ) ;\
					fflush(stderr);\
					instruction ;\
					fflush(stdout);\
					fprintf( stderr," FRANCHIE\n" ) ;\
					fflush(stderr);\
				}







/* --- Pour afficher un message d'interruption volontaire et retourner le code retour   --- */
/* --- "code"                                                                           --- */

# define INTERRUPTION(code)	{\
					ICI ;\
					fprintf( stderr," INTERRUPTION code = %d",code) ;\
					fprintf(stderr,"\n") ;\
					exit(code) ;\
				}







/* --- Pour conditionner la poursuite du traitement par la validite de la condition     --- */
/* --- "condiiton".                                                                     --- */

# ifndef ASSERT
# define ASSERT(condition)	if( !(condition) ){\
					ICI ;\
					fprintf(stderr,"condition %s VIOLEE\n",#condition);\
					INTERRUPTION(17);\
				}
# endif		/* # ifndef ASSERT */







/* --- Pour afficher sur la stderr la valeur d'une variable precedee de son nom.        --- */

# define ISCRUTE(entier)	{\
					ICI ;\
					fprintf(stderr,"%s = %d\n",#entier,entier) ;\
					fflush(stderr) ;\
				}
# define RSCRUTE(reel)		{\
					ICI ;\
					fprintf(stderr,"%s = %f\n",#reel,reel) ;\
					fflush(stderr) ;\
				}
# define XSCRUTE(pointeur)		{\
					ICI ;\
					fprintf(stderr,"%s = %x\n",#pointeur,pointeur) ;\
					fflush(stderr) ;\
				}
# define CSCRUTE(car)		{\
					ICI ;\
					fprintf(stderr,"%s = %c\n",#car,car) ;\
					fflush(stderr) ;\
				}
# define SSCRUTE(chaine)	{\
					ICI ;\
					fprintf(stderr,"%s = \"%s\"\n",#chaine,chaine) ;\
					fflush(stderr) ;\
				}
# define MESSAGE(chaine)	{\
					ICI ;\
					fprintf(stderr,"%s\n",chaine) ;\
					fflush(stderr) ;\
				}
# define FIN(nom)		{\
					ICI ;\
					fprintf( stderr , "} FIN %s\n\n\n" , nom ) ;\
					fflush(stderr) ;\
				}
# define DEBUT(nom)		{\
					fprintf( stderr , "\n\n\n") ;\
					ICI ;\
					fprintf( stderr , "{ DEBUT %s\n" , nom ) ;\
					fflush(stderr) ;\
				}


# else		/* # ifdef _DEBOG_ */



# define EXECUTION(instruction) instruction
# define INTERRUPTION(code)

# ifndef ASSERT
# define ASSERT(condition)
# endif

# define ISCRUTE(entier)
# define RSCRUTE(reel)
# define CSCRUTE(car)
# define SSCRUTE(chaine)
# define MESSAGE(chaine)
# define DEBUT(nom)
# define FIN(nom)
 

# endif		/* # ifdef _DEBOG_ */


# endif		/* # ifndef __UTILITES_H__ */
