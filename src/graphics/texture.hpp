#pragma once

#include "../common.hpp"
#include "../memory/pool.hpp"

namespace RedFox
{
	class Texture
	{
		public:
			Texture();
			Texture(u32 _handle);
			Texture(const str& _filename);

			//Usa questa texture nel contesto corrente
			void bind(u16 _slot) const;

		private:
			u32 m_handle;
	};
}