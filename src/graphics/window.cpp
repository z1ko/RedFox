#include "window.hpp"

namespace RedFox
{
	Window::Window(const str& _title, u32 _width, u32 _height)
	{
		//Carica dll glfw
		glfwInit();

		m_handle = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_handle);

		//glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();

		return !glfwWindowShouldClose(m_handle);
	}
}