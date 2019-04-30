#pragma once


#include "../../Engine/Graphics/Renderers/DeferredRenderer.h"
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

class Menu {
private:
	struct Character {
		GLuint texture_id;
		glm::ivec2 size;
		glm::ivec2 bearing;
		GLuint advance;
	};
	std::map<GLchar, Character> characters_;

	FT_Library free_type_lib_;
	FT_Face free_type_face_;

	GLuint vertex_array_object_;
	GLuint vertex_buffer_object_;

	int window_width = 1600;
	int window_height = 900;

	int selected_item_index_;

	void RenderText(
		ShaderSet* menu_shader,
		std::string text,
		GLfloat x,
		GLfloat y,
		GLfloat scale,
		glm::vec3 color
	);

public:
	Menu();
	~Menu();

	void Init();
	void RenderRealtimeMenu(ShaderSet* menu_shader);
};

