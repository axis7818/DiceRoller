#ifndef _ROLL_H
#define _ROLL_H

#include <iostream>
#include "Bounce.h"
#include "MatrixStack.h"
#include "Dice.h"
#include "Face.h"

class Roll {
   Dice *dice;
   Material *material;
   Material *dotMaterial;
   Face *target;
   glm::vec3 startPos;
   glm::vec3 endPos;
   std::vector<Bounce*> bounces;
   float startTime = -1;
   float totalTime;
   float targetYRot;
   float startYRot;
   float startTheta;
   float startPhi;

public:
   Roll(Dice *dice, Material *material, Material *dotMaterial,
    glm::vec3 startPos, glm::vec3 endPos,
    int bounceCount, glm::vec2 xBounds, glm::vec2 zBounds);
   virtual ~Roll();

   void draw(std::shared_ptr<Program> prog,
      std::shared_ptr<MatrixStack> M);
   void start();
   int getValue() const { return target->value; }
};

#endif /* end of include guard: _ROLL_H */
