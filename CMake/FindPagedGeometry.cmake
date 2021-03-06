if (NOT "$ENV{PAGEDGEOMETRY_HOME}" STREQUAL "")
	set (PAGEDGEOMETRY_DIR $ENV{PAGEDGEOMETRY_HOME})
endif()

find_path(PAGEDGEOMETRY_INCLUDE_DIRS PagedGeometry.h PATHS ${PAGEDGEOMETRY_DIR}/include)

set(PAGEDGEOMETRY_LIBRARY_DIRS ${PAGEDGEOMETRY_DIR}/lib)
find_library(PAGEDGEOMETRY_LIBRARY_RELEASE PagedGeometry HINTS ${PAGEDGEOMETRY_LIBRARY_DIRS} ${PAGEDGEOMETRY_LIBRARY_DIRS}/release)
find_library(PAGEDGEOMETRY_LIBRARY_DEBUG PagedGeometry_d PagedGeometry HINTS ${PAGEDGEOMETRY_LIBRARY_DIRS} ${PAGEDGEOMETRY_LIBRARY_DIRS}/debug)
		
mark_as_advanced(PAGEDGEOMETRY_LIBRARY_RELEASE PAGEDGEOMETRY_LIBRARY_DEBUG)

set(PAGEDGEOMETRY_LIBRARIES optimized ${PAGEDGEOMETRY_LIBRARY_RELEASE}
	 debug ${PAGEDGEOMETRY_LIBRARY_DEBUG})
