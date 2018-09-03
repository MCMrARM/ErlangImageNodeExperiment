# Finds the erlang interface library.
# Once done this will define
#  ERLANG_FOUND - Set to true if erl interface has been found
#  ERLANG_INCLUDE_DIRS - The erl interface include directories
#  ERLANG_LIBRARIES - The libraries needed to use erl interface

find_package(Threads REQUIRED)

set(ERLANG_INCLUDE_PATHS /usr/lib/erlang/usr/include)
set(ERLANG_LIB_PATHS /usr/lib/erlang/usr/lib)

find_path(ERLANG_EI_INCLUDE_DIR NAMES ei.h
        PATHS ${ERLANG_INCLUDE_PATHS})
find_library(ERLANG_EI_LIBRARY NAMES ei
        PATHS ${ERLANG_LIB_PATHS})

find_path(ERLANG_ERL_INTERFACE_INCLUDE_DIR NAMES erl_interface.h
        PATHS ${ERLANG_INCLUDE_PATHS})
find_library(ERLANG_ERL_INTERFACE_LIBRARY NAMES erl_interface
        PATHS ${ERLANG_LIB_PATHS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Erlang DEFAULT_MSG
        ERLANG_EI_LIBRARY ERLANG_ERL_INTERFACE_LIBRARY ERLANG_EI_INCLUDE_DIR ERLANG_ERL_INTERFACE_INCLUDE_DIR)

mark_as_advanced(ERLANG_INCLUDE_DIR ERLANG_LIBRARY)

set(ERLANG_LIBRARIES Threads::Threads ${ERLANG_ERL_INTERFACE_LIBRARY} ${ERLANG_EI_LIBRARY})
set(ERLANG_INCLUDE_DIRS ${ERLANG_ERL_INTERFACE_INCLUDE_DIR} ${ERLANG_EI_INCLUDE_DIR})
set(ERLANG_COMPILE_DEFS _REENTRANT)