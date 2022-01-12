cmake_minimum_required (VERSION 3.8)

project ("ImGuizmo-cmake")

set(CMAKE_CXX_STANDARD 23)

set(SOURCES
    ../ImGuizmo/GraphEditor.cpp
    ../ImGuizmo/ImCurveEdit.cpp
    ../ImGuizmo/ImGradient.cpp
    ../ImGuizmo/ImGuizmo.cpp
    ../ImGuizmo/ImSequencer.cpp
)

add_library (${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} imgui-cmake)
target_include_directories(${PROJECT_NAME} PUBLIC ../ImGuizmo)