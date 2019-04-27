#pragma once

#include <cmath>

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif

/*
Easing function parameters

Penner uses:
t = current time
b = beginning value
c = change between beginning and destination
d = total time of the tween

Unity/Lerping uses
startPos = starting position
targetPos = target position
progress = fraction of the journey completed this frame based on speed

progress = (speed * dt) / distance

dt = t
d = distance/speed

progress = t/d

c = abs(startPos - targetPos)

b = startPos

*/

//Easing functions
//Starting value, target value, percent complete
float lerp(float t, float b, float c, float d);
float lerp(float startPos, float targetPos, float progress);
float customTween(float startPos, float targetPos, float progress);

float bounceEaseIn(float t, float b, float c, float d);
float bounceEaseOut(float t, float b, float c, float d);
float bounceEaseInOut(float t, float b, float c, float d);

float circEaseIn(float t, float b, float c, float d);
float circEaseOut(float t, float b, float c, float d);
float circEaseInOut(float t, float b, float c, float d);

float elasticEaseIn(float t, float b, float c, float d);
float elasticEaseOut(float t, float b, float c, float d);
float elasticEaseInOut(float t, float b, float c, float d);