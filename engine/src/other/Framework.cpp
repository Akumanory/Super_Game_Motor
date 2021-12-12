﻿#include <motor/other/Framework.h>
#include <motor/other/Logs.h>
#include <motor/ECS/ComponentSystems.h>

using namespace DirectX;

extern lua_State* lState;
void LoadImguiBindings();

bool Framework::Initialize(HINSTANCE hInstance, std::string window_class, int width, int height)
{
	Logs::Debug("Framework constructor"); // Тестовый лог

	lState = lua_.lua_state();
    lua_.open_libraries(sol::lib::base, sol::lib::jit);
    LoadImguiBindings();

	//lua_["addCube"] = [this](float x, float y, float z) {
    //    gfx.addCube(x, y, z);
    //};
    lua_["addLightCube"] = [this](float x, float y, float z) {
        gfx.addLightCube(x, y, z);
    };

	gfx.setConsole(&consoleUI_, &showConsole_);

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
				gfx.cam_container.GetCurrentCamera().AdjustRotation(
					(float)me.GetPosY() * 0.005f,
					(float)me.GetPosX() * 0.005f,
					0
				);
			}
		}

		/*if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
		{
			std::string outmsg = "X: ";
			outmsg += std::to_string(me.GetPosX());
			outmsg += "Y: ";
			outmsg += std::to_string(me.GetPosY());
			outmsg += "\n";
			OutputDebugStringA(outmsg.c_str());
		}*/
		// Тестовый вывод
		/*std::string outmsg = "X: ";
		outmsg += std::to_string(me.GetPosX());
		outmsg += "Y: ";
		outmsg += std::to_string(me.GetPosY());
		outmsg += "\n";
		OutputDebugStringA(outmsg.c_str());*/
	}

	gfx.solar_system_scene.Update(delta);

	ComponentSystems::AjustRotation(gfx.entity1, DirectX::XMFLOAT3(0.0001f, 0.0f, 0.0002f), delta);


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


	/*gfx.model2.SetPosition(
		gfx.model1.GetPositionFloat3().x,
		gfx.model1.GetPositionFloat3().y,
		gfx.model1.GetPositionFloat3().z,
		false
	);*/
	/*float x = gfx.model1.GetLocalRotationFloat3().y;
	Logs::Debug("y rot: " + std::to_string(x));
	float y = gfx.model1.GetLocalPositionFloat3().y;
	Logs::Debug("y pos: " + std::to_string(y));*/

	//gfx.model1.AdjustRotation(0.0f, -0.001f * delta, 0.0f, true);
	//gfx.model1.SetPosition(2.0f, 0.0f, 0.0f, false);
	
	
	

	float camera_speed = 0.004f;
	
	if (display.keyboard.KeyIsPressed(VK_SHIFT))
	{
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

	if (display.keyboard.KeyIsPressed('C'))
	{
		XMVECTOR lightPosition = gfx.cam_container.GetCurrentCamera().GetPositionVector();
		lightPosition += gfx.cam_container.GetCurrentCamera().GetForwardVector();
		gfx.light.SetPosition(lightPosition);
		gfx.light.SetRotation(gfx.cam_container.GetCurrentCamera().GetRotationFloat3());
	}

	if (display.keyboard.KeyIsPressed('W'))
	{
		gfx.cam_container.GetCurrentCamera().AdjustPosition(gfx.cam_container.GetCurrentCamera().GetForwardVector() * camera_speed * delta);
	}
	if (display.keyboard.KeyIsPressed('S'))
	{
		gfx.cam_container.GetCurrentCamera().AdjustPosition(gfx.cam_container.GetCurrentCamera().GetBackwardVector() * camera_speed * delta);
	}
	if (display.keyboard.KeyIsPressed('A'))
	{
		gfx.cam_container.GetCurrentCamera().AdjustPosition(gfx.cam_container.GetCurrentCamera().GetLeftVector() * camera_speed * delta);
	}
	if (display.keyboard.KeyIsPressed('D'))
	{
		gfx.cam_container.GetCurrentCamera().AdjustPosition(gfx.cam_container.GetCurrentCamera().GetRightVector() * camera_speed * delta);
	}
	if (display.keyboard.KeyIsPressed(VK_SPACE))
	{
		gfx.cam_container.GetCurrentCamera().AdjustPosition(0.0f, camera_speed * delta, 0.0f );
	}
	if (display.keyboard.KeyIsPressed('Z'))
	{
		gfx.cam_container.GetCurrentCamera().AdjustPosition(0.0f, -camera_speed * delta, 0.0f);
	}
}

void Framework::RenderFrame()
{
	gfx.RenderFrame();
}
