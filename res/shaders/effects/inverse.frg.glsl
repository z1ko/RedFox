#version 330 core

//Output dello shader precedente per questo shader
struct pixel_t
{
    vec2 uv;
}; 
in pixel_t pixel;

uniform sampler2D frame;

out vec4 final_color;
void main()
{
    final_color = vec4((1.0 - texture(frame, pixel.uv)).xyz, 1.0);
}