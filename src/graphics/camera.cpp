#include "camera.hpp"

namespace RedFox
{
	Camera::Camera(const mat4 _projection)
		: m_proj(_projection)
	{
	}

	mat4 Camera::view() const
	{
		mat4 translation;
		translation = glm::translate(translation, -position);

		return translation;
	}
}