
#include "InputManager.h"

InputManager::InputManager()
{
	_isDown[Key::Arrow_Up] = Up;
	_isDown[Key::Arrow_Down] = Up;
	_isDown[Key::Arrow_Left] = Up;
	_isDown[Key::Arrow_Right] = Up;
	_isDown[Key::NP0] = Up;
	_isDown[Key::NP1] = Up;
	_isDown[Key::NP2] = Up;
	_isDown[Key::NP3] = Up;
	_isDown[Key::NP4] = Up;
	_isDown[Key::NP5] = Up;
	_isDown[Key::NP6] = Up;
	_isDown[Key::NP7] = Up;
	_isDown[Key::NP8] = Up;
	_isDown[Key::NP9] = Up;
}

void InputManager::ChangeState(const Key& key, const bool& isDown)
{
	if (_isDown.find(key) != _isDown.end())
	{
		switch (_isDown[key])
		{
		case KeyState::Up:
			if (isDown)
			{
				_isDown[key] = Down;
			}
			break;
		case KeyState::Down:
			_isDown[key] = isDown == true ? Pushing : Up;
			break;
		case KeyState::Pushing:
			if (isDown == false)
			{
				_isDown[key] = Up;
			}
			break;
		}
	}
}

bool InputManager::isKey(const Key& key)
{
	return _isDown[key] == Pushing || _isDown[key] == Down;
}

bool InputManager::isKeyDown(const Key& key)
{
	return _isDown[key] == Down;
}
bool InputManager::isKeyUp(const Key& key)
{
	return _isDown[key] == Up;
}

void InputManager::AddLog(const int& id)
{
	_Log.push_back(id);
}

void InputManager::Loging()
{
	int idx = 0;
	for (auto const& i : _Log)
	{
		idx++;
	}
}