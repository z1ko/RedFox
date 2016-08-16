#include "window.hpp"

#include "../core/events.hpp"

namespace RedFox
{
	Window::Window(const str& _title, u32 _width, u32 _height)
	{
		//Carica dll glfw
		glfwInit();

		m_handle = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_handle);

		//glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//Callbacks per l'input
		glfwSetKeyCallback(m_handle, keyCallback);

		//Carica estensioni opengl
		glewInit();

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glViewport(0, 0, _width, _height);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//Ottiene input utente e swappa buffers
	bool Window::update()
	{
		glfwSwapBuffers(m_handle);
		glfwPollEvents();

		return !glfwWindowShouldClose(m_handle);
	}

	void Window::keyCallback(GLFWwindow* _handle, int _key, int _scancode, int _action, int _mods)
	{
		switch (_action)
		{
			case GLFW_PRESS:
				Events::Input::KeyDown((u32)_key);
				break;
			case GLFW_RELEASE:
				Events::Input::KeyUp((u32)_key);
				break;
		}
	}

	void Window::mouseMovementCallback(GLFWwindow* _handle, double _X, double _y)
	{
	}

	void Window::mouseButtonsCallback(GLFWwindow* _handle, int _button, int _action, int _mods)
	{
	}
}