cmake_minimum_required(VERSION 3.16)
project(msglib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Keep the DLL next to the example on Windows so it can be run without PATH tweaks.
if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif()

add_library(msglib SHARED
    src/msglib.cpp
)

add_library(msglib::msglib ALIAS msglib)

target_include_directories(msglib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(msglib
    PRIVATE
        MSGLIB_EXPORTS
)

set_target_properties(msglib PROPERTIES
    OUTPUT_NAME msglib
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
)

if(WIN32)
    # MinGW defaults to libmsglib.dll; use msglib.dll on Windows.
    set_target_properties(msglib PROPERTIES PREFIX "")
endif()

if(NOT WIN32)
    set_target_properties(msglib PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION 1
    )
endif()

# MinGW: statically link libgcc/libstdc++/winpthread so no extra runtime DLLs are required.
# (--whole-archive is required; otherwise the linker still pulls libwinpthread-1.dll.)
function(msglib_mingw_static_runtime target)
    target_link_options(${target} PRIVATE
        -static-libgcc
        -static-libstdc++
        "LINKER:-Bstatic,--whole-archive"
        -lwinpthread
        "LINKER:--no-whole-archive,-Bdynamic"
    )
endfunction()

if(WIN32)
    target_link_libraries(msglib PRIVATE user32)
    if(MINGW)
        msglib_mingw_static_runtime(msglib)
    endif()
elseif(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET gtk+-3.0)
    target_link_libraries(msglib PRIVATE PkgConfig::GTK3)
else()
    message(FATAL_ERROR "msglib supports Windows and Linux only")
endif()

option(MSGLIB_BUILD_EXAMPLE "Build the MsgBox example program" OFF)
if(MSGLIB_BUILD_EXAMPLE)
    add_executable(msglib_example examples/example.cpp)
    target_link_libraries(msglib_example PRIVATE msglib)

    if(MINGW)
        msglib_mingw_static_runtime(msglib_example)
    endif()
endif()

include(GNUInstallDirs)
install(TARGETS msglib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES include/msglib.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
