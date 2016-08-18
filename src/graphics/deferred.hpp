#pragma once

#include "../common.hpp"
#include "../graphics/renderer.hpp"

namespace RedFox
{
	class GBuffer
	{
		public:
			GBuffer(u32 _width, u32 _height);

			void bindForWriting() const;
			void bindForReading() const;
			void unbind() const;

			void bindTextures() const;

		private:
			u32 m_handle, m_rbo;
			u32 m_position, m_normal, m_color;
	};

	class DeferredRenderer
	{
		public:
			void submitCommand(const RenderCommand& _command);

			void present(const Camera& _camera);

		private:
			vector<RenderCommand> m_commands;
	};
}