#version 330 core

struct pixel_t
{
	vec2 uv;
};
in pixel_t pixel;

uniform sampler2D albedo;

//Colore finale del pixel
out vec4 color;

void main()
{
    color = texture(albedo, pixel.uv);
}