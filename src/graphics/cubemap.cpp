#include "cubemap.hpp"

#include "../core/engine.hpp"

namespace RedFox
{
	CubeMap::CubeMap()
		: m_handle(0)
	{
	}

	CubeMap::CubeMap(const array<str, 6>& _filenames)
	{
		glGenTextures(1, &m_handle);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);

		for(u32 i = 0; i < 6; i++)
		{
			 str path = Globals::Directories::Textures + _filenames[i];

			 i32 width, height;
			 BYTE* bitmap = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

			 glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height,
							  0, GL_RGB, GL_UNSIGNED_BYTE, bitmap);

			 SOIL_free_image_data(bitmap);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	//Usa questa cubemap nel contesto corrente
	void CubeMap::bind() const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);
	}
}