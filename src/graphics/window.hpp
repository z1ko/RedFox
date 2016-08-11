#pragma once

#include "../common.hpp"

namespace RedFox
{
	class Window
	{
		friend class Engine;

		public:
			Window(const string& _title, u32 _width, u32 _height);

		private:
			//Ottiene input utente e swappa buffers
			bool update();

		private:
			GLFWwindow* m_handle;
	};
}