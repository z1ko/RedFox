#version 330 core

//Informazioni fornite dall'engine
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

struct camera_t
{
    mat4 view;
    mat4 proj;
};
uniform camera_t   camera;

struct instance_t
{
    mat4 model;
    mat3 normalizer; //transpose(inverse(mat3(instance.model)))
};
uniform instance_t instance;

struct pixel_t
{
    vec3 position;
    vec3 normal;
	vec2 uv;
};
out pixel_t pixel;

void main()
{
    //Trasforma il vertice
    gl_Position = camera.proj * camera.view * instance.model * vec4(position, 1.0);

    pixel.position = (instance.model * vec4(position, 1.0)).xyz;
    pixel.normal = instance.normalizer * normal;
	pixel.uv = uv;
}