#include "camera.hpp"

#include "../core/events.hpp"

namespace RedFox
{
	Camera::Camera()
		: position(0,0,0)
	{
	}

	Camera::Camera(const mat4 _projection)
		: position(0,0,0), m_proj(_projection)
	{
		RFX_CONNECT(Camera, onKeyDown, Events::Input::KeyDown);
		RFX_CONNECT(Camera, onKeyUp,   Events::Input::KeyUp);
	}

	Camera::~Camera()
	{
		RFX_DISCONNECT(Camera, Events::Input::KeyDown);
		RFX_DISCONNECT(Camera, Events::Input::KeyUp);
	}

	//Aggiorna posizione e rotazione camera
	void Camera::update()
	{
	}

	mat4 Camera::view() const
	{
		mat4 translation;
		translation = glm::translate(translation, -position);

		return translation;
	}

	//Eventi
	void Camera::onKeyDown(u32 _key)
	{
		//switch (_key)
		//{
		//	case GLFW_KEY_A: 
		//		this->position.x -= 1.0f;
		//		break;

		//	case GLFW_KEY_D: 
		//		this->position.x += 1.0f;
		//		break;
		//}
	}

	void Camera::onKeyUp(u32 _key)
	{

	}
}