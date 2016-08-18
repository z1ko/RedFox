#pragma once

#include "model.hpp"

namespace RedFox
{
	class Shape
	{
		public:
			Shape();
			Shape(const vector<Vertex>& _vertices, const vector<u32>& _indices);

			//Renderizza shape
			void render() const;

		public:
			static Shape Screen();
			static Shape Cube();

		private:
			u32 m_handle, m_count, m_vbo, m_ebo;
	};
}