#include <motor/other/Framework.h>
#include <motor/other/Logs.h>
#include <motor/ECS/ComponentSystems.h>
#include <DirectXCollision.h>

using namespace DirectX;

//extern lua_State* lState;
//void LoadImguiBindings();

bool Framework::Initialize(HINSTANCE hInstance, std::string window_class, int width, int height)
{
	Logs::Debug("Framework constructor"); // Тестовый лог

	tpool_ = std::make_unique<motor::task_system::thread_pool>();

	/*lState = lua_.lua_state();
    lua_.open_libraries(
      sol::lib::base,
      sol::lib::package,
      sol::lib::math,
      sol::lib::string,
      sol::lib::table,
      sol::lib::debug,
      sol::lib::jit);
    LoadImguiBindings();

	lua_["addCube"] = [this](float x, float y, float z) {
        gfx.addCube(x, y, z);
    };
    lua_["addLightCube"] = [this](float x, float y, float z) {
        gfx.addLightCube(x, y, z);
    };
    lua_["inferno"] = [this] {
        for (int i = 0; i <= 20; ++i) {
            for (int j = 0; j <= 20; ++j) {
                for (int k = 0; k <= 20; ++k) {
                    gfx.addCube(5 + i * 3, 5 + j * 3, 5 + k * 3);
                }
            }
        }
    };

	gfx.setConsole(&consoleUI_, &showConsole_);

    gfx.lua_state = lState;*/

	timer.Start();
	if (!display.Initialize(hInstance, window_class, width, height))
	{
		Logs::Error("Display is not initialized");
		return false;
	}

	if (!gfx.Initialize(display.GetHWND(), width, height))
	{
		Logs::Error("Graphics is not initialized");
		return false;
	}

	return true;
}

bool Framework::ProcessMessages()
{
	return display.ProcessMessages();
}

void Framework::Update()
{
	auto delta = static_cast<float>(timer.GetMilisecondsElapsed());
	timer.Restart();

    Entity primary_camera = gfx.test_entt_scene.GetPrimaryCamera();


	while (!display.keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = display.keyboard.ReadChar();

		// Тестовый вывод
		/*std::string outmsg = "Char: ";
		outmsg += std::to_string(ch);
		outmsg += "\n";
		OutputDebugStringA(outmsg.c_str());*/
	}

	while (!display.keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = display.keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();

		// Тестовый вывод
		/*std::string outmsg = "Key: ";
		outmsg += std::to_string(keycode);
		outmsg += "\n";
		OutputDebugStringA(outmsg.c_str());*/
	}

	while (!display.mouse.EventBufferIsEmpty())
	{
		MouseEvent me = display.mouse.ReadEvent();
		if (display.mouse.IsRightDown())
		{
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
			{
                if (primary_camera) {
                    /*ComponentSystems::AjustRotation(
                        primary_camera,
                        XMFLOAT3(
                          (float)me.GetPosY() * 0.005f,
                          (float)me.GetPosX() * 0.005f,
                          0),
                        1);*/
                }
                else 
                {
                    gfx.cam_container.GetCurrentCamera().AdjustRotation(
                      (float)me.GetPosY() * 0.005f,
                      (float)me.GetPosX() * 0.005f,
                      0);
                }
			}
		}
	}
	

    // TODO: Тест для проверки симуляции потом убрать
    if (gfx.state == Graphics::States::Simulate) 
    {   
        //auto& transform_comp = gfx.entity1.GetComponent<TransformComponent>();
        //ComponentSystems::AjustPosition(gfx.entity1, XMFLOAT3(0.0f, 0.002f, 0.003f), delta);
        auto script_comps = gfx.test_entt_scene.GetEntitysByComponent<ScriptComponent>();

        for (auto& i : script_comps) 
        {

            bool remove_entity = false;
            Entity entity_to_remove;

            auto& script_comp = i.GetComponent<ScriptComponent>();

            script_comp.lua_state.open_libraries(
              sol::lib::base,
              sol::lib::package,
              sol::lib::math,
              sol::lib::string,
              sol::lib::table,
              sol::lib::debug,
              sol::lib::jit);

            script_comp.lua_state["this_entity"] = [=]() mutable {
                return i;
            };

            script_comp.lua_state["delete_entity"] = [&](Entity entity_t) {
                remove_entity = true;
                entity_to_remove = entity_t;
            };

            script_comp.lua_state["get_direction_xmvector"] = [&](Entity entity, const char dir) {
                if (dir == 'U') {
                    return entity.GetComponent<TransformComponent>().GetUpVector();
                }
                if (dir == 'F') {
                    return entity.GetComponent<TransformComponent>().GetForwardVector();
                }
                if (dir == 'R') {
                    return entity.GetComponent<TransformComponent>().GetRightVector();
                }
            };


            script_comp.lua_state["set_global_pos_1"] = [&](XMFLOAT3 pos) {
                _global_pos_1 = pos;
            };

            script_comp.lua_state["set_global_int_1"] = [&](int number) {
                _global_int = number;
            };


            script_comp.lua_state["get_global_pos_1"] = [&]() {
                return _global_pos_1;
            };

            script_comp.lua_state["get_global_int_1"] = [&]() {
                return _global_int;
            };

            script_comp.lua_state["set_script"] = [](Entity entity, const std::string script) {
                if (!entity.HasComponent<ScriptComponent>()) 
                {
                    entity.AddComponent<ScriptComponent>();
                }
                entity.GetComponent<ScriptComponent>().script = script;
            };

            script_comp.lua_state["get_world_pos"] = [&](Entity entity) {
                return entity.GetComponent<TransformComponent>().world_position;
            };

            script_comp.lua_state["set_world_pos"] = [](Entity entity, float x, float y, float z) {
                ComponentSystems::SetPosition(entity, XMFLOAT3(x, y, z));
            };

            script_comp.lua_state["set_world_pos_XMFLOAT"] = [](Entity entity, XMFLOAT3 pos, float x, float y, float z) {
                ComponentSystems::SetPosition(entity, XMFLOAT3(pos.x + x, pos.y + y, pos.z + z));
            };

            script_comp.lua_state["ajust_position_self"] = [=](float x, float y, float z, float time) mutable {
                ComponentSystems::AjustPosition(i, XMFLOAT3(x, y, z), time);
            };

            script_comp.lua_state["ajust_rotation_self"] = [=](float x, float y, float z, float time) mutable {
                ComponentSystems::AjustRotation(i, XMFLOAT3(x, y, z), time);
            };

            script_comp.lua_state["set_rotation_self"] = [=](float x, float y, float z, float time) mutable {
                ComponentSystems::SetRotation(i, XMFLOAT3(x, y, z));
            };

            script_comp.lua_state["ajust_position_XMVECTOR"] = [](Entity entity, XMVECTOR pos, float speed, float time) {
                ComponentSystems::AjustPosition(entity, pos * speed, time);
            };

            script_comp.lua_state["ajust_position_XMFLOAT"] = [](Entity entity, XMFLOAT3 pos, float speed, float time) {
                ComponentSystems::AjustPosition(entity, pos, time * speed);
            };

            script_comp.lua_state["ajust_position"] = [](Entity entity, float x, float y, float z, float time) {
                ComponentSystems::AjustPosition(entity, XMFLOAT3(x, y, z), time);
            };

            script_comp.lua_state["create_entity"] = [&](const std::string name) {
                return gfx.test_entt_scene.CreateEntity(name);
            };

            script_comp.lua_state["set_model_to_entity"] = [&](Entity entity, int model_number) {
                if (!entity.HasComponent<MeshComponent>()) {
                    entity.AddComponent<MeshComponent>();
                }

                if (model_number >= 0) {
                    ComponentSystems::SetModel(entity, gfx.test_entt_scene.GetModel(model_number));
                }
            };

            script_comp.lua_state["is_key_pressed"] = [&](const char key) {
                if (display.keyboard.KeyIsPressed(key)) {
                    return true;
                }
                return false;
            };


            script_comp.lua_state.script(script_comp.script);
            
            if (!script_comp.is_initialized) 
            {
                script_comp.lua_state["on_start"]();

                script_comp.is_initialized = true;
            }

            auto mesh_comps = gfx.test_entt_scene.GetEntitysByComponent<MeshComponent>();

            //bool collided = false;

            for (auto& j : mesh_comps) {
                if (i != j) {
                    if (i.GetComponent<MeshComponent>().transformed_bounding_box.Intersects(j.GetComponent<MeshComponent>().transformed_bounding_box)) {
                        script_comp.lua_state["on_collide"]();
                    }
                }
            } 

            script_comp.lua_state["on_update"](delta);
        
            if (remove_entity) 
            {
                gfx.test_entt_scene.DestroyEntity(entity_to_remove);
            }
        }

        /*auto mesh_comps = gfx.test_entt_scene.GetEntitysByTwoComponents<MeshComponent, ScriptComponent>();

        for (auto&  i : mesh_comps) 
        {
            for (auto& j : mesh_comps) {

                if (i != j) {
                    if (i.GetComponent<MeshComponent>().transformed_bounding_box.Intersects(j.GetComponent<MeshComponent>().transformed_bounding_box)) {
                        if (i.HasComponent<ScriptComponent>()) {
                            gfx.test_entt_scene.lua_state["on_collide"]();
                        }
                    }
                }
            } 
        }*/
    }


	
	if (primary_camera) 
	{
		// TODO: без векторов направления будет кататся неправильно

		/*float camera_speed = 0.004f;

		auto& transform = primary_camera.GetComponent<TransformComponent>();

            if (display.keyboard.KeyIsPressed(VK_SHIFT)) {
                camera_speed = 0.03f;
            }

            if (display.keyboard.KeyIsPressed('W')) {
                ComponentSystems::AjustPosition(primary_camera, transform.GetForwardVector(), camera_speed * delta);
            }
            if (display.keyboard.KeyIsPressed('S')) {
                ComponentSystems::AjustPosition(primary_camera, -transform.GetForwardVector(), camera_speed * delta);
            }
            if (display.keyboard.KeyIsPressed('A')) {
                ComponentSystems::AjustPosition(primary_camera, -transform.GetRightVector(), camera_speed * delta);
            }
            if (display.keyboard.KeyIsPressed('D')) {
                ComponentSystems::AjustPosition(primary_camera, transform.GetRightVector(), camera_speed * delta);
            }
            if (display.keyboard.KeyIsPressed(VK_SPACE)) {
                ComponentSystems::AjustPosition(primary_camera, XMFLOAT3(0, 1.0f, 0), camera_speed * delta);
            }
            if (display.keyboard.KeyIsPressed('Z')) {
                ComponentSystems::AjustPosition(primary_camera, XMFLOAT3(0, -1.0f, 0), camera_speed * delta);
            }*/
	} 
	else 
	{
            float camera_speed = 0.004f;

            if (display.keyboard.KeyIsPressed(VK_SHIFT)) {
                camera_speed = 0.03f;
            }

            /*if (display.keyboard.KeyIsPressed('C')) {
                XMVECTOR lightPosition = gfx.cam_container.GetCurrentCamera().GetPositionVector();
                lightPosition += gfx.cam_container.GetCurrentCamera().GetForwardVector();
                gfx.light.SetPosition(lightPosition);
                gfx.light.SetRotation(gfx.cam_container.GetCurrentCamera().GetRotationFloat3());
            }*/

            if (display.keyboard.KeyIsPressed('W')) {
                gfx.cam_container.GetCurrentCamera().AdjustPosition(gfx.cam_container.GetCurrentCamera().GetForwardVector() * camera_speed * delta);
            }
            if (display.keyboard.KeyIsPressed('S')) {
                gfx.cam_container.GetCurrentCamera().AdjustPosition(gfx.cam_container.GetCurrentCamera().GetBackwardVector() * camera_speed * delta);
            }
            if (display.keyboard.KeyIsPressed('A')) {
                gfx.cam_container.GetCurrentCamera().AdjustPosition(gfx.cam_container.GetCurrentCamera().GetLeftVector() * camera_speed * delta);
            }
            if (display.keyboard.KeyIsPressed('D')) {
                gfx.cam_container.GetCurrentCamera().AdjustPosition(gfx.cam_container.GetCurrentCamera().GetRightVector() * camera_speed * delta);
            }
            if (display.keyboard.KeyIsPressed(VK_SPACE)) {
                gfx.cam_container.GetCurrentCamera().AdjustPosition(0.0f, camera_speed * delta, 0.0f);
            }
            if (display.keyboard.KeyIsPressed('Z')) {
                gfx.cam_container.GetCurrentCamera().AdjustPosition(0.0f, -camera_speed * delta, 0.0f);
            }
	}

	


    //=========================================
    // Updating systems
    //=========================================

	using namespace motor;

	std::vector<std::packaged_task<void(std::stop_token)>> frame_tasks;
    frame_tasks.reserve(4);
    frame_tasks.emplace_back([](std::stop_token) {
        //utils::debug_write::info("Updating scripts...");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    });
    frame_tasks.emplace_back([](std::stop_token) {
        //utils::debug_write::info("Updating physics...");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    });
    frame_tasks.emplace_back([](std::stop_token) {
        //utils::debug_write::info("Updating sound...");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    });
    frame_tasks.emplace_back([](std::stop_token) {
        //utils::debug_write::info("Updating AI...");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    });

    std::vector<std::future<void>> frame_futures;
    frame_futures.reserve(frame_tasks.size());
    for (auto&& task : frame_tasks) {
        frame_futures.emplace_back(task.get_future());
    }

    for (auto&& task : frame_tasks) {
        motor::ThreadPool().submit_frame([&task](std::stop_token token) { task(token); });
    }

    for (auto&& future : frame_futures) {
        future.wait();
    }
}

void Framework::RenderFrame()
{
	gfx.RenderFrame();
}
