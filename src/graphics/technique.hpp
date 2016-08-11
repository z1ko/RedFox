#pragma once

#include "../common.hpp"

namespace RedFox
{
	//Singolo stage di una tecnica
	class Shader
	{
		friend class Technique;

		public:
			//Crea shader nullo
			Shader();

			//Crea shader da source
			Shader(const string& _source, u32 _target);

		private:
			u32 m_handle;
	};

	class Technique
	{
		public:
			//Tecnica nulla
			Technique();

			//Crea tecnica dalle source degli shader
			Technique(const string& _vtx, const string& _frg);

			//Usa questa tecnica di rendering
			void enable() const;

		private:
			Shader m_vtxShader;
			Shader m_frgShader;

		protected:
			u32 m_handle;
	};

	//Tecnica generica, può fare tutto
	class GenericTechnique : public Technique
	{
		public:
			template<typename T>
			void setUniform(const string& _name, const T& _value);

		private:
			u32 location(const string& _name);

		private:
			umap<string, u32> m_locations;
	};
}