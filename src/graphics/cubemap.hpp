#pragma once

#include "texture.hpp"

namespace RedFox
{
	class CubeMap
	{
		public:
			CubeMap();
			CubeMap(const array<str, 6>& _filenames);

			//Usa questa cubemap nel contesto corrente
			void bind() const;

		private:
			u32 m_handle;
	};
};