#include "Dice.h"

using namespace std;
using namespace glm;

Dice::Dice(string name, const string &objFile, int shapeIndex, const string &texFile) :
   name(name)
{
   // load the mesh
   shape = make_shared<Shape>();
   shape->loadMesh(objFile);
   shape->resize();
   shape->init();

   // load the texture
   tex = make_shared<Texture>();
   tex->setFilename(texFile);
   tex->init();
   tex->setUnit(shapeIndex);
   tex->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

   initFaces();
   assert(faces.size() >= 2); // must be at least 2 faces
}

Dice::~Dice() {
   for (int i = 0; i < faces.size(); ++i) {
      delete faces[i];
   }
}

void Dice::bindTexture(const shared_ptr<Program> prog) {
   tex->bind(prog->getUniform("Texture"));
}

void Dice::render(const shared_ptr<Program> prog,
 shared_ptr<MatrixStack> M) {
   M->pushMatrix();
   glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE,
    value_ptr(M->topMatrix()));
   shape->draw(prog);
   M->popMatrix();
}

Face *Dice::randomFace() const {
   const int index = rand() % faces.size();
   return faces[index];
}

void Dice::initFaces() {
   if (name == D_6) {
      faces.push_back(new Face(1, 0.6347f, 0.6836f, 0.7607f, 0.9121f, shape, false));
      faces.push_back(new Face(2, 0.6347f, 0.6836f, 0.4385f, 0.5752f, shape, false));
      faces.push_back(new Face(3, 0.2930f, 0.3516f, 0.0879f, 0.2344f, shape, false));
      faces.push_back(new Face(4, 0.6347f, 0.6836f, 0.0879f, 0.2344f, shape, false));
      faces.push_back(new Face(5, 0.2930f, 0.3516f, 0.4385f, 0.5752f, shape, false));
      faces.push_back(new Face(6, 0.2930f, 0.3516f, 0.7607f, 0.9121f, shape, false));
   }

   else if (name == D_4) {
      faces.push_back(new Face(1, 0.5127f, 0.6445f, 0.7559f, 0.9023f, shape, true));
      faces.push_back(new Face(2, 0.6103f, 0.8056f, 0.3408f, 0.4628f, shape, true));
      faces.push_back(new Face(3, 0.1563f, 0.3516f, 0.1211f, 0.2676f, shape, true));
      faces.push_back(new Face(4, 0.17578125, 0.3662109375, 0.619140625, 0.765625, shape, true));
   }

   else if (name == D_8) {
      faces.push_back(new Face(1, 0.439453125, 0.48828125, 0.6826171875, 0.7607421875, shape, false));
      faces.push_back(new Face(2, 0.6884765625, 0.7373046875, 0.43359375, 0.501953125, shape, false));
      faces.push_back(new Face(3, 0.2099609375, 0.3125, 0.873046875, 0.921875, shape, false));
      faces.push_back(new Face(4, 0.5029296875, 0.54931640625, 0.248046875, 0.3212890625, shape, false));
      faces.push_back(new Face(5, 0.068359375, 0.107421875, 0.6875, 0.75341796875, shape, false));
      faces.push_back(new Face(6, 0.6591796875, 0.7421875, 0.08203125, 0.1259765625, shape, false));
      faces.push_back(new Face(7, 0.25390625, 0.2978515625, 0.462890625, 0.58984375, shape, false));
      faces.push_back(new Face(8, 0.8740234375, 0.9326171875, 0.2529296875, 0.31640625, shape, false));
   }

   else if (name == D_10) {
      faces.push_back(new Face(0, 0.8642578125, 0.927734375, 0.306640625, 0.404296875, shape, false));
      faces.push_back(new Face(1, 0.0732421875, 0.1171875, 0.609375, 0.7021484375, shape, false));
      faces.push_back(new Face(2, 0.4638671875, 0.52734375, 0.306640625, 0.404296875, shape, false));
      faces.push_back(new Face(3, 0.6640625, 0.7177734375, 0.609375, 0.7021484375, shape, false));
      faces.push_back(new Face(4, 0.068359375, 0.126953125, 0.306640625, 0.404296875, shape, false));
      faces.push_back(new Face(5, 0.46875, 0.517578125, 0.609375, 0.7021484375, shape, false));
      faces.push_back(new Face(6, 0.2685546875, 0.3271484375, 0.306640625, 0.404296875, shape, false));
      faces.push_back(new Face(7, 0.8642578125, 0.9326171875, 0.609375, 0.7021484375, shape, false));
      faces.push_back(new Face(8, 0.6591796875, 0.72265625, 0.306640625, 0.404296875, shape, false));
      faces.push_back(new Face(9, 0.2685546875, 0.322265625, 0.609375, 0.7021484375, shape, false));
   }

   else if (name == D_12) {
      faces.push_back(new Face(1, 0.3857421875, 0.4638671875, 0.84375, 0.931640625, shape, false));
      faces.push_back(new Face(2, 0.732421875, 0.8056640625, 0.35546875, 0.404296875, shape, false));
      faces.push_back(new Face(3, 0.72265625, 0.8203125, 0.1455078125, 0.208984375, shape, false));
      faces.push_back(new Face(4, 0.1708984375, 0.25390625, 0.78515625, 0.873046875, shape, false));
      faces.push_back(new Face(5, 0.3173828125, 0.4052734375, 0.7021484375, 0.74609375, shape, false));
      faces.push_back(new Face(6, 0.498046875, 0.595703125, 0.7021484375, 0.74609375, shape, false));
      faces.push_back(new Face(7, 0.4052734375, 0.4931640625, 0.2626953125, 0.3017578125, shape, false));
      faces.push_back(new Face(8, 0.5322265625, 0.595703125, 0.404296875, 0.4775390625, shape, false));
      faces.push_back(new Face(9, 0.3955078125, 0.4638671875, 0.5068359375, 0.599609375, shape, false));
      faces.push_back(new Face(10, 0.17578125, 0.2587890625, 0.58984375, 0.658203125, shape, false));
      faces.push_back(new Face(11, 0.52734375, 0.6005859375, 0.072265625, 0.16015625, shape, false));
      faces.push_back(new Face(12, 0.5810546875, 0.6640625, 0.2431640625, 0.31640625, shape, false));
   }

   else if (name == D_20) {
      faces.push_back(new Face(1, 0.4296875, 0.478515625, 0.51171875, 0.55078125, shape, false));
      faces.push_back(new Face(2, 0.791015625, 0.83984375, 0.51171875, 0.55078125, shape, false));
      faces.push_back(new Face(3, 0.2490234375, 0.2978515625, 0.51171875, 0.55078125, shape, false));
      faces.push_back(new Face(4, 0.6982421875, 0.751953125, 0.3408203125, 0.3896484375, shape, false));
      faces.push_back(new Face(5, 0.6103515625, 0.6591796875, 0.51171875, 0.5556640625, shape, false));
      faces.push_back(new Face(6, 0.15625, 0.205078125, 0.3408203125, 0.3896484375, shape, false));
      faces.push_back(new Face(7, 0.4150390625, 0.4833984375, 0.609375, 0.6826171875, shape, false));
      faces.push_back(new Face(8, 0.05859375, 0.1171875, 0.51171875, 0.5556640625, shape, false));
      faces.push_back(new Face(9, 0.3466796875, 0.380859375, 0.326171875, 0.3896484375, shape, false));
      faces.push_back(new Face(10, 0.05859375, 0.1171875, 0.609375, 0.66796875, shape, false));
      faces.push_back(new Face(11, 0.5126953125, 0.56640625, 0.345703125, 0.3896484375, shape, false));
      faces.push_back(new Face(12, 0.791015625, 0.83984375, 0.619140625, 0.6630859375, shape, false));
      faces.push_back(new Face(13, 0.5126953125, 0.5712890625, 0.4482421875, 0.4970703125, shape, false));
      faces.push_back(new Face(14, 0.87890625, 0.9375, 0.3505859375, 0.3896484375, shape, false));
      faces.push_back(new Face(15, 0.6103515625, 0.6591796875, 0.619140625, 0.6630859375, shape, false));
      faces.push_back(new Face(16, 0.146484375, 0.205078125, 0.4482421875, 0.4970703125, shape, false));
      faces.push_back(new Face(17, 0.25390625, 0.29296875, 0.619140625, 0.66796875, shape, false));
      faces.push_back(new Face(18, 0.693359375, 0.751953125, 0.4482421875, 0.5068359375, shape, false));
      faces.push_back(new Face(19, 0.33203125, 0.390625, 0.4482421875, 0.501953125, shape, false));
      faces.push_back(new Face(20, 0.87890625, 0.9375, 0.443359375, 0.4970703125, shape, false));
   }

   else if (name == D_00) {
      faces.push_back(new Face(0, 0.8544921875, 0.9326171875, 0.306640625, 0.3896484375, shape, false));
      faces.push_back(new Face(10, 0.0732421875, 0.13671875, 0.609375, 0.6923828125, shape, false));
      faces.push_back(new Face(20, 0.458984375, 0.5322265625, 0.306640625, 0.3896484375, shape, false));
      faces.push_back(new Face(30, 0.6591796875, 0.732421875, 0.609375, 0.6923828125, shape, false));
      faces.push_back(new Face(40, 0.05859375, 0.1416015625, 0.306640625, 0.3896484375, shape, false));
      faces.push_back(new Face(50, 0.458984375, 0.537109375, 0.609375, 0.6923828125, shape, false));
      faces.push_back(new Face(60, 0.25390625, 0.341796875, 0.306640625, 0.3896484375, shape, false));
      faces.push_back(new Face(70, 0.8544921875, 0.927734375, 0.609375, 0.6923828125, shape, false));
      faces.push_back(new Face(80, 0.6591796875, 0.732421875, 0.306640625, 0.3896484375, shape, false));
      faces.push_back(new Face(90, 0.263671875, 0.3369140625, 0.609375, 0.6923828125, shape, false));
   }

   else {
      cerr << "unknown dice name: " << name <<
       ", can't initialize faces." << endl;
   }
}
