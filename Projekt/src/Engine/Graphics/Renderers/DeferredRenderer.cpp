#include "DeferredRenderer.h"

DeferredRenderer::DeferredRenderer(sf::Window *originalWindow, Scene *scene) {
	m_window = originalWindow;
	m_scene = scene;

	//Shadowmapping resolution
	m_shadowWidth = 4096 * 1;
	m_shadowHeight = 4096 * 1;

	//Shadowmap matrices
	m_lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, 1.0f, 100.0f);
	//m_lightProjection = glm::perspective(glm::radians(75.0f), 1.0f, 0.01f, 100.0f);
	m_lightOffset = m_scene->getDirectionalLight()->getDirection() * -30.0f;
	m_biasMatrix = glm::mat4(//this matrix transforms the value of the position in a [0,1] range
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

	//Create shader sets
	m_gBufferShaders = new ShaderSet("geometryPass_Vertex.glsl", "geometryPass_Frag.glsl", "BackfaceCulling_Geometry.glsl");
	m_shadowShaders = new ShaderSet("shadowPass_Vertex.glsl", "shadowPass_Frag.glsl");
	m_lightingShaders = new ShaderSet("lightingPass_Vertex.glsl", "lightingPass_Frag.glsl");

	//Create buffers
	m_gBufferShaders->use();
	createGBuffer();
	createShadowBuffer();

	// Set samplers
	m_lightingShaders->use();
	glUniform1i(glGetUniformLocation(m_lightingShaders->getShaderProgram(), "gPosition"), 0);
	glUniform1i(glGetUniformLocation(m_lightingShaders->getShaderProgram(), "gNormal"), 1);
	glUniform1i(glGetUniformLocation(m_lightingShaders->getShaderProgram(), "gColourSpec"), 2);
	glUniform1i(glGetUniformLocation(m_lightingShaders->getShaderProgram(), "depthMap"), 3);

	//Get uniform locations
	m_sLightSpaceMatrixLoc = glGetUniformLocation(m_shadowShaders->getShaderProgram(), "lightSpaceMatrix");
	m_lLightSpaceMatrixLoc = glGetUniformLocation(m_lightingShaders->getShaderProgram(), "lightSpaceMatrix");
	
}

DeferredRenderer::~DeferredRenderer() {
	//Delete buffer(s)
	glDeleteFramebuffers(1, &m_gBuffer);
	glDeleteFramebuffers(1, &m_shadowBuffer);

	//Delete shader(s)
	delete m_lightingShaders;
	delete m_shadowShaders;
	delete m_gBufferShaders;
}

void DeferredRenderer::createGBuffer() {

	//initializing gBuffer
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

	//Creating 3 textures for the gBuffer, one for position, one for normal and one for Color + specular.
	// Setting up the position texture for the gBuffer
	glGenTextures(1, &m_gPosition);
	glBindTexture(GL_TEXTURE_2D, m_gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_window->getSize().x, m_window->getSize().y, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPosition, 0);

	// Setting up the normal texture for the gBuffer
	glGenTextures(1, &m_gNormal);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_window->getSize().x, m_window->getSize().y, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);

	// Setting up the colour+spec texture for the gBuffer
	glGenTextures(1, &m_gColourSpec);
	glBindTexture(GL_TEXTURE_2D, m_gColourSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_window->getSize().x, m_window->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gColourSpec, 0);

	//attaching to gBuffer
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	//Setting up depth buffer for gBuffer
	glGenRenderbuffers(1, &m_rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_window->getSize().x, m_window->getSize().y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	//unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::createShadowBuffer() {
	//Generating shadow buffer
	glGenFramebuffers(1, &m_shadowBuffer);

	//Generating depth map texture
	glGenTextures(1, &m_depthMap);

	glBindTexture(GL_TEXTURE_2D, m_depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowWidth, m_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//Attaching texture to shadow buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//checking for any errors
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Shadow Buffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderer::draw() {
	glEnable(GL_DEPTH_TEST);
	setGeometryPass();
	setShadowPass();
	setLightPass();
	renderQuad();
}

void DeferredRenderer::renderScene(GLuint shaderProgram) {
	//Draw the scene
	m_scene->draw3D(shaderProgram);
}

void DeferredRenderer::setGeometryPass() {
	//Running geometry pass - creating the gBuffer textures
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_gBufferShaders->use();

	//Set uniforms
	m_scene->setGeometryUniforms(m_gBufferShaders->getShaderProgram());

	//Render geometry
	renderScene(m_gBufferShaders->getShaderProgram());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void DeferredRenderer::setShadowPass() {
	m_shadowShaders->use();
	glViewport(0, 0, m_shadowWidth, m_shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowBuffer);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::vec3 cameraTargetPos = m_scene->getCamera()->getPosition();

	//Light space matrix for calculation of depth map
	m_lightOffset = m_scene->getDirectionalLight()->getDirection() * -20.0f;
	m_lightView = glm::lookAt(cameraTargetPos + m_lightOffset, cameraTargetPos, glm::vec3(0.0f, 1.0f, 0.0f));
	m_lightSpace = m_lightProjection * m_lightView;

	//Set uniforms
	glUniformMatrix4fv(m_sLightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_lightSpace));
	m_scene->setShadowUniforms(m_shadowShaders->getShaderProgram());

	//Render shadow pass with front face culling to avoid peter panning
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	renderScene(m_shadowShaders->getShaderProgram());
	glDisable(GL_CULL_FACE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_window->getSize().x, m_window->getSize().y);
}

void DeferredRenderer::setLightPass() {
	//Running lighting pass - shading and creating shadows from the gBuffer and shadowmap textures
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_lightingShaders->use();

	//Binding textures from the different passes
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_gColourSpec);
	//Shadows
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);

	//Calculate light space matrix for lighting shaders
	m_lightSpace = m_biasMatrix * m_lightProjection * m_lightView;

	//Set uniforms
	glUniformMatrix4fv(m_lLightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(m_lightSpace));
	m_scene->setLightingUniforms(m_lightingShaders->getShaderProgram());
}

void DeferredRenderer::renderQuad() {
	//Render quad for deferred shading and post processing
	if (m_VAO == 0) {

		Vertex vertices[]{
			//Position				//TexCoords
			-1.0f, -1.0f,  0.0f,	0.0f, 0.0f,
			1.0f,  1.0f,  0.0f,		1.0f, 1.0f,
			1.0f, -1.0f,  0.0f,		1.0f, 0.0f,
			-1.0f,  1.0f,  0.0f,	0.0f, 1.0f,
			1.0f,  1.0f,  0.0f,		1.0f, 1.0f,
			-1.0f, -1.0f,  0.0f,	0.0f, 0.0f,
		};

		glGenVertexArrays(1, &m_VAO); //Declared in beginning of program
		glGenBuffers(1, &m_VBO); //Declared in Fbeginning of program

								 // 1. Bind Vertex Array Object
		glBindVertexArray(m_VAO);
		// 2. Copy our vertices array in a vertex buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}