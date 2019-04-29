#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texCoords;

out vec2 texCoords;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main() {
	texCoords = vertex_texCoords;
	gl_Position = projection * view * model * vec4(vertex_position, 1.0f);
}