#version 400

uniform sampler2D tex;

in vec2 texCoords;

out vec4 final_colour;

void main() {
	final_colour = texture(tex, texCoords); //Set colour of fragment
}