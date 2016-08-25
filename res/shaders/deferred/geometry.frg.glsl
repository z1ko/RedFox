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

struct material_t
{
	sampler2D albedo;
};
uniform material_t material;

void main()
{
    position = pixel.position;
    normal = normalize(pixel.normal);
    color = texture(material.albedo, pixel.uv).xyz;
}