#ifndef MESH_H
#define MESH_H

#include <GL\glew.h>

#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <vector>

#include "..\..\Shader compilation\ShaderSet.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh {
private:
	GLuint VAO, VBO, EBO;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	void setupMesh();
public:
	Mesh(std::vector<Vertex> newVertices, std::vector<GLuint> newIndices, std::vector<Texture> newTextures);

	void draw(GLuint shaderProgram);
};


#endif