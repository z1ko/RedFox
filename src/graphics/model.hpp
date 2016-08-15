#pragma once

#include "../common.hpp"
#include "../core/entity.hpp"

#include "texture.hpp"
#include "technique.hpp"

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

			//Distringingugge il buffer
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
		vec3 normal;
		vec2 uv;
	};

	using VtxBuffer = Buffer<Vertex>;
	using IdxBuffer = Buffer<u32>;

	class Material
	{
		public:
			Material();
			//Carica materiale con assimp
			Material(const aiMaterial* _material);

		public:
			Texture albedo;
	};

	class Mesh
	{
		public:
			Mesh();
			//Crea mesh tramite assimp
			Mesh(const aiScene* _scene, const aiMesh* _mesh);

			//Renderizza mesh
			void render() const;

		private:
			u32 m_vao, m_vbo, m_ebo, m_count;
			Material m_material;
	};

	class Model : public Component
	{
		public:
			Model();
			//Carica mesh da file
			Model(const str& _filename);

			//Renderizza tutte le mesh
			void render() override;

		private:
			vector<Mesh> m_meshes;
	};

}