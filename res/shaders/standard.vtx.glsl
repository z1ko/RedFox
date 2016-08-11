#version 330 core

//Informazioni fornite dall'engine
layout (location = 0) in vec3 position;

struct camera_t
{
    mat4 view;
    mat4 proj;
};
uniform camera_t camera;

struct instance_t
{
    mat4 model;
};
uniform instance_t instance;

void main()
{
    //Trasforma il vertice
    gl_Position = camera.proj * camera.view * instance.model * vec4(position, 1.0);
}