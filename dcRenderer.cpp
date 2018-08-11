//Dan Cochran
//Based on https://learnopengl.com/In-Practice/2D-Game/Setting-up
#include "dcRenderer.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // !M_PI

void dcRender::CircleRenderer::init(int numPoints, Shader* shader) {
	float radius = 1;
	m_shader = shader;
	m_numPoints = numPoints;
	m_vertArraySize = numPoints * 2;

	GLuint VBO;
	GLfloat* vertices = new float[m_vertArraySize];

	glm::mat4 model(1);
	model = glm::translate(model, glm::vec3(200, 200, 0.0f));
	model = glm::scale(model, glm::vec3(100, 100, 1.0f));

	for (int i = 0; i < numPoints * 2; i += 2) {
		float angle = (float)i * 2.0f * (float)M_PI / (float)numPoints;
		vertices[i] = cos(angle) * radius;
		vertices[i + 1] = sin(angle) * radius;

		glm::vec4 vert = model * glm::vec4(vertices[i], vertices[i + 1], 0.0f, 1.0f);
		printf("Vert%d: x:%f, y:%f\n", i, vert.x, vert.y);
	}

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_vertArraySize, vertices, GL_STATIC_DRAW);

	glBindVertexArray(m_VAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	delete vertices;
	vertices = nullptr;
}

void dcRender::CircleRenderer::draw(glm::vec2 position, float rotation, float radius, glm::vec3 color) {
	m_shader->use();
	glm::mat4 model(1);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(radius, radius, 1.0f));

	m_shader->SetMatrix4("model", model);
	m_shader->SetVector3f("circleColor", color);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_numPoints);
	glBindVertexArray(0);
}

void dcRender::PolyRenderer::init(GLfloat* points, int size, glm::vec2 center, GLuint drawMethod, Shader* shader) {
	float radius = 1;
	m_shader = shader;
	m_vertArraySize = size;
	m_drawMethod = drawMethod;
	m_center = center;

	GLuint VBO;
	GLfloat* vertices = points;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*m_vertArraySize, vertices, GL_STATIC_DRAW);

	glBindVertexArray(m_VAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void dcRender::PolyRenderer::draw(glm::vec2 position, float rotation, glm::vec2 scale, glm::vec3 color) {
	m_shader->use();
	glm::mat4 model(1);

	model = glm::translate(model, glm::vec3(position, 0.0f));

	//model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::translate(model, glm::vec3(m_center.x * scale.x, m_center.y * scale.y, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-m_center.x * scale.x, -m_center.y * scale.y, 0.0f));
	//model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(scale, 1.0f));

	m_shader->SetMatrix4("model", model);
	m_shader->SetVector3f("circleColor", color);

	glBindVertexArray(m_VAO);
	glDrawArrays(m_drawMethod, 0, m_numPoints);
	glBindVertexArray(0);
}

dcRender::Shader::Shader() {

}

void dcRender::Shader::compile(const GLchar* vertexSource, const GLchar* fragmentSource) {
	GLuint sVertex, sFragment;
	// Vertex Shader
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	glCompileShader(sVertex);
	checkCompileErrors(sVertex, "VERTEX");
	// Fragment Shader
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");
	// Shader Program
	m_id = glCreateProgram();
	glAttachShader(m_id, sVertex);
	glAttachShader(m_id, sFragment);
	glLinkProgram(m_id);
	checkCompileErrors(m_id, "PROGRAM");
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	printf("Shader probably compiled!\n");
}

void dcRender::Shader::loadFromFile(const char* vertexFile, const char* fragmentFile) {
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
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	// 2. Now create shader object from source code
	this->compile(vShaderCode, fShaderCode);
}

void dcRender::Shader::checkCompileErrors(GLuint object, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}

void dcRender::Shader::SetMatrix4(const GLchar *name, const glm::mat4 &matrix) {
	GLuint uniform = glGetUniformLocation(m_id, name);
	if (uniform == -1) {
		printf("Couldn't find mat4 uniform: %s\n", name);
	}
	glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
}

void dcRender::Shader::SetFloat(const GLchar *name, GLfloat value) {
	GLuint uniform = glGetUniformLocation(m_id, name);
	if (uniform == -1) {
		printf("Couldn't find mat4 uniform: %s\n", name);
	}
	glUniform1f(uniform, value);
}

void dcRender::Shader::SetVector3f(const GLchar *name, const glm::vec3 &value) {
	GLuint uniform = glGetUniformLocation(m_id, name);
	if (uniform == -1) {
		printf("Couldn't find mat4 uniform: %s\n", name);
	}
	glUniform3f(uniform, value.x, value.y, value.z);
}