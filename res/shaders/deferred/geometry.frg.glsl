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

const float far  = 100.0;
const float near = 0.1;

float linearize_depth(float _depth)
{
    float z = _depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    position = pixel.position;
    normal = normalize(pixel.normal);
    color = texture(material.albedo, pixel.uv).xyz;
}