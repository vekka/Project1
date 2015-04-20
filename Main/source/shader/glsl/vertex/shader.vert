#version 440

layout(location = 0) in vec3 mesh;
layout(location = 1) in vec3 color;

uniform mat4 perspectiveMatrix;

uniform vec4 offset;
out vec3 colorOut;

void main()
{
	vec4 cameraPos;
	cameraPos = vec4( mesh, 1.0f) + offset;

	colorOut = color;
	gl_Position = perspectiveMatrix * cameraPos;
	
}