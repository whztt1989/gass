# create_source_group(relativeSourcePath sourceGroupName files)
    #
    # Creates a source group with the specified name relative to the relative path
    # specified.
    #
    # Parameters:
    #    - sourceGroupName: Name of the source group to create.
    #    - relativeSourcePath: Relative path to the files.
    #    - sourceFiles: Files to add to the source group.
    #
    # For example if you have the following directory structure:
    #
    #    - ExampleApplication
    #        - include
    #            - Main.h
    #                - Window
    #                    Window.h
    #        - source
    #            - Main.cpp
    #                - Window
    #                    Window.cpp
    #
    # You can get your list of files and call create_source_group the following way
    #
    #    file(GLOB_RECURSE my_source_files ${CMAKE_CURRENT_SOURCE_DIR}/source/*)
    #    create_source_group("Source Files"  "${CMAKE_CURRENT_SOURCE_DIR}/source" ${my_source_files})
    #    file(GLOB_RECURSE my_header_files ${CMAKE_CURRENT_SOURCE_DIR}/include/*)
    #    create_source_group("Header Files" "${CMAKE_CURRENT_SOURCE_DIR}/include" ${my_header_files})
    #    add_executable(ExampleApplication ${my_source_files} ${my_header_files})
    #
    # Then the generated solution would look like this
    #
    #    - ExampleApplication (project)
    #        - Header Files
    #            - Main.h
    #                - Window
    #                    Window.h
    #        - Source Files
    #            - Main.cpp
    #                - Window
    #                    Window.cpp
    #
function(create_source_group sourceGroupName relativeSourcePath sourceFiles)
		#dont know why but foreach skip first element if we use ARGN
		set(my_files ${ARGN})
		
		FOREACH(currentSourceFile ${my_files})
		   
			FILE(RELATIVE_PATH folder ${relativeSourcePath} ${currentSourceFile})
            get_filename_component(filename ${folder} NAME)
			
            string(REPLACE ${filename} "" folder ${folder})
			
            if(NOT folder STREQUAL "")
                string(REGEX REPLACE "/+$" "" folderlast ${folder})
                string(REPLACE "/" "\\" folderlast ${folderlast})
                SOURCE_GROUP("${sourceGroupName}\\${folderlast}" FILES ${currentSourceFile})
            endif(NOT folder STREQUAL "")
        ENDFOREACH(currentSourceFile ${ARGN})
endfunction(create_source_group)

macro(deprecated_add_source_from_current_dir)
	#Grab cpp and h  files from path recursively
	file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
	file(GLOB_RECURSE H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

	#add dummy element 
	set(TEMP_CPP_FILES "dummy" ${CPP_FILES})
	set(TEMP_H_FILES "dummy" ${H_FILES})
	
	#Create ide source groups that repilicates folder structure 
	create_source_group("Source Files"  "${CMAKE_CURRENT_SOURCE_DIR}" ${TEMP_CPP_FILES})
	create_source_group("Header Files"  "${CMAKE_CURRENT_SOURCE_DIR}" ${TEMP_H_FILES})	
endmacro()

macro(gass_get_source_from_current_dir _SOURCE_FILES _HEADER_FILES)
	#Grab cpp and h  files from path recursively
	file(GLOB_RECURSE ${_SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
	file(GLOB_RECURSE ${_HEADER_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
endmacro()

macro(gass_filter_list INPUT OUTPUT GOOD BAD)
  set(LST ${INPUT})   # can we avoid this?
  set(PICKME YES)
  foreach(ELEMENT IN LISTS LST)
    if(${ELEMENT} STREQUAL general OR ${ELEMENT} STREQUAL ${GOOD})
      set(PICKME YES)
    elseif(${ELEMENT} STREQUAL ${BAD})
      set(PICKME NO)
    elseif(PICKME)
      list(APPEND ${OUTPUT} ${ELEMENT})
    endif()
  endforeach()
endmacro()

macro(gass_get_header_directories _RETURN_DIRS)
    file(GLOB_RECURSE NEW_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
    set(DIR_LIST "")
    foreach(C_FILE_PATH ${NEW_LIST})
        get_filename_component(DIR_PATH ${C_FILE_PATH} PATH)
        set(DIR_LIST ${DIR_LIST} ${DIR_PATH})
    endforeach()
    list(REMOVE_DUPLICATES DIR_LIST)
    set(${_RETURN_DIRS} ${DIR_LIST})
endmacro()

macro(gass_setup_plugin _PLUGIN_NAME)
	gass_get_source_from_current_dir(SOURCE_FILES HEADER_FILES)
	gass_setup_lib(${_PLUGIN_NAME} 
					${ARGN} 
					BUILDTYPE ${GASS_BUILDTYPE}
					SOURCE_FILES ${SOURCE_FILES} 
					HEADER_FILES ${HEADER_FILES} 
					SKIP_HEADER_INSTALL )
	gass_get_header_directories(HEADER_SUBDIRS)
	foreach(INC_DIR ${HEADER_SUBDIRS})
		target_include_directories(${_PLUGIN_NAME} PRIVATE  $<BUILD_INTERFACE:${INC_DIR}>)
	endforeach()
	
	target_link_libraries(${_PLUGIN_NAME} PRIVATE GASSSim)
	
	target_compile_definitions(${_PLUGIN_NAME} PRIVATE ${GASS_COMMON_DEFINITIONS} GASS_PLUGIN_EXPORTS)
	#set_target_properties(${_PLUGIN_NAME} PROPERTIES SUFFIX .galp)
	set_target_properties(${_PLUGIN_NAME} PROPERTIES FOLDER "Plugins")
endmacro()

include(CMakeParseArguments)

macro(gass_setup_lib _LIB_NAME)
	cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "SKIP_HEADER_INSTALL" # list of names of the boolean arguments (only defined ones will be true)
        "BUILDTYPE" # list of names of mono-valued arguments
        "HEADER_FILES;SOURCE_FILES;PUBLIC_INCLUDE_DIRS;PRIVATE_INCLUDE_DIRS;PUBLIC_DEPS;PRIVATE_DEPS;PUBLIC_DEFINITIONS;PRIVATE_DEFINITIONS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	
	set(ALL_FILES ${PARSED_ARGS_SOURCE_FILES} ${PARSED_ARGS_HEADER_FILES})
	
	add_library (${_LIB_NAME} ${PARSED_ARGS_BUILDTYPE} ${PARSED_ARGS_SOURCE_FILES} ${PARSED_ARGS_HEADER_FILES})
	
	foreach(INC_DIR ${PARSED_ARGS_PUBLIC_INCLUDE_DIRS})
		target_include_directories(${_LIB_NAME} PUBLIC $<BUILD_INTERFACE:${INC_DIR}>)
	endforeach()
	
	foreach(INC_DIR ${PARSED_ARGS_PRIVATE_INCLUDE_DIRS})
		target_include_directories(${_LIB_NAME} PRIVATE $<BUILD_INTERFACE:${INC_DIR}>)
	endforeach()

	foreach(CUR_DEP ${PARSED_ARGS_PUBLIC_DEPS})
		target_link_libraries(${_LIB_NAME} PUBLIC ${CUR_DEP})
		#message("${_LIB_NAME} PUBLIC ${CUR_DEP}")
	endforeach()
	
	foreach(CUR_DEP ${PARSED_ARGS_PRIVATE_DEPS})
		target_link_libraries(${_LIB_NAME} PRIVATE ${CUR_DEP})
	endforeach()

	target_compile_definitions(${_LIB_NAME} PUBLIC ${PARSED_ARGS_PUBLIC_DEFINITIONS})
	target_compile_definitions(${_LIB_NAME} PRIVATE ${PARSED_ARGS_PRIVATE_DEFINITIONS})
	set_target_properties(${_LIB_NAME} PROPERTIES DEBUG_POSTFIX _d)
	
	if(NOT PARSED_ARGS_SKIP_HEADER_INSTALL)
		#INSTALL(FILES ${PARSED_ARGS_HEADER_FILES} DESTINATION include)
		install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${GASS_INSTALL_INCLUDE_DIR} FILES_MATCHING PATTERN "*.h")
	endif()
	
	install(TARGETS ${_LIB_NAME}
		RUNTIME DESTINATION ${GASS_INSTALL_BIN_DIR_DEBUG} CONFIGURATIONS Debug	
		LIBRARY DESTINATION ${GASS_INSTALL_LIB_DIR_DEBUG} CONFIGURATIONS Debug
		ARCHIVE DESTINATION ${GASS_INSTALL_LIB_DIR_DEBUG} CONFIGURATIONS Debug)

	install(TARGETS ${_LIB_NAME}
	  RUNTIME DESTINATION ${GASS_INSTALL_BIN_DIR_RELEASE} CONFIGURATIONS Release
	  LIBRARY DESTINATION ${GASS_INSTALL_LIB_DIR_RELEASE} CONFIGURATIONS Release
	  ARCHIVE DESTINATION ${GASS_INSTALL_LIB_DIR_RELEASE} CONFIGURATIONS Release)

endmacro()

macro(gass_create_dep_target DEP_NAME)
	cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "" # list of names of mono-valued arguments
        "LIBRARIES;INCLUDE_DIRS;DEFINITIONS;BINARIES_REL;BINARIES_DBG" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	add_library(${DEP_NAME} INTERFACE IMPORTED)
	set_property(TARGET ${DEP_NAME} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${PARSED_ARGS_INCLUDE_DIRS})
	set(_RELEASE_LIBS "")
	set(_DEBUG_LIBS "")
	gass_filter_list("${PARSED_ARGS_LIBRARIES}" _RELEASE_LIBS optimized debug)
	gass_filter_list("${PARSED_ARGS_LIBRARIES}" _DEBUG_LIBS debug optimized)
	set_property(TARGET ${DEP_NAME} PROPERTY INTERFACE_LINK_LIBRARIES $<$<CONFIG:Debug>:${_DEBUG_LIBS}> $<$<NOT:$<CONFIG:Debug>>:${_RELEASE_LIBS}>)
	set_property(TARGET ${DEP_NAME} PROPERTY INTERFACE_COMPILE_DEFINITIONS ${PARSED_ARGS_DEFINITIONS})
	#message("${DEP_NAME} INTERFACE_COMPILE_DEFINITIONS ${PARSED_ARGS_DEFINITIONS}")
	if(GASS_INSTALL_DEP_BINARIES)
		if(PARSED_ARGS_BINARIES_REL)
			INSTALL(FILES ${PARSED_ARGS_BINARIES_REL} DESTINATION ${GASS_INSTALL_BIN_DIR_RELEASE} CONFIGURATIONS Release)
			if(WIN32)
				FILE(COPY ${PARSED_ARGS_BINARIES_REL}  DESTINATION  ${CMAKE_BINARY_DIR}/out/release) 
			endif()
		endif()
		if(PARSED_ARGS_BINARIES_DBG)
			INSTALL(FILES ${PARSED_ARGS_BINARIES_DBG} DESTINATION ${GASS_INSTALL_BIN_DIR_DEBUG} CONFIGURATIONS Debug)
			if(WIN32)
				FILE(COPY ${PARSED_ARGS_BINARIES_DBG}  DESTINATION  ${CMAKE_BINARY_DIR}/out/debug) 
			endif()
		endif()
	endif()
endmacro()

macro(gass_setup_sim_sample SAMPLE_NAME)
	cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "" # list of names of mono-valued arguments
        "DEPS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
	gass_get_source_from_current_dir(CPP_FILES H_FILES)
	add_executable (${SAMPLE_NAME} ${CPP_FILES} ${H_FILES})
	
	gass_get_header_directories(HEADER_SUBDIRS)
	
	foreach(INC_DIR ${HEADER_SUBDIRS})
		target_include_directories(${SAMPLE_NAME} PRIVATE  $<BUILD_INTERFACE:${INC_DIR}>)
	endforeach()
	
	foreach(CUR_DEP ${PARSED_ARGS_DEPS})
		target_link_libraries(${SAMPLE_NAME} ${CUR_DEP})
		#message("${_LIB_NAME} PUBLIC ${CUR_DEP}")
	endforeach()
	
	target_link_libraries(${SAMPLE_NAME} GASSSim)
	
	target_compile_definitions(${SAMPLE_NAME} PRIVATE ${GASS_COMMON_DEFINITIONS})
	set_target_properties(${SAMPLE_NAME} PROPERTIES DEBUG_POSTFIX _d)
	set_target_properties(${SAMPLE_NAME} PROPERTIES FOLDER "SimSamples")
	
	set(SAMPLE_CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/${SAMPLE_NAME}.xml)

	#copy configurations to enable execution from build folder 
	FILE(COPY ${SAMPLE_CONFIG} DESTINATION  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/release)
	FILE(COPY ${SAMPLE_CONFIG} DESTINATION  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug)

	#install executable
	install(TARGETS ${SAMPLE_NAME}  RUNTIME DESTINATION ${GASS_INSTALL_BIN_DIR_RELEASE} CONFIGURATIONS Release)
	install(TARGETS ${SAMPLE_NAME}  RUNTIME DESTINATION ${GASS_INSTALL_BIN_DIR_DEBUG} CONFIGURATIONS Debug)

	#install configuration files
	install(FILES ${SAMPLE_CONFIG} DESTINATION ${GASS_INSTALL_BIN_DIR_RELEASE} CONFIGURATIONS Release)
	install(FILES ${SAMPLE_CONFIG} DESTINATION ${GASS_INSTALL_BIN_DIR_DEBUG} CONFIGURATIONS Debug)
endmacro()

macro(gass_setup_core_sample SAMPLE_NAME)

	set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed" )
	gass_get_source_from_current_dir(CPP_FILES H_FILES)
	add_executable (${SAMPLE_NAME} ${CPP_FILES} ${H_FILES})
	target_link_libraries(${SAMPLE_NAME} GASSCore)
	target_compile_definitions(${SAMPLE_NAME} PRIVATE ${GASS_COMMON_DEFINITIONS})
	set_target_properties(${SAMPLE_NAME} PROPERTIES DEBUG_POSTFIX _d)
	set_target_properties(${SAMPLE_NAME} PROPERTIES FOLDER "CoreSamples")
	
	#install executable
	install(TARGETS ${SAMPLE_NAME}  RUNTIME DESTINATION ${GASS_INSTALL_BIN_DIR_RELEASE} CONFIGURATIONS Release)
	install(TARGETS ${SAMPLE_NAME}  RUNTIME DESTINATION ${GASS_INSTALL_BIN_DIR_DEBUG} CONFIGURATIONS Debug)

	#install configuration files
	install(FILES ${SAMPLE_CONFIG} DESTINATION ${GASS_INSTALL_BIN_DIR_RELEASE} CONFIGURATIONS Release)
	install(FILES ${SAMPLE_CONFIG} DESTINATION ${GASS_INSTALL_BIN_DIR_DEBUG} CONFIGURATIONS Debug)
endmacro()


