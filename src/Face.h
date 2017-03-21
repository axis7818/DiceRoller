#ifndef _FACE_H
#define _FACE_H

#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Shape.h"

class Face {
   void initWithNormal(glm::vec3 normal);

public:
   Face(const int value, glm::vec3 normal);
   Face(const int value, const float targetTheta, const float targetPhi);
   Face(const int value, const float mintx, const float maxtx,
    const float minty, const float maxty, std::shared_ptr<Shape> shape, bool flip);
   virtual ~Face();

   int getValue() const { return value; }

   const int value;
   // glm::vec3 normal;
   float targetTheta;
   float targetPhi;
};

#endif /* end of include guard: _FACE_H */
