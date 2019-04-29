#include "Model.h"

Model::Model(std::string path) {
	//Geometry uniform locations
	m_geometryModelMatrixLoc = -1;
	m_normalMatrixLoc = -1;

	//Shadow uniform locations
	m_shadowModelMatrixLoc = -1;

	maxDistance = glm::vec3(0.01f, 0.01f, 0.01f);
	minDistance = glm::vec3(-0.01f, -0.01f, -0.01f);
	loadModel(path);
}

void Model::checkDistances(glm::vec3 testVector) {
	if (testVector.x > maxDistance.x) {
		maxDistance.x = testVector.x;
	}
	else if (testVector.x < minDistance.x) {
		minDistance.x = testVector.x;
	}

	if (testVector.y > maxDistance.y) {
		maxDistance.y = testVector.y;
	}
	else if (testVector.y < minDistance.y) {
		minDistance.y = testVector.y;
	}

	if (testVector.z > maxDistance.z) {
		maxDistance.z = testVector.z;
	}
	else if (testVector.z < minDistance.z) {
		minDistance.z = testVector.z;
	}
}

void Model::loadModel(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene) {
	// Process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(this->processMesh(mesh, scene));
	}
	// Then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		// Process vertex positions, normals and texture coordinates
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		checkDistances(vector);

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0]) { // Does the mesh contain texture coordinates?
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else {
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}
	// Process indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// Process material
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		GLboolean skip = false;
		for (GLuint j = 0; j < loadedTextures.size(); j++) {
			if (std::strcmp(loadedTextures[j].path.C_Str(), str.C_Str()) == 0) {
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip) { // If texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
		}
	}
	return textures;
}

GLint Model::TextureFromFile(const char* path, std::string directory) {
	//Generate texture ID and load texture data 
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	sf::Image image;
	image.loadFromFile(filename);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}

std::vector<glm::vec3> Model::getSize() {
	std::vector<glm::vec3> returnVector;
	returnVector.push_back(minDistance);
	returnVector.push_back(maxDistance);
	return returnVector;
}

void Model::setModelMatrix(glm::mat4 modelMatrix) {
	m_modelMatrix = modelMatrix;
	m_normalMatrix = glm::transpose(glm::inverse(glm::mat3(m_modelMatrix)));
}

glm::mat4 Model::getModelMatrix() {
	return m_modelMatrix;
}

void Model::draw(GLuint shaderProgram) {
	if (m_geometryModelMatrixLoc == -1)
		m_geometryModelMatrixLoc = glGetUniformLocation(shaderProgram, "model");
	if (m_normalMatrixLoc == -1)
		m_normalMatrixLoc = glGetUniformLocation(shaderProgram, "normalMatrix");
	if (m_shadowModelMatrixLoc == -1) 
		m_shadowModelMatrixLoc = glGetUniformLocation(shaderProgram, "shadowModelMatrix");

	glUniformMatrix4fv(m_geometryModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
	glUniformMatrix3fv(m_normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_normalMatrix));

	glUniformMatrix4fv(m_shadowModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shaderProgram);
	}
}
