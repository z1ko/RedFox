#version 330 core

//GBuffer
layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;
layout (location = 2) out vec3 color;

struct pixel_t
{
	vec3 position;
    vec3 normal;
	vec2 uv;
};
in pixel_t pixel;

uniform sampler2D albedo;

void main()
{
    position = pixel.position;
    normal = normalize(pixel.normal);
    color = texture(albedo, pixel.uv);
}