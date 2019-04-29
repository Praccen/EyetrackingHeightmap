#version 400

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT{

	vec3 normal;
	vec2 texCoords;

}gs_in[];

out vec3 normalFrag;
out vec3 gsFragPos;
out vec2 texCoordsFrag;

uniform vec3 cameraPos;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat3 normalMatrix;

void main()
{


	vec3 vect1 = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);
	vec3 vect2 = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position); 
	vec3 normalCulling = cross(vect1, vect2);
	
	vec4 normalvec4 = normalize(view * vec4(normalMatrix * normalCulling, 0.0f));

	vec4 cameraFragVector =  view * vec4(cameraPos - vec3(model * gl_in[0].gl_Position), 0.0f);
	normalize(cameraFragVector);

	
	if(dot(cameraFragVector, normalvec4) >= 0.0f )
	{
		for(int i = 0; i< gl_in.length(); i++)
		{
			vec4 objWorldPosition = model * gl_in[i].gl_Position; 
			vec3 normal = normalize(normalMatrix * gs_in[i].normal);
			gsFragPos = objWorldPosition.xyz;
			gl_Position = projection * view * objWorldPosition;
			texCoordsFrag = gs_in[i].texCoords;
			normalFrag = normal; 
			EmitVertex();		
		}
		EndPrimitive();
	}
}
