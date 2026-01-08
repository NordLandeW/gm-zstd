cmake_minimum_required (VERSION 3.12)

project ("gm-zstd" C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")

add_library (gm_zstd SHARED "main.cpp")

include_directories ("${CMAKE_SOURCE_DIR}/lib/zstd")
include_directories ("${CMAKE_SOURCE_DIR}/lib/lz4")
file(GLOB ZSTD_SOURCES "${CMAKE_SOURCE_DIR}/lib/zstd/*/*.c" "${CMAKE_SOURCE_DIR}/lib/zstd/*/*.S")
file(GLOB LZ4_SOURCES "${CMAKE_SOURCE_DIR}/lib/lz4/*.c")

add_library(zstd STATIC ${ZSTD_SOURCES})
add_library(lz4 STATIC ${LZ4_SOURCES})

if(UNIX AND NOT APPLE)
  set_target_properties(zstd lz4 PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

target_link_libraries(gm_zstd PRIVATE zstd lz4)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET gm_zstd PROPERTY CXX_STANDARD 20)
endif()

set(GM_EXT_DIR "${CMAKE_SOURCE_DIR}/gamemaker-project/gm-zstd/extensions/__ext_gm_zstd")

install(
  DIRECTORY
  DESTINATION ${GM_EXT_DIR}
)

install(
  TARGETS gm_zstd
  RUNTIME DESTINATION ${GM_EXT_DIR}
)

add_custom_command(
  TARGET gm_zstd
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR} -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
  COMMENT "Copying library to GameMaker extension directory..."
)