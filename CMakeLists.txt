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
# basic
add_test(basic ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl -a ${CMAKE_BINARY_DIR}/basic.stl )
add_test(basic-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/basic.stl ${CMAKE_BINARY_DIR}/basic.stl)

# x-rotate-30
add_test(x-rotate-30 ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --x-rotate=30  -a ${CMAKE_BINARY_DIR}/x-rotate-30.stl )
add_test(x-rotate-30-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/x-rotate-30.stl ${CMAKE_BINARY_DIR}/x-rotate-30.stl)

# y-rotate-30
add_test(y-rotate-30 ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --y-rotate=30  -a ${CMAKE_BINARY_DIR}/y-rotate-30.stl )
add_test(y-rotate-30-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/y-rotate-30.stl ${CMAKE_BINARY_DIR}/y-rotate-30.stl)

# z-rotate-30
add_test(z-rotate-30 ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --z-rotate=30  -a ${CMAKE_BINARY_DIR}/z-rotate-30.stl )
add_test(z-rotate-30-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/z-rotate-30.stl ${CMAKE_BINARY_DIR}/z-rotate-30.stl)

# x-y-z-rotate-30
add_test(x-y-z-rotate-30 ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --x-rotate=30 --y-rotate=30 --z-rotate=30 -a ${CMAKE_BINARY_DIR}/x-y-z-rotate-30.stl )
add_test(x-y-z-rotate-30-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/x-y-z-rotate-30.stl ${CMAKE_BINARY_DIR}/x-y-z-rotate-30.stl)

# xy-mirror
add_test(xy-mirror ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --xy-mirror -a ${CMAKE_BINARY_DIR}/xy-mirror.stl)
add_test(xy-mirror-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/xy-mirror.stl ${CMAKE_BINARY_DIR}/xy-mirror.stl)

# yz-mirror
add_test(yz-mirror ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --yz-mirror -a ${CMAKE_BINARY_DIR}/yz-mirror.stl)
add_test(yz-mirror-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/yz-mirror.stl ${CMAKE_BINARY_DIR}/yz-mirror.stl)

# xz-mirror
add_test(xz-mirror ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --xz-mirror -a ${CMAKE_BINARY_DIR}/xz-mirror.stl)
add_test(xz-mirror-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/xz-mirror.stl ${CMAKE_BINARY_DIR}/xz-mirror.stl)

# scale
add_test(scale ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --scale 3 -a ${CMAKE_BINARY_DIR}/scale.stl)
add_test(scale-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/scale.stl ${CMAKE_BINARY_DIR}/scale.stl)

# scale-xyz
add_test(scale-xyz ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --scale-xyz=2,3,4 -a ${CMAKE_BINARY_DIR}/scale-xyz.stl)
add_test(scale-xyz-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/scale-xyz.stl ${CMAKE_BINARY_DIR}/scale-xyz.stl)

# translate
add_test(translate ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --translate=2,3,4 -a ${CMAKE_BINARY_DIR}/translate.stl)
add_test(translate-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/translate.stl ${CMAKE_BINARY_DIR}/translate.stl)

# translate-rel
add_test(translate-rel ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --translate-rel=2,3,4 -a ${CMAKE_BINARY_DIR}/translate-rel.stl)
add_test(translate-rel-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/translate-rel.stl ${CMAKE_BINARY_DIR}/translate-rel.stl)

# stretch
add_test(stretch ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --stretch=::22,::3,::5 -a ${CMAKE_BINARY_DIR}/stretch.stl)
add_test(stretch-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/stretch.stl ${CMAKE_BINARY_DIR}/stretch.stl)

# merge
add_test(merge ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --merge=${CMAKE_SOURCE_DIR}/test/stretch.stl -a ${CMAKE_BINARY_DIR}/merge.stl)
add_test(merge-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/merge.stl ${CMAKE_BINARY_DIR}/merge.stl)

# nearby
add_test(nearby ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/test/nearby-bad.stl -n -t 0.1 -a ${CMAKE_BINARY_DIR}/nearby.stl)
add_test(nearby-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/nearby-good.stl ${CMAKE_BINARY_DIR}/nearby.stl)

# remove-unconnected
add_test(remove-unconnected ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/test/remove-unconnected-bad.stl --remove-unconnected -a ${CMAKE_BINARY_DIR}/remove-unconnected.stl)
add_test(remove-unconnected-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/remove-unconnected-good.stl ${CMAKE_BINARY_DIR}/remove-unconnected.stl)

# fill-holes
add_test(fill-holes ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/test/fill-holes-bad.stl --fill-holes -a ${CMAKE_BINARY_DIR}/fill-holes.stl)
add_test(fill-holes-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/fill-holes-good.stl ${CMAKE_BINARY_DIR}/fill-holes.stl)

# normal-directions
add_test(normal-directions ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/test/normal-directions-bad.stl --normal-directions -a ${CMAKE_BINARY_DIR}/normal-directions.stl)
add_test(normal-directions-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/normal-directions-good.stl ${CMAKE_BINARY_DIR}/normal-directions.stl)

# binary-stl test
add_test(binary-stl ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl -b ${CMAKE_BINARY_DIR}/binary.stl )
add_test(binary-stl-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/binary.stl ${CMAKE_BINARY_DIR}/binary.stl)

# off-stl test
add_test(off-stl ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --write-off ${CMAKE_BINARY_DIR}/basic.off )
add_test(off-stl-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/basic.off ${CMAKE_BINARY_DIR}/basic.off)

# dxf-stl test
add_test(dxf-stl ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --write-dxf ${CMAKE_BINARY_DIR}/basic.dxf )
add_test(dxf-stl-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/basic.dxf ${CMAKE_BINARY_DIR}/basic.dxf)

# vrml-stl test
add_test(vrml-stl ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/block.stl --write-vrml ${CMAKE_BINARY_DIR}/basic.vrml )
add_test(vrml-stl-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/basic.vrml ${CMAKE_BINARY_DIR}/basic.vrml)