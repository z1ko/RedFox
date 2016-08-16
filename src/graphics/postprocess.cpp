#include "postprocess.hpp"

#include "../utils/io.hpp"

namespace RedFox
{
	//Crea l'effetto usando un fragment shader
	PostProcess::PostProcess(const str& _filename)
	{
		Shader vtx(RedFox::ReadFileText("C:/Development/RedFox/RedFox/res/shaders/final.vtx.glsl"), GL_VERTEX_SHADER);
		Shader frg(RedFox::ReadFileText(_filename), GL_FRAGMENT_SHADER);

		m_technique.initialize().attach(vtx).attach(frg).link();
	}

	void PostProcess::enable() const
	{
		m_technique.enable();
	}
}