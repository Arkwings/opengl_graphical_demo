#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <ios>
#include <ctime>
#include <cmath> 
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <GL/glew.h> 
#include <SOIL/SOIL.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "assimp/Importer.hpp"      // C++ importer interface
#include "assimp/scene.h"           // Output data structure
#include "assimp/postprocess.h"     // Post processing flags

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;
using namespace glm;

extern int timeline;

extern int w,h;
extern GLFWwindow* window;
extern float horizontalAngleCam;
extern float verticalAngleCam;
extern GLuint programID,skyboxID,shadowDepthID,shadowID,shamapID,debugID,waterID,portalID;
extern vec3 direction,position,positionbis,positionPerso;
extern vec3 lPos,lColor;
extern mat4 projectionM,viewM,viewOutM,viewLightM;
extern mat4 viewBoxM;
extern GLuint vao[100];
extern GLuint vbo[100];
extern GLuint tex[100];
extern GLuint sparkle_sprites[32];
extern GLuint fbo[100];
extern GLuint rbo[100];
extern GLuint SHADOW_WIDTH, SHADOW_HEIGHT;
extern Mix_Music * _wizard_;
extern Mix_Music * _psx_;
extern Mix_Chunk * _gem_took_;
extern Mix_Chunk * _clock_tick_;
extern Mix_Chunk * _clock_tick_max_;
extern Mix_Chunk * _button_push_;
extern Mix_Chunk * _earthquake_;
extern Mix_Chunk * _earthquake_end_;

extern int compteur_inc;
extern bool push_b1;

extern int anim;
extern float angle_key;

extern float fade;
extern float fadeb;
extern int fadeinv;


extern float xtest,ytest;

extern unsigned char* HMimage;
extern unsigned char ** HMimagebis;
extern int HMwidth,HMheight;

extern bool deplacement;


bool initAll(void);
bool initTex(void);
vec3 beziertest(vec3 depart,vec3 middle,vec3 arrivee,float t);
float bezierdegre2simple(float depart,float middle,float arrivee,float t);
vec3 vibrator(vec3 wheretovibrate,float intens);
pair<vec3,vec3> bezierdegre2(vec3 depart,vec3 arrivee,vec3 initialScale, vec3 endScale, float t,float hauteur);
void render(GLuint progID);
GLint TextureFromFile(const char* path, string directory, GLuint texture_type);
bool loadModelnul(string& filename,float** &vertexArray,float** &uvArray,float** &normalArray,int* &numVerts);
void printFps(void);
void lockCamera(vec3 personnage);
void changeView(float delta);
void loadCubemap(GLuint &texID,GLchar* faces[6]);
void loadTexture(GLuint &ID,GLchar* path,GLboolean alpha);
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
GLuint loadShaders(const char * vs_path,const char * fs_path,bool gs,const char * gs_path);
