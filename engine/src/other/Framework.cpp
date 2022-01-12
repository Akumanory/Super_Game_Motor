#include <motor/other/Framework.h>
#include <motor/other/Logs.h>
#include <motor/other/AssetManager.hpp>
#include <motor/ECS/ComponentSystems.h>

using namespace DirectX;

extern lua_State* lState;
void LoadImguiBindings();

bool Framework::Initialize(HINSTANCE hInstance, std::string window_class, int width, int height)
{
	Logs::Debug("Framework constructor"); // Тестовый лог

	tpool_ = std::make_unique<motor::task_system::thread_pool>();

	lState = lua_.lua_state();
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

	gfx.setWidgets(
      &consoleUI_, &showConsole_,
      &assetViewerUI_, &showAssetViewer_,
      &projectUI_, &showProject_, &project_);

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
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE and gfx.cam_container.CountCameras() != 0)
			{
                if (primary_camera) {
                    ComponentSystems::AjustRotation(
                        primary_camera,
                        XMFLOAT3(
                          (float)me.GetPosY() * 0.005f,
                          (float)me.GetPosX() * 0.005f,
                          0),
                        1);
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
	

	//gfx.solar_system_scene.Update(delta);

	//ComponentSystems::AjustRotation(gfx.entity1, DirectX::XMFLOAT3(0.0001f, 0.0f, 0.0002f), delta);


	//gfx.model1.AdjustRotation(0.0f, -0.001f * delta, 0.0f, false);
	/*gfx.model1.SetPosition(0.0f, 0.0f, 0.0f, true);*/

	//float radius = 2;

	//gfx.model1.SetPosition(0.0f, 0.0f, 0.0f, true);
	//gfx.model1.SetPosition(radius * cos(theta), radius * sin(theta), 0.0f, false, false);

	//theta += 0.001f * delta;

	//gfx.model1.SetPosition(2.0f, 0.0f, 0.0f, true, true);
	//gfx.model1.AdjustRotation(0.0f, -0.001f * delta, 0.0f, false, false);


	/*gfx.model1.RotateByRadiusAroundY(0.001f * delta, 3, true);
	gfx.model1.SetPosition(0.0f, 2.0f, 0.0f);*/

    // TODO: Тест для проверки симуляции потом убрать
    if (gfx.state == Graphics::States::Simulate) 
    {
        auto& transform_comp = gfx.entity1.GetComponent<TransformComponent>();
        ComponentSystems::AjustPosition(gfx.entity1, XMFLOAT3(0.0f, 0.002f, 0.003f), delta);
    }
	
	if (primary_camera) 
	{
		// TODO: без векторов направления будет кататся неправильно


		float camera_speed = 0.004f;

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
            }
	} 
	else {
            float camera_speed = 0.004f;

            if (display.keyboard.KeyIsPressed(VK_SHIFT)) {
                camera_speed = 0.03f;
            }

            // TODO Пока что вырезанно
            // Collision detection
            //if (display.keyboard.KeyIsPressed('F'))
            //{
            //       XMVECTOR boxPosition = gfx.cam_container.GetCurrentCamera().GetPositionVector();
            //       boxPosition += gfx.cam_container.GetCurrentCamera().GetForwardVector() * 4;
            //	gfx.gameObject.SetPosition(boxPosition);
            //	gfx.gameObject.SetRotation(gfx.cam_container.GetCurrentCamera().GetRotationFloat3());

            //	gfx.gameObject.BindOnIntersect(gfx.gameObject1);
            //	gfx.gameObject.BindOnIntersect(gfx.gameObject2);
            //	gfx.gameObject.BindOnIntersect(gfx.gameObject3);
            //	gfx.gameObject.BindOnIntersect(gfx.gameObject4);

            //	/*if (gfx.gameObject.CollisionIntersects(gfx.gameObject1.GetCollisionSphere()))
            //	{
            //		Logs::Debug("Intesected");
            //		gfx.gameObject1.SetRotation(gfx.gameObject.GetRotationVector());

            //
            //		gfx.gameObject1.BoundToIntersected(
            //			gfx.gameObject,
            //			XMFLOAT3(
            //				gfx.gameObject1.GetPositionFloat3().x - gfx.gameObject.GetPositionFloat3().x,
            //				gfx.gameObject1.GetPositionFloat3().y - gfx.gameObject.GetPositionFloat3().y,
            //				gfx.gameObject1.GetPositionFloat3().z - gfx.gameObject.GetPositionFloat3().z
            //			)
            //		);
            //	}*/
            //}

            if (gfx.cam_container.CountCameras() != 0) {

                //if (display.keyboard.KeyIsPressed('C')) {
                //    XMVECTOR lightPosition = gfx.cam_container.GetCurrentCamera().GetPositionVector();
                //    lightPosition += gfx.cam_container.GetCurrentCamera().GetForwardVector();
                //    gfx.light.SetPosition(lightPosition);
                //    gfx.light.SetRotation(gfx.cam_container.GetCurrentCamera().GetRotationFloat3());
                //}

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
