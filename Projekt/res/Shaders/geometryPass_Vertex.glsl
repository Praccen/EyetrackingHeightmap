#version 400

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texCoords;

out VS_OUT{
	vec3 normal;
	vec2 texCoords;
}vs_out;




void main()
{
	vs_out.normal = vertex_normal;
	vs_out.texCoords = vertex_texCoords;
	gl_Position = vec4(vertex_position, 1.0);
	
}