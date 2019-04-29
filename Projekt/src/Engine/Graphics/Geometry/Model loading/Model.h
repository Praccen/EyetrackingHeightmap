#ifndef MODEL_H
#define MODEL_H

#include <GL\glew.h>

#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>
#include <SFML\Graphics\Image.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <vector>

#include "..\..\Shader compilation\ShaderSet.h"
#include "Mesh.h"

class Model {
private:
	glm::mat4 m_modelMatrix;
	glm::mat3 m_normalMatrix;

	//Geometry uniform locations
	GLint m_geometryModelMatrixLoc, m_normalMatrixLoc;

	//Shadow uniform locations
	GLint m_shadowModelMatrixLoc;

	glm::vec3 maxDistance;
	glm::vec3 minDistance;

	void checkDistances(glm::vec3 testVector);

	std::vector<Mesh> meshes;
	std::vector<Texture> loadedTextures;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	GLint TextureFromFile(const char* path, std::string directory);
public:
	Model(std::string path);

	std::vector<glm::vec3> getSize();

	void setModelMatrix(glm::mat4 modelMatrix);
	glm::mat4 getModelMatrix();

	void draw(GLuint shaderProgram);
};


#endif