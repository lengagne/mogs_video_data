
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall  ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")

include_directories(imacq
    ../mftracker
    .
    ../cvblobs
    ${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

file(	GLOB_RECURSE
	source_files_libopentld
	../mftracker/*.cpp
	../mftracker/*.h
	*.cpp
	*.h
)

add_library(opentld SHARED
		${source_files_libopentld}
)

target_link_libraries(	opentld 
			${OpenCV_LIBS})

# set_target_properties(libopentld PROPERTIES OUTPUT_NAME opentld)

SET(OPENTLD_LIBS "opentld" CACHE INTERNAL "" )

SET(INSTALL_LIB_DIRS ${CMAKE_INSTALL_LIBDIR}/lib )
install( TARGETS opentld LIBRARY DESTINATION ${INSTALL_LIB_DIRS}  COMPONENT main)