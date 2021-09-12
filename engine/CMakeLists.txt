cmake_minimum_required (VERSION 3.8)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

set(HEADERS
    include/motor/utils.hpp
)

set(SOURCES
    src/main.cpp
    src/utils.cpp
)

set(LIBRARIES
    d3d11
    dxgi
    d3dcompiler
    dxguid
)

add_executable (${PROJECT_NAME} WIN32 ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/std:c++latest")
endif()