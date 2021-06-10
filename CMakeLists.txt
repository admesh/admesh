project(admesh C)
cmake_minimum_required(VERSION 3.2)

include(GNUInstallDirs)
include_directories(${CMAKE_SOURCE_DIR}/src)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

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

set (prefix ${CMAKE_INSTALL_PREFIX})
set (exec_prefix ${CMAKE_INSTALL_FULL_BINDIR})
set (libdir ${CMAKE_INSTALL_FULL_LIBDIR})
set (includedir ${CMAKE_INSTALL_FULL_INCLUDEDIR})
configure_file(${CMAKE_SOURCE_DIR}/libadmesh.pc.in ${CMAKE_BINARY_DIR}/libadmesh.pc)

install(TARGETS libadmesh EXPORT admesh DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS admesh EXPORT admesh DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${ADMESH_HDR_LIB} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/admesh)
install(FILES ${CMAKE_BINARY_DIR}/libadmesh.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig/)
install(FILES FindADMESH.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/admesh)
install(EXPORT admesh DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/admesh)
target_compile_definitions(libadmesh PUBLIC -DVERSION="${VERSION}")

enable_testing()
add_test(basic ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/block.stl )