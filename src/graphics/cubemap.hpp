#pragma once

#include "texture.hpp"

namespace RedFox
{
	class CubeMap
	{
		public:
			CubeMap();
			CubeMap(const array<str, 6>& _filenames);

			void bind() const;

		private:
			u32 m_handle;
	};
};