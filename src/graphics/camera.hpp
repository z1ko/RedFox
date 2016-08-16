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

			//Aggiorna posizione e rotazione camera
			void update();

			mat4 view() const;
			inline mat4 proj() const { return m_proj; }

		private:
			//Eventi
			void onKeyDown(u32 _key);
			void onKeyUp(u32 _key);

		public:
			vec3 position;

		private:
			float pitch;
			float distance;
			float angle;

		private:
			mat4 m_proj;
	};
}