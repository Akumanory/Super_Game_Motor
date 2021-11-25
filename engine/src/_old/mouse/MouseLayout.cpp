#include <motor/mouse/MouseLayout.h>

void MouseLayout::OnLeftPressed(int x, int y)
{
	leftIsDown = true;
	MouseEvent me(MouseEvent::EventType::LeftPress, x, y);
	eventBuffer.push(me);
}

void MouseLayout::OnLeftReleased(int x, int y)
{
	leftIsDown = false;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::LeftRelease, x, y));
}

void MouseLayout::OnRightPressed(int x, int y)
{
	rightIsDown = true;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RightPress, x, y));
}

void MouseLayout::OnRightReleased(int x, int y)
{
	rightIsDown = false;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RightRelease, x, y));
}

void MouseLayout::OnMiddlePressed(int x, int y)
{
	mbuttonDown = true;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::MiddlePress, x, y));
}

void MouseLayout::OnMiddleReleased(int x, int y)
{
	mbuttonDown = false;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::MiddleRelease, x, y));
}

void MouseLayout::OnWheelUp(int x, int y)
{
	eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void MouseLayout::OnWheelDown(int x, int y)
{
	eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void MouseLayout::OnMouseMove(int x, int y)
{
	x = x;
	y = y;
	eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void MouseLayout::OnMouseMoveRaw(int x, int y)
{
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
}

bool MouseLayout::IsLeftDown()
{
	return leftIsDown;
}

bool MouseLayout::IsMiddleDown()
{
	return mbuttonDown;
}

bool MouseLayout::IsRightDown()
{
	return rightIsDown;
}

int MouseLayout::GetPosX()
{
	return x;
}

int MouseLayout::GetPosY()
{
	return y;
}

MousePoint MouseLayout::GetPos()
{
	return{ x, y };
}

bool MouseLayout::EventBufferIsEmpty()
{
	return eventBuffer.empty();
}

MouseEvent MouseLayout::ReadEvent()
{
	if (eventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent event = eventBuffer.front(); //Get first event from buffer
		eventBuffer.pop(); //Remove first event from buffer
		return event;
	}
}