#pragma once

#include "../common.hpp"
#include "../memory/pool.hpp"

namespace RedFox
{
	class Texture
	{
		public:
			Texture();
			void dispose();

			Texture& operator = (const Texture& _other);

			//Binda la texture ad uno slot dello shader corrente
			void bind(u8 _slot) const;

	   private:
			 Texture(const str& _name, u32 _handle);

		private:
			u32 m_handle;
			str m_name;

		public:
			 //Carica texture da un file
			 static Texture load(const str& _filename, u32 _target);

		private:
			 //Contiene tutte le texture attive al momento
			 static umap<str, u32> m_cachedHandles;
	};
}