cmake_minimum_required (VERSION 3.8)

project ("engine")

set (CMAKE_CXX_STANDARD 23)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

set(ASYNC_HEADERS
    include/Motor/_old/async/resource.hpp
)

set(CORE_SYSTEM_HEADERS
    include/Motor/_old/core_system/console.hpp
    include/Motor/_old/core_system/core.hpp
    include/Motor/_old/core_system/lua.hpp
)

set(ECS_HEADERS
    include/Motor/_old/ECS/Components.h
    include/Motor/_old/ECS/Scene.h
    include/Motor/_old/ECS/Factory.h
    include/Motor/_old/ECS/CompRealization.h
)

set(APPLICATION_HEADERS
    include/Motor/Engine/Core.h
    include/Motor/Engine/Engine.h
    include/Motor/Engine/Scene.h
    include/Motor/Engine/Window.h
)

set(ENGINE_HEADERS
    include/Motor/Engine/Core.h
    include/Motor/Engine/Engine.h
    include/Motor/Engine/Scene.h
    include/Motor/Engine/Window.h
)

set(RENDERER_HEADERS
    include/Motor/Renderer/Renderer.h
    include/Motor/Renderer/Prerequisites.h
    include/Motor/Renderer/Resource/Material.h
    include/Motor/Renderer/Resource/Mesh.h
    include/Motor/Renderer/Resource/MeshManager.h
    include/Motor/Renderer/Resource/Resource.h
    include/Motor/Renderer/Resource/ResourceManager.h
    include/Motor/Renderer/Resource/Texture.h
    include/Motor/Renderer/Resource/TextureManager.h
    include/Motor/Renderer/RHI/ConstantBuffer.h
    include/Motor/Renderer/RHI/DeviceContext.h
    include/Motor/Renderer/RHI/IndexBuffer.h
    include/Motor/Renderer/RHI/VertexBuffer.h
    include/Motor/Renderer/RHI/RendererSystem.h
    include/Motor/Renderer/RHI/GraphicsEngine.h
    include/Motor/Renderer/RHI/SwapChain.h
    include/Motor/Renderer/RHI/PixelShader.h
    include/Motor/Renderer/RHI/VertexShader.h
)

set(SCENES_HEADERS
    include/Motor/_old/graphics/scenes/SolarSystem.h
)

set(KEYBOARD_HEADERS
    include/Motor/_old/keyboard/KeyboardEvent.h
    include/Motor/_old/keyboard/KeyboardLayout.h
)

set(MOUSE_HEADERS
    include/Motor/_old/mouse/MouseEvent.h
    include/Motor/_old/mouse/MouseLayout.h
)

set(OTHER_HEADERS
    include/Motor/_old/other/COMException.h
    include/Motor/_old/other/DirectXIncludes.h
    include/Motor/_old/other/Display.h
    include/Motor/_old/other/Framework.h
    include/Motor/_old/other/Logs.h
    include/Motor/_old/other/StringConverter.h
    include/Motor/_old/other/StringHelper.h
    include/Motor/_old/other/Timer.h
    include/Motor/_old/other/MathHelper.h
)

set(STATE_SYSTEM_HEADERS
    include/Motor/_old/state_system/state.hpp
)

set(TASK_SYSTEM_HEADERS
    include/Motor/_old/task_system/constants.hpp
    include/Motor/_old/task_system/thread_pool.hpp
    include/Motor/_old/task_system/tickable.hpp
    include/Motor/_old/task_system/ticker.hpp
    include/Motor/_old/task_system/timeline.hpp
    include/Motor/_old/task_system/timer.hpp
)

set(UI_SYSTEM_HEADERS
    include/Motor/_old/ui_system/ui.hpp
    include/Motor/_old/ui_system/ui_console.hpp
)

set(HEADERS
    include/Motor/_old/task_system.hpp
    include/Motor/_old/utils.hpp
)

set(CORE_SYSTEM_SOURCES
    src/_old/core_system/console.cpp
    src/_old/core_system/core.cpp
)

set(ECS_SOURCES
    src/_old/ECS/Scene.cpp
    src/_old/ECS/Factory.cpp
    src/_old/ECS/CompRealization.cpp
)

set(ENGINE_SOURCES
    src/Engine/Engine.cpp
    src/Engine/GraphicsSystem.cpp
    src/Engine/GraphicsSystem.h
    src/Engine/Scene.cpp
    src/Engine/Window.cpp
    src/Engine/Win32/Win32Window.cpp
)

set(RENDERER_SOURCES
    src/Renderer/Resource/Material.cpp
    src/Renderer/Resource/Mesh.cpp
    src/Renderer/Resource/MeshManager.cpp
    src/Renderer/Resource/Resource.cpp
    src/Renderer/Resource/ResourceManager.cpp
    src/Renderer/Resource/Texture.cpp
    src/Renderer/Resource/TextureManager.cpp
    src/Renderer/RHI/ConstantBuffer.cpp
    src/Renderer/RHI/IndexBuffer.cpp
    src/Renderer/RHI/VertexBuffer.cpp
    src/Renderer/RHI/DeviceContext.cpp
    src/Renderer/RHI/RenderSystem.cpp
    src/Renderer/RHI/GraphicsEngine.cpp
    src/Renderer/RHI/PixelShader.cpp
    src/Renderer/RHI/VertexShader.cpp
    src/Renderer/RHI/SwapChain.cpp
    src/Renderer/RHI/VertexMesh.cpp
)

set(KEYBOARD_SOURCES
    src/_old/keyboard/KeyboardEvent.cpp
    src/_old/keyboard/KeyboardLayout.cpp
)

set(MOUSE_SOURCES
    src/_old/mouse/MouseEvent.cpp
    src/_old/mouse/MouseLayout.cpp
)

set(OTHER_SOURCES
    src/_old/other/Display.cpp
    src/_old/other/Framework.cpp
    src/_old/other/Logs.cpp
    src/_old/other/StringConverter.cpp
    src/_old/other/StringHelper.cpp
    src/_old/other/Timer.cpp
    src/_old/other/MathHelper.cpp
)

set(STATE_SYSTEM_SOURCES
    src/_old/state_system/state.cpp
)

set(TASK_SYSTEM_SOURCES
    src/_old/task_system/thread_pool.cpp
)

set(UI_SYSTEM_SOURCES
    src/_old/ui_system/ui_console.cpp
)

set(SOURCES
    src/_old/imgui_lua_bindings.cpp
    src/_old/utils.cpp
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

source_group ("include/Motor/_old/async"                     FILES ${ASYNC_HEADERS})
source_group ("include/Motor/_old/ECS"                       FILES ${ECS_HEADERS})
source_group ("include/Motor/Renderer"                       FILES ${RENDERER_HEADERS})
source_group ("include/Motor/_old/graphics/scenes"           FILES ${SCENES_HEADERS})
source_group ("include/Motor/_old/keyboard"                  FILES ${KEYBOARD_HEADERS})
source_group ("include/Motor/_old/mouse"                     FILES ${MOUSE_HEADERS})
source_group ("include/Motor/_old/other"                     FILES ${OTHER_HEADERS})
source_group ("include/Motor/_old/task_system"               FILES ${TASK_SYSTEM_HEADERS})
source_group ("include/Motor"                                FILES ${HEADERS})

source_group ("src/Renderer"                                 FILES ${RENDERER_SOURCES})
source_group ("src/_old/core_system"                         FILES ${CORE_SYSTEM_SOURCES})
source_group ("src/_old/ECS"                                 FILES ${ECS_SOURCES})
source_group ("src/_old/keyboard"                            FILES ${KEYBOARD_SOURCES})
source_group ("src/_old/mouse"                               FILES ${MOUSE_SOURCES})
source_group ("src/_old/other"                               FILES ${OTHER_SOURCES})
source_group ("src/_old/task_system"                         FILES ${TASK_SYSTEM_SOURCES})
source_group ("src"                                          FILES ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC ../lib/sol)


add_executable (old_main src/old_main.cpp src/DeclareDPIAware.manifest)
target_link_libraries(old_main ${PROJECT_NAME})
add_custom_command(TARGET old_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/engine/data/ $<TARGET_FILE_DIR:old_main>/Data)
add_custom_command(TARGET old_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/engine/src/shaders/pixel_shaders/ $<TARGET_FILE_DIR:old_main>)
add_custom_command(TARGET old_main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/engine/src/shaders/vertex_shaders/ $<TARGET_FILE_DIR:old_main>)


#enable_testing()

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
