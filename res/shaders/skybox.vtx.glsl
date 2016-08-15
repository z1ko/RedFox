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
uniform camera_t camera;

struct pixel_t
{
	vec3 uvt;
};
out pixel_t pixel;

void main()
{
    //Trasforma il vertice
    gl_Position = camera.proj * camera.view  * vec4(position, 1.0);

	pixel.uvt = position;
}