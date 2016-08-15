#pragma once

#include "technique.hpp"

namespace RedFox
{
	class PostProcess
	{
		public:
			virtual void enable() const = 0;
	};

	class PostProcessChain
	{
		public:
			//Esegue tutti gli effetti
			void execute();

		private:
			u32 m_current; //Frame corrente
			Frame m_frames[2];

			vector<unique<PostProcess>> m_postProcesses;
	};
};