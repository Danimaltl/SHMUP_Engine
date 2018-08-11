#include "dcMath.h"

float dcMath::Magnitude(const glm::vec2& vector) {
	return sqrtf((vector.x * vector.x) + (vector.y * vector.y));
}

glm::vec2 dcMath::Normalize(glm::vec2 vector) {
	float length = Magnitude(vector);

	if (length == 0) return glm::vec2(0, 0);

	vector.x = vector.x / length;
	vector.y = vector.y / length;

	return vector;
}

void dcMath::Limit(glm::vec2& vector, float limit) {
	if (Magnitude(vector) > limit) {
		vector = Normalize(vector);
		vector.x *= limit;
		vector.y *= limit;
	}
}

float dcMath::Dot(glm::vec2 v1, glm::vec2 v2) {
	return v1.x*v2.x + v1.y*v2.y;
}

float dcMath::Dot(sf::Vector3f v1, sf::Vector3f v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

float dcMath::AngleBetween(glm::vec2 v1, glm::vec2 v2) {
	return acos(Dot(v1, v2) / (Magnitude(v1) * Magnitude(v2)));
}

float dcMath::Heading(const glm::vec2& vector) {
	float result = atan2(vector.y, vector.x);
	result = result * (float)(180 / M_PI);
	return result;
}

float dcMath::Map(float value, float start1, float  stop1, float  start2, float stop2) {
	float slope = (stop2 - start2) / (stop1 - start1);
	return start2 + slope * (value - start1);
}