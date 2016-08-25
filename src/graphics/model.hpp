#pragma once

#include "../common.hpp"

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

	//Rappresenta un insieme di textures
	class Material
	{
		public:
			Material();
			Material(const aiMaterial* _material);
			void dispose();

			Material& operator = (const Material& _other);

			//Binda le textures nello shader corrente
			void enable() const;

		private:
			Texture m_diffuse;
	};

	//Rappresenta un VAO
	class Shape
	{
		public:
			Shape();
			Shape(const aiMesh* _mesh);
			Shape(const vector<Vertex>& _vertices, const vector<u32>& _indices);
			void dispose();

			Shape& operator = (const Shape& _other);

			//Renderizza la forma
			void render() const;

		public:
			 static Shape Screen();
			 static Shape Cube();

		private:
			u32 m_vao, m_count, m_vbo, m_ebo;
	};

	//Rappresenta l'unione di forma e materiale
	class Mesh
	{
		public:
			 Mesh();
			 Mesh(const aiMesh* _mesh, const aiMaterial* _material);
			 void dispose();

			 Mesh& operator = (const Mesh& _other);

			 //Usa il materiale e renderizza la forma
			 void render() const;

		private:
			 Shape m_shape;
			 Material m_material;
	};

	class Model
	{
		 public:
			  Model();
			  Model(const str& _filename);
			  void dispose();

			  Model& operator = (const Model& _other);

			  //Renderizza tutte le mesh
			  void render() const;

			  //Ritorna un vettore con tutte le mesh di questo modello
			  inline auto& meshes() const { return m_meshes; }

		 private:
			  vector<Mesh> m_meshes;
	};

}