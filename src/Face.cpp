#include "Face.h"

using namespace std;
using namespace glm;

Face::Face(const int value, const float targetTheta, const float targetPhi) :
   value(value), targetTheta(targetTheta), targetPhi(targetPhi)
{
}

Face::Face(const int value, vec3 normal) :
   value(value)
{
   initWithNormal(normal);
}

Face::Face(const int value, const float mintx, const float maxtx, const float minty,
 const float maxty, shared_ptr<Shape> shape, bool flip) :
   value(value)
{
   bool found = false;
   size_t vndx = -1;
   for (size_t v = 0; !found && v < shape->texBuf.size(); ++v) {
      float texx = shape->texBuf[2 * v + 0];
      float texy = shape->texBuf[2 * v + 1];
      found = mintx <= texx && texx <= maxtx && minty <= texy && texy <= maxty;
      if (found) vndx = v;
   }
   assert(found);
   float normx = shape->norBuf[3 * vndx + 0];
   float normy = shape->norBuf[3 * vndx + 1];
   float normz = shape->norBuf[3 * vndx + 2];
   vec3 normal = flip ? vec3(-normx, -normy, -normz) : vec3(normx, normy, normz);
   initWithNormal(normal);
}

Face::~Face() {

}

void Face::initWithNormal(vec3 normal) {
   float h = sqrt(pow(normal.x, 2) + pow(normal.z, 2));
   float r = sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));
   float curTheta, curPhi;
   curTheta = asin(normal.z / h);
   if (isnan(curTheta)) curTheta = 0;
   curPhi = asin(normal.y / r);
   // cout << "target: " << value << ", curTheta: " << curTheta
   //  << ", curPhi: " << curPhi << endl;
   targetTheta = (normal.x < 0 ? -1 : 1) * (M_PI/2 - curTheta);
   targetPhi = M_PI/2 - curPhi;
   // cout << "           targetTheta: " << targetTheta
   //  << ", targetPhi: " << targetPhi << endl << endl;
}
