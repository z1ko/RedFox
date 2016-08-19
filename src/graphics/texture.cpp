#include "texture.hpp"

#include "../core/engine.hpp"

namespace RedFox
{
	 Texture::Texture()
		  : m_handle(0)
	 {
	 }

	 Texture::Texture(const str& _name, u32 _handle)
		  : m_name(_name), m_handle(_handle)
	 {
	 }

	 void Texture::dispose()
	 {
		  m_cachedHandles.erase(m_name);
		  glDeleteTextures(1, &m_handle);
	 }

	 Texture& Texture::operator = (const Texture& _other)
	 {
		  if (this != &_other)
		  {
				m_handle = _other.m_handle;
		  }
		  return *this;
	 }

	 //Binda la texture ad uno slot dello shader corrente
	 void Texture::bind(u8 _slot) const
	 {
		  if (m_handle == 0) return;

		  glActiveTexture(GL_TEXTURE0 + _slot);
		  glBindTexture(GL_TEXTURE_2D, m_handle);
	 }

	 //=======================================================================================================

	 //Contiene tutte le texture attive al momento
	 umap<str, u32> Texture::m_cachedHandles;

	 //Carica texture da un file
	 Texture Texture::load(const str& _filename, u32 _target)
	 {
		  printf("Caricamento immagine ==============================================\n");

		  str path = Globals::Directories::Textures + _filename;

		  //Controlla se la texture è già stata caricata
		  if (m_cachedHandles.find(path) != m_cachedHandles.end())
		  {
				RFX_WARNING("Texture gia caricata, verra' riutilizzata");
				return Texture(path, m_cachedHandles.at(path));
		  }

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

		  //Le immagini vengono salvate nella VRAM con 32 bpp
		  FIBITMAP* bitmap32 = nullptr;
		  if (FreeImage_GetBPP(bitmap) != 32)
		  {
				RFX_WARNING("L'immagine non ha 32bpp, verra' convertita");
				bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
				FreeImage_Unload(bitmap);
		  }
		  else
		  {
				RFX_MESSAGE("L'immagine ha 32bpp, perfetto");
				bitmap32 = bitmap;
		  }

		  //Crea texture vera e propria
		  u32 handle;
		  glGenTextures(1, &handle);
		  glBindTexture(_target, handle);

		  glTexParameteri(_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		  glTexParameteri(_target, GL_TEXTURE_WRAP_T, GL_REPEAT);

		  glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		  glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		  glTexImage2D(_target, 0, GL_RGBA, FreeImage_GetWidth(bitmap32), FreeImage_GetHeight(bitmap32),
							0, GL_RGBA, GL_UNSIGNED_BYTE, FreeImage_GetBits(bitmap32));

		  glGenerateMipmap(_target);

		  glBindTexture(_target, 0);
		  FreeImage_Unload(bitmap32);

		  m_cachedHandles.emplace(path, handle);
		  return Texture(path, handle);
	 }
}