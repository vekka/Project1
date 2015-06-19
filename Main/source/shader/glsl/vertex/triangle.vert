#version 330

layout(location = 0) in vec3 vVertex;
layout(location=1) in vec3 vNormal;
//layout(location = 1) in vec3 vNormal;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

out vec3 fragNormal;
out vec3 fragVert;

void main()
{
	
	gl_Position = P*V*M*vec4(vVertex,1);


	fragNormal = vNormal;
	fragVert = vVertex;
}