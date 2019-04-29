#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 shadowModelMatrix;
uniform mat4 lightSpaceMatrix;

void main(){
	gl_Position = lightSpaceMatrix * shadowModelMatrix * vec4(position, 1.0f);
}