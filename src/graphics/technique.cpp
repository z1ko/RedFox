#include "technique.hpp"

namespace RedFox
{
	//Crea shader nullo
	Shader::Shader()
		: m_handle(0)
	{
	}

	//Crea shader da source
	Shader::Shader(const string& _source, u32 _target)
	{
		m_handle = glCreateShader(_target);

		const char* source = _source.c_str();
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

	//=====================================================================================================================================

	//Tecnica nulla
	Technique::Technique()
		: m_handle(0)
	{
	}

	//Crea tecnica dalle source degli shader
	Technique::Technique(const Shader& _vtx, const Shader& _frg)
	{
		m_handle = glCreateProgram();

		glAttachShader(m_handle, _vtx.m_handle);
		glAttachShader(m_handle, _frg.m_handle);

		glLinkProgram(m_handle);

		int status;
		glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
		if (status != GL_TRUE)
		{
			char log[256];
			glGetProgramInfoLog(m_handle, 256, nullptr, log);
			RFX_ERROR("Creazione programma fallita");
		}

		glDetachShader(m_handle, _frg.m_handle);
		glDetachShader(m_handle, _frg.m_handle);
	}

	//Usa questa tecnica di rendering
	void Technique::enable() const
	{
		glUseProgram(m_handle);
	}

	//=====================================================================================================================================

	GenericTechnique::GenericTechnique(const Shader& _vtx, const Shader& _frg)
		: Technique(_vtx, _frg)
	{
	}

	u32 GenericTechnique::location(const string& _name)
	{
		//Se la uniform non è gia stata registrata allora viene aggiunta
		if(m_locations.find(_name) == m_locations.end())
		{
			u32 loc = glGetUniformLocation(m_handle, _name.c_str());
			m_locations[_name] = loc;
		}
		return m_locations.at(_name);
	}

	template<>
	void GenericTechnique::setUniform<vec3>(const string& _name, const vec3& _value) {
		glUniform3f(location(_name), _value.x, _value.y, _value.z);
	}

	template<>
	void GenericTechnique::setUniform<mat4>(const string& _name, const mat4& _value) {
		glUniformMatrix4fv(location(_name), 1, GL_FALSE, glm::value_ptr(_value));
	}

	//=====================================================================================================================================

	StandardTechnique::StandardTechnique(const Shader& _vtx, const Shader& _frg)
		: GenericTechnique(_vtx, _frg)
	{
	}

	void StandardTechnique::setCamera(const Camera& _camera)
	{
		setUniform("camera.view", _camera.view());
		setUniform("camera.proj", _camera.proj());
	}

	void StandardTechnique::setTransform(const Transform& _transform)
	{
		setUniform("instance.model", _transform.model());
	}
}