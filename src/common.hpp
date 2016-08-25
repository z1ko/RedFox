#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <array>
#include <algorithm>
#include <typeindex>
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
#include "../ext/freeimage/include/FreeImage.h"

#define RFX_ASSERT(expr) assert(expr)

#define RFX_MESSAGE(text)\
printf("[RFX][MSG]: %s\n", text)

#define RFX_WARNING(text)\
printf("[RFX][WRN]: %s\n", text)

#define RFX_ERROR(text)\
printf("[RFX][ERR]: %s\n", text)

#define RFX_POOL_CAPACITY 256

#define RFX_WINDOW_WIDTH  1600
#define RFX_WINDOW_HEIGHT 900

namespace RedFox
{
	using f64 = double;
	using f32 = float;

	using u32 = unsigned int;
	using i32 = int;

	using u16 = unsigned short;
	using i16 = short;

	using u8  = unsigned char;
	using i8  = char;

	//======================================================================

	using str = std::string;
	using std::stringstream;

	using std::type_index;
	using std::vector;
	using std::array;

	template<typename T>
	using unique = std::unique_ptr<T>;

	template<typename K, typename V>
	using umap = std::unordered_map<K, V>;

	//======================================================================

	using glm::vec2;
	using glm::vec3;
	using glm::mat3;
	using glm::mat4;

	//======================================================================
}