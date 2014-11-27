
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

add_library(opentld
		${source_files_libopentld}
)

target_link_libraries(opentld ${OpenCV_LIBS})

# set_target_properties(libopentld PROPERTIES OUTPUT_NAME opentld)

SET(OPENTLD_LIBS "opentld" CACHE INTERNAL "" )