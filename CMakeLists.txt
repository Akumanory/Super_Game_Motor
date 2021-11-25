cmake_minimum_required (VERSION 3.8)

project ("motor")

set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL
        CMAKE_C_FLAGS_RELWITHDEBINFO
        )
foreach(CompilerFlag ${CompilerFlags})
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    set(${CompilerFlag} "${${CompilerFlag}}" CACHE STRING "msvc compiler flags" FORCE)
    message("MSVC flags: ${CompilerFlag}:${${CompilerFlag}}")
endforeach()

add_subdirectory ("engine")
add_subdirectory ("editor")
#add_subdirectory ("launcher")
add_subdirectory ("lib/googletest")
add_subdirectory ("lib/imgui-cmake")
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "Build your own zlib")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build package with shared libraries.")
add_subdirectory ("lib/assimp")
set(BUILD_XAUDIO_WIN10 ON CACHE BOOL "Build for XAudio 2.9")
set(BUILD_XAUDIO_WIN8 OFF CACHE BOOL "Build for XAudio 2.8")
add_subdirectory ("lib/DirectXTK")
add_subdirectory ("lib/freetype")
add_subdirectory ("lib/LuaJIT-cmake")
