#ifndef _BOUNCE_H
#define _BOUNCE_H

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Bounce {
public:
   Bounce(glm::vec3 start, glm::vec3 end, float h, float t);
   ~Bounce();

   glm::vec3 getPos(float time);

   float h;
   float t;
   glm::vec3 start;
   glm::vec3 mid;
   glm::vec3 end;
};

#endif /* end of include guard: _BOUNCE_H */
