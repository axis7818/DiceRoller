#include "Material.h"

using namespace std;
using namespace glm;

Material::Material() {
}

Material::Material(vec3 ambience, float opacity, float roughness, float fresnel, float diffuse) :
   ambience(ambience), opacity(opacity), roughness(roughness), fresnel(fresnel), diffuse(diffuse)
{

}

Material::~Material() {

}

void Material::print() {
   cout << "ambience: " << ambience.x << ", " << ambience.y << ", " << ambience.z << endl;
   cout << "opacity: " << opacity << endl;
   cout << "roughness: " << roughness << endl;
   cout << "fresnel: " << fresnel << endl;
   cout << "diffuse: " << diffuse << endl << endl;
}

float randBtwn(float min, float max) {
   return min + (rand() / (float)RAND_MAX) * (max - min);
}

const int brightCounterparts[] = {COLOR_DARK, COLOR_GREYSCALE_DARK};
const int darkCounterparts[] = {COLOR_BRIGHT, COLOR_GREYSCALE_LIGHT};
const int pastelCounterparts[] = {COLOR_GREYSCALE_LIGHT, COLOR_GREYSCALE_DARK};
const int gsLightCounterparts[] = {COLOR_DARK, COLOR_PASTEL};
const int gsDarkCounterparts[] = {COLOR_GREYSCALE_LIGHT, COLOR_BRIGHT};

vec3 randomColor(float min, float max) {
   float val1 = randBtwn(min, max);
   float val2 = randBtwn(min, max);

   switch (rand() % 6) {
   case 0:
      return vec3(val1, 0, 0);
   case 1:
      return vec3(0, val1, 0);
   case 2:
      return vec3(0, 0, val1);
   case 3:
      return vec3(val1, val2, 0);
   case 4:
      return vec3(val1, 0, val2);
   case 5:
      return vec3(0, val1, val2);
   }
   return vec3(1, 0, 0);
}

vec3 randomPastel() {
   float min = 0.706f;
   float max = 0.784f;
   float val1 = randBtwn(min, max);
   float val2 = randBtwn(min, max);

   switch (rand() % 6) {
   case 0:
      return vec3(val1, 1, 1);
   case 1:
      return vec3(1, val1, 1);
   case 2:
      return vec3(1, 1, val1);
   case 3:
      return vec3(val1, val2, 1);
   case 4:
      return vec3(val1, 1, val2);
   case 5:
      return vec3(1, val1, val2);
   }
   return vec3(1, 1, 1);
}

vec3 randomGreyscale(int type) {
   float val = 0;
   switch (type) {
   case COLOR_GREYSCALE_LIGHT:
      val = randBtwn(0.6f, 0.9f);
      break;
   case COLOR_GREYSCALE_DARK:
      val = randBtwn(0.1f, 0.278f);
      break;
   }
   return vec3(val, val, val);
}

vec3 randomColor(int type) {
   switch (type) {
   case COLOR_BRIGHT:
      return randomColor(0.747f, 0.85f);
   case COLOR_DARK:
      return randomColor(0.054f, 0.476f);
   case COLOR_PASTEL:
      return randomPastel();
   case COLOR_GREYSCALE_LIGHT:
   case COLOR_GREYSCALE_DARK:
      return randomGreyscale(type);
   }
   cout << "returning black..." << endl;
   return vec3(0, 0, 0);
}

float randRoughness() {
   switch(rand() % 3) {
   case 0:
      return randBtwn(0.1f, 0.2f);
   case 1:
      return randBtwn(0.3f, 0.6f);
   case 2:
      return randBtwn(0.75f, 0.9f);
   }
}

void Material::generateRandom(Material &mat1, Material &mat2) {
   // material 1
   int mat1ct = rand() % 5;
   mat1.ambience = randomColor(mat1ct);
   mat1.opacity = 1;
   mat1.roughness = randRoughness();
   mat1.fresnel = randBtwn(0, 1 - mat1.roughness);
   // float diffuse = randBtwn(0, 1);
   // mat1.diffuse = diffuse;
   mat1.diffuse = 1 - mat1.fresnel;

   // material 2
   int mat2ct = rand() % 2;
   switch (mat1ct) {
   case COLOR_BRIGHT:
      mat2ct = brightCounterparts[mat2ct];
      break;
   case COLOR_DARK:
      mat2ct = darkCounterparts[mat2ct];
      break;
   case COLOR_PASTEL:
      mat2ct = pastelCounterparts[mat2ct];
      break;
   case COLOR_GREYSCALE_DARK:
      mat2ct = gsDarkCounterparts[mat2ct];
      break;
   case COLOR_GREYSCALE_LIGHT:
      mat2ct = gsLightCounterparts[mat2ct];
      break;
   }
   mat2.ambience = randomColor(COLOR_BRIGHT);
   mat2.opacity = 1;
   mat2.roughness = randRoughness();
   mat2.fresnel = randBtwn(0, 1);
   mat2.diffuse = mat1.diffuse;
}
