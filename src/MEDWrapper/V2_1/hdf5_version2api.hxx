
#ifndef _hdf5_version2api_hxx_
#define _hdf5_version2api_hxx_

#include <H5public.h>

#if H5_VERS_MAJOR < 1
#elif H5_VERS_MAJOR == 1

#if H5_VERS_MINOR < 6
#elif H5_VERS_MINOR == 6

#if H5_VERS_RELEASE < 4
#else
#define HDF_NEW_API
#endif

#else /* H5_VERS_MINOR >= 7 */
#define HDF_NEW_API
#endif /* H5_VERS_MINOR */

#else /* H5_VERS_MAJOR >= 2 */
#define HDF_NEW_API
#endif /* H5_VERS_MAJOR */

#endif
