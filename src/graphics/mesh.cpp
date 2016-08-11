#include "mesh.hpp"

namespace RedFox
{
	//Costruttori per comodità
	Vertex::Vertex(vec3 _position)
		: position(_position)
	{
	}

	void Vertex::describe()
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		//glEnableVertexAttribArray(1));
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));

		//glEnableVertexAttribArray(2));
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texuv)));
	}

	//=====================================================================================================================================

	//Oggetto nullo
	Shape::Shape()
		: m_handle(0)
	{
	}

	//Crea forma tramite vertici e indici
	Shape::Shape(const vector<Vertex>& _vertices, const vector<u32>& _indices)
		: m_vtxBuffer(_vertices, GL_ARRAY_BUFFER, GL_STATIC_DRAW), m_idxBuffer(_indices, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
	{
		glGenVertexArrays(1, &m_handle);
		glBindVertexArray(m_handle);

		m_vtxBuffer.bind();
		Vertex::describe();

		m_idxBuffer.bind();

		glBindVertexArray(0);
	}

	//Distrugge oggetto
	Shape::~Shape()
	{
		glDeleteVertexArrays(1, &m_handle);
	}

	//Usa questo oggetto nel contesto corrente
	void Shape::bind() const
	{
		glBindVertexArray(m_handle);
	}

	//Renderizza questa forma
	void Shape::render() const
	{
		glBindVertexArray(m_handle);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}