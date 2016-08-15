#pragma once

#include "../common.hpp"

namespace RedFox
{
	class Camera
	{
		public:
			Camera();
			Camera(const mat4 _projection);

			mat4 view() const;
			inline mat4 proj() const { return m_proj; }

		public:
			vec3 position;

		private:
			mat4 m_proj;
	};
}