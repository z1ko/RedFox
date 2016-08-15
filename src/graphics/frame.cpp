#include "frame.hpp"

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

		result = Texture(m_handle);
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

	//Usa questo backbuffer come target del rendering
	void Frame::enable() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	}

	//Usa il backbuffer standard
	void Frame::disable() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}