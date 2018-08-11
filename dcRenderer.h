//Dan Cochran
//Based on https://learnopengl.com/In-Practice/2D-Game/Setting-up

#pragma once
#include "Globals.h"

namespace dcRender {
	class Shader;

	class CircleRenderer {
	public:
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
		void init(const std::vector<glm::vec2>& points, glm::vec2 center, GLuint drawMethod, Shader* shader);
		void draw(glm::vec2 position, float rotation, glm::vec2 size, glm::vec3 color);
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
		Shader();
		void activate() { glUseProgram(m_id); }
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
