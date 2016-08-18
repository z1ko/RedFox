#include "deferred.hpp"

namespace RedFox
{
	GBuffer::GBuffer(u32 _width, u32 _height)
	{
		glGenFramebuffers(1, &m_handle);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_handle);

		//Genera texture per le posizioni
		glGenTextures(1, &m_position);
		glBindTexture(GL_TEXTURE_2D, m_position);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_position, 0);

		//Genera texture per le normali
		glGenTextures(1, &m_normal);
		glBindTexture(GL_TEXTURE_2D, m_normal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normal, 0);

		////Genera texture per i colori
		glGenTextures(1, &m_color);
		glBindTexture(GL_TEXTURE_2D, m_color);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_color, 0);

		//Setta su quali textures avviene il rendering
		u32 attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

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

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void GBuffer::bindForWriting() const
	{
		 glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_handle);
	}

	void GBuffer::bindForReading() const
	{
		 glBindFramebuffer(GL_READ_FRAMEBUFFER, m_handle);
	}

	void GBuffer::unbind() const
	{
		 glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GBuffer::bindTextures() const
	{
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, m_position);

		 glActiveTexture(GL_TEXTURE1);
		 glBindTexture(GL_TEXTURE_2D, m_normal);

		 glActiveTexture(GL_TEXTURE2);
		 glBindTexture(GL_TEXTURE_2D, m_color);

	}

	//====================================================================================================================

	void DeferredRenderer::submitCommand(const RenderCommand& _command)
	{
		m_commands.push_back(_command);
	}

	void DeferredRenderer::present(const Camera& _camera)
	{
		static Frame renderingFrame(1600, 900); //TODO: Permetti di cambiare dimensioni schermo
		static FrameTechnique finalTechnique;

		std::sort(m_commands.begin(), m_commands.end());
		renderingFrame.enable();
		
		/*static DeferredTechnique renderingTechnique;
		renderingTechnique.enable();*/

		for (const auto& command : m_commands)
		{



		}

		renderingFrame.disable();
		m_commands.clear();
	}
}