#include "SceneObj.h"
#include <glm\gtc\matrix_access.hpp>

SceneObj::SceneObj() {}
SceneObj::~SceneObj() {}

glm::vec3 SceneObj::Position() {
	glm::vec4 pos;
	pos = glm::column(_localM, 3);
	return glm::vec3(pos);
}
