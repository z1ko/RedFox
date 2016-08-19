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

		glfwSetWindowPos(m_handle, 50, 50);
		glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//Callbacks per l'input
		glfwSetKeyCallback(m_handle, keyCallback);
		glfwSetCursorPosCallback(m_handle, mouseMovementCallback);

		//Carica estensioni opengl
		glewInit();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glViewport(0, 0, _width, _height);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	//Ottiene input utente e swappa buffers
	bool Window::update()
	{
		glfwSwapBuffers(m_handle);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();

		//Resetta posizione mouse
		glfwSetCursorPos(m_handle, 0, 0);

		return !glfwWindowShouldClose(m_handle);
	}

	void Window::keyCallback(GLFWwindow* _handle, int _key, int _scancode, int _action, int _mods)
	{
		Events::Input::KeyAction(_key, _action);
	}

	void Window::mouseMovementCallback(GLFWwindow* _handle, double _x, double _y)
	{
		Events::Input::MouseMove(_x, _y);
	}

	void Window::mouseButtonsCallback(GLFWwindow* _handle, int _button, int _action, int _mods)
	{
		 Events::Input::BtnAction(_button, _action);
	}
}