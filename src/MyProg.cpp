#include "MyProg.h"

using namespace std;

MyProg::MyProg(string vertexShaderFile, string fragmentShaderFile) {
   prog = make_shared<Program>();
   prog->setVerbose(true);
   prog->setShaderNames(vertexShaderFile, fragmentShaderFile);
   prog->init();
   prog->addUniform("P");
   prog->addUniform("M");
   prog->addUniform("V");

   prog->addUniform("lightPos");
   prog->addUniform("lightColor");

   prog->addUniform("MatAmb");
   prog->addUniform("opacity");
   prog->addUniform("roughnessValue");
   prog->addUniform("F0");
   prog->addUniform("K");

   prog->addUniform("Texture");
   prog->addUniform("clipMode");

   prog->addAttribute("vertPos");
   prog->addAttribute("vertNor");
   prog->addAttribute("vertTex");
}

MyProg::~MyProg() {

}

void MyProg::bind() {
   prog->bind();
}

void MyProg::unbind() {
   prog->unbind();
}

GLint MyProg::getUniform(const std::string &name) {
   return prog->getUniform(name);
}

void MyProg::setModelMatrix(std::shared_ptr<MatrixStack> M) {
   glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));

}

void MyProg::setMaterial(Material *mat) {
   glUniform3f(prog->getUniform("MatAmb"), mat->ambience.x, mat->ambience.y, mat->ambience.z);
   glUniform1f(prog->getUniform("opacity"), mat->opacity);
   glUniform1f(prog->getUniform("roughnessValue"), mat->roughness);
   glUniform1f(prog->getUniform("F0"), mat->fresnel);
   glUniform1f(prog->getUniform("K"), mat->diffuse);
}
