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
# Basic test
add_test(basic ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/block.stl -a ${CMAKE_BINARY_DIR}/basic.stl )
add_test(basic-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/basic.stl ${CMAKE_BINARY_DIR}/basic.stl)

# x-rotate-30
add_test(x-rotate-30 ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/block.stl --x-rotate=30  -a ${CMAKE_BINARY_DIR}/x-rotate-30.stl )
add_test(x-rotate-30-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/x-rotate-30.stl ${CMAKE_BINARY_DIR}/x-rotate-30.stl)

# y-rotate-30
add_test(y-rotate-30 ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/block.stl --y-rotate=30  -a ${CMAKE_BINARY_DIR}/y-rotate-30.stl )
add_test(y-rotate-30-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/y-rotate-30.stl ${CMAKE_BINARY_DIR}/y-rotate-30.stl)

# z-rotate-30
add_test(z-rotate-30 ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/block.stl --z-rotate=30  -a ${CMAKE_BINARY_DIR}/z-rotate-30.stl )
add_test(z-rotate-30-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/z-rotate-30.stl ${CMAKE_BINARY_DIR}/z-rotate-30.stl)

# x-y-z-rotate-30
add_test(x-y-z-rotate-30 ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/block.stl --x-rotate=30 --y-rotate=30 --z-rotate=30 -a ${CMAKE_BINARY_DIR}/x-y-z-rotate-30.stl )
add_test(x-y-z-rotate-30-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/x-y-z-rotate-30.stl ${CMAKE_BINARY_DIR}/x-y-z-rotate-30.stl)