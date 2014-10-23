#pragma once
#include <matrix/nv_matrix.h>
#include <vector3f/vec.h>

class Camera {

	public:

	Camera();

	void buildFrustrumMatf(GLfloat, GLfloat, GLfloat, GLfloat);
	void buildViewMatf(vec3f, vec3f, vec3f);
	void buildModelViewProjection();

	GLfloat m_modelViewProjection[4][4];
	GLfloat m_view[4][4], m_model[4][4], m_projection[4][4];

	GLfloat m_modelView[4][4], m_viewProjection[4][4];

	GLfloat m_znear, m_zfar, m_aspectRatio, m_rightEdge;
	vec3f m_eyePos, m_earthPos, m_up;

	vec3f m_cameraMove, m_mousePosition;

    float m_speedX, m_speedZ;
    float m_angleX, m_angleY;

	vec3f rotateViewVecXY;

	GLfloat m_cameraRotateXY[4][4], m_cameraTranslate[4][4];
};