cmake_minimum_required (VERSION 2.8)
enable_language(CXX)

project (MOGS_VIDEO_DATA)
set(PROJECT_CONTACT sebastien.lengagne@univ-bpclermont.fr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
set(CMAKE_VERBOSE_MAKEFILE OFF)

set (MOGS_VIDEO_DATA_VERSION_MAJOR 0)
set (MOGS_VIDEO_DATA_VERSION_MINOR 1)

foreach(p LIB INCLUDE)
	set(var CMAKE_INSTALL_${p}DIR)
	if(NOT IS_ABSOLUTE "${${var}}")
		set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
	endif()
endforeach()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})


find_package( OpenCV REQUIRED )
find_package( MoGS_Tinyxml REQUIRED )

add_subdirectory(3rdparty)

file(	GLOB_RECURSE
	source_files_MOGS_VIDEO_DATA
	src/*.cpp
)

LIST(REMOVE_ITEM source_files_MOGS_VIDEO_DATA  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

configure_file ( "${PROJECT_SOURCE_DIR}/config_Video_data.h.in"
		 "${PROJECT_SOURCE_DIR}/include/config_Video_data.h" )

include_directories(	include
			3rdparty/tld
			${OpenCV_INCLUDE_DIRS}
			${MoGS_Tinyxml_INCLUDE_DIRS}
			)

link_directories(	${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}
)

ADD_LIBRARY(	MoGS_Video_Data SHARED 
		${source_files_MOGS_VIDEO_DATA}		
)
# 
target_link_libraries(MoGS_Video_Data
			${OpenCV_LIBS} 
			${OPENTLD_LIBS}
			${CONFIGPP_LIBS}
			${CVBLOBS_LIBS}
			${MoGS_Tinyxml_LIBRARIES})
			
link_directories(${MoGS_Tinyxml_LIBRARY_DIR})

ADD_EXECUTABLE(	MoGS_video_tracking
		"${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

target_link_libraries(	MoGS_video_tracking
			MoGS_Video_Data
)


# if the compilation is done for whole the project
set(MoGS_Video_Data_FOUND "TRUE" CACHE INTERNAL "")
GET_PROPERTY(Include_Directories DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
GET_PROPERTY(Library_Directories DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
SET(MoGS_Video_Data_INCLUDE_DIRS "${Include_Directories}" CACHE INTERNAL "" )
SET(MoGS_Video_Data_LIBRARY_DIRS "${Library_Directories}" CACHE INTERNAL "" )
set(MoGS_Video_Data_LIBRARIES  -lMoGS_Video_Data CACHE INTERNAL "")
set(MoGS_Video_Data_FLAGS " " CACHE INTERNAL "")

add_definitions(${MoGS_Video_Data_FLAGS})

SET(INSTALL_LIB_DIRS ${CMAKE_INSTALL_LIBDIR}/lib )

install(DIRECTORY 	${CMAKE_CURRENT_SOURCE_DIR}/tools/DTD
			${CMAKE_CURRENT_SOURCE_DIR}/tools/templates
	DESTINATION 	${CMAKE_INSTALL_LIBDIR}/share/MoGS)

install( TARGETS MoGS_video_tracking RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin  COMPONENT main)

install( FILES "tools/mogs_video_completion.sh" DESTINATION /etc/bash_completion.d/)

install( TARGETS MoGS_Video_Data LIBRARY DESTINATION ${INSTALL_LIB_DIRS}  COMPONENT main)

	### Configure file
configure_file(MoGS_Video_DataConfig.cmake.in "${PROJECT_BINARY_DIR}/MoGS_Video_DataConfig.cmake" @ONLY)

install ( FILES "${PROJECT_BINARY_DIR}/MoGS_Video_DataConfig.cmake"
	  DESTINATION "${INSTALL_LIB_PATH}/MoGS_Video_Data" COMPONENT dev ) 