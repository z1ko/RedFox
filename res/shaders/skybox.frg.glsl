#version 330 core

struct pixel_t
{
	vec3 uvt;
};
in pixel_t pixel;

uniform samplerCube skybox;

//Colore finale del pixel
out vec4 color;

void main()
{
    color = texture(skybox, pixel.uvt);
}