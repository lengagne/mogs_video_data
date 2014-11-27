file(	GLOB_RECURSE
	source_files_cvblobs
	*.cpp
	*.h
)

add_library(cvblobs SHARED ${source_files_cvblobs})

SET(INSTALL_LIB_DIRS ${CMAKE_INSTALL_LIBDIR}/lib )

install( TARGETS "cvblobs" LIBRARY DESTINATION ${INSTALL_LIB_DIRS}  COMPONENT main)

SET(CVBLOBS_LIBS "cvblobs" CACHE INTERNAL "" )