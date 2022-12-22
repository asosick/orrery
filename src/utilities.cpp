#include "utilities.h"

glm::vec3 vec3matCombination(glm::vec3 vector, glm::mat4 mat)
{
    auto c = glm::vec4(vector.x, vector.y, vector.z, 1.0f) * mat;
    return glm::vec3(c.x,c.y,c.z);
}

glm::vec2 vec2matCombination(glm::vec2 vector, glm::mat4 mat)
{
    auto c = glm::vec4(vector.x, vector.y, 0.f, 1.0f) * mat;
    return glm::vec2(c.x,c.y);
}
