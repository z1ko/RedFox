#pragma once

#include "../common.hpp"

#include "camera.hpp"
#include "transform.hpp"

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
			Technique(const Shader& _vtx, const Shader& _frg);

			//Usa questa tecnica di rendering
			void enable() const;

		protected:
			u32 m_handle;
	};

	//Tecnica generica, può fare tutto
	class GenericTechnique : public Technique
	{
		public:
			GenericTechnique(const Shader& _vtx, const Shader& _frg);

			template<typename T>
			void setUniform(const string& _name, const T& _value);

		private:
			u32 location(const string& _name);

		private:
			umap<string, u32> m_locations;
	};

	//Tecnica standard, accetta telecamera e transform
	class StandardTechnique : public GenericTechnique
	{
		public:
			//Usa shaders predefiniti
			StandardTechnique(const Shader& _vtx, const Shader& _frg);

			void setCamera(const Camera& _camera);
			void setTransform(const Transform& _transform);
	};
}