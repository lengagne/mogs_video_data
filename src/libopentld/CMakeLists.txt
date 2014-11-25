
include_directories(imacq
    mftracker
    tld
    ../3rdparty/cvblobs
    ${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

file(	GLOB_RECURSE
	source_files_TLD
	imacq/*.cpp
	imacq/*.h
	mftracker/*.cpp
	mftracker/*.h
	tld/*.cpp
	tld/*.h
)

add_library(libopentld SHARED 
	${source_files_TLD})

target_link_libraries(libopentld ${OpenCV_LIBS})

set_target_properties(libopentld PROPERTIES OUTPUT_NAME opentld)

SET(INSTALL_LIB_DIRS ${CMAKE_INSTALL_LIBDIR}/lib )

install( TARGETS "libopentld" LIBRARY DESTINATION ${INSTALL_LIB_DIRS}  COMPONENT main)

