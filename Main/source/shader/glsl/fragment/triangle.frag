#version 440

uniform mat4 M;

uniform struct Light 
{
	vec3 position;
	vec3 color;
} light;

out vec4 vFragColor;
in vec3 fragNormal;
in vec3 fragVert;
void main()
{
	mat3 normalMatrix = transpose( inverse(mat3(M)));
	vec3 normal = normalize(normalMatrix * fragNormal);

	vec3 fragPosition = vec3 (M * vec4(fragVert, 1));

	vec3 surfaceToLight = light.position - fragPosition;

	float brightness = dot( normal, surfaceToLight) / ( length(surfaceToLight)*length(normal)) ;
	brightness = clamp(brightness, 0,1);

	vec4 surfaceColor = vec4( 0.3f, 0.5f, 0.7f, 1.0f );
	vFragColor =  vec4( brightness * light.color * surfaceColor.rgb, surfaceColor.a );
}