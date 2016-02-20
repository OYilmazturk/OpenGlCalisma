#include "GlHelper.h"

GLuint GlHelper::CreateShader(GLenum eShaderType, const std::string &strShaderFile) {
	GLuint shader = glCreateShader(eShaderType);
	std::string shaderCode;
	std::ifstream shaderStream(strShaderFile.c_str(), std::ios::in);
	if (shaderStream.is_open()) {
		std::string line = "";
		while(getline(shaderStream, line))
			shaderCode += "\n" + line;
		shaderStream.close();
	} else {
		std::cerr << strShaderFile.c_str() << " dosyasi acilamadi. Dosyanin bulundugu yeri kontrol ediniz." << std::endl;
		return 0;
	}

	const char* shaderCodePtr = shaderCode.c_str();
	glShaderSource(shader, 1, &shaderCodePtr, NULL);
	
	glCompileShader(shader);

	GLint status;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
		const char *strShaderType = NULL;

		switch(eShaderType) {
			case GL_VERTEX_SHADER:
				strShaderType = "vertex";
				break;
			case GL_GEOMETRY_SHADER:
				strShaderType = "geometry";
				break;
			case GL_FRAGMENT_SHADER:
				strShaderType = "fragment";
				break;
		}
		std::cerr << strShaderType << " tipi shader dosyasi derlenemedi. Detaylar: " << std::endl << strInfoLog << std::endl;
		delete[] strInfoLog;
	}
	return shader;
}

GLuint GlHelper::CreateProgram(const std::vector<GLuint> &shaderList) {
	GLuint program = glCreateProgram();
	
	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++) 
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		std::cerr << "Linkleme sorunu: " << std::endl << strInfoLog << std::endl;
		delete[] strInfoLog;
	}

	offsetLocation = glGetUniformLocation(program, "offset");
	perspectiveMatrixLocation = glGetUniformLocation(program, "perspectiveMatrix");

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++) 
		glDetachShader(program, shaderList[iLoop]);
	return program;
}

void::GlHelper::InitializeProgram() {
	std::vector<GLuint> shaderList;
	
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "./vertex.shader"));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "./fragment.shader"));
	
	theProgram = CreateProgram(shaderList);

	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
/*	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
*/
}

void GlHelper::InitializeVertexBuffer() {
	//positioning
	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &colorBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIndexes), vertexIndexes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GlHelper::GlHelper() {
	float vertexPositions[] = {
		 0.5f,  0.5f, -1.75, 1.0f,
		-0.5f, -0.5f, -1.75, 1.0f,
		-0.5f,  0.5f, -2.25, 1.0f,
		 0.5f, -0.5f, -2.25, 1.0f,

		-0.5f, -0.5f, -2.25, 1.0f,
		 0.5f,  0.5f, -2.25, 1.0f,
		 0.5f, -0.5f, -1.75, 1.0f,
		-0.5f,  0.5f, -1.75, 1.0f

	};
	
	float vertexColors[]= {
		0,     1, 0, 1,
		0,     0, 1, 1,
		1,     0, 0, 1,
		0.5, 0.5, 0, 1,
		0,     1, 0, 1,
		0,     0, 1, 1,
		1,     0, 0, 1,
		0.5, 0.5, 0, 1
	};
	
	unsigned short vertexIndexes[]= {
		0,1,2,
		1,0,3,
		2,3,0,
		3,2,1,
		5,4,6,
		4,5,7,
		7,6,4,
		6,7,5
	};
	xOffset = 0.0f;
	yOffset = 0.0f;
	origX = 0.0f;
	origY = 0.0f;
	std::copy(vertexPositions, vertexPositions + 8*4, this->vertexPositions);

	std::copy(vertexColors, vertexColors + 8*4, this->vertexColors);
	std::copy(vertexIndexes, vertexIndexes + 8*3, this->vertexIndexes);
	InitializeProgram();
	InitializeVertexBuffer();
	
	float zFar, zNear;
	zFar=3.0f;
	zNear=1.0f;
	memset(perspectiveMatrix, 0, 16 * sizeof(float));
	perspectiveMatrix[0] = 3/4.0f;
	perspectiveMatrix[5] = 1;
	perspectiveMatrix[10] = (zFar + zNear) / (zNear - zFar);
	perspectiveMatrix[11] = 2 * zFar * zNear / (zNear - zFar);
	perspectiveMatrix[14] = -1;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f,1.0f);
}

void GlHelper::render() {
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(theProgram);

	glUniform2f(offsetLocation, xOffset, yOffset);
	glUniformMatrix4fv(perspectiveMatrixLocation, 1, GL_TRUE, perspectiveMatrix);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);	

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferObject);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);	
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);	
	
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	glDrawElements(GL_TRIANGLES, 8*3, GL_UNSIGNED_SHORT, 0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

void GlHelper::move(GlHelper::directions direction) {
	float movementAmount = 0.1f;
	switch(direction) {
		case UP:
			yOffset += movementAmount;	
			break;
		case DOWN:
			yOffset -= movementAmount;	
			break;
		case RIGHT:
			xOffset += movementAmount;	
			break;
		case LEFT:
			xOffset -= movementAmount;	
			break;
	}
}

void GlHelper::mmove(double xOffset, double yOffset) {
	if (origX < xOffset) move(RIGHT);
	if (origX > xOffset) move(LEFT);
	if (origY > yOffset) move(UP);
	if (origY < yOffset) move(DOWN);

	origX = xOffset;
	origY = yOffset;
}
