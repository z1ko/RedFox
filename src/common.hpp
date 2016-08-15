#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

//Opengl
#include "../ext/glew/include/glew.h"
#include "../ext/glfw/include/glfw3.h"

//Matematica
#include "../ext/glm/include/glm.hpp"
#include "../ext/glm/include/gtc/type_ptr.hpp"
#include "../ext/glm/include/gtc/quaternion.hpp"
#include "../ext/glm/include/gtx/quaternion.hpp"
#include "../ext/glm/include/gtx/euler_angles.hpp"
#include "../ext/glm/include/gtx/rotate_vector.hpp"
#include "../ext/glm/include/gtc/matrix_transform.hpp"

//Caricamento modelli
#include "../ext/assimp/include/scene.h"
#include "../ext/assimp/include/Importer.hpp"
#include "../ext/assimp/include/postprocess.h"

//Caricamento immagini
#include "../ext/soil/include/SOIL.h"

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

	using str = std::string;
	using std::vector;

	template<typename K, typename V>
	using umap = std::unordered_map<K, V>;

	//======================================================================

	using glm::vec2;
	using glm::vec3;
	using glm::mat4;

	//======================================================================
}