#version 440 

layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec3 vColor;
out vec4 color;
uniform mat4 MVP;
void main()
{
	
	color = vec4(vColor,1);
	gl_Position = MVP * vec4(vVertex,1);
}