#version 400

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColourSpec;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D splatMap;
	int blendCheck;
};

in vec3 normalFrag;
in vec3 gsFragPos;
in vec2 texCoordsFrag;


uniform Material material;


void main() 
{	
	if(material.blendCheck == 1)
	{	
		vec2 flippedTexCoords = texCoordsFrag.yx;
		gColourSpec.rgb = texture(material.diffuse, flippedTexCoords).rgb * (1.0f - texture(material.splatMap, flippedTexCoords).r) + texture(material.specular, flippedTexCoords).rgb * texture(material.splatMap, flippedTexCoords).r;
		gColourSpec.a = 0.0f;
	}
	else
	{
		gColourSpec.rgb = texture(material.diffuse, texCoordsFrag).rgb;
		gColourSpec.a = texture(material.specular, texCoordsFrag).r;
	}
	gPosition = gsFragPos;
	gNormal = normalize(normalFrag);


}
