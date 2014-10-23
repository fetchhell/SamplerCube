#include "shader.h"

//--------------------------
bool Shader::loadShader(const char *pathVertexShader, const char *pathFragmentShader) {

	string vertexShaderCode, fragmentShaderCode;
	if(!File::ReadFile(pathVertexShader, vertexShaderCode)) return false;
	if(!File::ReadFile(pathFragmentShader, fragmentShaderCode)) return false;

	if(!compileShader(vertexShaderCode, GL_VERTEX_SHADER)) return false;
	if(!compileShader(fragmentShaderCode, GL_FRAGMENT_SHADER)) return false;

	if(!linkProgram()) return false;

	return true;
}

//-------------------------
bool Shader::compileShader(string shaderCode, unsigned int type) {
     
	 int shaderHandle = glCreateShader(type);
	 int shaderLength = shaderCode.length();

	 char* strAlloc = new char[shaderLength + 1];
	 memcpy(strAlloc, &shaderCode[0], shaderLength);
	 strAlloc[shaderLength] = 0;

	 const char* str = strAlloc;

	 glShaderSource(shaderHandle, 1, &str, &shaderLength);
	 
	 delete [] strAlloc;
	 glCompileShader(shaderHandle);
	 
	 int shaderCompiled;
	 glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &shaderCompiled);
	 if(!shaderCompiled) return false;

	 shaderHandles.push_back(shaderHandle);

	 return true;
}

//-----------------------------------------------------------
bool Shader::linkProgram() {

	program_handle = glCreateProgram(); 
	glAttachShader(program_handle, shaderHandles[0]);
	glAttachShader(program_handle, shaderHandles[1]);
	glLinkProgram(program_handle);

	int status;
	glGetProgramiv(program_handle, GL_LINK_STATUS, &status);
	if(!status) return false;

	int m_nAttributes = -1;
    glGetProgramiv(program_handle, GL_ACTIVE_ATTRIBUTES, &m_nAttributes );

	return true;
}

//-----------------------------------------------------------
void Shader::drawScene(object& obj, GLuint *vbo_Ids) {

	glBindBuffer(GL_ARRAY_BUFFER, *vbo_Ids);
	this->setAttribute("aPosition", obj.getMesh().getVertex()[0], 3);
	
	glBindBuffer(GL_ARRAY_BUFFER, *(vbo_Ids + 1));
	this->setAttribute("aNormal", obj.getMesh().getNormals()[0], 3);

	glBindBuffer(GL_ARRAY_BUFFER, *(vbo_Ids + 2));
	this->setAttribute("aTexCoord", obj.getMesh().getTexCoord()[0], 2); 

	glBindBuffer(GL_ARRAY_BUFFER, *vbo_Ids);
	glDrawArrays(GL_TRIANGLES, 0, obj.getMesh().getNumFaces() * 3);
}