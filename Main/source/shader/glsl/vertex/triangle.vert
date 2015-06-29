#version 330

layout(location = 0) in vec3 vVertex;
layout(location=1) in vec3 vNormal;
//layout(location = 1) in vec3 vNormal;

uniform mat4 MVP;


out vec3 fragNormal;
out vec3 fragVert;

void main()
{
	// P V M
	gl_Position = MVP * vec4(vVertex,1);


	fragNormal = vNormal;
	fragVert = vVertex;
}