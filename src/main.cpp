#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLSL.h"
#include "MyProg.h"
#include "MatrixStack.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Dice.h"
#include "Roll.h"
#include "RollSet.h"
#include "Skybox.h"

#define SHOW_VERSIONS 0

using namespace std;
using namespace glm;

string RESOURCE_DIR;
GLFWwindow *window;
int g_width = 1524;
int g_height = 1024;

string ROLL_DESCRIPTION;
shared_ptr<Shape> surface;
shared_ptr<MyProg> dice_prog, surface_prog;
shared_ptr<Program> skybox_prog;

const int DICE_COUNT = 7;
Dice *allDice[DICE_COUNT];
vec3 lightColor;
RollSet *rollSet;
Material *surfaceMaterial;
Skybox *skybox;

// camera stuff
vec3 camLookAt = vec3(0, 0, 0);
vec3 camUp = vec3(0, 1, 0);
float camTheta = M_PI / 2.0;
float camPhi = M_PI / 5.0;
float camRadius = 17;

vec3 onSphere(float theta, float phi, float radius) {
   return vec3(
      radius * cos(phi) * cos(theta),
      radius * sin(phi),
      radius * cos(phi) * cos((M_PI / 2.0) - theta)
   );
}

void key_callback(GLFWwindow* window, int key, int scancode,
   int action, int mods)
{
   if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      if (key == GLFW_KEY_A) {
         camTheta += 0.1f;
      } else if (key == GLFW_KEY_D) {
         camTheta -= 0.1f;
      } else if (key == GLFW_KEY_W) {
         camPhi += 0.1f;
      } else if (key == GLFW_KEY_S) {
         camPhi -= 0.1f;
      }
   }
}

static void error_callback(int error, const char *description) {
   cerr << description << endl;
}

static void resize_callback(GLFWwindow *window, int width, int height) {
   g_width = width;
   g_height = height;
   glViewport(0, 0, width, height);
}

static void initGL() {
   GLSL::checkVersion();
   glClearColor(1, 1, 1, 1);
   glEnable(GL_DEPTH_TEST);

   // enable alpha/depth blending
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   dice_prog = make_shared<MyProg>(RESOURCE_DIR + "vertex.glsl",
    RESOURCE_DIR + "ct_frag.glsl");
   surface_prog = make_shared<MyProg>(RESOURCE_DIR + "vertex.glsl",
    RESOURCE_DIR + "ct_frag.glsl");

   skybox_prog = make_shared<Program>();
   skybox_prog->setVerbose(true);
   skybox_prog->setShaderNames(RESOURCE_DIR + "skybox_vert.glsl", RESOURCE_DIR + "skybox_frag.glsl");
   skybox_prog->init();
   skybox_prog->addUniform("P");
   skybox_prog->addUniform("V");
   skybox_prog->addUniform("M");
   skybox_prog->addUniform("Texture");
   skybox_prog->addAttribute("vertPos");
   skybox_prog->addAttribute("vertNor");
   skybox_prog->addAttribute("vertTex");
}

static void init(int argc, char **argv) {
   int texNum = 0;
   // the dice
   allDice[0] = new Dice(D_6, RESOURCE_DIR + "dice/d6.obj", texNum++, RESOURCE_DIR + "dice/textures/D6_White_2k.jpg");
   allDice[1] = new Dice(D_4, RESOURCE_DIR + "dice/d4.obj", texNum++, RESOURCE_DIR + "dice/textures/D4_White_2k.jpg");
   allDice[2] = new Dice(D_8, RESOURCE_DIR + "dice/d8.obj", texNum++, RESOURCE_DIR + "dice/textures/D8_White_2k.jpg");
   allDice[3] = new Dice(D_10, RESOURCE_DIR + "dice/d10.obj", texNum++, RESOURCE_DIR + "dice/textures/D10_0-9_White_2k.jpg");
   allDice[4] = new Dice(D_12, RESOURCE_DIR + "dice/d12.obj", texNum++, RESOURCE_DIR + "dice/textures/D12_White_2k.jpg");
   allDice[5] = new Dice(D_20, RESOURCE_DIR + "dice/d20.obj", texNum++, RESOURCE_DIR + "dice/textures/D20_White_2k.jpg");
   allDice[6] = new Dice(D_00, RESOURCE_DIR + "dice/d00.obj", texNum++, RESOURCE_DIR + "dice/textures/D10_00-90_White_2k.jpg");

   // the rolls
   if (argc < 3) {
      ROLL_DESCRIPTION = "default";
      rollSet = new RollSet(allDice, DICE_COUNT);
   } else {
      ROLL_DESCRIPTION = argv[2];
      rollSet = new RollSet(ROLL_DESCRIPTION, allDice, DICE_COUNT);
   }


   // the surface
   surfaceMaterial = new Material(vec3(0, 0.1, 0.25), 1, 1, 1, 0.3);
   surface = make_shared<Shape>();
   surface->loadMesh(RESOURCE_DIR + "cube.obj");
   surface->resize();
   surface->init();

   // the light
   lightColor = vec3(0.6, 0.6, 0.6);

   // the skybox
   skybox = new Skybox(RESOURCE_DIR + "test_cube.obj", RESOURCE_DIR + "skybox2.jpg", texNum++, 30);
}

static void render() {
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   float aspect = g_width / (float)g_height;

   auto P = make_shared<MatrixStack>();
   auto M = make_shared<MatrixStack>();
   auto V = make_shared<MatrixStack>();

   P->perspective(70.0f, aspect, 0.01f, 100.0f);
   camPhi = camPhi < 0.0 ? 0.0 : camPhi > M_PI/2 - 0.01 ? M_PI/2 - 0.01 : camPhi;
   V->lookAt(onSphere(camTheta, camPhi, camRadius), camLookAt, camUp);

   // skybox
   skybox_prog->bind();
   glUniformMatrix4fv(skybox_prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
   glUniformMatrix4fv(skybox_prog->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
   M->pushMatrix();
      M->loadIdentity();
      skybox->render(skybox_prog, M);
   M->popMatrix();
   skybox_prog->unbind();

   // surface
   surface_prog->bind();
   glUniformMatrix4fv(dice_prog->getUniform("P"), 1, GL_FALSE,
    value_ptr(P->topMatrix()));
   glUniformMatrix4fv(dice_prog->getUniform("V"), 1, GL_FALSE,
    value_ptr(V->topMatrix()));
   glUniform3f(dice_prog->getUniform("lightPos"), 10.0f, 10.0f, 10.0f);
   glUniform3f(dice_prog->getUniform("lightColor"), lightColor.x,
    lightColor.y, lightColor.z);

   M->pushMatrix();
      M->loadIdentity();
      M->translate(vec3(0, -1, 0));
      M->scale(vec3(10, 0.5, 10));
      surface_prog->setMaterial(surfaceMaterial);
      surface_prog->setModelMatrix(M);
      surface->draw(surface_prog->prog);
   M->popMatrix();
   surface_prog->unbind();

   // dice
   dice_prog->bind();
   glUniformMatrix4fv(dice_prog->getUniform("P"), 1, GL_FALSE,
    value_ptr(P->topMatrix()));
   glUniformMatrix4fv(dice_prog->getUniform("V"), 1, GL_FALSE,
    value_ptr(V->topMatrix()));
   glUniform3f(dice_prog->getUniform("lightPos"), 1.0f, 1.0f, 1.0f);
   glUniform3f(dice_prog->getUniform("lightColor"), lightColor.x,
    lightColor.y, lightColor.z);

   M->pushMatrix();
      M->loadIdentity();
      M->translate(vec3(0, 0.5, 0));
      rollSet->draw(dice_prog->prog, M);
   M->popMatrix();

   dice_prog->unbind();
}

static void cleanup() {
   delete rollSet;
   delete skybox;
   for (int i = 0; i < DICE_COUNT; ++i)
      delete allDice[i];
   delete surfaceMaterial;
}

int main(int argc, char **argv) {
   if (argc < 2) {
      cout << "Usage: ./dice RESOURCE_DIR [Roll Description]" << endl;
      return 0;
   }
   RESOURCE_DIR = argv[1] + string("/");
   srand(static_cast <unsigned> (time(0)));

   glfwSetErrorCallback(error_callback);
   if (!glfwInit()) return -1;
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

   window = glfwCreateWindow(g_width, g_height, "Dice", NULL, NULL);
   if (!window) {
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window);
   glewExperimental = true;
   if (glewInit() != GLEW_OK) {
      cerr << "Failed to initialize GLEW" << endl;
      return -1;
   }
   glGetError();
   if (SHOW_VERSIONS) {
      cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
      cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
       << endl;
   }

   glfwSwapInterval(1);
   glfwSetFramebufferSizeCallback(window, resize_callback);
   glfwSetKeyCallback(window, key_callback);
   initGL();

   init(argc, argv);
   while (!glfwWindowShouldClose(window)) {
      render();
      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   glfwDestroyWindow(window);
   glfwTerminate();

   cleanup();
   return 0;
}
