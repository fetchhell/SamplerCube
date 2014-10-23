#include <include/includeAll.h>
#include "scene.h"

#define SCREEN_WIDTH 2048
#define SCREEN_HEIGHT 1024

//----------------------------
char* shader_program [] = { "simple" };

Scene scene(SCREEN_WIDTH, SCREEN_HEIGHT);

void buildViewMatrix(vec3f viewDirection, vec3f eyePos);

//----------------------------
void initVBO(GLuint buffer, object obj, GLvoid* data, GLuint size) {

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, size * scene.getObject().getMesh().getNumFaces() * 3, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

//----------------------------
void KeyboardEvent(unsigned char Key, int x, int y) {

	/* handle keyboard event; translate camera */
	Camera *cam = &scene.getCamera();

	vec3f view(cam->m_view[0][2], cam->m_view[1][2], cam->m_view[2][2]);   

	GLfloat walk_speed = 20.0f;
    switch (Key) {

		case 'w': 
		{
			cam->m_cameraMove = vec3f(0.0, 0.0, walk_speed);  
			break;
		}
		case 's':
		{
			cam->m_cameraMove = vec3f(0.0, 0.0, -walk_speed); 
			break;
		}
		case 'a':
		{
			cam->m_cameraMove = vec3f(walk_speed, 0.0, 0.0); 
			break;
		}
		case 'd':
		{
			cam->m_cameraMove = vec3f(-walk_speed, 0.0, 0.0); 
			break;
		}
	}

	/* update camera changes */
	NvBuildTranslateMatf(cam->m_cameraTranslate, cam->m_cameraMove.x, cam->m_cameraMove.y, cam->m_cameraMove.z);

	glutPostRedisplay();
}

//---------------------------
void buildViewMatrix(vec3f viewDirection, vec3f eyePos) {

	/* rebuild view matrix */
	Camera *cam = &scene.getCamera();
	
	vec3f up(0.0f, 1.0f, 0.0f);

	vec3f view(viewDirection);
	view.normalize();

	vec3f right = up.cross(view);
	right.normalize();

	up = view.cross(right);
	up.normalize();

	cam->m_view[0][2] = view.x, cam->m_view[1][2] = view.y, cam->m_view[2][2] = view.z;
	cam->m_view[0][1] = up.x, cam->m_view[1][1] = up.y, cam->m_view[2][1] = up.z;
	cam->m_view[0][0] = right.x, cam->m_view[1][0] = right.y, cam->m_view[2][0] = right.z;

	cam->m_view[3][0] = -right.x * eyePos.x + -right.y * eyePos.y + -right.z * eyePos.z;
    cam->m_view[3][1] = -up.x * eyePos.x + -up.y * eyePos.y + -up.z * eyePos.z;
    cam->m_view[3][2] = -view.x * eyePos.x + -view.y * eyePos.y + -view.z * eyePos.z;
    cam->m_view[3][3] = 1.0f;
}

void Mouse(int button, int state, int x, int y)
{
	/* set mouse position */
	Camera *cam = &scene.getCamera();

	cam->m_mousePosition.x = x - (glutGet(GLUT_WINDOW_WIDTH) / 2);
	cam->m_mousePosition.y = y - (glutGet(GLUT_WINDOW_HEIGHT) / 2);

    glutPostRedisplay();
}

//---------------------------
void MouseEvent(int x, int y) {

	/* handle mouse event; rotate camera */
	Camera *cam = &scene.getCamera();

	int ww = glutGet(GLUT_WINDOW_WIDTH);
    int wh = glutGet(GLUT_WINDOW_HEIGHT);

	x -= (glutGet(GLUT_WINDOW_WIDTH) / 2);
	y -= (glutGet(GLUT_WINDOW_HEIGHT) / 2);

	int deltaX = x - cam->m_mousePosition.x;
	int deltaY = y - cam->m_mousePosition.y;

	glutSetCursor(GLUT_CURSOR_NONE);

	cam->m_mousePosition.x = x;
	cam->m_mousePosition.y = y;

	cam->m_angleX = -(float) deltaX * M_PI / 360.0f;
    cam->m_angleY = -(float) deltaY * M_PI / 360.0f;

	vec3f view(cam->m_view[0][2], cam->m_view[1][2], cam->m_view[2][2]);
	
	/* build rotation matrices; rotate the camera along the axis XY */
	GLfloat rotX[4][4], rotY[4][4];

	NvBuildRotXRadMatf(rotX, cam->m_angleY);
	NvBuildRotYRadMatf(rotY, cam->m_angleX);

	NvMultMatf(cam->m_cameraRotateXY, rotX, rotY);

	/* refresh changes */
	glutPostRedisplay();
}

//---------------------------
void setTextureParam(png &tex) {

	glTexImage2D(GL_TEXTURE_2D, 0, tex.getFormat2(), tex.getWidth(), tex.getHeight(), 0, tex.getFormat2(), GL_UNSIGNED_BYTE, tex.getImageData());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex.config->getMinFilterMode());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex.config->getMagFilterMode());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex.config->getWrapsMode());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex.config->getWraptMode());

	if(tex.config->generateMipmaps())
       glGenerateMipmap(GL_TEXTURE_2D);
}

//----------------------------
bool init() {

	/* load shader */
	string shaderPath("../shader_program/");
	shaderPath.append(shader_program[0]);

	if(!scene.getShader().loadShader(string(shaderPath + ".vs").c_str(), 
		string(shaderPath + ".fs").c_str())) return false;

	/* load 3ds model */
	scene.getObject().getMesh().loadModel("../objects/figures.3ds");

	/* build matrices */
	scene.getCamera().buildFrustrumMatf(scene.getScreenWidth(), scene.getScreenHeight(), 2.f, 10000.f);
	scene.getCamera().buildViewMatf(vec3f(0,500,-500), vec3f(0,500,-1), vec3f(0,1,0));
	scene.getCamera().buildModelViewProjection();

	/* generate VBO object */
	glGenBuffers(3, scene.getObject().getVBO());

	initVBO(*scene.getObject().getVBO(), scene.getObject(), scene.getObject().getMesh().getVertex(), sizeof(Lib3dsVector));
	initVBO(*(scene.getObject().getVBO() + 1), scene.getObject(), scene.getObject().getMesh().getNormals(), sizeof(Lib3dsVector));
	initVBO(*(scene.getObject().getVBO() + 2), scene.getObject(), scene.getObject().getMesh().getTexCoord(), sizeof(Lib3dsTexel));

	/* load texture */
	if(scene.getTexture().png_texture_load("../textures/logo.png")) {

		scene.getTexture().config = new Config(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT, false);
		
		glGenTextures(1, &scene.getTextureHandle());
		glBindTexture(GL_TEXTURE_2D, scene.getTextureHandle());

		setTextureParam(scene.getTexture());
	}

	return true;
}

//----------------------------
void draw() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(scene.getShader().getProgramHandle());

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	/* make texture active */
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, scene.getTextureHandle());
	glUniform1i(glGetUniformLocation(scene.getShader().getProgramHandle(), "texture"), 0);

	/* update view matrix */
	GLfloat translate_rotate_camera[4][4];
	NvMultMatf(translate_rotate_camera, scene.getCamera().m_cameraRotateXY, scene.getCamera().m_cameraTranslate);
	
	GLfloat newView[4][4];
	NvMultMatf(newView, translate_rotate_camera, scene.getCamera().m_view);
	memcpy(scene.getCamera().m_view, newView, 16 * sizeof(float));

	/* rebuild modelViewProjection matrix */
	scene.getCamera().buildModelViewProjection();

	/* set uniform */
	scene.getShader().setUniform("u_modelViewProjection", scene.getCamera().m_modelViewProjection);

	/* identity previous changes */
	NvBuildIdentityMatf(scene.getCamera().m_cameraRotateXY);
	NvBuildIdentityMatf(scene.getCamera().m_cameraTranslate);

	/* draw the scene */
	scene.getShader().drawScene(scene.getObject(), scene.getObject().getVBO());

	/* swap front and back buffers */
	glutSwapBuffers();
}

//-----------------------------
void idle()
{
	glutPostRedisplay();
}

//-----------------------------
int main(int argc, char** argv) {

	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("SamplerCube");

	GLenum res = glewInit();
    if (res) {

      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }
    
    printf("GL version: %s\n", glGetString(GL_VERSION));

	/* init */
	if(!init()) return 0;

    glutDisplayFunc(draw);
	glutMouseFunc(Mouse);
    glutMotionFunc(MouseEvent);
    glutKeyboardFunc(KeyboardEvent);
	glutIdleFunc(idle);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glutMainLoop();

	return 0;
}
