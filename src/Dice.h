#ifndef _DICE_H
#define _DICE_H

#include <cassert>
#include <iostream>
#include <time.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shape.h"
#include "Texture.h"
#include "Program.h"
#include "Material.h"
#include "MatrixStack.h"
#include "Face.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define D_20 "d20"
#define D_10 "d10"
#define D_12 "d12"
#define D_8 "d8"
#define D_4 "d4"
#define D_6 "d6"
#define D_00 "d00"

class Dice {
   std::string name;
   std::shared_ptr<Shape> shape;
   std::vector<Face*> faces;
   std::shared_ptr<Texture> tex;

   void initFaces();

public:
   Dice(std::string name, const std::string &objFile,
    int shapeIndex, const std::string &texFile);
   virtual ~Dice();

   std::string getName() const { return name; }

   // void setMaterial(std::shared_ptr<Program> prog, int clipMode);
   void render(const std::shared_ptr<Program> prog,
    std::shared_ptr<MatrixStack> M);
   Face *randomFace() const;
   void bindTexture(const std::shared_ptr<Program> prog);
};

#endif /* end of include guard: _DICE_H */
