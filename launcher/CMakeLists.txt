cmake_minimum_required (VERSION 3.8)

project ("launcher")

set (CMAKE_CXX_STANDARD 23)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

add_executable (${PROJECT_NAME} WIN32 src/main.cpp src/DeclareDPIAware.manifest resource.rc)
target_link_libraries(${PROJECT_NAME} engine)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_SOURCE_DIR}/engine/src/data/Fonts $<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts)
