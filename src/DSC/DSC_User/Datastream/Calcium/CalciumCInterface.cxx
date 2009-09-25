#include "CalciumCInterface.hxx"
#include "CalciumCxxInterface.hxx"

#include <stdio.h>


//#define MYDEBUG
#ifdef MYDEBUG
#define DEBTRACE(msg) {std::cerr<<std::flush<<__FILE__<<" ["<<__LINE__<<"] : "<<msg<<std::endl<<std::flush;}
#else
#define DEBTRACE(msg)
#endif


/* Définition de l'Interface entre l'API C  et l'API C++
   L'utilisateur CALCIUM n'a normalement pas a utliser cette interface
   En C/C++ il utilisera celle définie dans Calcium.c (calcium.h)
   En C++/CORBA directement celle de CalciumCxxInterface.hxx
*/


#define STAR *

/* Définition de ecp_lecture_... , ecp_ecriture_..., ecp_free_... */

/*  Le premier argument est utilisée :
    - comme suffixe dans la définition des noms ecp_lecture_ , ecp_ecriture_ et ecp_free_
    - comme second argument template à l'appel de la méthode C++ correspondante
        ( le type de port correspondant est alors obtenu par un trait)
   Le second argument est utilisée :
   - pour typer le paramètre data de la procédure générée 
   - pour déduire le type des paramètres t, ti tf via un trait
   - comme premier paramètre template à l'appel de la méthode C++ correspondante
         (pour typer les données passées en paramètre )
   Notons que dans le cas CALCIUM_C2CPP_INTERFACE_(int,int,), le type int n'existe pas
   en CORBA, le port CALCIUM correspondant utilise une séquence de long. La méthode
   C++ CALCIUM de lecture repère cette différence de type et charge 
   le manipulateur de données d'effectuer  une recopie (qui fonctionne si les types sont compatibles). 
*/
// CALCIUM_C2CPP_INTERFACE_CXX_(_name,_porttype,_type,_qual)
CALCIUM_C2CPP_INTERFACE_CXX_(intc,int,int,);
CALCIUM_C2CPP_INTERFACE_CXX_(long,long,long,);

CALCIUM_C2CPP_INTERFACE_CXX_(integer,integer,cal_int,);
CALCIUM_C2CPP_INTERFACE_CXX_(int2integer ,integer,  int,);
CALCIUM_C2CPP_INTERFACE_CXX_(long2integer, integer, long,);

CALCIUM_C2CPP_INTERFACE_CXX_(float,float,float, );
CALCIUM_C2CPP_INTERFACE_CXX_(double,double,double,);
/*  Fonnctionne mais essai suivant pour simplification de Calcium.c CALCIUM_C2CPP_INTERFACE_(bool,bool,);*/
CALCIUM_C2CPP_INTERFACE_CXX_(bool,bool,int,);
CALCIUM_C2CPP_INTERFACE_CXX_(cplx,cplx,float,);
CALCIUM_C2CPP_INTERFACE_CXX_(str,str,char*,);

/* Définition de ecp_fin */
extern "C" CalciumTypes::InfoType 
ecp_fin_ (void * component, int code) {

  Superv_Component_i * _component = static_cast<Superv_Component_i *>(component); 

  bool provideLastGivenValue = false;
  if (code == CalciumTypes::CP_CONT ) provideLastGivenValue = true;

  try {									
    CalciumInterface::ecp_fin( *_component,				
			       provideLastGivenValue); 
  } catch ( const CalciumException & ex) { //tester l'arrêt par exception
    DEBTRACE( ex.what() );
    return ex.getInfo();						
  }									
  return CalciumTypes::CPOK;
};

extern "C" CalciumTypes::InfoType 
ecp_cd_ (void * component, char * instanceName) {
  Superv_Component_i * _component = static_cast<Superv_Component_i *>(component); 
  std::string name;
  CalciumInterface::ecp_cd( *_component,name);
  strcpy(instanceName,name.c_str());
  return CalciumTypes::CPOK;
}

// INTERFACE C/CPP pour les chaines de caractères
// Le paramètre supplémentaire strsize n'étant pas utilisé
// j'utilise la génération par la macro CALCIUM_C2CPP_INTERFACE_(str,char*,);
// extern "C" CalciumTypes::InfoType ecp_lecture_str (void * component, int dependencyType, 
// 						   float * ti, float * tf, long * i, 
// 						   const char * const nomvar, size_t bufferLength, 
// 						   size_t * nRead, char ** *data, size_t strsize ) { 

//   Superv_Component_i * _component = static_cast<Superv_Component_i *>(component); 
//   double         _ti=*ti;						
//   double         _tf=*tf;						
//   size_t         _nRead=0;						
//   size_t         _bufferLength=bufferLength;				
//   CalciumTypes::DependencyType dependencyType=			
//     static_cast<CalciumTypes::DependencyType>(dependencyType);	
  
//   // - GERER POINTEUR NULL : NOTHING TODO 
//   // - VERIFIER LA TAILLE DES CHAINES RETOURNEES (ELLES DEVRAIENT ETRES CORRECTES SI L'ECRITURE EST BIEN CODEE.)

//   DEBTRACE( "-------- CalciumInterface(lecture Inter Part) MARK 1 ------------------" ) 
//     try {								
//       CalciumInterface::ecp_lecture< char*, char* >( *_component,	
// 						     dependencyType, 
// 						     _ti, _tf, *i,	
// 						     nomvar,		
// 						     _bufferLength, _nRead, *data); 
//     } catch ( const CalciumException & ex) {				
//       DEBTRACE( ex.what() );
//       return ex.getInfo();						
//     }									
    
//     *nRead = _nRead;						
    
//     if (dependencyType == CalciumTypes::CP_SEQUENTIEL ) 
//       *ti=(float)(_ti);			
    
//     DEBTRACE( "-------- CalciumInterface(lecture Inter Part), Data Ptr :" << *data ) ;

//     return CalciumTypes::CPOK;
//   };									
  									

// extern "C" void ecp_lecture_str_free (char** data) {	
//   CalciumInterface::ecp_free< char*, char* >(data);			
// };		                                                        
									
									
// extern "C" CalciumTypes::InfoType ecp_ecriture_str (void * component, int dependencyType, 
// 						    float *t, long  i,	
// 						    const char * const nomvar, size_t bufferLength, 
// 						    char ** data, int strsize ) { 

//     Superv_Component_i * _component = static_cast<Superv_Component_i *>(component); 
//     /* Je ne sais pas pourquoi, je n'arrive pas à passer t par valeur : corruption de la pile*/ 
//     double         _t=*t;						
//     size_t         _bufferLength=bufferLength;				

//     // - VERIFIER LA TAILLE DES CHAINES RETOURNEES (ELLES DEVRAIENT ETRES CORRECTES SI L'ECRITURE EST BIEN CODEE.)

//     DEBTRACE( "-------- CalciumInterface(ecriture Inter Part) MARK 1 ------------------" ) 
//     try {								
//       std::string essai(nomvar);					
//       DEBTRACE( "----------->-" << nomvar )		
// 	CalciumInterface::ecp_ecriture< char*, char* >( *_component,	
// 							static_cast<CalciumTypes::DependencyType>(dependencyType), 
// 							_t,i,nomvar,_bufferLength,*data); 
//     } catch ( const CalciumException & ex) {				
//       std::cerr << ex.what() << std::endl;				
//       return ex.getInfo();						
//     }									
//     DEBTRACE( "-------- CalciumInterface(ecriture Inter Part), Valeur de data :" << data ) 
//     return CalciumTypes::CPOK;						
//   };									
