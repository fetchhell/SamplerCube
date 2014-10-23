#version 330

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform	mat4 u_modelViewProjection;

out vec2 vTexCoord; 

void main() {
	
	vTexCoord   = aTexCoord;
	gl_Position = u_modelViewProjection * vec4(aPosition, 1.0);
}