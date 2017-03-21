#include "Bounce.h"

using namespace std;
using namespace glm;

Bounce::Bounce(vec3 start, vec3 end, float h, float t) :
   start(start), end(end), h(h), t(t)
{
   mid = vec3((start.x + end.x) / 2.0f, h, (start.z + end.z) / 2.0f);
}

Bounce::~Bounce() {

}

vec3 Bounce::getPos(float time) {
   // TODO: use a parametric equation for a 3D parabola
   vec3 a = time < t / 2.0f ? start : mid;
   vec3 b = time < t / 2.0f ? mid : end;
   time = time > t / 2.0f ? time - t / 2.0f : time;

   return a + (2.0f * time / t) * (b - a);
}
