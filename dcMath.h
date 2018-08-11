#pragma once
#include "SFML\System.hpp"
#include <cmath>
#include "Globals.h"

namespace dcMath {
	float Magnitude(const glm::vec2& vector);

	glm::vec2 Normalize(glm::vec2 vector);

	void Limit(glm::vec2& vector, float limit);

	float Dot(glm::vec2 v1, glm::vec2 v2);

	float Dot(sf::Vector3f v1, sf::Vector3f v2);

	float AngleBetween(glm::vec2 v1, glm::vec2 v2);

	float Heading(const glm::vec2& vector);

	float Map(float value, float start1, float  stop1, float  start2, float stop2);
}