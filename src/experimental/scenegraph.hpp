#pragma once

#include "../common.hpp"
#include "../core/events.hpp"
#include "../memory/pool.hpp"

namespace RedFox { namespace Exp {

	//Permette di modificare facilmente il world matrix di un nodo,
	//inoltre costituisce la gerarchia spaziale della scena
	class Transform
	{
		friend class Node;

	public:
		Transform();

		//Fa dipendere la trasformazione dal parente
		void attachTo(Transform* parent);

		//Aggiorna la trasformazione di tutti i figli in relazione a quella di questo transform
		void bake(const mat4& world, bool dirty);

	public: //Getters e Setters vari

		//Setta nodo che possiede questo transform
		void setOwner(Node* node);

		//Aggiunge traslazione
		void translate(const vec3& position);

		void setPosition(const vec3& position);
		vec3 getPosition() const;

		//Aggiunge rotazione
		void rotate(const vec3& rotation);

		void setRotation(const vec3& rotation);
		vec3 getRotation() const;

		//Aggiunge scaling
		void scale(const vec3& scale);

		void setScale(const vec3& scale);
		vec3 getScale() const;

	private:
		//Componenti della trasformazione
		vec3 position_, rotation_, scale_;

		//Vero se è stato modificato un componente della trasformazione
		//dall'ultimo bake eseguito
		bool dirty_;

		//Conserva la trasformazione in modo da non doverla calcolare ad ogni update
		mat4 world_;

		//Transform a cui appartiene questo transform
		Transform* parent_;

		//Non a cui appartiene questo transform
		Node* node_;

		//I figli dipendono dalla trasformazione di questo transform
		vector<Transform*> children_;
	};

	//Caratteristica di un nodo
	class IComponent
	{
		friend class Node;

	public:
		inline IComponent(Node* owner)
			: owner_(owner)
		{
		}

	protected:
		//Crea un nuovo UID
		inline static u32 generate()
		{
			static u32 lastID = 0;
			return lastID++;
		}

	private:
		Node* owner_;
	};

	template<class T>
	class Component : public IComponent
	{
	public:
		inline Component(Node* owner)
			: IComponent(owner)
		{
		}

		//Crea un id specifico per T
		inline static u32 uid()
		{
			static u32 typeID = generate();
			return typeID;
		}
	};

	//Singola unità che compone la scena
	class Node
	{
	public:
		Node();

		//Aggiunge un componente
		template<class T, class ... Ts>
		void assign(Ts&& ... args)
		{
			static_assert(std::is_base_of<Component<T>, T>::value, "T non è un componente");
			T* component = new T(this, std::forward<Ts>(args)...);
			components_[T::uid()] = component;

			scene_->componentCreation(this, T::uid(), component);
		}

		//Trova un componente
		template<class T>
		T* find()
		{
			static_assert(std::is_base_of<Component<T>, T>::value, "T non è un componente");
			return (T*)components_[T::uid()];
		}

	public:
		//Ogni nodo ha di default un transform
		Transform transform_;

	private:
		//Array di tutti i componenti installati
		IComponent* components_[64];
	};

	//Wrapper, premette di accedere ad un nodo
	class NodePtr
	{
		friend class Scene;

	public:
		NodePtr();
		NodePtr(Pool<Node, 1024>* pool, Key<Node> key);

		Node* operator -> ()
		{
			return (*pool_)[key_];
		}

	private:
		//Pool che contiene tutti i nodi della scena
		Pool<Node, 1024>* pool_;

		//Chiave per accedere al nodo
		Key<Node> key_;
	};

	//Cuore logico dell'applicazione
	class Scene
	{
	public:
		//Crea un nuovo nodo vuoto
		NodePtr instantiate();

		//Crea una nuova copia di un nodo
		//NodePtr instantiate(const NodePtr& prefab);

		//Elimina un nodo, se ha come componente un transform elimina anche tutti i nodi figli
		void kill(const NodePtr& node);

	public: //Eventi
		Event<NodePtr, u32, IComponent*> componentCreation;

	private:
		//Tutti i nodi pre-allocati
		Pool<Node, 1024> nodes_;
	};

	//========================================================================================================

} }