#include "model.hpp"

#include "../core/engine.hpp"

namespace RedFox
{
	 Material::Material()
	 {
	 }

	 Material::Material(const aiMaterial* _material)
	 {
		  if (_material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		  {
				aiString textureName;
				_material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName, NULL, NULL, NULL, NULL, NULL);
				m_diffuse = Texture::load(textureName.C_Str(), GL_TEXTURE_2D);
		  }
		  else
		  {
				RFX_ERROR("Il materiale non ha una texture diffusiva");
		  }
	 }

	 void Material::dispose()
	 {
	 }

	 Material& Material::operator = (const Material& _other)
	 {
		  if (this != &_other)
		  {
				m_diffuse = _other.m_diffuse;
		  }
		  return *this;
	 }

	 //Binda le textures nello shader corrente
	 void Material::enable() const
	 {
		  m_diffuse.bind(0);
	 }

	 //==========================================================================================================

	 Shape::Shape()
	 {
	 }

	 Shape::Shape(const aiMesh* _mesh)
	 {
		  glGenVertexArrays(1, &m_vao);
		  glBindVertexArray(m_vao);

		  vector<Vertex> vertices(_mesh->mNumVertices);

		  for (u32 i = 0; i < _mesh->mNumVertices; i++)
		  {
				//Posizione
				vertices[i].position.x = _mesh->mVertices[i].x;
				vertices[i].position.y = _mesh->mVertices[i].y;
				vertices[i].position.z = _mesh->mVertices[i].z;


				//Normale
				vertices[i].normal.x = _mesh->mNormals[i].x;
				vertices[i].normal.y = _mesh->mNormals[i].y;
				vertices[i].normal.z = _mesh->mNormals[i].z;

				//TexUV
				if (_mesh->mTextureCoords[0])
				{
					 vertices[i].uv.x = _mesh->mTextureCoords[0][i].x;
					 vertices[i].uv.y = _mesh->mTextureCoords[0][i].y;
				}
				else RFX_WARNING("La mesh non contiene cordinate UV");
		  }

		  glGenBuffers(1, &m_vbo);
		  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		  glEnableVertexAttribArray(0);
		  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		  glEnableVertexAttribArray(1);
		  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		  glEnableVertexAttribArray(2);
		  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		  m_count = _mesh->mNumFaces * 3;
		  vector<u32> indices(m_count);

		  for (u32 i = 0; i < _mesh->mNumFaces; i++)
		  {
				indices[i * 3 + 0] = _mesh->mFaces[i].mIndices[0];
				indices[i * 3 + 1] = _mesh->mFaces[i].mIndices[1];
				indices[i * 3 + 2] = _mesh->mFaces[i].mIndices[2];
		  }

		  glGenBuffers(1, &m_ebo);
		  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indices.size(), indices.data(), GL_STATIC_DRAW);

		  glBindVertexArray(0);
	 }

	 Shape::Shape(const vector<Vertex>& _vertices, const vector<u32>& _indices)
		  : m_count(_indices.size())
	 {
		  glGenVertexArrays(1, &m_vao);
		  glBindVertexArray(m_vao);

		  glGenBuffers(1, &m_vbo);
		  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

		  glEnableVertexAttribArray(0);
		  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		  glEnableVertexAttribArray(1);
		  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		  glEnableVertexAttribArray(2);
		  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		  glGenBuffers(1, &m_ebo);
		  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * _indices.size(), _indices.data(), GL_STATIC_DRAW);

		  glBindVertexArray(0);
	 }

	 void Shape::dispose()
	 {
		  glDeleteVertexArrays(1, &m_vao);

		  glDeleteBuffers(1, &m_vbo);
		  glDeleteBuffers(1, &m_ebo);
	 }

	 Shape& Shape::operator = (const Shape& _other)
	 {
		  if (this != &_other)
		  {
				m_vao = _other.m_vao;
				m_vbo = _other.m_vbo;
				m_ebo = _other.m_ebo;

				m_count = _other.m_count;
		  }
		  return *this;
	 }

	 //Renderizza la forma
	 void Shape::render() const
	 {
		  glBindVertexArray(m_vao);
		  glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, nullptr);
	 }

	 Shape Shape::Screen()
	 {
		  static const vector<Vertex> vertices =
		  {
				{ {  1.0f,  1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
				{ { -1.0f,  1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
				{ { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
				{ {  1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } }
		  };
		  static const vector<u32> indices =
		  {
				0,1,2, 2,3,0
		  };

		  return Shape(vertices, indices);
	 }

	 Shape Shape::Cube()
	 {
		  static const vector<Vertex> vertices =
		  {
				{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
				{ { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
				{ { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
				{ { -1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
				{ {  1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
				{ {  1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
				{ {  1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
				{ {  1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } }
		  };
		  static const vector<u32> indices =
		  {
				0,6,4, 0,2,6, 0,3,2, 0,1,3, 2,7,6, 2,3,7, 4,6,7, 4,7,5, 0,4,5, 0,5,1, 1,5,7, 1,7,3
		  };

		  return Shape(vertices, indices);
	 }

	 //==========================================================================================================

	 Mesh::Mesh()
	 {
	 }

	 Mesh::Mesh(const aiMesh* _mesh, const aiMaterial* _material)
		  : m_shape(_mesh), m_material(_material)
	 {
	 }

	 void Mesh::dispose()
	 {
	 }

	 Mesh& Mesh::operator = (const Mesh& _other)
	 {
		  if (this != &_other)
		  {
				m_shape = _other.m_shape;
				m_material = _other.m_material;
		  }
		  return *this;
	 }

	 //Usa il materiale e renderizza la forma
	 void Mesh::render() const
	 {
		  m_material.enable();
		  m_shape.render();
	 }

	 //==========================================================================================================

	 Model::Model()
	 {
	 }

	 Model::Model(const str& _filename)
	 {
		  Assimp::Importer importer;

		  str path = Globals::Directories::Models + _filename;
		  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

		  //Carica tutte le mesh
		  for (u32 i = 0; i < scene->mNumMeshes; i++)
		  {
				const aiMesh* mesh = scene->mMeshes[i];
				m_meshes.emplace_back(mesh, scene->mMaterials[mesh->mMaterialIndex]);
		  }
	 }

	 void Model::dispose()
	 {
	 }

	 Model& Model::operator = (const Model& _other)
	 {
		  if (this != &_other)
		  {
				m_meshes = _other.m_meshes;
		  }
		  return *this;
	 }

	 //Renderizza tutte le mesh
	 void Model::render() const
	 {
		  for (const auto& mesh : m_meshes)
				mesh.render();
	 }

	 //==========================================================================================================
}