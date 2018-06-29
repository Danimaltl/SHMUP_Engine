#include <iostream>
#include "Easings.h";

using namespace std;

/*Based on Robert Penner's easing functions: http://robertpenner.com/easing/ */

//Starting value, target value, percent complete
float lerp(float t, float b, float c, float d) {
	return c * t / d + b;
}

float lerp(float startPos, float targetPos, float progress) {
	if ((1 - progress) < .01) return targetPos;
	return (1 - progress) * startPos + progress * targetPos;
}

float smoothStep(float x) {
	return ((x) * (x) * (3 - 2 * (x)));
}

float wAverage(float x) {
	return ((x * 19) + 1) / 20;
}

float customTween(float startPos, float targetPos, float progress) {
	return lerp(startPos,targetPos, progress);
}

float bounceEaseOut(float t, float b, float c, float d) {
	if ((t /= d) < (1 / 2.75f)) {
		return c*(7.5625f*t*t) + b;
	}
	else if (t < (2 / 2.75f)) {
		float postFix = t -= (1.5f / 2.75f);
		return c*(7.5625f*(postFix)*t + .75f) + b;
	}
	else if (t < (2.5 / 2.75)) {
		float postFix = t -= (2.25f / 2.75f);
		return c*(7.5625f*(postFix)*t + .9375f) + b;
	}
	else {
		float postFix = t -= (2.625f / 2.75f);
		return c*(7.5625f*(postFix)*t + .984375f) + b;
	}
}

float bounceEaseIn(float t, float b, float c, float d) {
	return c - bounceEaseOut(d - t, 0, c, d) + b;
}

float bounceEaseInOut(float t, float b, float c, float d) {
	if (t < d / 2) return bounceEaseIn(t * 2, 0, c, d) * .5f + b;
	else return bounceEaseOut(t * 2 - d, 0, c, d) * .5f + c*.5f + b;
}

float circEaseIn(float t, float b, float c, float d) {
	return c * (1 - sqrt(1 - (t /= d)*t)) + b;
}
float circEaseOut(float t, float b, float c, float d) {
	return c * sqrt(1 - (t = t / d - 1)*t) + b;
}

float circEaseInOut(float t, float b, float c, float d) {
	if ((t /= d / 2) < 1) return -c / 2 * (sqrt(1 - t*t) - 1) + b;
	return c / 2 * (sqrt(1 - t*(t -= 2)) + 1) + b;
}

float elasticEaseIn(float t, float b, float c, float d) {
	if (t == 0) return b;  if ((t /= d) == 1) return b + c;
	float p = d*.3f;
	float a = c;
	float s = p / 4;
	float postFix = a*pow(2, 10 * (t -= 1)); // this is a fix, again, with post-increment operators
	return -(postFix * sin((t*d - s)*(2 * PI) / p)) + b;
}

float elasticEaseOut(float t, float b, float c, float d) {
	if (t == 0) return b;  if ((t /= d) == 1) return b + c;
	float p = d*.3f;
	float a = c;
	float s = p / 4;
	return (a*pow(2, -10 * t) * sin((t*d - s)*(2 * PI) / p) + c + b);
}

float elasticEaseInOut(float t, float b, float c, float d) {
	if (t == 0) return b;  if ((t /= d / 2) == 2) return b + c;
	float p = d*(.3f*1.5f);
	float a = c;
	float s = p / 4;

	if (t < 1) {
		float postFix = a*pow(2, 10 * (t -= 1)); // postIncrement is evil
		return -.5f*(postFix* sin((t*d - s)*(2 * PI) / p)) + b;
	}
	float postFix = a*pow(2, -10 * (t -= 1)); // postIncrement is evil
	return postFix * sin((t*d - s)*(2 * PI) / p)*.5f + c + b;
}