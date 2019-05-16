//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//SFML (with opengl)
#include <SFML\Window.hpp>
#include <SFML\OpenGL.hpp>
//Other
#include <iostream>
#include <tobii/tobii.h>
#include <tobii/tobii_streams.h>
#include <assert.h>

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

glm::vec2 g_eyePos;

void gaze_point_callback(tobii_gaze_point_t const* gaze_point, void* user_data)
{
	if (gaze_point->validity == TOBII_VALIDITY_VALID) {
		/*
		printf("Gaze point: %f, %f\n",
			gaze_point->position_xy[0],
			gaze_point->position_xy[1]);*/
		g_eyePos.x = gaze_point->position_xy[0];
		g_eyePos.y = gaze_point->position_xy[1];
	}
}

static void url_receiver(char const* url, void* user_data)
{
	char* buffer = (char*)user_data;
	if (*buffer != '\0') return; // only keep first value

	if (strlen(url) < 256)
		strcpy(buffer, url);
}

int main()
{
	//Memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//Eyetracking initialization
	tobii_api_t* api;
	tobii_error_t error = tobii_api_create(&api, NULL, NULL);
	tobii_device_t* device;
	bool mouse = false;
	//assert(error == TOBII_ERROR_NO_ERROR);

	char url[256] = { 0 };
	error = tobii_enumerate_local_device_urls(api, url_receiver, url);
	if (error == TOBII_ERROR_NO_ERROR) {
		//assert(error == TOBII_ERROR_NO_ERROR && *url != '\0');


		error = tobii_device_create(api, url, &device);
		//assert(error == TOBII_ERROR_NO_ERROR);
		if(error == TOBII_ERROR_NO_ERROR)
			error = tobii_gaze_point_subscribe(device, gaze_point_callback, 0);
	}
	//assert(error == TOBII_ERROR_NO_ERROR);
	//--------------------------
	if (error != TOBII_ERROR_NO_ERROR) {
		mouse = true;
	}


	//OpenGL settings
	sf::ContextSettings openGLSettings;
	SetOpenGLSettings(openGLSettings);

	// create the window
	sf::Window window(sf::VideoMode(1920, 1080), "OpenGL", sf::Style::Fullscreen, openGLSettings);
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
	EyetrackingPrototype game(&window, mouse);

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
			if (maxCounter >= 20) {
				break;
			}

			//Eyetracking
			if (!mouse) {
				error = tobii_wait_for_callbacks(NULL, 1, &device);
				assert(error == TOBII_ERROR_NO_ERROR || error == TOBII_ERROR_TIMED_OUT);

				error = tobii_device_process_callbacks(device);
				assert(error == TOBII_ERROR_NO_ERROR);
			}
			else {
				g_eyePos.x = sf::Mouse::getPosition(window).x;
				g_eyePos.y = sf::Mouse::getPosition(window).y;
			}

			game.setEyePos(g_eyePos);
			//-----------

			game.update(updateRatio);
			game.updateEngine(updateRatio);
			updateTimer -= updateRatio;
			maxCounter++;
		}

		if (maxCounter == 0) {
			//Eyetracking
			if (!mouse) {
				error = tobii_wait_for_callbacks(NULL, 1, &device);
				assert(error == TOBII_ERROR_NO_ERROR || error == TOBII_ERROR_TIMED_OUT);

				error = tobii_device_process_callbacks(device);
				assert(error == TOBII_ERROR_NO_ERROR);
			}
			else {
				g_eyePos.x = sf::Mouse::getPosition(window).x;
				g_eyePos.y = sf::Mouse::getPosition(window).y;
			}

			game.setEyePos(g_eyePos);
			//-----------

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

	//Eyetracking destroy
	if (!mouse) {
		error = tobii_gaze_point_unsubscribe(device);
		assert(error == TOBII_ERROR_NO_ERROR);

		error = tobii_device_destroy(device);
		assert(error == TOBII_ERROR_NO_ERROR);

		error = tobii_api_destroy(api);
		assert(error == TOBII_ERROR_NO_ERROR);
	}

	return 0;
}