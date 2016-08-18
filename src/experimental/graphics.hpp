#pragma once

#include "../common.hpp"
#include "../core/engine.hpp"
#include "../graphics/model.hpp"

namespace RedFox { namespace Exp {

	class Texture
	{
		public:
			Texture();
			Texture(const str& _filename);
			void dispose();

			Texture& operator = (const Texture& _other);

			//Binda la texture ad uno slot dello shader corrente
			void bind(u8 _slot) const;

		private:
			u32 m_handle;
	};

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
			void dispose();

			Shape& operator = (const Shape& _other);

			//Renderizza la forma
			void render() const;

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

		 private:
			  vector<Mesh> m_meshes;
	};

} }