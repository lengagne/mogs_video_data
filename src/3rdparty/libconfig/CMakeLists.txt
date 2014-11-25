#This are the defines for building a static libconfig for windows
if(WIN32)
	add_definitions(-DLIBCONFIGXX_STATIC -DLIBCONFIG_STATIC -DYY_NO_UNISTD_H -DYY_USE_CONST -D_CRT_SECURE_NO_DEPRECATE -D_STDLIB_H)
endif(WIN32)


file(	GLOB_RECURSE
	source_files_config++
	*.c
	*.h
)


add_library(config++ SHARED ${source_files_config++})

SET(INSTALL_LIB_DIRS ${CMAKE_INSTALL_LIBDIR}/lib )

install( TARGETS "config++" LIBRARY DESTINATION ${INSTALL_LIB_DIRS}  COMPONENT main)

