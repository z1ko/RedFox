#include "transform.hpp"

namespace RedFox
{
	mat4 Transform::model() const
	{
		mat4 transformation;
		transformation = glm::translate(transformation, position);
		
		if(m_parent != nullptr)
			transformation *= m_parent->model();

		return transformation;
	}

	void Transform::attachTo(Transform* _parent)
	{
		m_parent = _parent;
	}
}