using namespace std;
#include "ServiceUnreachable.hxx"

ServiceUnreachable::ServiceUnreachable( void ): SALOME_Exception( "ServiceUnreachable" )
{
	;
}
ServiceUnreachable::ServiceUnreachable( const ServiceUnreachable &ex  ): SALOME_Exception( ex ) 
{
	;
}
