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
    void set_position(glm::vec3 _position);
    void set_rotation(float _angle);
    void set_rotation_angle(glm::vec3 _direction);

    glm::mat4 model{ 1.f };
    glm::vec3 position{ 0 };
    glm::vec3 scale{ 1.f };
    float angle = 20.f;
    glm::vec3 rotation_angle{ 0.f, 1.f, 0.f };

    Shader* shader;
private:
    VertexArrayObject vao;

    void apply_transformations();
};


#endif //CMAKELEARNOPENGL_CUBE_HPP
