#version 440

in vec4 color;
//layout(location=0) out vec4 vFragColor;

out vec4 vFragColor;
void main()
{
	vFragColor = color;
}