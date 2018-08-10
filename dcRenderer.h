//Dan Cochran
//Based on https://learnopengl.com/In-Practice/2D-Game/Setting-up

#pragma once
#include "Globals.h";

namespace dcRender {
	class Shader;

	class CircleRenderer {
	public:
		void init(int numPoints, Shader* shader);
		void draw(glm::vec2 position, float radius, glm::vec3 color);
	private:
		GLuint m_VAO;
		Shader* m_shader = nullptr;
		int m_numPoints;
	};

	class Shader {
	public:
		void activate() { glUseProgram(m_id); }
		void compile(const GLchar* vertexSource, const GLchar* fragmentSource);
		void loadFromFile(std::string& vertexFile, std::string& fragmentFile);

		void SetMatrix4(const GLchar *name, const glm::mat4 &matrix);
		void SetFloat(const GLchar *name, GLfloat value);
		void SetVector3f(const GLchar *name, const glm::vec3 &value);

		GLuint getID() { return m_id; }
	private:
		GLuint m_id;
	};
}
