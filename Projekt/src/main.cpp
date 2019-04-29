//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//SFML (with opengl)
#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>
//Other
#include <iostream>

//Only import the game you want to run
#include "EyetrackingPrototype/EyetrackingPrototype.h"

void SetOpenGLSettings(sf::ContextSettings &settings) {
	//Settings
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 0;
	//Set OpenGL version to 4.0
	settings.majorVersion = 4; //4
	settings.minorVersion = 0; //.0
}

int main()
{
	//Memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	
	//OpenGL settings
	sf::ContextSettings openGLSettings;
	SetOpenGLSettings(openGLSettings);

	// create the window
	sf::Window window(sf::VideoMode(1600, 900), "OpenGL", sf::Style::Default, openGLSettings);
	//window.setVerticalSyncEnabled(true);

	//----Initialize the OpenGL states----
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	//------------------------------------

	sf::Clock gameTime; //Clock to keep track of update rate
	float dt;
	float fps = 0.0f;
	float tempFps = 0.0f;
	int counter = 0;
	float fpsUpdate = 1.0f;
	float fpsUpdateTimer = 0.0f;

	float updateRatio = 1.0f / 60.0f;
	float updateTimer = 0.0f;
	int maxCounter = 0;


	//Only create the game you want to run
	EyetrackingPrototype game(&window);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// run the main loop
	bool running = true;
	while (running) {
		// handle events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				// end the program
				running = false;
			}
			else if (event.type == sf::Event::Resized) {
				// adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				running = false;
			}
		}

		dt = gameTime.restart().asSeconds(); //Gets the update time

		// clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (fpsUpdateTimer <= fpsUpdate) {
			fpsUpdateTimer += dt;
			tempFps += (1 / dt);
			counter++;
		}
		else {
			fps = tempFps / counter;
			tempFps = 0.0f;
			counter = 0;
			fpsUpdateTimer = 0.0f;
			//std::cout << fps;
			window.setTitle("OpenGL FPS: " + std::to_string((int)fps)); //Prints FPS in title
		}

		// Constant update rate
		updateTimer += dt;
		maxCounter = 0;

		while (updateTimer >= updateRatio) {
			if (maxCounter >= 20)
				break;

			game.update(updateRatio);
			game.updateEngine(updateRatio);
			updateTimer -= updateRatio;
			maxCounter++;
		}

		if (maxCounter == 0) {
			game.update(updateTimer);
			game.updateEngine(updateTimer);
			updateTimer = 0;
		}

		// Render scene
		game.draw();
	

		// end the current frame (internally swaps the front and back buffers)
		window.display();
	}

	//Release resource

	return 0;
}