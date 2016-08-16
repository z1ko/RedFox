#version 330 core

struct gbuffer_t
{
    sampler2D position;
    sampler2D normal;
    sampler2D color;
};
uniform gbuffer_t gbuffer;

struct light_t
{
    vec3 position;
    vec3 color;
};
uniform light_t lights[256];

out vec4 final_color;
void main()
{
    vec3 position = texture(gbuffer.position, pixel.uv).rgb;
    vec3 normal = texture(gbuffer.normal, pixel.uv).rgb;
    vec3 color = texture(gbuffer.color, pixel.uv).rgb;

    //Calcola luce
    for(int i = 0; i < 256; i++)
    {
        //TODO
    }

    final_color = color;
}