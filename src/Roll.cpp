#include "Roll.h"

using namespace std;

Roll::Roll(Dice *dice, Material *material, Material *dotMaterial,
 vec3 startPos, vec3 endPos, int bounceCount,
 vec2 xBounds, vec2 zBounds) :
   dice(dice), material(material), dotMaterial(dotMaterial),
   startPos(startPos), endPos(endPos)
{
   target = dice->randomFace();
   cout << "rolling " << target->getValue() << " on a " <<
    dice->getName() << endl;

   totalTime = 0;
   for (int i = 0; i < bounceCount; ++i) {
      vec3 start = i == 0 ? startPos : bounces[i - 1]->end;
      vec3 end = i == bounceCount - 1 ? endPos :
       vec3(xBounds.x + ((float)rand() / RAND_MAX) *
       (xBounds.y - xBounds.x), endPos.y, zBounds.x +
       ((float)rand() / RAND_MAX) * (zBounds.y - zBounds.x));

      float h = (1 - i / (float)bounceCount) * startPos.y;
      float t = (bounceCount - i) / (float)bounceCount;

      Bounce *b = new Bounce(start, end, h, t);
      bounces.push_back(b);
      totalTime += t;
   }

   targetYRot = (rand() / (float)RAND_MAX) * 2.0f * M_PI;
   startYRot = (rand() / (float)RAND_MAX) * 2.0f * M_PI;
   startTheta = (4.0f * M_PI) + (rand() / (float)RAND_MAX) *
    (4.0f * M_PI);
   startPhi = (4.0f * M_PI) + (rand() / (float)RAND_MAX) *
    (4.0f * M_PI);
}

Roll::~Roll() {
   for (int i = 0; i < bounces.size(); ++i) {
      delete bounces[i];
   }
   delete material;
   delete dotMaterial;
}

void setMaterial(shared_ptr<Program> prog, Material *mat, int clipMode) {
   glUniform3f(prog->getUniform("MatAmb"), mat->ambience.x,
    mat->ambience.y, mat->ambience.z);
   glUniform1f(prog->getUniform("opacity"), mat->opacity);
   glUniform1f(prog->getUniform("roughnessValue"), mat->roughness);
   glUniform1f(prog->getUniform("F0"), mat->fresnel);
   glUniform1f(prog->getUniform("K"), mat->diffuse);
   glUniform1i(prog->getUniform("clipMode"), clipMode);
}

void Roll::draw(shared_ptr<Program> prog, shared_ptr<MatrixStack> M) {
   if (startTime == -1) {
      cerr << "MUST START THE ROLL BEFORE DRAWING!!!" << endl;
      return;
   }
   float t = glfwGetTime() - startTime;

   // get the current bounce
   Bounce *b = NULL;
   float accTime = 0;
   for (int i = 0; i < bounces.size() && b == NULL; ++i) {
      if (t < accTime + bounces[i]->t)
         b = bounces[i];
      accTime += bounces[i]->t;
   }

   // determine the dice's position and rotation
   vec3 dicePos;
   float theta;
   float phi;
   float yRot;
   if (b == NULL) {
      dicePos = endPos;
      theta = target->targetTheta;
      phi = target->targetPhi;
      yRot = targetYRot;
   } else {
      dicePos = b->getPos(t - accTime + b->t);
      theta = startTheta + (t / totalTime) *
       (target->targetTheta - startTheta);
      phi = startPhi + (t / totalTime) *
       (target->targetPhi - startPhi);
      yRot = startYRot + (t / totalTime) *
       (targetYRot - startYRot);
   }

   // draw the dice at the correct position
   M->pushMatrix();
   M->translate(dicePos);
   M->rotate(yRot, vec3(0, 1, 0));
   M->rotate(phi, vec3(1, 0, 0));
   M->rotate(theta, vec3(0, 1, 0));

   dice->bindTexture(prog);

   // material 1
   setMaterial(prog, material, 2);
   dice->render(prog, M);

   // material 2
   setMaterial(prog, dotMaterial, 1);
   dice->render(prog, M);

   M->popMatrix();
}

void Roll::start() {
   startTime = glfwGetTime();
}
