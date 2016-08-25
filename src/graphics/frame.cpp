#include "frame.hpp"

#include "model.hpp"

namespace RedFox
{
	Frame::Frame()
		: m_handle(0), m_result(0)
	{
	}

	//Crea il backbuffer
	Frame::Frame(u32 _width, u32 _height)
	{
		//Crea texture dove verrà renderizzata la scena
		glGenTextures(1, &m_result);
		glBindTexture(GL_TEXTURE_2D, m_result);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, &m_handle);
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

		//Setta su quale texture avverrà il rendering
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_result, 0);

		//Crea render buffer per la depth map e la stencil map
		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//Assegna il renderbuffer al framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		//Controlla che il framebuffer sia stato creato correttamente
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			exit(-1);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//Usa questo framebuffer come target del rendering
	void Frame::bindForReading() const
	{
		 glBindFramebuffer(GL_READ_FRAMEBUFFER, m_handle);
	}

	//Usa questo framebuffer come target del blit
	void Frame::bindForWriting() const
	{
		 glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_handle);
	}

	//Usa il backbuffer standard
	void Frame::disable() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//Usa la texture risultante nel contesto attuale
	void Frame::bind() const
	{
		 glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_result);
	}

	//==============================================================================

	RenderTarget::RenderTarget()
	{
		glGenFramebuffers(1, &handle_);
		glBindFramebuffer(GL_FRAMEBUFFER, handle_);

		//Crea texture per il colore
		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, RFX_WINDOW_WIDTH, RFX_WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Setta su quale texture avverrà il rendering
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_, 0);

		//Crea render buffer object per il depth e lo stencil buffer
		glGenRenderbuffers(1, &rbo_);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, RFX_WINDOW_WIDTH, RFX_WINDOW_HEIGHT);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//Assegna il renderbuffer al framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

		//Controlla che il framebuffer sia stato creato correttamente
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			//Errore creazione framebuffer
			exit(-1);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//Usa questo framebuffer
	void RenderTarget::bind(u32 mode) const
	{
		glBindFramebuffer(mode, handle_);
	}

	//Usa il framebuffer standard
	void RenderTarget::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//Usa la texture generata
	void RenderTarget::bindBuffer(u32 slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, texture_);
	}
}