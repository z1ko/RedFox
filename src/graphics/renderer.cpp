#include "renderer.hpp"

#include "../utils/io.hpp"

namespace RedFox
{
	MeshRenderer::MeshRenderer()
	{
		m_renderables = vector<IRenderable*>();

		Shader vtx(RedFox::ReadFileText("C:/Development/RedFox/RedFox/res/shaders/standard.vtx.glsl"), GL_VERTEX_SHADER);
		Shader frg(RedFox::ReadFileText("C:/Development/RedFox/RedFox/res/shaders/standard.frg.glsl"), GL_FRAGMENT_SHADER);

		m_technique = new StandardTechnique(vtx, frg);
	}

	void MeshRenderer::submit(IRenderable* _renderable)
	{
		m_renderables.push_back(_renderable);
	}

	void MeshRenderer::flush(const Camera& _camera)
	{
		m_technique->enable();
		m_technique->setCamera(_camera);

		for (auto renderable : m_renderables)
			renderable->render(m_technique);

		m_renderables.clear();
	}
}