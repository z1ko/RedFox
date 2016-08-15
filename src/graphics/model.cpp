#include "model.hpp"

namespace RedFox
{

	Material::Material()
	{
	}

	//Carica materiale con assimp
	Material::Material(const aiMaterial* _material)
	{
		//Carica albedo
		if(_material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString tmpName;
			_material->GetTexture(aiTextureType_DIFFUSE, 0, &tmpName, NULL, NULL, NULL, NULL, NULL);

			str name = tmpName.C_Str();
			u32 last = name.find_last_of('\\') + 1;
			if(last != 0)
				name = name.substr(last);

			albedo = Texture("C:/Development/RedFox/RedFox/res/textures/" + name);
		}
	}

	//=====================================================================================================================================

	Mesh::Mesh()
	{
	}

	//Crea mesh tramite assimp
	Mesh::Mesh(const aiScene* _scene, const aiMesh* _mesh)
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		vector<Vertex> vertices(_mesh->mNumVertices);

		for(u32 i = 0; i < _mesh->mNumVertices; i++)
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

		for(u32 i = 0; i < _mesh->mNumFaces; i++)
		{
			indices[i * 3 + 0] = _mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = _mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = _mesh->mFaces[i].mIndices[2];
		}

		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indices.size(), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);

		//Carica materiale
		m_material = Material(_scene->mMaterials[_mesh->mMaterialIndex]);
	}

	//Renderizza mesh
	void Mesh::render() const
	{
		//Setta materiale
		m_material.albedo.bind(0);

		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, nullptr);
	}

	//=====================================================================================================================================

	Model::Model()
	{
	}

	//Carica mesh da file
	Model::Model(const str& _filename)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(_filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

		//Carica tutte le mesh nella scena
		for (u32 i = 0; i < scene->mNumMeshes; i++)
			m_meshes.emplace_back(scene, scene->mMeshes[i]);
	}

	void Model::render()
	{
		for (const auto& mesh : m_meshes)
			mesh.render();
	}
}