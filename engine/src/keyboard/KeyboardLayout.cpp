#include <motor/keyboard/KeyboardLayout.h>
#include <motor/other/Logs.h>

KeyboardLayout::KeyboardLayout()
{
	for (int i = 0; i < 256; i++)
		keyStates[i] = false; 
}

bool KeyboardLayout::KeyIsPressed(const unsigned char keycode)
{
	return keyStates[keycode];
}

bool KeyboardLayout::KeyBufferIsEmpty()
{
	return keyBuffer.empty();
}

bool KeyboardLayout::CharBufferIsEmpty()
{
	return charBuffer.empty();
}

KeyboardEvent KeyboardLayout::ReadKey()
{
	//Logs::Debug("KeyboardLayout:ReadKey"); // Тестовый лог
	if (keyBuffer.empty()) 
	{
		return KeyboardEvent(); 
	}
	else
	{
		KeyboardEvent event = keyBuffer.front(); 
		keyBuffer.pop(); 
		return event; 
	}
}

unsigned char KeyboardLayout::ReadChar()
{
	//Logs::Debug("KeyboardLayout:Readchar"); // Тестовый лог
	if (charBuffer.empty()) 
	{
		return 0u; 
	}
	else
	{
		unsigned char event = charBuffer.front(); 
		charBuffer.pop(); 
		return event;
	}
}

void KeyboardLayout::OnKeyPressed(const unsigned char key)
{
	keyStates[key] = true;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyboardLayout::OnKeyReleased(const unsigned char key)
{
	keyStates[key] = false;
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyboardLayout::OnChar(const unsigned char key)
{
	charBuffer.push(key);
}

void KeyboardLayout::EnableAutoRepeatKeys()
{
	autoRepeatKeys = true;
}

void KeyboardLayout::DisableAutoRepeatKeys()
{
	autoRepeatKeys = false;
}

void KeyboardLayout::EnableAutoRepeatChars()
{
	autoRepeatChars = true;
}

void KeyboardLayout::DisableAutoRepeatChars()
{
	autoRepeatChars = false;
}

bool KeyboardLayout::IsKeysAutoRepeat()
{
	return autoRepeatKeys;
}

bool KeyboardLayout::IsCharsAutoRepeat()
{
	return autoRepeatChars;
}