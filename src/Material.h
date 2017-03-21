#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define COLOR_BRIGHT 0
#define COLOR_DARK 1
#define COLOR_PASTEL 2
#define COLOR_GREYSCALE_LIGHT 3
#define COLOR_GREYSCALE_DARK 4

class Material {
public:
   Material();
   Material(glm::vec3 ambience, float opacity, float roughness,
    float fresnel, float diffuse);
   virtual ~Material();

   void print();
   static void generateRandom(Material &mat1, Material &mat2);

   glm::vec3 ambience;
   float opacity;
   float roughness; // 0->smooth, 1->rough
   float fresnel; // fresnel reflectance at normal incidence 0->1
   float diffuse; // fraction of diffuse reflection 0->1
};

#endif /* end of include guard: _MATERIAL_H */
