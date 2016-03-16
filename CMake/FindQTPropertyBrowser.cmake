if (NOT "$ENV{QTPROPERTYBROWSER_DIR}" STREQUAL "")
	set(QTPROPERTYBROWSER_DIR $ENV{QTPROPERTYBROWSER_HOME})
endif()

find_path(QTPROPERTYBROWSER_DIR_INCLUDE_DIRS qtpropertybrowser.h PATHS ${QTPROPERTYBROWSER_DIR}/include/QtPropertyBrowser ${QTPROPERTYBROWSER_DIR}/src)

set(QTPROPERTYBROWSER_LIBRARY_DIRS ${QTPROPERTYBROWSER_DIR}/lib)

find_library(QTPROPERTYBROWSER_LIBRARY QtPropertyBrowser HINTS ${QTPROPERTYBROWSER_LIBRARY_DIRS} ${QTPROPERTYBROWSER_LIBRARY_DIRS}/release)
find_library(QTPROPERTYBROWSER_LIBRARY_DEBUG QtPropertyBrowser_d HINTS ${QTPROPERTYBROWSER_LIBRARY_DIRS} ${QTPROPERTYBROWSER_LIBRARY_DIRS}/debug)
		
mark_as_advanced(QTPROPERTYBROWSER_LIBRARY QTPROPERTYBROWSER_LIBRARY_DEBUG)

set(QTPROPERTYBROWSER_LIBRARIES optimized ${QTPROPERTYBROWSER_LIBRARY}
	 debug ${QTPROPERTYBROWSER_LIBRARY_DEBUG})
	 
if (WIN32)
	  set(QTPROPERTYBROWSER_BINARY_DIRS ${QTPROPERTYBROWSER_DIR}/lib)
	  find_file(QTPROPERTYBROWSER_BINARY_REL NAMES QtSolutions_PropertyBrowser-2.5.dll HINTS ${QTPROPERTYBROWSER_BINARY_DIRS})
	  find_file(QTPROPERTYBROWSER_BINARY_DBG NAMES QtSolutions_PropertyBrowser-2.5d.dll HINTS ${QTPROPERTYBROWSER_BINARY_DIRS})
endif()
