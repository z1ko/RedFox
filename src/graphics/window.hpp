#pragma once

#include "../common.hpp"

namespace RedFox
{
	//Preset per opengl
	class Preset
	{
		public:
			virtual void use() const = 0;
	};

	class StandardPreset
	{
	};

	class WirePreset
	{
	};

	//=======================================================================================

	class Window
	{
		friend class Engine;

		public:
			Window(const str& _title, u32 _width, u32 _height);

		private:
			//Ottiene input utente e swappa buffers
			bool update();

		private:
			GLFWwindow* m_handle;
	};
}