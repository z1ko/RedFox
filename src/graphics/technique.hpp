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
			Shader(const str& _source, u32 _target);

		private:
			//Risolve #include
			str resolve(const str& _source);

		private:
			u32 m_handle;
	};

	struct Sampler
	{
		u32	slot;
		str name;
	};

	class Technique
	{
		public:
			//Tecnica nulla
			Technique();

			//Crea tecnica
			void initialize(const Shader& _vtx, const Shader& _frg);

			//Usa questa tecnica di rendering
			void enable() const;

			template<typename T>
			void setUniform(const str& _name, const T& _value);

			void setSamplers(const vector<Sampler>& _samplers);

			void setCamera(const Camera& _camera);
			void setTransform(const Transform& _transform);
			
			u32 location(const str& _name);

		protected:
			u32 m_handle;
			umap<str, u32> m_locations;
	};

	//Tecnica standard, accetta telecamera e transform
	class StandardTechnique : public Technique
	{
		public:
			//Usa shaders predefiniti
			StandardTechnique();
	};

	class FrameTechnique : public Technique
	{
		public:
			FrameTechnique();
	};

	class SkyBoxTechnique : public Technique
	{
		public:
			SkyBoxTechnique();
	};
}