project(admesh C)
cmake_minimum_required(VERSION 3.2)

include(GNUInstallDirs)
include_directories(${CMAKE_SOURCE_DIR}/src)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

set (VERSION 1.0.0)
set (SOVERSION 1)

add_executable(admesh
    src/admesh.c
)

set(ADMESH_SRC_LIB
  src/connect.c
  src/normals.c
  src/shared.c
  src/stl_io.c
  src/stlinit.c
  src/util.c
)

set(ADMESH_HDR_LIB
  src/stl.h
)

add_library(libadmesh SHARED ${ADMESH_SRC_LIB})
set_target_properties(libadmesh PROPERTIES
  VERSION ${VERSION}
  SOVERSION ${SOVERSION}
  PREFIX ""
)

target_link_libraries(admesh libadmesh m)

install(TARGETS libadmesh DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS admesh DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${ADMESH_HDR_LIB} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
target_compile_definitions(libadmesh PUBLIC -DVERSION="${VERSION}")