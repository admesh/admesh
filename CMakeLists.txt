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


#foreach(testfile "block" "conveyor" "hourglass")
foreach(testfile "block" "conveyor" "hourglass")
  MESSAGE("Test ${testfile}")

  # basic
  add_test(${testfile}-basic ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl -a ${CMAKE_BINARY_DIR}/basic.stl )
  add_test(${testfile}-basic-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/basic.stl ${CMAKE_BINARY_DIR}/basic.stl)

  # x-rotate-30
  add_test(${testfile}-x-rotate-30 ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --x-rotate=30  -a ${CMAKE_BINARY_DIR}/x-rotate-30.stl )
  add_test(${testfile}-x-rotate-30-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/x-rotate-30.stl ${CMAKE_BINARY_DIR}/x-rotate-30.stl)

  # y-rotate-30
  add_test(${testfile}-y-rotate-30 ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --y-rotate=30  -a ${CMAKE_BINARY_DIR}/y-rotate-30.stl )
  add_test(${testfile}-y-rotate-30-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/y-rotate-30.stl ${CMAKE_BINARY_DIR}/y-rotate-30.stl)

  # z-rotate-30
  add_test(${testfile}-z-rotate-30 ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --z-rotate=30  -a ${CMAKE_BINARY_DIR}/z-rotate-30.stl )
  add_test(${testfile}-z-rotate-30-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/z-rotate-30.stl ${CMAKE_BINARY_DIR}/z-rotate-30.stl)

  # x-y-z-rotate-30
  add_test(${testfile}-x-y-z-rotate-30 ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --x-rotate=30 --y-rotate=30 --z-rotate=30 -a ${CMAKE_BINARY_DIR}/x-y-z-rotate-30.stl )
  add_test(${testfile}-x-y-z-rotate-30-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/x-y-z-rotate-30.stl ${CMAKE_BINARY_DIR}/x-y-z-rotate-30.stl)

  # xy-mirror
  add_test(${testfile}-xy-mirror ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --xy-mirror -a ${CMAKE_BINARY_DIR}/xy-mirror.stl)
  add_test(${testfile}-xy-mirror-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/xy-mirror.stl ${CMAKE_BINARY_DIR}/xy-mirror.stl)

  # yz-mirror
  add_test(${testfile}-yz-mirror ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --yz-mirror -a ${CMAKE_BINARY_DIR}/yz-mirror.stl)
  add_test(${testfile}-yz-mirror-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/yz-mirror.stl ${CMAKE_BINARY_DIR}/yz-mirror.stl)

  # xz-mirror
  add_test(${testfile}-xz-mirror ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --xz-mirror -a ${CMAKE_BINARY_DIR}/xz-mirror.stl)
  add_test(${testfile}-xz-mirror-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/xz-mirror.stl ${CMAKE_BINARY_DIR}/xz-mirror.stl)

  # scale
  add_test(${testfile}-scale ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --scale 3 -a ${CMAKE_BINARY_DIR}/scale.stl)
  add_test(${testfile}-scale-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/scale.stl ${CMAKE_BINARY_DIR}/scale.stl)

  # scale-xyz
  add_test(${testfile}-scale-xyz ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --scale-xyz=2,3,4 -a ${CMAKE_BINARY_DIR}/scale-xyz.stl)
  add_test(${testfile}-scale-xyz-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/scale-xyz.stl ${CMAKE_BINARY_DIR}/scale-xyz.stl)

  # translate
  add_test(${testfile}-translate ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --translate=2,3,4 -a ${CMAKE_BINARY_DIR}/translate.stl)
  add_test(${testfile}-translate-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/translate.stl ${CMAKE_BINARY_DIR}/translate.stl)

  # translate-rel
  add_test(${testfile}-translate-rel ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --translate-rel=2,3,4 -a ${CMAKE_BINARY_DIR}/translate-rel.stl)
  add_test(${testfile}-translate-rel-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/translate-rel.stl ${CMAKE_BINARY_DIR}/translate-rel.stl)

  # stretch
  add_test(${testfile}-stretch ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --stretch=::22,::3,::5 -a ${CMAKE_BINARY_DIR}/stretch.stl)
  add_test(${testfile}-stretch-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/stretch.stl ${CMAKE_BINARY_DIR}/stretch.stl)

  # merge
  add_test(${testfile}-merge ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --merge=${CMAKE_SOURCE_DIR}/test/${testfile}/stretch.stl -a ${CMAKE_BINARY_DIR}/merge.stl)
  add_test(${testfile}-merge-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/merge.stl ${CMAKE_BINARY_DIR}/merge.stl)

  # nearby
  add_test(${testfile}-nearby ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/test/${testfile}/nearby-bad.stl -n -t 0.1 -a ${CMAKE_BINARY_DIR}/nearby.stl)
  add_test(${testfile}-nearby-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/nearby-good.stl ${CMAKE_BINARY_DIR}/nearby.stl)

  # remove-unconnected
  add_test(${testfile}-remove-unconnected ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/test/${testfile}/remove-unconnected-bad.stl --remove-unconnected -a ${CMAKE_BINARY_DIR}/remove-unconnected.stl)
  add_test(${testfile}-remove-unconnected-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/remove-unconnected-good.stl ${CMAKE_BINARY_DIR}/remove-unconnected.stl)

  # fill-holes
  add_test(${testfile}-fill-holes ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/test/${testfile}/fill-holes-bad.stl --fill-holes -a ${CMAKE_BINARY_DIR}/fill-holes.stl)
  add_test(${testfile}-fill-holes-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/fill-holes-good.stl ${CMAKE_BINARY_DIR}/fill-holes.stl)

  # normal-directions
  add_test(${testfile}-normal-directions ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/test/${testfile}/normal-directions-bad.stl --normal-directions -a ${CMAKE_BINARY_DIR}/normal-directions.stl)
  add_test(${testfile}-normal-directions-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/normal-directions-good.stl ${CMAKE_BINARY_DIR}/normal-directions.stl)

  # binary-stl test
  #add_test(${testfile}-binary-stl ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl -b ${CMAKE_BINARY_DIR}/binary.stl )
  #add_test(${testfile}-binary-stl-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/binary.stl ${CMAKE_BINARY_DIR}/binary.stl)

  # off-stl test
  add_test(${testfile}-off-stl ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --write-off ${CMAKE_BINARY_DIR}/basic.off )
  add_test(${testfile}-off-stl-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/basic.off ${CMAKE_BINARY_DIR}/basic.off)

  # dxf-stl test
  add_test(${testfile}-dxf-stl ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --write-dxf ${CMAKE_BINARY_DIR}/basic.dxf )
  add_test(${testfile}-dxf-stl-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/basic.dxf ${CMAKE_BINARY_DIR}/basic.dxf)

  # vrml-stl test
  add_test(${testfile}-vrml-stl ${CMAKE_BINARY_DIR}/admesh ${CMAKE_SOURCE_DIR}/examples/${testfile}.stl --write-vrml ${CMAKE_BINARY_DIR}/basic.vrml )
  add_test(${testfile}-vrml-stl-compare ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/${testfile}/basic.vrml ${CMAKE_BINARY_DIR}/basic.vrml)

endforeach()
