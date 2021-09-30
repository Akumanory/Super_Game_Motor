cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 23)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

set(ASYNC_HEADEERS
    include/motor/async/resource.hpp
)

set(TASK_SYSTEM_HEADERS
    include/motor/task_system/clocker.hpp
    include/motor/task_system/constants.hpp
    include/motor/task_system/thread_pool.hpp
    include/motor/task_system/timer.hpp
)

set(HEADERS
    include/motor/task_system.hpp
    include/motor/utils.hpp
)

set(TASK_SYSTEM_SOURCES
    src/task_system/clocker.cpp
    src/task_system/thread_pool.cpp
    src/task_system/timer.cpp
)

set(SOURCES
    src/utils.cpp
)

set(LIBRARIES
    d3d11
    dxgi
    d3dcompiler
    dxguid
)

add_library (${PROJECT_NAME}_lib
    ${HEADERS}
    ${ASYNC_HEADEERS}
    ${TASK_SYSTEM_HEADERS}

    ${SOURCES}
    ${TASK_SYSTEM_SOURCES}
)

source_group ("include/motor"                           FILES ${HEADERS})
source_group ("include/motor/async"                     FILES ${ASYNC_HEADERS})
source_group ("include/motor/task_system"               FILES ${TASK_SYSTEM_HEADERS})

source_group ("src"                                     FILES ${SOURCES})
source_group ("src/task_system"                         FILES ${TASK_SYSTEM_SOURCES})

add_executable (${PROJECT_NAME} WIN32 src/main.cpp)

target_link_libraries(${PROJECT_NAME}_lib ${LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)

target_include_directories(${PROJECT_NAME}_lib PUBLIC include)

enable_testing()

set(TESTING_NAME run_unit_tests)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_FILES
    tests/debug_test.cpp
    tests/shared_resource.cpp
)

add_executable(${TESTING_NAME} ${TEST_FILES})

target_link_libraries(${TESTING_NAME}
    gtest
    gtest_main

    ${PROJECT_NAME}_lib
)

include(GoogleTest)
gtest_discover_tests(${TESTING_NAME})
