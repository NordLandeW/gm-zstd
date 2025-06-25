
cmake_minimum_required (VERSION 3.8)

project ("gm-zstd")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")

add_library (gm_zstd SHARED "main.cpp")

include_directories ("${CMAKE_SOURCE_DIR}/lib/zstd")
file(GLOB ZSTD_SOURCES "${CMAKE_SOURCE_DIR}/lib/zstd/*/*.c")

add_library(zstd STATIC ${ZSTD_SOURCES})

target_link_libraries(gm_zstd PRIVATE zstd)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET gm_zstd PROPERTY CXX_STANDARD 20)
endif()