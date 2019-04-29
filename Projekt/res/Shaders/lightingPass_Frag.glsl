#version 400

in vec2 texCoords;
out vec4 final_colour;


uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColourSpec;
uniform sampler2D depthMap;

struct PointLight {
	vec3 position;
	vec3 colour;

	float constant;
	float linear;
	float quadratic;
	float radius;
};

struct DirectionalLight {
	vec3 direction;
	vec3 colour;
	float ambientMultiplier;
};

#define NR_POINT_LIGHTS 20

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int nrOfPointLights;
uniform vec3 cameraPos; //Used for specular lighting
uniform mat4 lightSpaceMatrix; //Used to calculate shadow with shadow mapping

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 cameraDir, vec3 diffuse, float specular, float shininess, vec4 depthMapCoords);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 cameraDir, vec3 diffuse, float specular, float shininess, vec4 depthMapCoords);
float CalcShadow(vec4 depthMapCoords, vec3 normal, vec3 lightPos);

void main() {

	vec3 fragPos = texture(gPosition, texCoords).rgb;
	vec3 normal = texture(gNormal, texCoords).xyz;
	float shininess = 32.0f;
	vec3 diffuse = texture(gColourSpec, texCoords).rgb;
	float specular = texture(gColourSpec, texCoords).a;
	vec4 depthMapCoords = lightSpaceMatrix * vec4(fragPos, 1.0f);


	vec3 cameraDir = normalize(cameraPos - fragPos); //Direction vector from fragment to camera

	vec3 result = vec3(0.0f, 0.0f, 0.0f);

	result += CalcDirectionalLight(directionalLight, normal, fragPos, cameraDir, diffuse, specular, shininess, depthMapCoords);

	for (int i = 0; i < nrOfPointLights; i++) {
		float distance = length(pointLights[i].position - fragPos);
		if (distance < pointLights[i].radius) { // Checking if fragment is within the light volume of each point light
			result += CalcPointLight(pointLights[i], normal, fragPos, cameraDir, diffuse, specular, shininess, depthMapCoords);
		}
	}

	final_colour = vec4(result, 1.0f); //Set colour of fragment
}

// Calculates the colour when using a directional light
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 cameraDir, vec3 diffuse, float specular, float shininess, vec4 depthMapCoords) {
	vec3 ambient = diffuse * light.ambientMultiplier; //Ambient lighting
	vec3 lightDir = normalize(-light.direction); //light direction from the fragment position

	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(cameraDir, reflectDir), 0.0), shininess);

	// Combine results
	vec3 finalDiffuse = light.colour * diff * diffuse;
	vec3 finalSpecular = light.colour * spec * specular;
	// SHADOW
	float shadow = CalcShadow(depthMapCoords, normal, lightDir);
	//float shadow = 0.0f;
	vec3 lighting = (ambient + (1.0f - shadow) * (finalDiffuse + finalSpecular));
	return lighting;
}

// Calculates the colour when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 cameraDir, vec3 diffuse, float specular, float shininess, vec4 depthMapCoords) {
	vec3 lighting;
	vec3 lightDir = normalize(light.position - fragPos); //light direction from the fragment position

	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(cameraDir, reflectDir), 0.0), shininess);

	// Attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	// Combine results
	vec3 finalDiffuse = light.colour * diff * diffuse;
	vec3 finalSpecular = light.colour * spec * specular;
	finalDiffuse *= attenuation;
	finalSpecular *= attenuation;
	lighting = finalDiffuse + finalSpecular;
	//lighting = finalSpecular;
	return lighting;
}

float CalcShadow(vec4 depthMapCoords, vec3 normal, vec3 lightDir) {
	float shadow = 0.0f;
	//float bias = clamp(0.0005*tan(acos(dot(normal, lightDir))), 0,0.05f);
	float bias = 0.0f;
	float closestDistance = texture(depthMap, depthMapCoords.xy).z;
	float currentDistance = depthMapCoords.z;
	if(currentDistance - bias > closestDistance)
	{
		shadow = 1.0f;
	} 
	if(currentDistance > 1.0f)
		shadow = 0.0f;
	return shadow;
}