#version 330 core

struct gbuffer_t
{
    sampler2D position;
    sampler2D normal;
    sampler2D color;
};
uniform gbuffer_t gbuffer;

struct pixel_t
{
    vec2 uv;
}; 
in pixel_t pixel;

struct camera_t
{
    vec3 position;
};
uniform camera_t camera;

//Luci ad area
struct light_t
{
    vec3 position;
    vec3 color;
};
uniform light_t lights[64];

out vec4 final_color;
void main()
{
	//Ottiene colore del pixel tramite gbuffer
    vec4 albedo = texture(gbuffer.color, pixel.uv);

	if(albedo.a == 0.0) discard;

    //Ottiene posizione del pixel tramite gbuffer
    vec3 position = texture(gbuffer.position, pixel.uv).rgb;

    //Ottiene normale del pixel tramire gbuffer
    vec3 normal = texture(gbuffer.normal, pixel.uv).rgb;

    //Colore di base
    final_color = albedo * 0.6;

    //Direzione frammento - camera
    vec3 view_dir = normalize(camera.position - position);

    //Calcola contributo di ogni luce nel colore finale
    //TODO: Usa delle ottimizzazioni per la luce (Tiles)
    for(int i = 0; i < 64; i++)
    {
        //Blinn-Phong
        vec3 light_dir = normalize(lights[i].position - position);
        vec3 half = normalize(view_dir + light_dir);

        //Colore diffusivo
        final_color += vec4(max(dot(normal, light_dir), 0.0) * vec3(albedo) * lights[i].color, 1.0);
        
        //Colore speculare
        float spec_color = pow(max(dot(normal, half), 0.0), 32);
        final_color += vec4(0.5f * spec_color * lights[i].color, 1.0);
    }
}