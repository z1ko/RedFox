#include "technique.hpp"

#include "../utils/io.hpp"
#include "../utils/string.hpp"

namespace RedFox
{
	//Crea shader nullo
	Shader::Shader()
		: m_handle(0)
	{
	}

	//Crea shader da source
	Shader::Shader(const str& _source, u32 _target)
	{
		m_handle = glCreateShader(_target);

		str resolvedSource = resolve(_source);
		const char* source = resolvedSource.c_str();

		glShaderSource(m_handle, 1, &source, nullptr);
		glCompileShader(m_handle);

		int status;
		glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE)
		{
			char log[516];
			glGetShaderInfoLog(m_handle, 516, nullptr, log);
			RFX_ERROR("Compilazione shader fallita");
		}
	}

	//Risolve #include
	str Shader::resolve(const str& _source)
	{
		//stringstream stream;

		//vector<str> lines = SplitLines(_source);
		//for(u32 i = 0; i < lines.size(); i++)
		//{
		//	//Se è una direttiva
		//	if(lines[i][0] == '#')
		//	{
		//		str directive = lines[i].substr(1, lines[i].find_first_of(' ') - 1);
		//		if(directive == "include")
		//		{
		//			str other = lines[i].substr(lines[i].find_first_of('"') + 1, lines[i].find_last_of('"'));
		//			str source = ReadFileText("C:/Development/RedFox/RedFox/res/shaders/" + other.substr(0, other.size() - 1));

		//			stream << source.substr(0, source.size() - 2);
		//		}
		//		if(directive == "version")
		//			stream << lines[i] << '\n';
		//	}
		//	else stream << lines[i];
		//}

		//return stream.str();
		return _source;
	}

	//=====================================================================================================================================

	//Tecnica nulla
	Technique::Technique()
		: m_handle(0)
	{
	}

	//Crea handle per la tecnica
	Technique& Technique::initialize()
	{
		m_handle = glCreateProgram();
		return *this;
	}

	//Aggiunge uno shader alla pipeline temporanea
	Technique& Technique::attach(const Shader& _shader)
	{
		glAttachShader(m_handle, _shader.m_handle);
		return *this;
	}

	//Toglie uno shader dalla pipeline temporanea
	Technique& Technique::detach(const Shader& _shader)
	{
		glDetachShader(m_handle, _shader.m_handle);
		return *this;
	}

	//Finalizza la tecnica usando la pipeline temporanea
	void Technique::link()
	{
		glLinkProgram(m_handle);

		int status;
		glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
		if (status != GL_TRUE)
		{
			char log[256];
			glGetProgramInfoLog(m_handle, 256, nullptr, log);
			RFX_ERROR("Creazione programma fallita");
		}
	}

	//Usa questa tecnica di rendering
	void Technique::enable() const
	{
		glUseProgram(m_handle);
	}

	void Technique::setSamplers(const vector<Sampler>& _samplers)
	{
		for (const auto& sampler : _samplers)
		{
			u32 loc = location(sampler.name);
			glUniform1i(loc, sampler.slot);
		}
	}

	u32 Technique::location(const str& _name)
	{
		//Se la uniform non è gia stata registringingata allora viene aggiunta
		if(m_locations.find(_name) == m_locations.end())
		{
			u32 loc = glGetUniformLocation(m_handle, _name.c_str());
			m_locations[_name] = loc;
		}
		return m_locations.at(_name);
	}

	template<>
	void Technique::setUniform<u32>(const str& _name, const u32& _value) {
		glUniform1i(location(_name), _value);
	}

	template<>
	void Technique::setUniform<vec3>(const str& _name, const vec3& _value) {
		glUniform3f(location(_name), _value.x, _value.y, _value.z);
	}

	template<>
	void Technique::setUniform<mat4>(const str& _name, const mat4& _value) {
		glUniformMatrix4fv(location(_name), 1, GL_FALSE, glm::value_ptr(_value));
	}

	void Technique::setCamera(const Camera& _camera)
	{
		setUniform("camera.position", _camera.position);

		setUniform("camera.view", _camera.view());
		setUniform("camera.proj", _camera.proj());
	}

	void Technique::setTransform(const Transform& _transform)
	{
		setUniform("instance.model", _transform.model());
	}

	//=====================================================================================================================================

	StandardTechnique::StandardTechnique()
	{
		Shader vtx(RedFox::ReadFileText("C:/Development/RedFox/RedFox/res/shaders/standard.vtx.glsl"), GL_VERTEX_SHADER);
		Shader frg(RedFox::ReadFileText("C:/Development/RedFox/RedFox/res/shaders/standard.frg.glsl"), GL_FRAGMENT_SHADER);

		initialize().attach(vtx).attach(frg).link();
	}

	//=====================================================================================================================================

	FrameTechnique::FrameTechnique()
	{
		Shader vtx(RedFox::ReadFileText("C:/Development/RedFox/RedFox/res/shaders/final.vtx.glsl"), GL_VERTEX_SHADER);
		Shader frg(RedFox::ReadFileText("C:/Development/RedFox/RedFox/res/shaders/final.frg.glsl"), GL_FRAGMENT_SHADER);

		initialize().attach(vtx).attach(frg).link();
	}

	//=====================================================================================================================================

	SkyBoxTechnique::SkyBoxTechnique()
	{
		Shader vtx(RedFox::ReadFileText("C:/Development/RedFox/RedFox/res/shaders/skybox.vtx.glsl"), GL_VERTEX_SHADER);
		Shader frg(RedFox::ReadFileText("C:/Development/RedFox/RedFox/res/shaders/skybox.frg.glsl"), GL_FRAGMENT_SHADER);

		initialize().attach(vtx).attach(frg).link();
	}
}