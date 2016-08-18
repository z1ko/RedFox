#include "camera.hpp"

#include "../core/events.hpp"

namespace RedFox
{
	Camera::Camera()
		: position(0,0,0), m_upAngle(0), m_rightAngle(0)
	{
	}

	Camera::Camera(const mat4 _projection)
		: position(0,0,0), m_proj(_projection), m_upAngle(0), m_rightAngle(0)
	{
		RFX_CONNECT(Camera, onMouseMove, Events::Input::MouseMove);
		RFX_CONNECT(Camera, onKeyAction, Events::Input::KeyAction);
	}

	Camera::~Camera()
	{
		RFX_DISCONNECT(Camera, Events::Input::MouseMove);
		RFX_DISCONNECT(Camera, Events::Input::KeyAction);
	}

	vec3 Camera::up() const
	{
		glm::vec4 result = glm::inverse(orientation()) * glm::vec4(0, 1, 0, 1);
		return vec3(result);
	}

	vec3 Camera::right() const
	{
		glm::vec4 result = glm::inverse(orientation()) * glm::vec4(1, 0, 0, 1);
		return vec3(result);
	}

	vec3 Camera::forward() const
	{
		glm::vec4 result = glm::inverse(orientation()) * glm::vec4(0, 0, -1, 1);
		return vec3(result);
	}

	mat4 Camera::view() const
	{
		mat4 translation;
		translation = glm::translate(translation, -position);

		return orientation() * translation;
	}

	mat4 Camera::orientation() const
	{
		mat4 orientation;
		orientation = glm::rotate(orientation, glm::radians(m_upAngle), vec3(1, 0, 0));
		orientation = glm::rotate(orientation, glm::radians(m_rightAngle), vec3(0, 1, 0));
		return orientation;
	}

	//Eventi
	void Camera::onMouseMove(f64 _x, f64 _y)
	{
		static float sensitivity = 0.1f;
		static vec2  last;

		static bool first = true;
		if(first)
		{
			last = vec2(_x, _y);
			first = false;
		}

		float xOffset = last.x - _x;
		float yOffset = last.y - _y;

		last = vec2(_x, _y);

		m_rightAngle -= xOffset * sensitivity;
		m_upAngle -= yOffset * sensitivity;

		//Normalizza valori
		m_rightAngle = fmodf(m_rightAngle, 360.0f);
		if (m_rightAngle < 0.0f) m_rightAngle += 360.0f;

		if (m_upAngle >  89.0f) m_upAngle =  89.0f;
		if (m_upAngle < -89.0f) m_upAngle = -89.0f;
	}

	void Camera::onKeyAction(u32 _key, u32 _action)
	{
		static float speed = 0.16f;

		if(_key == GLFW_KEY_S)
		{
			vec3 direction = forward();
			position -= direction * speed;
		}
		if (_key == GLFW_KEY_W)
		{
			vec3 direction = forward();
			position += direction * speed;
		}
		if (_key == GLFW_KEY_A)
		{
			vec3 direction = right();
			position -= direction * speed;
		}
		if (_key == GLFW_KEY_D)
		{
			vec3 direction = right();
			position += direction * speed;
		}
		if (_key == GLFW_KEY_Q)
		{
			vec3 direction = up();
			position += direction * speed;
		}
		if (_key == GLFW_KEY_E)
		{
			vec3 direction = up();
			position -= direction * speed;
		}
	}
}