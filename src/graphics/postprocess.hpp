#pragma once

#include "frame.hpp"
#include "technique.hpp"

namespace RedFox
{
	class PostProcess
	{
		public:
			//Crea l'effetto usando un fragment shader
			PostProcess(const str& _filename);

			void enable() const;

		protected:
			Technique m_technique;
	};
};