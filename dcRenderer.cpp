//Dan Cochran
//Based on https://learnopengl.com/In-Practice/2D-Game/Setting-up

#include "dcRenderer.h"

void dcRender::CircleRenderer::init(int numPoints, Shader* shader) {
	float radius = 1;
	m_shader = shader;
	m_numPoints = numPoints;

	GLuint VBO;
	GLfloat* vertices = new float[numPoints];

	for (int i = 0; i < numPoints; i += 2) {
		float angle = (i * 2 * M_PI) / numPoints - M_PI / 2;
		vertices[i] = cos(angle) * radius;
		vertices[i+1] = sin(angle) * radius;
	}

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numPoints, vertices, GL_STATIC_DRAW);

	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void dcRender::CircleRenderer::draw(glm::vec2 position, float radius, glm::vec3 color) {
	m_shader->activate();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(radius, radius, 1.0f));

	m_shader->SetMatrix4("model", model);
	m_shader->SetVector3f("circleColor", color);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_numPoints);
	glBindVertexArray(0);
}

void dcRender::Shader::compile(const GLchar* vertexSource, const GLchar* fragmentSource) {
	GLuint sVertex, sFragment;
	// Vertex Shader
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	glCompileShader(sVertex);
	// Fragment Shader
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	// Shader Program
	m_id = glCreateProgram();
	glAttachShader(m_id, sVertex);
	glAttachShader(m_id, sFragment);
	glLinkProgram(m_id);
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
}

void dcRender::Shader::loadFromFile(std::string& vertexFile, std::string& fragmentFile) {
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	try
	{
		// Open files
		std::ifstream vertexShaderFile(vertexFile);
		std::ifstream fragmentShaderFile(fragmentFile);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	// 2. Now create shader object from source code
	Shader shader;
	shader.compile(vShaderCode, fShaderCode);
}

void dcRender::Shader::SetMatrix4(const GLchar *name, const glm::mat4 &matrix) {
	glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void dcRender::Shader::SetFloat(const GLchar *name, GLfloat value) {
	glUniform1f(glGetUniformLocation(m_id, name), value);
}

void dcRender::Shader::SetVector3f(const GLchar *name, const glm::vec3 &value) {
	glUniform3f(glGetUniformLocation(m_id, name), value.x, value.y, value.z);
}