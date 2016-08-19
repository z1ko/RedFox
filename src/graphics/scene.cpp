#include "scene.hpp"

#include "../core/engine.hpp"

namespace RedFox
{
	 SceneNode::SceneNode()
		  : m_dirty(true)
	 {
	 }

	 SceneNode::~SceneNode()
	 {
		  //Elimina tutti i figli
		  for (auto child : m_children) delete child;
	 }

	 void SceneNode::addChild(SceneNode* _node)
	 {
		  m_children.push_back(_node);
	 }

	 //Registra questo nodo e i suoi figli nei sistema addetti al loro rendering,
	 //aggiorna la trasformazione se necessario
	 void SceneNode::update(const mat4& _world, bool _dirty, u32 _depth)
	 {
		  _dirty |= m_dirty;
		  if(m_dirty)
		  {
				printf("Aggiornamento trasformazione [%i]\n", _depth);
				m_world = m_transform.model() * _world;
				m_dirty = false;
		  }

		  //Registra comando nel renderer
		  submit();

		  //Propaga comando a tutti i figli
		  for (auto child : m_children) child->update(m_world, m_dirty, ++_depth);
	 }

	 //Ottiene un figlio tramite index
	 SceneNode& SceneNode::operator[] (u32 _index)
	 {
		  RFX_ASSERT(_index < m_children.size());
		  return *m_children[_index];
	 }

	 //Modifica il transform corrente
	 void SceneNode::setTransform(const Transform& _transform)
	 {
		  m_transform = _transform;
		  m_dirty = true;
	 }

	 //Muove il nodo
	 void SceneNode::translate(const vec3& _vector)
	 {
		  m_transform.position += _vector;
		  m_dirty = true;
	 }

	 void SceneNode::setPosition(const vec3& _vector)
	 {
		  m_transform.position = _vector;
		  m_dirty = true;
	 }

	 //Ruota il nodo
	 void SceneNode::rotate(const vec3& _vector)
	 {
		  m_transform.rotation += _vector;
		  m_dirty = true;
	 }

	 void SceneNode::setRotation(const vec3& _vector)
	 {
		  m_transform.rotation = _vector;
		  m_dirty = true;
	 }

	 //Scala il nodo
	 void SceneNode::scale(const vec3& _vector)
	 {
		  m_transform.scale += _vector;
		  m_dirty = true;
	 }

	 void SceneNode::setScale(const vec3& _vector)
	 {
		  m_transform.scale = _vector;
		  m_dirty = true;
	 }

	 //==========================================================================================

	 MeshNode::MeshNode(const aiMesh* _mesh)
	 {
	 }

	 MeshNode::MeshNode(const aiMesh* _mesh, const aiMaterial* _material)
		  : m_mesh(_mesh, _material)
	 {
	 }

	 MeshNode::~MeshNode()
	 {
		  //TODO: Conserva mesh se necessario
	 }

	 //Registra oggetto nel renderer adeguato
	 void MeshNode::submit()
	 {
		  //Facilita l'accesso al renderer
		  static DeferredRenderer& renderer = DeferredRenderer::instance();
		  renderer.submitMeshCommand(&m_mesh, m_world);
	 }

	 //==========================================================================================

	 //Carica modello da file
	 ModelNode::ModelNode(const str& _filename)
	 {
		  Assimp::Importer importer;

		  str path = Globals::Directories::Models + _filename;
		  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

		  //Carica tutte le mesh
		  for (u32 i = 0; i < scene->mNumMeshes; i++)
		  {
				const aiMesh* mesh = scene->mMeshes[i];
				addChild(new MeshNode(mesh, scene->mMaterials[mesh->mMaterialIndex]));
		  }
	 }

	 //==========================================================================================

	 LightNode::LightNode(const vec3& _color)
		  : color(_color)
	 {
	 }

	 //Registra oggetto nel renderer adeguato
	 void LightNode::submit()
	 {
		  //Facilita l'accesso al renderer
		  static DeferredRenderer& renderer = DeferredRenderer::instance();
		  renderer.submitLightCommand({ m_transform.position, color });
	 }
}