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
    vec4 color = texture(frame, pixel.uv);
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    
    final_color = vec4(average, average, average, 1.0);
}