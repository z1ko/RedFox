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

			 //Controlla il formato dell'immagine
			 FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);
			 if (format == FIF_UNKNOWN)
			 {
				  RFX_WARNING("Impossibile identificare formato immagine, verra' usata l'estensione del file");

				  format = FreeImage_GetFIFFromFilename(path.c_str());
				  if (!FreeImage_FIFSupportsReading(format))
				  {
						RFX_ERROR("Impossibile caricare immagine, formato non supportato");
						exit(-1);
				  }
			 }
			 //Carica immagine
			 FIBITMAP* bitmap = FreeImage_Load(format, path.c_str());

			 //Le immagini vengono salvate nella VRAM con 24 bpp
			 FIBITMAP* bitmap24 = nullptr;
			 if (FreeImage_GetBPP(bitmap) != 24)
			 {
				  RFX_WARNING("L'immagine non ha 24bpp, verra' convertita");
				  bitmap24 = FreeImage_ConvertTo32Bits(bitmap);
				  FreeImage_Unload(bitmap);
			 }
			 else
			 {
				  RFX_MESSAGE("L'immagine ha 24bpp, perfetto");
				  bitmap24 = bitmap;
			 }

			 glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, FreeImage_GetWidth(bitmap24), FreeImage_GetHeight(bitmap24),
							  0, GL_RGB, GL_UNSIGNED_BYTE, FreeImage_GetBits(bitmap24));

			 FreeImage_Unload(bitmap24);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

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