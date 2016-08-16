#pragma once

#include "../common.hpp"

namespace RedFox
{

	class Window
	{
		friend class Engine;

		public:
			Window(const str& _title, u32 _width, u32 _height);

		private:
			//Ottiene input utente e swappa buffers
			bool update();

		private:
			static void keyCallback(GLFWwindow* _handle, int _key, int _scancode, int _action, int _mods);
			static void mouseMovementCallback(GLFWwindow* _handle, double _X, double _y);
			static void mouseButtonsCallback(GLFWwindow* _handle, int _button, int _action, int _mods);

		private:
			GLFWwindow* m_handle;
	};
}