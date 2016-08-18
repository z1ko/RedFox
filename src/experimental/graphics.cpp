#include "graphics.hpp"

namespace RedFox { namespace Exp {

	Texture::Texture()
		: m_handle(0)
	{
	}

	Texture::Texture(const str& _filename)
	{
		str path = Globals::Directories::Textures + _filename;

		str ext = _filename.substr(_filename.size() - 3, 3);
		u32 mode = (ext == "png") ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB;

		int width, height;
		u8* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_AUTO);

		glGenTextures(1, &m_handle);
		glBindTexture(GL_TEXTURE_2D, m_handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		u32 format = (ext == "png") ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, image);

		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
	}

	void Texture::dispose()
	{
		glDeleteTextures(1, &m_handle);
	}

	Texture& Texture::operator = (const Texture& _other)
	{
		 if (this != &_other)
		 {
			  m_handle = _other.m_handle;
		 }
		 return *this;
	}

	//Binda la texture ad uno slot dello shader corrente
	void Texture::bind(u8 _slot) const
	{
		 glActiveTexture(GL_TEXTURE0 + _slot);
		 glBindTexture(GL_TEXTURE_2D, m_handle);
	}

	//==========================================================================================================

	Material::Material()
	{
	}

	Material::Material(const aiMaterial* _material)
	{
		if(_material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString textureName;
			_material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName, NULL, NULL, NULL, NULL, NULL);
			m_diffuse = Texture(textureName.C_Str());
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
		 for(u32 i = 0; i < scene->mNumMeshes; i++)
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

} }
