#version 330 core

//Informazioni fornite dall'engine
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

//Output di questo shader per lo shader successivo
struct pixel_t
{
    vec2 uv;
}; 
out pixel_t pixel;

void main()
{
    gl_Position = vec4(position, 1.0);
    pixel.uv = uv;
}