//
// Created by viktor on 21/11/23.
//

#ifndef CMAKELEARNOPENGL_CUBE_HPP
#define CMAKELEARNOPENGL_CUBE_HPP

#include "VertexArrayObject.h"
#include <glm/glm.hpp>

class Shader;

class Cube
{
public:
    Cube();
    Cube(Shader* shader);
    ~Cube();

    void update(float delta_time);

    glm::mat4 model;
    glm::vec3 position;
    float angle = 20.f;

private:
    VertexArrayObject vao;
    Shader* shader;
};


#endif //CMAKELEARNOPENGL_CUBE_HPP
