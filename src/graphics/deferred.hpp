#pragma once

#include "../common.hpp"

namespace RedFox
{
	class GBuffer
	{
		public:
			GBuffer(u32 _width, u32 _height);

		private:
			u32 m_handle, m_rbo;
			u32 m_positionMap, m_normalMap, m_albedoMap;
	};
}