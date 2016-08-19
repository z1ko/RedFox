#version 330 core

//Informazioni fornite dall'engine
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

struct camera_t
{
    mat4 view;
    mat4 proj;
};
uniform camera_t camera;

struct pixel_t
{
	vec3 uvt;
};
out pixel_t pixel;

void main()
{
    //Trasforma il vertice
    vec4 result = camera.proj * camera.view  * vec4(position, 1.0);

	//Usa W al posto di Z per fallire sempre nel depth test
	gl_Position = result.xyww;

    //Non ho la benché minima idea del perchè sia necessario flippare y per raddrizzare la cubemap...
	pixel.uvt = vec3(position.x, -position.y, position.z);
}