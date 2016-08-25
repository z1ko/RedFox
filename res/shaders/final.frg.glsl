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

	//Se il pixel non ha un colore allora non lo processa,
	//Questo permette di non sovrascrivere lo skybox
	//if(color.a == 0.0) discard;

	final_color = color;
}