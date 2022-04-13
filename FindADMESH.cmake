# - Find ADMESH library
# This module defines
#  ADMESH_INCLUDE_DIR, where to find tiff.h, etc.
#  ADMESH_LIBRARIES, libraries to link against to use ADMESH.
#  ADMESH_FOUND, If false, do not try to use ADMESH.
# also defined, but not for general use are
#  ADMESH_LIBRARY, where to find the ADMESH library.

FIND_PATH(ADMESH_INCLUDE_DIR stl.h HINTS "/usr/include/admesh")
FIND_LIBRARY(ADMESH_LIBRARY NAMES admesh)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ADMESH  DEFAULT_MSG  ADMESH_LIBRARY  ADMESH_INCLUDE_DIR)

IF(ADMESH_FOUND)
  SET( ADMESH_LIBRARIES ${ADMESH_LIBRARY} )
ENDIF(ADMESH_FOUND)

MARK_AS_ADVANCED(ADMESH_INCLUDE_DIR ADMESH_LIBRARY)
