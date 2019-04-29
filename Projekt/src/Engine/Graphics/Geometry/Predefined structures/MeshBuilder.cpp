#include "MeshBuilder.h"

MeshBuilder::MeshBuilder() {
	m_VAO = 0;
	m_VBO = 0;
	m_textureDiffuse = 0;
	m_textureSpecular = 0;

	//Geometry uniform locations
	m_textureDiffuseLoc = -1;
	m_textureSpecularLoc = -1;
	m_geometryModelMatrixLoc = -1;
	m_normalMatrixLoc = -1;

	//Shadow uniform locations
	m_shadowModelMatrixLoc = -1;

	m_nrOfVertices = 0;
	m_modelMatrix = glm::mat4();
	m_normalMatrix = glm::mat3();

	m_readyToDraw = false;
}

MeshBuilder::~MeshBuilder() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

int MeshBuilder::addTriangle(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2, glm::vec2 uv0, glm::vec2 uv1, glm::vec2 uv2) {
	int returnValue;

	glm::vec3 normal = glm::normalize(glm::cross(pos1 - pos0, pos2 - pos0));

	if (m_freeArraySpaces.size() <= 0) {
		Vertex tempVertex;

		tempVertex.x = pos0.x;
		tempVertex.y = pos0.y;
		tempVertex.z = pos0.z;
		tempVertex.nx = normal.x;
		tempVertex.ny = normal.y;
		tempVertex.nz = normal.z;
		tempVertex.u = uv0.x;
		tempVertex.v = uv0.y;
		m_vertices.push_back(tempVertex);

		tempVertex.x = pos1.x;
		tempVertex.y = pos1.y;
		tempVertex.z = pos1.z;
		tempVertex.nx = normal.x;
		tempVertex.ny = normal.y;
		tempVertex.nz = normal.z;
		tempVertex.u = uv1.x;
		tempVertex.v = uv1.y;
		m_vertices.push_back(tempVertex);

		tempVertex.x = pos2.x;
		tempVertex.y = pos2.y;
		tempVertex.z = pos2.z;
		tempVertex.nx = normal.x;
		tempVertex.ny = normal.y;
		tempVertex.nz = normal.z;
		tempVertex.u = uv2.x;
		tempVertex.v = uv2.y;
		m_vertices.push_back(tempVertex);

		m_nrOfVertices += 3;
		returnValue = m_nrOfVertices - 3;
	}
	else {
		int tempInt = m_freeArraySpaces.size() - 1; //Get last element

		m_vertices[m_freeArraySpaces[tempInt]].x = pos0.x;
		m_vertices[m_freeArraySpaces[tempInt]].y = pos0.y;
		m_vertices[m_freeArraySpaces[tempInt]].z = pos0.z;
		m_vertices[m_freeArraySpaces[tempInt]].nx = normal.x;
		m_vertices[m_freeArraySpaces[tempInt]].ny = normal.y;
		m_vertices[m_freeArraySpaces[tempInt]].nz = normal.z;
		m_vertices[m_freeArraySpaces[tempInt]].u = uv0.x;
		m_vertices[m_freeArraySpaces[tempInt]].v = uv0.y;

		m_vertices[m_freeArraySpaces[tempInt] + 1].x = pos1.x;
		m_vertices[m_freeArraySpaces[tempInt] + 1].y = pos1.y;
		m_vertices[m_freeArraySpaces[tempInt] + 1].z = pos1.z;
		m_vertices[m_freeArraySpaces[tempInt] + 1].nx = normal.x;
		m_vertices[m_freeArraySpaces[tempInt] + 1].ny = normal.y;
		m_vertices[m_freeArraySpaces[tempInt] + 1].nz = normal.z;
		m_vertices[m_freeArraySpaces[tempInt] + 1].u = uv1.x;
		m_vertices[m_freeArraySpaces[tempInt] + 1].v = uv1.y;

		m_vertices[m_freeArraySpaces[tempInt] + 2].x = pos2.x;
		m_vertices[m_freeArraySpaces[tempInt] + 2].y = pos2.y;
		m_vertices[m_freeArraySpaces[tempInt] + 2].z = pos2.z;
		m_vertices[m_freeArraySpaces[tempInt] + 2].nx = normal.x;
		m_vertices[m_freeArraySpaces[tempInt] + 2].ny = normal.y;
		m_vertices[m_freeArraySpaces[tempInt] + 2].nz = normal.z;
		m_vertices[m_freeArraySpaces[tempInt] + 2].u = uv2.x;
		m_vertices[m_freeArraySpaces[tempInt] + 2].v = uv2.y;

		returnValue = m_freeArraySpaces[tempInt];
		m_freeArraySpaces.pop_back(); //Remove last element as it is no longer free
	}

	return returnValue;
}

void MeshBuilder::setTexture(char* textureFileDiffuse, char* textureFileSpecular) {
	// Load and create a texture 
	glGenTextures(1, &m_textureDiffuse);
	glGenTextures(1, &m_textureSpecular);

	glBindTexture(GL_TEXTURE_2D, m_textureDiffuse); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
													// Load image, create texture (and generate mipmaps)
	sf::Image img_data;
	if (!img_data.loadFromFile(textureFileDiffuse))
	{
		std::cout << "Failed to load texture: ";
		std::cout << textureFileDiffuse;
		std::cout << "\n";
	}
	else {
		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_MIPMAP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_data.getSize().x, img_data.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data.getPixelsPtr());
		//glEnable(GL_TEXTURE_2D);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, m_textureSpecular); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
													 // Load image, create texture (and generate mipmaps)
	if (!img_data.loadFromFile(textureFileSpecular))
	{
		std::cout << "Failed to load texture: ";
		std::cout << textureFileSpecular;
		std::cout << "\n";
	}
	else {
		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_MIPMAP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_data.getSize().x, img_data.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data.getPixelsPtr());
		//glEnable(GL_TEXTURE_2D);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void MeshBuilder::setVertexPosition(int vertexIndex, glm::vec3 pos) {
	m_vertices[vertexIndex].x = pos.x;
	m_vertices[vertexIndex].y = pos.y;
	m_vertices[vertexIndex].z = pos.z;

	//Recalculate normal
	glm::vec3 normal;
	int temp = vertexIndex % 3;
	glm::vec3 pos0(m_vertices[vertexIndex - temp].x, m_vertices[vertexIndex - temp].y, m_vertices[vertexIndex - temp].z);
	glm::vec3 pos1(m_vertices[vertexIndex - temp + 1].x, m_vertices[vertexIndex - temp + 1].y, m_vertices[vertexIndex - temp + 1].z);
	glm::vec3 pos2(m_vertices[vertexIndex - temp + 2].x, m_vertices[vertexIndex - temp + 2].y, m_vertices[vertexIndex - temp + 2].z);
	
	normal = glm::normalize(glm::cross(pos1 - pos0, pos2 - pos0));

	m_vertices[vertexIndex - temp].nx = normal.x;
	m_vertices[vertexIndex - temp].ny = normal.y;
	m_vertices[vertexIndex - temp].nz = normal.z;

	m_vertices[vertexIndex - temp + 1].nx = normal.x;
	m_vertices[vertexIndex - temp + 1].ny = normal.y;
	m_vertices[vertexIndex - temp + 1].nz = normal.z;

	m_vertices[vertexIndex - temp + 2].nx = normal.x;
	m_vertices[vertexIndex - temp + 2].ny = normal.y;
	m_vertices[vertexIndex - temp + 2].nz = normal.z;
}

void MeshBuilder::setVertexNormal(int vertexIndex, glm::vec3 normal) {
	m_vertices[vertexIndex].nx = normal.x;
	m_vertices[vertexIndex].ny = normal.y;
	m_vertices[vertexIndex].nz = normal.z;
}

void MeshBuilder::setVertexUvCoords(int vertexIndex, glm::vec2 uv) {
	m_vertices[vertexIndex].u = uv.x;
	m_vertices[vertexIndex].v = uv.y;
}

void MeshBuilder::removePrimative(int baseIndex) {
	bool allreadyRemoved = false;
	for (unsigned int i = 0; i < m_freeArraySpaces.size(); i++) {
		if (m_freeArraySpaces[i] == baseIndex) {
			allreadyRemoved = true;
			i = m_freeArraySpaces.size();
		}
	}

	if (!allreadyRemoved) {
		m_freeArraySpaces.push_back(baseIndex);
		for (int i = 0; i < 3; i++) {
			m_vertices[baseIndex + i].x = 0.0f;
			m_vertices[baseIndex + i].y = 0.0f;
			m_vertices[baseIndex + i].z = 0.0f;
		}
	}
}

void MeshBuilder::createBuffers() {
	// Initialization code (done once (unless your object frequently changes))
	glGenVertexArrays(1, &m_VAO); //Declared in beginning of program
	glGenBuffers(1, &m_VBO); //Declared in beginning of program

	// 1. Bind Vertex Array Object
	glBindVertexArray(m_VAO);
	// 2. Copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
	// 3. Then set the vertex attributes pointers
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//TexCoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	// 4. Unbind VAO
	glBindVertexArray(0);

	// Uncommenting this call will result in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_readyToDraw = true;
}

void MeshBuilder::updateBuffers() {
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
}

glm::vec3 MeshBuilder::getVertexPosition(int vertexIndex) const {
	return glm::vec3(m_modelMatrix * glm::vec4(m_vertices[vertexIndex].x, m_vertices[vertexIndex].y, m_vertices[vertexIndex].z, 1.0f));
}

glm::vec3 MeshBuilder::getVertexNormal(int vertexIndex) const {
	return glm::vec3(m_vertices[vertexIndex].nx, m_vertices[vertexIndex].ny, m_vertices[vertexIndex].nz);
}

int MeshBuilder::getNumberOfVertices() const {
	return m_nrOfVertices;
}

void MeshBuilder::setModelMatrix(glm::mat4 modelMatrix) {
	m_modelMatrix = modelMatrix;
	m_normalMatrix = glm::transpose(glm::inverse(glm::mat3(m_modelMatrix)));
}

void MeshBuilder::draw(GLuint shaderProgram) {
	if (m_readyToDraw) {
		if (m_textureDiffuseLoc == -1)
			m_textureDiffuseLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
		if (m_textureSpecularLoc == -1)
			m_textureSpecularLoc = glGetUniformLocation(shaderProgram, "material.specular");
		if (m_geometryModelMatrixLoc == -1)
			m_geometryModelMatrixLoc = glGetUniformLocation(shaderProgram, "model");
		if (m_normalMatrixLoc == -1)
			m_normalMatrixLoc = glGetUniformLocation(shaderProgram, "normalMatrix");
		if (m_shadowModelMatrixLoc == -1)
			m_shadowModelMatrixLoc = glGetUniformLocation(shaderProgram, "shadowModelMatrix");

		// Bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(m_textureDiffuseLoc, 0);
		glBindTexture(GL_TEXTURE_2D, m_textureDiffuse);
		// Bind specular map
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(m_textureSpecularLoc, 1);
		glBindTexture(GL_TEXTURE_2D, m_textureSpecular);

		glUniformMatrix4fv(m_geometryModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));
		glUniformMatrix3fv(m_normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_normalMatrix));

		glUniformMatrix4fv(m_shadowModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

		//Draw vertices
		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, m_nrOfVertices);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}