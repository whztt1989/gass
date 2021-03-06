#This file hold "proxy" build targets for dependencies that can be 
#used by any GASS lib by just refering to dependencies like 
#internal targets

include(Common)

#TBB
find_package(TBB REQUIRED)
#post-process results 
set(TBB_LIBRARIES optimized ${TBB_LIBRARY} debug ${TBB_LIBRARY_DEBUG})

if(WIN32)
	set(TBB_LIBRARIES ${TBB_LIBRARIES}
	 optimized ${TBB_MALLOC_LIBRARY}
	 debug ${TBB_MALLOC_LIBRARY_DEBUG})
endif()	 

gass_create_dep_target(TBB INCLUDE_DIRS ${TBB_INCLUDE_DIRS} LIBRARIES ${TBB_LIBRARIES} BINARIES_REL ${TBB_BINARY_REL} BINARIES_DBG ${TBB_BINARY_DBG})

#TinyXML2
find_package(TinyXML2 REQUIRED)
gass_create_dep_target(TinyXML2 INCLUDE_DIRS ${TINYXML2_INCLUDE_DIRS} LIBRARIES ${TINYXML2_LIBRARIES})

#Boost
find_package(Boost REQUIRED filesystem system)
if("${CMAKE_VERSION}" VERSION_LESS 3.5.0)
	gass_create_dep_target(Boost INCLUDE_DIRS ${Boost_INCLUDE_DIR})
	set(BOOST_FILE_LIBRARIES optimized ${Boost_FILESYSTEM_LIBRARY_RELEASE}
		 optimized ${Boost_SYSTEM_LIBRARY_RELEASE}
		 debug ${Boost_FILESYSTEM_LIBRARY_DEBUG}
		 debug ${Boost_SYSTEM_LIBRARY_DEBUG})
	gass_create_dep_target(Boost::filesystem INCLUDE_DIRS ${Boost_INCLUDE_DIR} LIBRARIES ${BOOST_FILE_LIBRARIES})
endif()

if(GASS_BUILD_SIM)
	find_package(AngelScript REQUIRED)
	gass_create_dep_target(AngelScript INCLUDE_DIRS ${ANGELSCRIPT_INCLUDE_DIRS} LIBRARIES ${ANGELSCRIPT_LIBRARIES})
endif()

if(GASS_BUILD_PLUGIN_OGRE)
	#FindOgre.cmake use environment var OGRE_HOME
	find_package(OGRE REQUIRED)

	set(OGRE_INCLUDE_DIRS
		${OGRE_INCLUDE_DIR}
		${OGRE_Paging_INCLUDE_DIR}
		${OGRE_Terrain_INCLUDE_DIR}
		${OGRE_Overlay_INCLUDE_DIR})
	set(OGRE_LIBRARY_LIST ${OGRE_LIBRARIES}
		 ${OGRE_Terrain_LIBRARIES}
		 ${OGRE_Paging_LIBRARIES}
		 ${OGRE_Overlay_LIBRARIES})
		 
	set(OGRE_BIN_FILES_RELEASE 
		${OGRE_BINARY_REL}
		${OGRE_Overlay_BINARY_REL}
		${OGRE_Paging_BINARY_REL}
		${OGRE_Terrain_BINARY_REL}
		${OGRE_Plugin_CgProgramManager_REL}
		${OGRE_Plugin_OctreeSceneManager_REL}
		${OGRE_Plugin_ParticleFX_REL}
		${OGRE_RenderSystem_Direct3D9_REL}
		${OGRE_RenderSystem_GL_REL}
		${OGRE_PLUGIN_DIR_REL}/cg.dll)
		
	set(OGRE_BIN_FILES_DEBUG 
		${OGRE_BINARY_DBG}
		${OGRE_Overlay_BINARY_DBG}
		${OGRE_Paging_BINARY_DBG}
		${OGRE_Terrain_BINARY_DBG}
		${OGRE_Plugin_CgProgramManager_DBG}
		${OGRE_Plugin_OctreeSceneManager_DBG}
		${OGRE_Plugin_ParticleFX_DBG}
		${OGRE_RenderSystem_Direct3D9_DBG}
		${OGRE_RenderSystem_GL_DBG}
		${OGRE_PLUGIN_DIR_DBG}/cg.dll)
	
	gass_create_dep_target(Ogre 
		INCLUDE_DIRS ${OGRE_INCLUDE_DIRS} 
		LIBRARIES ${OGRE_LIBRARY_LIST}
		BINARIES_REL ${OGRE_BIN_FILES_RELEASE}
		BINARIES_DBG ${OGRE_BIN_FILES_DEBUG})

endif()

if(GASS_BUILD_PLUGIN_ENVIRONMENT)
	#set(SKYX_DIR   $ENV{SKYX_HOME} CACHE PATH "SkyX folder")
	find_package(SkyX REQUIRED)
	gass_create_dep_target(SkyX 
					INCLUDE_DIRS ${SKYX_INCLUDE_DIRS} 
					LIBRARIES ${SKYX_LIBRARIES}
					BINARIES_REL ${SKYX_BINARY_REL}
					BINARIES_DBG ${SKYX_BINARY_DBG})
	
	#set(HYDRAX_DIR $ENV{HYDRAX_HOME} CACHE PATH "Hydrax folder")
	find_package(Hydrax REQUIRED)
	gass_create_dep_target(Hydrax INCLUDE_DIRS ${HYDRAX_INCLUDE_DIRS} LIBRARIES ${HYDRAX_LIBRARIES})
endif()


if(GASS_BUILD_PLUGIN_OIS)
	find_package(OIS REQUIRED)
	gass_create_dep_target(OIS 
				INCLUDE_DIRS ${OIS_INCLUDE_DIRS} 
				LIBRARIES ${OIS_LIBRARIES}
				BINARIES_REL ${OIS_BINARY_REL}
				BINARIES_DBG ${OIS_BINARY_DBG})
endif()

if(GASS_BUILD_PLUGIN_ODE)
	find_package(ODE REQUIRED)
	gass_create_dep_target(ODE INCLUDE_DIRS ${ODE_INCLUDE_DIRS} LIBRARIES ${ODE_LIBRARIES} DEFINITIONS dDOUBLE)
endif()

if(GASS_BUILD_PLUGIN_MYGUI)
	find_package(MyGUI REQUIRED)
	gass_create_dep_target(MyGUI INCLUDE_DIRS ${MYGUI_INCLUDE_DIRS}  LIBRARIES ${MYGUI_LIBRARIES})
endif()

if(GASS_BUILD_PLUGIN_OPENAL)
	find_package(OpenALExt REQUIRED)
	gass_create_dep_target(OpenAL 
		INCLUDE_DIRS ${OPENAL_INCLUDE_DIR}  
		LIBRARIES ${OPENAL_LIBRARY}
		BINARIES_REL ${OPENAL_BIN_FILES_RELEASE}
		BINARIES_DBG ${OPENAL_BIN_FILES_RELEASE})
endif()

if(GASS_BUILD_PLUGIN_PAGED_GEOMETRY)
	find_package(PagedGeometry)
	gass_create_dep_target(PagedGeometry 
		INCLUDE_DIRS ${PAGEDGEOMETRY_INCLUDE_DIRS}  
		LIBRARIES ${PAGEDGEOMETRY_LIBRARIES})
endif()

#RakNet
if(GASS_BUILD_PLUGIN_RAKNET)
	#set(RAKNET_DIR  $ENV{RAKNET_HOME} CACHE PATH "RakNet folder")
	find_package(RakNet)
	if(WIN32)
		set(RAKNET_LIBRARIES ${RAKNET_LIBRARIES} debug ws2_32 optimized ws2_32)
	endif()
	gass_create_dep_target(RakNet INCLUDE_DIRS ${RAKNET_INCLUDE_DIRS}  LIBRARIES ${RAKNET_LIBRARIES})
endif()

#OSG
if(GASS_BUILD_PLUGIN_OSG)
	find_package(OSGExt 3.2.1 REQUIRED osgUtil osgDB osgGA osgText osgShadow osgViewer osgSim osgTerrain)
	gass_create_dep_target(OSG INCLUDE_DIRS ${OPENSCENEGRAPH_INCLUDE_DIRS} LIBRARIES ${OPENSCENEGRAPH_LIBRARIES} BINARIES_REL ${OSG_BINARIES_REL} BINARIES_DBG ${OSG_BINARIES_DBG})
	
	if(GASS_INSTALL_DEP_BINARIES AND WIN32) #install osg plugins to subfolder
		install(FILES ${OSGPLUGIN_BINARIES_REL} DESTINATION ${GASS_INSTALL_BIN_DIR_RELEASE}/osgPlugins-${OSG_VERSION} CONFIGURATIONS Release)
		install(FILES ${OSGPLUGIN_BINARIES_DBG} DESTINATION ${GASS_INSTALL_BIN_DIR_DEBUG}/osgPlugins-${OSG_VERSION} CONFIGURATIONS Debug)
		
		file(COPY ${OSGPLUGIN_BINARIES_REL} DESTINATION  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/release)
		file(COPY ${OSGPLUGIN_BINARIES_DBG} DESTINATION  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug)
	endif()
endif()

if(GASS_BUILD_PLUGIN_OSGEARTH)
	set(CMAKE_MODULE_PATH
		${CMAKE_MODULE_PATH}
		$ENV{OSGEARTHDIR}/CMakeModules)
	find_package(OSGEarth)
	set(OSGEARTH_LIBRARIES optimized ${OSGEARTH_LIBRARY}
						optimized ${OSGEARTHFEATURES_LIBRARY}
						optimized ${OSGEARTHUTIL_LIBRARY}
						optimized ${OSGEARTHSYMBOLOGY_LIBRARY}
						optimized ${OSGEARTHANNOTATION_LIBRARY}
						debug ${OSGEARTH_LIBRARY_DEBUG}
						debug ${OSGEARTHFEATURES_LIBRARY_DEBUG}
						debug ${OSGEARTHUTIL_LIBRARY_DEBUG}
						debug ${OSGEARTHSYMBOLOGY_LIBRARY_DEBUG}
						debug ${OSGEARTHANNOTATION_LIBRARY_DEBUG})
						
	message(OSGEARTH_LIBRARY: ${OSGEARTH_LIBRARY_DBG})
	gass_create_dep_target(OSGEarth 
					INCLUDE_DIRS ${OSGEARTH_INCLUDE_DIRS} 
					LIBRARIES ${OSGEARTH_LIBRARIES})
endif()