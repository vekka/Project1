#version 440 

layout(location = 0) in vec3 vVertex;
//layout(location = 1) in vec3 vColor;
out vec4 color;
uniform mat4 P;
uniform mat4 M;
void main()
{
	
	//color = vec4(vColor,1);
	gl_Position = 
	vec4(vVertex,1);
}