
# Boost detection dor Salome

set(Boost_ROOT_DIR $ENV{Boost_ROOT_DIR} CACHE PATH "Path to Boost directory")
if(EXISTS ${Boost_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${Boost_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${Boost_ROOT_DIR}/lib)
endif(EXISTS ${Boost_ROOT_DIR})
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
#find_package( Boost 1.49 REQUIRED  )#COMPONENTS thread date_time signals filesystem system)
find_package( Boost REQUIRED  )#COMPONENTS thread date_time signals filesystem system)
if(WINDOWS)
  set(BOOST_DEFINITIONS -DBOOST_DISABLE_ASSERTS)
endif(WINDOWS)
