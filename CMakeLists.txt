cmake_minimum_required(VERSION 3.12)
project(ImageNode)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_STANDARD 11)

find_package(Erlang REQUIRED)
find_package(Vips REQUIRED)

add_executable(ImageNode main.cpp ErlangCNode.cpp ErlangCNode.h Log.h Log.cpp Application.cpp Application.h ErlangBufferReadHelper.h)
target_include_directories(ImageNode PUBLIC ${ERLANG_INCLUDE_DIRS} ${VIPS_INCLUDE_DIRS})
target_link_libraries(ImageNode PUBLIC ${ERLANG_LIBRARIES} ${VIPS_LIBRARIES})
target_compile_definitions(ImageNode PUBLIC ${ERLANG_COMPILE_DEFS})