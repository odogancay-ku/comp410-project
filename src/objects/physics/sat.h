
#ifndef SAT_H
#define SAT_H

#include <vector>
#include <glew.h>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>
#include "objects/Entity.h"


class Entity;

// Function to calculate projection of a hitbox onto an axis
std::pair<GLfloat, GLfloat> project(const std::vector<glm::vec3> &hitboxVertices, const glm::vec3 &axis);

bool overlap(const std::pair<GLfloat, GLfloat> &p1, const std::pair<GLfloat, GLfloat> &p2);

bool calculateCollision(const std::vector<glm::vec3> &hitboxVertices1, const std::vector<glm::vec3> &hitboxVertices2,
                        glm::vec3 &MTV);

void detectAndHandleCollisionSAT(Entity *object1, Entity *object2);


#endif //SAT_H
