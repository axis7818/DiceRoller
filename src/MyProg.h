#ifndef _MY_PROG_H
#define _MY_PROG_H

#include <iostream>
#include "Program.h"
#include "MatrixStack.h"
#include "Material.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MyProg {
public:
   MyProg(std::string vertexShaderFile, std::string fragmentShaderFile);
   virtual ~MyProg();

   std::shared_ptr<Program> prog;
   void bind();
   void unbind();
   GLint getUniform(const std::string &name);
   void setMaterial(Material *mat);
   void setModelMatrix(std::shared_ptr<MatrixStack> M);
};

#endif /* end of include guard: _MY_PROG_H */
