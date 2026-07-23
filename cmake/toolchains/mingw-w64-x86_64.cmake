# MinGW-w64 toolchain for cross-compiling msglib to 64-bit Windows from Linux.
#
# Usage:
#   cmake -S . -B build-win64 \
#     -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/mingw-w64-x86_64.cmake \
#     -DMSGLIB_BUILD_EXAMPLE=ON
#   cmake --build build-win64

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(MINGW_TRIPLE "x86_64-w64-mingw32" CACHE STRING "MinGW target triple")

set(CMAKE_C_COMPILER   "${MINGW_TRIPLE}-gcc")
set(CMAKE_CXX_COMPILER "${MINGW_TRIPLE}-g++")
set(CMAKE_RC_COMPILER  "${MINGW_TRIPLE}-windres")
set(CMAKE_AR           "${MINGW_TRIPLE}-ar"      CACHE FILEPATH "" FORCE)
set(CMAKE_RANLIB       "${MINGW_TRIPLE}-ranlib"  CACHE FILEPATH "" FORCE)
set(CMAKE_STRIP        "${MINGW_TRIPLE}-strip"   CACHE FILEPATH "" FORCE)

# Prefer the MinGW sysroot when resolving libraries and headers.
if(EXISTS "/usr/${MINGW_TRIPLE}")
    set(CMAKE_FIND_ROOT_PATH "/usr/${MINGW_TRIPLE}")
elseif(EXISTS "/usr/lib/gcc/${MINGW_TRIPLE}")
    set(CMAKE_FIND_ROOT_PATH "/usr")
endif()

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
