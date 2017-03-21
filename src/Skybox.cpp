#include "Skybox.h"

using namespace std;

Skybox::Skybox (const string &boxFile, const string &texture,
 const int texUnit, const float scale) :
   scale(scale)
{
   // load the shape
   box = make_shared<Shape>();
   box->loadMesh(boxFile);
   box->resize();
   box->init();

   // load the texture
   tex = make_shared<Texture>();
   tex->setFilename(texture);
   tex->init();
   tex->setUnit(texUnit);
   tex->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

Skybox::~Skybox() {

}

void Skybox::render(shared_ptr<Program> prog, shared_ptr<MatrixStack> M) {
   tex->bind(prog->getUniform("Texture"));

   M->pushMatrix();

   M->scale(scale);
   glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));

   box->draw(prog);

   M->popMatrix();
}
