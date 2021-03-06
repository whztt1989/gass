
if (NOT "$ENV{GASS_HOME}" STREQUAL "")
	set (GASS_DIR $ENV{GASS_HOME} CACHE PATH "GASS home")
endif()

if (WIN32)
	set(_SHARED_LIB_EXT .dll)
else() #assume linux
	set(_SHARED_LIB_EXT .so)
endif()

set (Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.46.1 REQUIRED filesystem system)

find_path(GASS_CORE_INCLUDE_DIRS Core/Prerequisits.h PATHS ${GASS_DIR}/include ${GASS_DIR}/source)

set(GASS_CORE_INCLUDE_DIRS  ${GASS_CORE_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
set(GASS_LIBRARY_DIRS ${GASS_DIR}/lib ${GASS_DIR}/lib/debug ${GASS_DIR}/lib/release)
set(GASS_BINARY_DIRS ${GASS_DIR}/bin ${GASS_DIR}/bin/debug ${GASS_DIR}/bin/release)

find_library(GASS_CORE_LIBRARY_RELEASE GASSCore HINTS ${GASS_LIBRARY_DIRS})
find_library(GASS_CORE_LIBRARY_DEBUG GASSCore_d HINTS ${GASS_LIBRARY_DIRS})

set(GASS_CORE_LIBRARIES optimized ${GASS_CORE_LIBRARY_RELEASE}
	 debug ${GASS_CORE_LIBRARY_DEBUG})

find_file(GASS_CORE_BINARY_RELEASE NAMES GASSCore${_SHARED_LIB_EXT} HINTS ${GASS_BINARY_DIRS})
find_file(GASS_CORE_BINARY_DEBUG NAMES GASSCore_d${_SHARED_LIB_EXT} HINTS ${GASS_BINARY_DIRS})
 
if (WIN32) #find third party shared binaries	 
	   #TBB
	   find_package(TBB)
	   
	   #Set GASSSim out binaries
	   set(GASS_CORE_BINARIES_REL ${GASS_CORE_BINARY_RELEASE}
		${TBB_BINARY_REL})
	
	   set(GASS_CORE_BINARIES_DBG ${GASS_CORE_BINARY_DEBUG}
		${TBB_BINARY_DBG})
endif()
