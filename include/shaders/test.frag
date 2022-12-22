//Based partially off the guide on learnopengl https://learnopengl.com/Lighting/Basic-Lighting
#version 330 core

in vec3 fragPos;
in vec3 fragColor;
in vec3 n;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D sampler;
uniform vec3 lPosition;
uniform vec3 vPos;

float specularStrength = 0.5;
float ambientStrength = 0.02;
vec3 lightColour = vec3(1.0,1.0,1.0);
int specularLightPower = 50;

void main() {
	//Diffuse lighting
	vec3 lightDir = normalize(lPosition - fragPos);
	float dotted_diffuse = dot(normalize(n), lightDir);
	if(dotted_diffuse < 0)
		dotted_diffuse = 0;
	vec3 diffuse = dotted_diffuse * lightColour;

	//Specular lighting
	vec3 direction_of_reflection = reflect(-lightDir, normalize(n));
	vec3 viewing_directions = normalize(vPos - fragPos);
	float dotted_specular = dot(viewing_directions, direction_of_reflection);
	if(dotted_specular < 0)
		dotted_specular = 0;
	vec3 specular = specularStrength * pow(dotted_specular, specularLightPower) * lightColour;

	//ambient lighting
	vec3 ambient = ambientStrength * lightColour;

	color = vec4((specular + diffuse + ambient) * vec3(texture(sampler, TexCoord)), 1.0);
}
