#version 400

struct Material {
	sampler2D diffuse;
	sampler2D specular;
};

in vec2 texCoords;

out vec4 final_colour;

uniform Material material;

void main() {
	final_colour = texture(material.diffuse, texCoords); //Set colour of fragment
}