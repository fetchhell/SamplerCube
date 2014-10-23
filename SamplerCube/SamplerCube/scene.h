#pragma once
#include <shader/shader.h>
#include <object/my_object.h>
#include <camera/my_camera.h>
#include <png/my_png.h>

typedef unsigned int ui;

class Scene {

	public:

	Scene(int screen_width, int screen_height): m_screen_width(screen_width), m_screen_height(screen_height) {};

	/* get */
	Shader& getShader() { return shader; }
	object& getObject() { return obj; }
	Camera& getCamera() { return camera; }
	png&    getTexture() { return texture; }
	ui&     getTextureHandle() { return texture_handle[0]; }

	int getScreenWidth() { return m_screen_width; }
	int getScreenHeight() { return m_screen_height; }

	private:
	
	/* width & height of the screen */
	int m_screen_width, m_screen_height; 

	/* model */
	object obj;

	/* shader */
	Shader shader;

	/* camera */
	Camera camera;

	/* texture */
	png texture;

	/* texture handles */
	ui texture_handle[1];
};