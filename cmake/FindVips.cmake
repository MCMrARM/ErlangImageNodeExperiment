# Finds the `vips` library.
# This file is released under the Public Domain.
# Once done this will define
#  VIPS_FOUND - Set to true if vips has been found
#  VIPS_INCLUDE_DIRS - The vips include directories
#  VIPS_LIBRARIES - The libraries needed to use vips

find_package(PkgConfig)
pkg_check_modules(PC_VIPS QUIET vips)

find_package(GLIB REQUIRED COMPONENTS gobject)

find_path(VIPS_INCLUDE_DIR
        NAMES vips/vips8
        PATH_SUFFIXES libvips
        HINTS ${PC_VIPS_INCLUDEDIR} ${PC_VIPS_INCLUDE_DIRS})
find_library(VIPS_LIBRARY
        NAMES vips libvips
        HINTS ${PC_VIPS_LIBDIR} ${PC_VIPS_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Vips DEFAULT_MSG
        VIPS_LIBRARY VIPS_INCLUDE_DIR)

mark_as_advanced(VIPS_INCLUDE_DIR VIPS_LIBRARY)

set(VIPS_LIBRARIES ${VIPS_LIBRARY} ${GLIB_LIBRARIES} ${GLIB_GOBJECT_LIBRARIES})
set(VIPS_INCLUDE_DIRS ${VIPS_INCLUDE_DIR} ${GLIB_INCLUDE_DIRS})