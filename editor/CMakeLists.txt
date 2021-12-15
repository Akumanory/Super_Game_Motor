cmake_minimum_required (VERSION 3.8)

project ("editor")

set (CMAKE_CXX_STANDARD 23)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

set (HEADERS
    include/motor_editor/UI/EditorUI.hpp
)

set (SOURCES
)

add_executable (${PROJECT_NAME} WIN32 ${HEADERS} ${SOURCES}
    src/main.cpp src/DeclareDPIAware.manifest resource.rc)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} engine socket mime)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_SOURCE_DIR}/engine/src/data/Fonts $<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_SOURCE_DIR}/editor/src/scripts $<TARGET_FILE_DIR:${PROJECT_NAME}>/scripts
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    $<TARGET_FILE_DIR:socket>/socket $<TARGET_FILE_DIR:${PROJECT_NAME}>/scripts/utils
)
