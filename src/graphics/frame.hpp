#pragma once

#include "../common.hpp"
#include "../graphics/texture.hpp"

namespace RedFox
{
	class Frame
	{
		public:
			Frame();
			//Crea il backbuffer
			Frame(u32 _width, u32 _height);

			//Usa questo backbuffer come target del rendering
			void enable()  const;

			//Usa il backbuffer standard
			void disable() const;

			//Usa la texture risultante nel contesto attuale
			void bind() const;

		private:
			u32 m_handle, m_rbo, m_result;
	};
}