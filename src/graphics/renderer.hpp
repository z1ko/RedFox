#pragma once

#include "../common.hpp"

#include "technique.hpp"

namespace RedFox
{
	class IRenderable
	{
		public:
			virtual void render(StandardTechnique* _technique) const = 0;
	};

	//Classe dedicata a renderizzare le mesh
	class MeshRenderer
	{
		public:
			MeshRenderer();

			void submit(IRenderable* _renderable);
			void flush(const Camera& _camera);

		private:
			vector<IRenderable*> m_renderables;
			StandardTechnique*   m_technique;
	};
}