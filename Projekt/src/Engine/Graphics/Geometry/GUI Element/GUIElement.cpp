#include "GUIElement.h"

GUIElement::GUIElement() {
	m_size = glm::vec2(1.0f, 1.0f);
	m_position = glm::vec2(0.0f, 0.0f);
	m_rotation = 0.0f;
	
	m_modelMatrixLoc = -1;
	m_textureLoc = -1;

	createBuffer();
}

GUIElement::~GUIElement() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void GUIElement::createBuffer() {
	// Initialization code (done once (unless your object frequently changes))
	glGenVertexArrays(1, &m_VAO); //Declared in beginning of program
	glGenBuffers(1, &m_VBO); //Declared in beginning of program

	// 1. Bind Vertex Array Object
	glBindVertexArray(m_VAO);
	// 2. Copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
	// 3. Then set the vertex attributes pointers
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//TexCoords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// 4. Unbind VAO
	glBindVertexArray(0);
}

void GUIElement::updateModelMatrix() {
	m_modelMatrix = glm::mat4();
	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(m_position.x, m_position.y, 0.0));
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation), glm::vec3(0.0, 0.0, -1.0));
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_size.x, m_size.y, 1.0));
	m_mmNeedsUpdate = false;
}

void GUIElement::setTexture(char* textureFile) {
	// Load and create a texture 
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
													// Load image, create texture (and generate mipmaps)
	sf::Image img_data;
	if (!img_data.loadFromFile(textureFile))
	{
		std::cout << "Failed to load texture: ";
		std::cout << textureFile;
		std::cout << "\n";
	}
	else {
		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_data.getSize().x, img_data.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data.getPixelsPtr());
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void GUIElement::setPosition(glm::vec2 position) {
	m_position = position;
	m_mmNeedsUpdate = true;
}

void GUIElement::setRotation(float rotation) {
	m_rotation = rotation;
	m_mmNeedsUpdate = true;
}

void GUIElement::setSize(glm::vec2 size) {
	m_size = size;
	m_mmNeedsUpdate = true;
}

void GUIElement::setUV(glm::vec2 uv0, glm::vec2 uv1) {
	m_vertices[0].u = uv0.x;
	m_vertices[0].v = uv0.y;
	m_vertices[1].u = uv1.x;
	m_vertices[1].v = uv1.y;
	m_vertices[2].u = uv1.x;
	m_vertices[2].v = uv0.y;
	m_vertices[3].u = uv0.x;
	m_vertices[3].v = uv1.y;
	m_vertices[4].u = uv1.x;
	m_vertices[4].v = uv1.y;
	m_vertices[5].u = uv0.x;
	m_vertices[5].v = uv0.y;

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
}

void GUIElement::draw(GLuint shaderProgram) {
	if (m_mmNeedsUpdate) {
		updateModelMatrix();
	}

	if (m_textureLoc == -1) {
		m_textureLoc = glGetUniformLocation(shaderProgram, "texture");
	}
	if (m_modelMatrixLoc == -1) {
		m_modelMatrixLoc = glGetUniformLocation(shaderProgram, "worldMatrix");
	}

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(m_textureLoc, 0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}