# ifndef SALOME_STRING
# define SALOME_STRING

# include <string>
# include <sstream>

class SALOME_STRING : public string
{
private :
	ostringstream _s ;
public :
	operator const char*() const
	{
		return _s.str().c_str() ;
	}
	SALOME_STRING() : _s()
	{
	}
	template <class T> SALOME_STRING( const T &valeur ) : _s()
	{
		_s << valeur ;
	}
	template <class T> SALOME_STRING &operator<<( const T &valeur )
	{
		_s << valeur ;
		return *this ;
	}
} ;

// Exemple d'utilisation avec les exceptions SALOME
//      SALOME_STRING msgErr;
//      msgErr << "ESSAI::ESSAI() : This Object cannot be instanciated by now ! Try "<< 11 << "times just to see what happens ";
//      throw SALOME_EXCEPTION (LOCALIZED(msgErr)) ;

# endif
