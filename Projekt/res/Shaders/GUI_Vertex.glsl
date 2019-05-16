#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_texCoords;

uniform mat4 worldMatrix;

out vec2 texCoords;

void main() {
	texCoords = vertex_texCoords;
	gl_Position = worldMatrix * vec4(vertex_position, 1.0f);
}