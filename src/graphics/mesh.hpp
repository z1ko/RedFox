#pragma once

#include "../common.hpp"
#include "../graphics/renderer.hpp"

namespace RedFox
{
	// Shape + Material (+ Technique) = Mesh
	// N * Mesh = Model

	template<typename T>
	class Buffer
	{
		public:
			//Buffer nullo
			Buffer()
				: m_count(0), m_target(0), m_usage(0)
			{
			}

			//Alloca il buffer senza settarne il contenuto
			Buffer(u32 _count, u32 _target, u32 _usage)
				: m_count(_count), m_target(_target), m_usage(_usage)
			{
				glGenBuffers(1, &m_handle);
				glBindBuffer(m_target, m_handle);
				glBufferData(m_target, sizeof(T) * m_count, nullptr, m_usage);
				glBindBuffer(m_target, 0);
			}

			//Alloca il buffer e ne setta il contenuto
			Buffer(const vector<T>& _elements, u32 _target, u32 _usage)
				: m_count(_elements.size()), m_target(_target), m_usage(_usage)
			{
				glGenBuffers(1, &m_handle);
				glBindBuffer(m_target, m_handle);
				glBufferData(m_target, sizeof(T) * m_count, _elements.data(), m_usage);
				glBindBuffer(m_target, 0);
			}

			//Distrugge il buffer
			~Buffer()
			{
				glDeleteBuffers(1, &m_handle);
			}

			//Usa questo buffer nel contesto corrente
			inline void bind() const
			{
				glBindBuffer(m_target, m_handle);
			}

			//Ritorna il numero di elementi
			inline u32 count() const { return m_count; }

		private:
			u32 m_handle, m_target, m_usage, m_count;
	};

	struct Vertex
	{
		vec3 position;
		//vec3 normal;
		//vec2 texuv;

		//Costruttori per comodità
		Vertex(vec3 _position);

		//Descrive il layout del vertice
		static void describe();
	};

	using VtxBuffer = Buffer<Vertex>;
	using IdxBuffer = Buffer<u32>;

	class Shape
	{
		public:
			//Oggetto nullo
			Shape();

			//Crea forma tramite vertici e indici
			Shape(const vector<Vertex>& _vertices, const vector<u32>& _indices);

			//Distrugge oggetto
			~Shape();

			//Usa questo oggett nel contesto corrente
			void bind() const;

			//Renderizza questa forma
			void render() const;

		private:
			VtxBuffer m_vtxBuffer;
			IdxBuffer m_idxBuffer;

			u32 m_handle;
	};

	class Mesh : public IRenderable
	{
		public:
			Mesh();
			//Crea mesh tramite vertici, indici e materiale
			Mesh(const vector<Vertex>& _vertices, const vector<u32>& _indices);

			//Renderizza questa forma
			void render(StandardTechnique* _technique) const;

		public:
			Transform transform;

		private:
			Shape m_shape;
	};
}