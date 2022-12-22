//Based partially off the guide on learnopengl https://learnopengl.com/Lighting/Basic-Lighting
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 fragPos;
out vec3 fragColor;
out vec3 n;
out vec2 TexCoord;


void main() {
	fragPos = vec3(M * vec4(pos, 1.0f));
	fragColor = color;
	n = mat3(transpose(inverse(M))) * normal;
	gl_Position = P * V * M * vec4(pos, 1.0);
	TexCoord = aTexCoord;
}
