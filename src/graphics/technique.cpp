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
	Technique::Technique(const string& _vtx, const string& _frg)
		: m_vtxShader(_vtx, GL_VERTEX_SHADER), m_frgShader(_frg, GL_FRAGMENT_SHADER)
	{
		m_handle = glCreateProgram();

		glAttachShader(m_handle, m_vtxShader.m_handle);
		glAttachShader(m_handle, m_vtxShader.m_handle);

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

	//=====================================================================================================================================

	u32 GenericTechnique::location(const string& _name)
	{
		//Se la uniform non è gia stata registrata allora viene aggiunta
		if(m_locations.find(_name) == m_locations.end())
		{
			u32 loc = glGetUniformLocation(m_handle, _name.c_str());
			m_locations.at(_name) = loc;
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
}