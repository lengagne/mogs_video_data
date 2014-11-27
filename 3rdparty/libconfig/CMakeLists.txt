
file(	GLOB_RECURSE
	source_files_config++
	*.c
	*.h
)

add_library(config++ SHARED ${source_files_config++})

SET(INSTALL_LIB_DIRS ${CMAKE_INSTALL_LIBDIR}/lib )

install( TARGETS "config++" LIBRARY DESTINATION ${INSTALL_LIB_DIRS}  COMPONENT main)

SET(CONFIGPP_LIBS "config++" CACHE INTERNAL "" )

