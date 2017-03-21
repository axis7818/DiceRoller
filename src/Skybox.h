#ifndef _SKYBOX_H
#define _SKYBOX_H

#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shape.h"
#include "Texture.h"
#include "Program.h"
#include "MatrixStack.h"

class Skybox {
   std::shared_ptr<Shape> box;
   std::shared_ptr<Texture> tex;
   const float scale;

public:
   Skybox(const std::string &boxFile, const std::string &texture,
    const int texUnit, const float scale);
   virtual ~Skybox();

   void render(std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> M);
};

#endif /* end of include guard: _SKYBOX_H */
