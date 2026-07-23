# MinGW-w64 toolchain for cross-compiling msglib to 32-bit Windows from Linux.
#
# Usage:
#   cmake -S . -B build-win32 \
#     -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/mingw-w64-i686.cmake \
#     -DMSGLIB_BUILD_EXAMPLE=ON
#   cmake --build build-win32

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86)

set(MINGW_TRIPLE "i686-w64-mingw32" CACHE STRING "MinGW target triple")

set(CMAKE_C_COMPILER   "${MINGW_TRIPLE}-gcc")
set(CMAKE_CXX_COMPILER "${MINGW_TRIPLE}-g++")
set(CMAKE_RC_COMPILER  "${MINGW_TRIPLE}-windres")
set(CMAKE_AR           "${MINGW_TRIPLE}-ar"      CACHE FILEPATH "" FORCE)
set(CMAKE_RANLIB       "${MINGW_TRIPLE}-ranlib"  CACHE FILEPATH "" FORCE)
set(CMAKE_STRIP        "${MINGW_TRIPLE}-strip"   CACHE FILEPATH "" FORCE)

if(EXISTS "/usr/${MINGW_TRIPLE}")
    set(CMAKE_FIND_ROOT_PATH "/usr/${MINGW_TRIPLE}")
elseif(EXISTS "/usr/lib/gcc/${MINGW_TRIPLE}")
    set(CMAKE_FIND_ROOT_PATH "/usr")
endif()

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
