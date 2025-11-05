cmake_minimum_required(VERSION 3.16)
project(pileup-events CXX)

option(ENABLE_DEBUG_FLAGS "Enable extra debug compile options" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(HTSLIB REQUIRED IMPORTED_TARGET htslib)
pkg_check_modules(CXXOPTS REQUIRED IMPORTED_TARGET cxxopts)
# pkg_check_modules(CATCH REQUIRED catch2-with-main>=3.9)

add_executable(pileup-events
  src/main.cpp
)

target_compile_options(pileup-events PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wsign-conversion
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
)

if(ENABLE_DEBUG_FLAGS)
  target_compile_options(pileup-events PRIVATE
    -fsanitize=address,undefined
    -fno-omit-frame-pointer
    -O0 -g
  )
  target_link_options(pileup-events PRIVATE
    -fsanitize=address,undefined
  )
endif()

target_include_directories(pileup-events PRIVATE
  src
  PkgConfig::CXXOPTS
  PkgConfig::HTSLIB
)

target_link_libraries(pileup-events PRIVATE
  PkgConfig::CXXOPTS
  PkgConfig::HTSLIB
)

