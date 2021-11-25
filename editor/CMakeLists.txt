cmake_minimum_required (VERSION 3.8)

project ("editor")

set (CMAKE_CXX_STANDARD 23)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

set(EDITOR_FILES
    Editor.cpp
    Editor.h
)

add_executable (${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} engine)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/engine/data/Fonts/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts)
