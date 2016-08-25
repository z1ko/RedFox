#pragma once

#include "../common.hpp"
#include "../graphics/texture.hpp"

namespace RedFox
{
	class Frame
	{
		public:
			Frame();
			//Crea il backbuffer
			Frame(u32 _width, u32 _height);

			//Usa questo framebuffer come target del rendering
			void bindForReading() const;

			//Usa questo framebuffer come target del blit
			void bindForWriting() const;

			//Usa il backbuffer standard
			void disable() const;

			//Usa la texture risultante nel contesto attuale
			void bind() const;

		private:
			u32 m_handle, m_rbo, m_result;
	};

	class RenderTarget
	{
	public:
		RenderTarget();

		//Usa questo framebuffer
		void bind(u32 mode) const;

		//Usa il framebuffer standard
		void unbind() const;

		//Usa la texture generata
		void bindBuffer(u32 slot) const;

	private:
		u32 handle_;
		u32 texture_, rbo_;
	};
}