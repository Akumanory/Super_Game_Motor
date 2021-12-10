cmake_minimum_required (VERSION 3.8)

project ("engine")

set (CMAKE_CXX_STANDARD 23)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

set(ASYNC_HEADERS
    include/motor/async/resource.hpp
)

set(CORE_SYSTEM_HEADERS
    include/motor/core_system/console.hpp
    include/motor/core_system/core.hpp
    include/motor/core_system/lua.hpp
)

set(ECS_HEADERS
    include/motor/ECS/Components.h
    include/motor/ECS/Scene.h
    include/motor/ECS/Factory.h
    include/motor/ECS/ComponentSystems.h
    include/motor/ECS/Entity.h
    include/motor/ECS/SceneHierarchy.h
)

set(GRAPHICS_HEADERS
    include/motor/graphics/Camera.h
    include/motor/graphics/Color.h
    include/motor/graphics/ConstantBuffer.h
    include/motor/graphics/ConstantBufferTypes.h
    include/motor/graphics/GameObject.h
    include/motor/graphics/Graphics.h
    include/motor/graphics/IndexBuffer.h
    include/motor/graphics/Light.h
    include/motor/graphics/Mesh.h
    include/motor/graphics/Model.h
    include/motor/graphics/RenderableGameObject.h
    include/motor/graphics/Shaders.h
    include/motor/graphics/Texture.h
    include/motor/graphics/UpdatedModel.h
    include/motor/graphics/Vertex.h
    include/motor/graphics/VertexBuffer.h
    include/motor/graphics/CameraContainer.h
    include/motor/graphics/DebugDraw.h
    include/motor/graphics/MeshForComponents.h
)

set(SCENES_HEADERS
    include/motor/graphics/scenes/SolarSystem.h
)

set(KEYBOARD_HEADERS
    include/motor/keyboard/KeyboardEvent.h
    include/motor/keyboard/KeyboardLayout.h
)

set(MOUSE_HEADERS
    include/motor/mouse/MouseEvent.h
    include/motor/mouse/MouseLayout.h
)

set(OTHER_HEADERS
    include/motor/other/COMException.h
    include/motor/other/DirectXIncludes.h
    include/motor/other/Display.h
    include/motor/other/Framework.h
    include/motor/other/Logs.h
    include/motor/other/StringConverter.h
    include/motor/other/StringHelper.h
    include/motor/other/Timer.h
    include/motor/other/MathHelper.h
    include/motor/other/ModelLoader.h
)

set(STATE_SYSTEM_HEADERS
    include/motor/state_system/state.hpp
)

set(TASK_SYSTEM_HEADERS
    include/motor/task_system/constants.hpp
    include/motor/task_system/thread_pool.hpp
    include/motor/task_system/tickable.hpp
    include/motor/task_system/ticker.hpp
    include/motor/task_system/timeline.hpp
    include/motor/task_system/timer.hpp
)

set(UI_SYSTEM_HEADERS
    include/motor/ui_system/ui.hpp
    include/motor/ui_system/ui_console.hpp
)

set(HEADERS
    include/motor/task_system.hpp
    include/motor/utils.hpp
)

set(CORE_SYSTEM_SOURCES
    src/core_system/console.cpp
    src/core_system/core.cpp
)

set(ECS_SOURCES
    src/ECS/Scene.cpp
    src/ECS/Factory.cpp
    src/ECS/ComponentSystems.cpp
    src/ECS/Entity.cpp
    src/ECS/SceneHierarchy.cpp
)

set(GRAPHICS_SOURCES
    src/graphics/Camera.cpp
    src/graphics/Color.cpp
    src/graphics/GameObject.cpp
    src/graphics/Graphics.cpp
    src/graphics/Light.cpp
    src/graphics/Mesh.cpp
    src/graphics/Model.cpp
    src/graphics/RenderableGameObject.cpp
    src/graphics/Shaders.cpp
    src/graphics/Texture.cpp
    src/graphics/UpdatedModel.cpp
    src/graphics/CameraContainer.cpp
    src/graphics/DebugDraw.cpp
    src/graphics/MeshForComponents.cpp
)

set(KEYBOARD_SOURCES
    src/keyboard/KeyboardEvent.cpp
    src/keyboard/KeyboardLayout.cpp
)

set(MOUSE_SOURCES
    src/mouse/MouseEvent.cpp
    src/mouse/MouseLayout.cpp
)

set(OTHER_SOURCES
    src/other/Display.cpp
    src/other/Framework.cpp
    src/other/Logs.cpp
    src/other/StringConverter.cpp
    src/other/StringHelper.cpp
    src/other/Timer.cpp
    src/other/MathHelper.cpp
    src/other/ModelLoader.cpp
)

set(STATE_SYSTEM_SOURCES
    src/state_system/state.cpp
)

set(TASK_SYSTEM_SOURCES
    src/task_system/thread_pool.cpp
)

set(UI_SYSTEM_SOURCES
    src/ui_system/ui_console.cpp
)

set(SOURCES
    src/imgui_lua_bindings.cpp
    src/utils.cpp
)

set(LIBRARIES
    d3d11
    dxgi
    d3dcompiler
    dxguid
    imgui-cmake
    assimp
    DirectXTK
    liblua-static
)

add_library (${PROJECT_NAME}
    ${ASYNC_HEADERS}
    ${CORE_SYSTEM_HEADERS}
    ${ECS_HEADERS}
    ${GRAPHICS_HEADERS}
    ${SCENES_HEADERS}
    ${KEYBOARD_HEADERS}
    ${MOUSE_HEADERS}
    ${OTHER_HEADERS}
    ${STATE_SYSTEM_HEADERS}
    ${TASK_SYSTEM_HEADERS}
    ${HEADERS}

    ${CORE_SYSTEM_SOURCES}
    ${ECS_SOURCES}
    ${GRAPHICS_SOURCES}
    ${KEYBOARD_SOURCES}
    ${MOUSE_SOURCES}
    ${OTHER_SOURCES}
    ${STATE_SYSTEM_SOURCES}
    ${TASK_SYSTEM_SOURCES}
    ${UI_SYSTEM_SOURCES}
    ${SOURCES}

    src/rapidjson.natvis
)

source_group ("include/motor/async"                     FILES ${ASYNC_HEADERS})
source_group ("include/motor/ECS"                       FILES ${ECS_HEADERS})
source_group ("include/motor/graphics"                  FILES ${GRAPHICS_HEADERS})
source_group ("include/motor/graphics/scenes"           FILES ${SCENES_HEADERS})
source_group ("include/motor/keyboard"                  FILES ${KEYBOARD_HEADERS})
source_group ("include/motor/mouse"                     FILES ${MOUSE_HEADERS})
source_group ("include/motor/other"                     FILES ${OTHER_HEADERS})
source_group ("include/motor/task_system"               FILES ${TASK_SYSTEM_HEADERS})
source_group ("include/motor"                           FILES ${HEADERS})

source_group ("src/core_system"                         FILES ${CORE_SYSTEM_SOURCES})
source_group ("src/ECS"                                 FILES ${ECS_SOURCES})
source_group ("src/graphics"                            FILES ${GRAPHICS_SOURCES})
source_group ("src/keyboard"                            FILES ${KEYBOARD_SOURCES})
source_group ("src/mouse"                               FILES ${MOUSE_SOURCES})
source_group ("src/other"                               FILES ${OTHER_SOURCES})
source_group ("src/task_system"                         FILES ${TASK_SYSTEM_SOURCES})
source_group ("src"                                     FILES ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC ../lib/sol)


add_executable (old_main src/old_main.cpp src/DeclareDPIAware.manifest)
target_link_libraries(old_main ${PROJECT_NAME})
add_custom_command(TARGET old_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/engine/src/data/ $<TARGET_FILE_DIR:old_main>/Data)
add_custom_command(TARGET old_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/engine/src/shaders/pixel_shaders/ $<TARGET_FILE_DIR:old_main>)
add_custom_command(TARGET old_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/engine/src/shaders/vertex_shaders/ $<TARGET_FILE_DIR:old_main>)


enable_testing()

set(TESTING_NAME run_unit_tests)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_FILES
    tests/task_system/ticker.cpp
    tests/debug_test.cpp
    tests/shared_resource.cpp
)

add_executable(${TESTING_NAME} ${TEST_FILES})

target_link_libraries(${TESTING_NAME}
    gtest
    gtest_main

    ${PROJECT_NAME}
)

include(GoogleTest)
gtest_discover_tests(${TESTING_NAME})
