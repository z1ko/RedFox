#include "texture.hpp"

namespace RedFox
{
	Texture::Texture()
		: m_handle(0)
	{
	}

	Texture::Texture(u32 _handle)
		: m_handle(_handle)
	{
	}

	Texture::Texture(const str& _filename)
	{
		str ext = _filename.substr(_filename.size() - 3, 3);
		u32 mode = (ext == "png") ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB;

		int width, height;
		u8* image = SOIL_load_image(_filename.c_str(), &width, &height, 0, SOIL_LOAD_AUTO);

		glGenTextures(1, &m_handle);
		glBindTexture(GL_TEXTURE_2D, m_handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		u32 format = (ext == "png") ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, image);

		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
	}

	//Usa questa texture nel contesto corrente
	void Texture::bind(u16 _slot) const
	{
		glActiveTexture(GL_TEXTURE0 + _slot);
		glBindTexture(GL_TEXTURE_2D, m_handle);
	}
}