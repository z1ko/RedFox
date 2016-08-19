#include "transform.hpp"

namespace RedFox
{
	Transform::Transform()
		: scale(1,1,1)
	{
	}

	mat4 Transform::model() const
	{
		mat4 transformation;
		transformation = glm::translate(transformation, position);

		transformation *= glm::orientate4(rotation);
		transformation = glm::scale(transformation, scale);

		return transformation;
	}
}