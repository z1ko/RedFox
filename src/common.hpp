#pragma once

#include <string>
#include <vector>
#include <unordered_map>

//Opengl
#include "../ext/glew/include/glew.h"
#include "../ext/glfw/include/glfw3.h"

//Matematica
#include "../ext/glm/include/glm.hpp"
#include "../ext/glm/include/gtc/type_ptr.hpp"
#include "../ext/glm/include/gtc/matrix_transform.hpp"

#define RFX_WARNING(text)\
printf("[WRN]: %s", text)

#define RFX_ERROR(text)\
printf("[ERR]: %s", text)

namespace RedFox
{
	using u32 = unsigned int;
	using i32 = int;

	using u16 = unsigned short;
	using i16 = short;

	using u8  = unsigned char;
	using i8  = char;

	//======================================================================

	using std::string;
	using std::vector;

	template<typename K, typename V>
	using umap = std::unordered_map<K, V>;

	//======================================================================

	using glm::vec2;
	using glm::vec3;
	using glm::mat4;

	//======================================================================
}