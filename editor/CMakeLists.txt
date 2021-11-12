cmake_minimum_required (VERSION 3.8)

project ("editor")

set (CMAKE_CXX_STANDARD 23)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

set(SCRIPTS
    src/scripts/script.lua
    src/scripts/dkjson.lua
    src/scripts/vscode-debuggee.lua
)

add_executable (${PROJECT_NAME} WIN32 src/main.cpp src/DeclareDPIAware.manifest resource.rc ${SCRIPTS})
target_link_libraries(${PROJECT_NAME} engine socket mime)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/engine/src/data/Fonts/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:socket>/socket/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/scripts/utils
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/editor/src/scripts/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/scripts
)