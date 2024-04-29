#pragma once

#include "EngineUtil.h"

namespace HM
{
	enum Key
	{
		Arrow_Up = 0x26,
		Arrow_Down = 0x28,
		Arrow_Left = 0x25,
		Arrow_Right = 0x27,
		NP0 = 0x30,
		NP1 = 0x31,
		NP2 = 0x32,
		NP3 = 0x33,
		NP4 = 0x34,
		NP5 = 0x35,
		NP6 = 0x36,
		NP7 = 0x37,
		NP8 = 0x38,
		NP9 = 0x39,
	};

	enum KeyState
	{
		Up,
		Down,
		Pushing,
	};

	class InputManager
	{
	public:
		InputManager();

		map<Key, KeyState> _isDown;


		void ChangeState(const Key& key, const bool& isDown);

		bool isKeyDown(const Key& key);
		bool isKeyUp(const Key& key);
		bool isKey(const Key& key);

		void AddLog(const int& id);
		void Loging();
		std::vector<int> _Log;



	};
}