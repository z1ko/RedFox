#pragma once

#include "../common.hpp"

namespace RedFox
{
	class Camera
	{
		public:
			Camera();
			Camera(const mat4 _projection);
			virtual ~Camera();

			mat4 view() const;
			inline mat4 proj() const { return m_proj; }

			vec3 up() const;
			vec3 right() const;
			vec3 forward() const;

		private:
			mat4 orientation() const;

		private:
			//Eventi
			void onMouseMove(f64 _x, f64 _y);
			void onKeyAction(u32 _key, u32 _action);

		public:
			vec3 position;
		
			float m_upAngle;
			float m_rightAngle;

		private:
			mat4 m_proj;
	};
}