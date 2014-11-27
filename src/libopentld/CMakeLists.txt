
include_directories(    mftracker
			tld
			../3rdparty/cvblobs
			${OpenCV_INCLUDE_DIRS})
			
link_directories(${OpenCV_LIB_DIR})

file(	GLOB_RECURSE
	source_files_TLD
	mftracker/*.cpp
	mftracker/*.h
	tld/*.cpp
	tld/*.h
)

add_library( 	libopentld SHARED 
		${source_files_TLD})

target_link_libraries(libopentld ${OpenCV_LIBS})

# set_target_properties(libopentld PROPERTIES OUTPUT_NAME opentld)

SET(INSTALL_LIB_DIRS ${CMAKE_INSTALL_LIBDIR}/lib )

install( TARGETS "libopentld" LIBRARY DESTINATION ${INSTALL_LIB_DIRS}  COMPONENT main)


# SET(MoGS_Problem_INCLUDE_DIRS "${Include_Directories}" CACHE INTERNAL "" )
# SET(MoGS_Problem_LIBRARY_DIRS "${Library_Directories}" CACHE INTERNAL "" )
# SET(MoGS_Problem_LIBRARIES  -lMoGS_Problem ${MoGS_RBDL_LIBRARIES} ${MoGS_Environment_LIBRARIES} ${MoGS_Motion_Capture_LIBRARIES} ${MoGS_Contact_LIBRARIES} CACHE INTERNAL "")

