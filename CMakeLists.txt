cmake_minimum_required(VERSION 3.16)
project(pileup-events CXX)

option(ENABLE_DEBUG_FLAGS "Enable extra debug compile options" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(pileup-events
  src/main.cpp
)

# --- Dependencies: prefer pkg-config, otherwise search common macOS/Linux prefixes ---
find_package(PkgConfig QUIET)

# Common prefixes (Linux, macOS Intel/Apple Silicon, MacPorts)
set(_COMMON_HINT_PREFIXES
  /usr /usr/local /opt/local /opt/homebrew
)

# ---------------- cxxopts (header-only) ----------------
set(CXXOPTS_TARGET "")
if(PkgConfig_FOUND)
  pkg_check_modules(CXXOPTS QUIET IMPORTED_TARGET cxxopts)
  if(CXXOPTS_FOUND)
    set(CXXOPTS_TARGET PkgConfig::CXXOPTS)
  endif()
endif()

if(NOT CXXOPTS_TARGET)
  find_path(CXXOPTS_INCLUDE_DIR
    NAMES cxxopts.hpp
    HINTS ${_COMMON_HINT_PREFIXES}
    PATH_SUFFIXES include
  )
  if(NOT CXXOPTS_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find cxxopts.hpp (tried standard prefixes).")
  endif()
  add_library(cxxopts INTERFACE)
  add_library(cxxopts::cxxopts ALIAS cxxopts)
  target_include_directories(cxxopts INTERFACE "${CXXOPTS_INCLUDE_DIR}")
  set(CXXOPTS_TARGET cxxopts::cxxopts)
endif()

# ---------------- htslib (library + headers) ----------------
set(HTSLIB_TARGET "")
if(PkgConfig_FOUND)
  pkg_check_modules(HTSLIB QUIET IMPORTED_TARGET htslib)
  if(HTSLIB_FOUND)
    set(HTSLIB_TARGET PkgConfig::HTSLIB)
  endif()
endif()

if(NOT HTSLIB_TARGET)
  find_path(HTSLIB_INCLUDE_DIR
    NAMES htslib/hts.h
    HINTS ${_COMMON_HINT_PREFIXES}
    PATH_SUFFIXES include
  )
  find_library(HTSLIB_LIBRARY
    NAMES hts htslib
    HINTS ${_COMMON_HINT_PREFIXES}
    PATH_SUFFIXES lib lib64
  )
  if(NOT HTSLIB_INCLUDE_DIR OR NOT HTSLIB_LIBRARY)
    message(FATAL_ERROR "Could not find htslib (headers+lib) in standard prefixes.")
  endif()
  add_library(htslib::hts UNKNOWN IMPORTED)
  set_target_properties(htslib::hts PROPERTIES
    IMPORTED_LOCATION "${HTSLIB_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${HTSLIB_INCLUDE_DIR}"
  )
  set(HTSLIB_TARGET htslib::hts)
endif()

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

target_link_libraries(pileup-events PRIVATE
  ${CXXOPTS_TARGET}
  ${HTSLIB_TARGET}
)

