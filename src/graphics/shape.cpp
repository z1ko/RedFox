#include "shape.hpp"

namespace RedFox
{
	Shape::Shape()
		: m_handle(0)
	{
	}

	Shape::Shape(const vector<Vertex>& _vertices, const vector<u32>& _indices)
		: m_count(_indices.size())
	{
		glGenVertexArrays(1, &m_handle);
		glBindVertexArray(m_handle);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * _indices.size(), _indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	//Renderizza shape
	void Shape::render() const
	{
		glBindVertexArray(m_handle);
		glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, nullptr);
	}

	Shape Shape::Screen()
	{
		static const vector<Vertex> vertices =
		{
			{ {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
			{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
			{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
			{ {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } }
		};
		static const vector<u32> indices =
		{
			0,1,2, 2,3,0
		};

		return Shape(vertices, indices);
	}
}