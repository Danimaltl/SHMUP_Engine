//Dan Cochran
//Based on https://learnopengl.com/In-Practice/2D-Game/Setting-up

#pragma once
#include "Globals.h"

namespace dcRender {
	class Shader;

	class CircleRenderer {
	public:
		CircleRenderer();
		~CircleRenderer();
		void init(int numPoints, Shader* shader);
		void draw(glm::vec2 position, float rotation, float radius, glm::vec3 color);
	private:
		GLuint m_VAO;
		Shader* m_shader = nullptr;
		int m_numPoints;
		int m_vertArraySize;
	};

	class PolyRenderer {
	public:
		PolyRenderer();
		~PolyRenderer();
		void init(GLfloat* points, int size, glm::vec2 center, GLuint drawMethod, Shader* shader);
		void draw(glm::vec2 position, float rotation, glm::vec2 scale, glm::vec3 color);
	private:
		GLuint m_VAO;
		Shader* m_shader = nullptr;
		int m_numPoints;
		int m_vertArraySize;
		GLuint m_drawMethod;
		glm::vec2 m_center;
	};

	class Shader {
	public:
		const glm::mat4 projection = glm::ortho(0.0f, 600.0f, 800.0f, 0.0f, -1.0f, 1.0f);
		Shader();
		~Shader();
		void use() { glUseProgram(m_id); }
		void compile(const GLchar* vertexSource, const GLchar* fragmentSource);
		void loadFromFile(const char* vertexFile, const char* fragmentFile);

		void SetMatrix4(const GLchar *name, const glm::mat4 &matrix);
		void SetFloat(const GLchar *name, GLfloat value);
		void SetVector3f(const GLchar *name, const glm::vec3 &value);

		GLuint getID() { return m_id; }
	private:
		void checkCompileErrors(GLuint object, std::string type);
		GLuint m_id;
	};
}
