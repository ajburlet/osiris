
set (SOURCES
	source/gl_load.c
	source/gl_load_cpp.cpp
)

file (GLOB HEADERS
	include/glload/gl_*.hpp	
	include/glload/*_gl_*.hpp	
)

source_group("Sources" FILES ${SOURCES})
source_group("Headers" FILES ${HEADERS})

include_directories(./include)

add_library(glload STATIC ${SOURCES} ${HEADERS})

