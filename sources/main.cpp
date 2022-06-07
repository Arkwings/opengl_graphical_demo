#include "../headers/init.h"
#include "../headers/model.h"    
#include "../headers/oglmesh.h"    
#include "../headers/ogltech.h"       
#include <unistd.h>      
 
int timeline=0; 

GLFWwindow* window; 
float horizontalAngleCam = M_PI;
float verticalAngleCam = 0;
GLuint programID(0),skyboxID(0),shadowDepthID(0),shadowID(0),shamapID(0),debugID(0),fadeID(0),waterID(0),ps1ID(0),portalID(0);
double last_clk=glfwGetTime();double clk; double delta;
int w=1600,h=900; 
vec3 direction,position=vec3( 0, 0, 5 ),positionbis=position,positionPerso=vec3( 0, 0, 0);
vec3 lPos,lInvDir,lColor=vec3(1.0f,0.4f,0.10f); 
mat4 projectionM,viewM,viewOutM,viewLightM;
mat4 viewBoxM;
GLuint vao[100];    
GLuint vbo[100];      
GLuint tex[100];
GLuint sparkle_sprites[32];  
GLuint fbo[100];     
GLuint rbo[100];     
GLuint SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;  
int anim=0;
float angle_key=0;
mat4 swordFollow;
bool display_world=false;


mat4 portal1M= translate(vec3(0.0f,20.0f,-2.0f)) * scale(vec3(2.0f,2.0f,2.0f));
mat4 portal2M= rotate((float)M_PI/2,vec3(0.0f,1.0f,0.0f)) * translate(vec3(20.0f,15.0f,-2.0f)) * scale(vec3(2.0f,2.0f,2.0f));
mat4 portalView;

Mix_Music * _wizard_;
Mix_Music * _psx_;
Mix_Chunk * _gem_took_;
Mix_Chunk * _clock_tick_;
Mix_Chunk * _clock_tick_max_;
Mix_Chunk * _button_push_;
Mix_Chunk * _earthquake_;
Mix_Chunk * _earthquake_end_;

bool deplacement(false);


int compteur_inc=1;
bool push_b1(false);
bool startcountbefore(false);
bool startcount(false);

float fade=0;
float fadeb=0;
int fadeinv=1;
float xtest=0,ytest=0; 
mat4 lightSpaceMatrix;

int Twidth,Theight;
unsigned char* Timage = SOIL_load_image("rsc/rvb.png", &Twidth, &Theight, 0,SOIL_LOAD_RGB); 

unsigned char* HMimage;
unsigned char ** HMimagebis;
int HMwidth,HMheight;

void renderText(GLuint shaderID,string msg,vec3 text_pos,float text_rot);
void renderTextStatic(GLuint shaderID,string msg,vec3 text_pos,float text_rot);




void renderBox(void){    
  static Model skybox((GLchar*)"rsc/map/skybox.obj",skyboxID,2);
  glDepthFunc(GL_LEQUAL);
  glUseProgram(skyboxID);
  glUniformMatrix4fv(glGetUniformLocation(skyboxID,"V"), 1, GL_FALSE, &viewBoxM[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(skyboxID,"P"), 1, GL_FALSE, &projectionM[0][0]);
  skybox.draw();
  glDepthFunc(GL_LESS);
}       


void renderfade(void){
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(fadeID);
  glUniformMatrix4fv(glGetUniformLocation(fadeID,"V"), 1, GL_FALSE, &viewBoxM[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(fadeID,"P"), 1, GL_FALSE, &projectionM[0][0]);
  glUniform1f(glGetUniformLocation(fadeID,"fade"),fade);
  glBindVertexArray(vao[0]);
  glBindTexture(GL_TEXTURE_2D, tex[10]);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES,0,6);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}  

void renderps1(void){
  static bool first=true;
  static float appear1=0.0;
  static float effect1=0.0;

  if(first==true){
    Mix_PlayMusic(_psx_, -1);
    first=false;
  }


  glDisable(GL_DEPTH_TEST);
  glUseProgram(ps1ID);
  static mat4 modelM;
  modelM= translate(vec3(0.0f,0.0f,-1.0f))*scale(vec3(100.0f,100.0f,100.0f));
  glUniformMatrix4fv(glGetUniformLocation(ps1ID,"P"), 1, GL_FALSE, &projectionM[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(ps1ID,"V"), 1, GL_FALSE, &viewM[0][0]);
  glUniform1f(glGetUniformLocation(ps1ID,"appear"),appear1);
  glUniform1f(glGetUniformLocation(ps1ID,"effect"),effect1);

  glUniformMatrix4fv(glGetUniformLocation(ps1ID,"M"), 1, GL_FALSE, &modelM[0][0]);
  glUniform1i(glGetUniformLocation(ps1ID,"type"),0);
  glBindVertexArray(vao[0]);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES,0,6);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);

  if(appear1>=1.0){
    modelM=translate(vec3(0.0f,0.0f,1.4f))*scale(vec3(1.6f*h/w,1.6f,1.6f))*rotate((float)M_PI/4.0f,vec3(0.0f,0.0f,1.0f));
    glUniformMatrix4fv(glGetUniformLocation(ps1ID,"M"), 1, GL_FALSE, &modelM[0][0]);
    glUniform1i(glGetUniformLocation(ps1ID,"type"),1);
    glBindVertexArray(vao[0]);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES,0,6);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

    modelM=translate(vec3(0.0f,-effect1*9/10,1.4f))*scale(vec3((1.6f-effect1)*h/w,1.6f-effect1,1.6f))*rotate((float)M_PI*3.0f/4.0f,vec3(0.0f,0.0f,1.0f));
    glUniformMatrix4fv(glGetUniformLocation(ps1ID,"M"), 1, GL_FALSE, &modelM[0][0]);
    glUniform1i(glGetUniformLocation(ps1ID,"type"),2);
    glBindVertexArray(vao[0]);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES,0,3);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    modelM=translate(vec3(0.0f,effect1*9/10,1.4f))*scale(vec3((1.6f-effect1)*h/w,1.6f-effect1,1.6f))*rotate((float)-M_PI/4.0f,vec3(0.0f,0.0f,1.0f));
    glUniformMatrix4fv(glGetUniformLocation(ps1ID,"M"), 1, GL_FALSE, &modelM[0][0]);
    glUniform1i(glGetUniformLocation(ps1ID,"type"),2);
    glBindVertexArray(vao[0]);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES,0,3);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

    if(effect1<0.96)
      effect1+=0.015;
  }
  glEnable(GL_DEPTH_TEST);
  if(effect1>=0.96){
  	renderTextStatic(shamapID,"arkwings",vec3(0.2f,3.2f,0.6f),4);
  	renderTextStatic(shamapID,"tm",vec3(1.4f,-8.5f,-9.0f),4);
  	renderTextStatic(shamapID,"contrefacon",vec3(0.2f,-5.0f,-1.8f),4);
  	renderTextStatic(shamapID,"entertainment",vec3(0.2f,-6.5f,-2.7f),4);
  }
  if(appear1<1.01)
    appear1+=0.015;
}  

void renderwater(void){
  static float waveTime=0.5;
  static float waveWidth=0.3;
  static float waveHeight=0.3;
  static float waveFreq=0.02;
  static mat4 modelM=translate(vec3(35.0f,-2.3f,25.0f))*scale(vec3(1.3f,1.0f,1.7f));
  glEnable(GL_BLEND);
glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);//or glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA)
glUseProgram(waterID);
glUniformMatrix4fv(glGetUniformLocation(waterID,"M"), 1, GL_FALSE, &modelM[0][0]);
glUniformMatrix4fv(glGetUniformLocation(waterID,"P"), 1, GL_FALSE, &projectionM[0][0]);
glUniform3fv(glGetUniformLocation(waterID,"lightColor"), 1, &lColor[0]);
glUniform3fv(glGetUniformLocation(waterID,"lightPos"), 1, &lPos[0]);
glUniform3fv(glGetUniformLocation(waterID,"eyePos"), 1, &position[0]);
glUniform1f(glGetUniformLocation(waterID,"waveTime"),waveTime+=waveFreq);
glUniform1f(glGetUniformLocation(waterID,"waveWidth"),waveWidth);
glUniform1f(glGetUniformLocation(waterID,"waveHeight"),waveHeight);
glBindVertexArray(vao[3]);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, tex[9]);
glEnableVertexAttribArray(0);
glDisable(GL_CULL_FACE); 
glDrawArrays(GL_TRIANGLES,0,6*32*32);
glEnable(GL_CULL_FACE); 
glDisableVertexAttribArray(0);
glBindVertexArray(0);
glDisable(GL_BLEND);
}  

void renderfadebehind(void){
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//or glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA)
glUseProgram(fadeID);
glUniformMatrix4fv(glGetUniformLocation(fadeID,"V"), 1, GL_FALSE, &viewBoxM[0][0]);
glUniformMatrix4fv(glGetUniformLocation(fadeID,"P"), 1, GL_FALSE, &projectionM[0][0]);
glUniform1f(glGetUniformLocation(fadeID,"fade"),fadeb);
glBindVertexArray(vao[0]);
glBindTexture(GL_TEXTURE_2D, tex[10]);
glEnableVertexAttribArray(0);
glDrawArrays(GL_TRIANGLES,0,6);
glDisableVertexAttribArray(0);
glBindVertexArray(0);
glDisable(GL_BLEND);
glEnable(GL_DEPTH_TEST);    
}   






void renderskinned(GLuint shaderID,bool portal,bool viewType){     

  static bool first=1;
  static DirectionalLight m_directionalLight;

  static mat4 scaleM = scale(vec3(0.01f,0.01f,0.01f)); 
  static mat4 translateM;
  static mat4 rotateM;  
  static mat4 modelM;
  static mat4 mvpM;  

  static SkinnedMesh bob("rsc/crash0/cartoon_boy193/BOY_H/boy_H_animations_split.x"); 
  static SkinningTechnique *bobeffect;
  static vector<mat4> transformbob;
//static bool b_anim[26];
  static int oldanim=0;  
  static float animating=0;
  static float a_first=0;
  static float a_last=1;




  if(first==1){
//b_anim[0]=true;
//for(int i=1;i<26;++i){
//  b_anim[i]=false;
//}
    m_directionalLight.Color = vec3(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.55f;
    m_directionalLight.DiffuseIntensity = 0.9f;
    m_directionalLight.Direction = vec3(1.0f, 0.0, 0.0);
    bobeffect= new SkinningTechnique();
    bobeffect->Init(shaderID);
    bobeffect->SetColorTextureUnit(1);
    bobeffect->SetDirectionalLight(m_directionalLight);
    bobeffect->SetMatSpecularIntensity(0.0f);
    bobeffect->SetMatSpecularPower(0);
    first=0;  
    Mix_PlayMusic(_wizard_, -1);
  }

// cout<<shaderID<<endl;     
// glUseProgram(shamapID);
// //cout<<HMwidth<<<<' '<<HMheight<<endl;
//scaleM=scale(1.0f,-1.0f,1.0f);
  if(positionPerso[0]<1)positionPerso[0]=1;
  if(positionPerso[0]>106)positionPerso[0]=106;
  if(positionPerso[2]<1)positionPerso[2]=1;
  if(positionPerso[2]>106)positionPerso[2]=106;
  unsigned char HMcolor=((HMimagebis[(int)(positionPerso[2]*2048/108)][(int)(positionPerso[0]*2048/108)]+HMimagebis[(int)(positionPerso[2]*2048/108)][(int)((positionPerso[0]+1)*2048/108)]+HMimagebis[(int)(positionPerso[2]*2048/108)][(int)((positionPerso[0]+2)*2048/108)])/3+
    (HMimagebis[(int)(positionPerso[2]*2048/108)+1][(int)(positionPerso[0]*2048/108)]+HMimagebis[(int)(positionPerso[2]*2048/108)+1][(int)((positionPerso[0]+1)*2048/108)]+HMimagebis[(int)(positionPerso[2]*2048/108)+1][(int)((positionPerso[0]+2)*2048/108)])/3+
    (HMimagebis[(int)(positionPerso[2]*2048/108)-1][(int)(positionPerso[0]*2048/108)]+HMimagebis[(int)(positionPerso[2]*2048/108)-1][(int)((positionPerso[0]+1)*2048/108)]+HMimagebis[(int)(positionPerso[2]*2048/108)-1][(int)((positionPerso[0]+2)*2048/108)])/3+
    (HMimagebis[(int)(positionPerso[2]*2048/108)][(int)(positionPerso[0]*2048/108)+1]+HMimagebis[(int)(positionPerso[2]*2048/108)][(int)((positionPerso[0]+1)*2048/108)+1]+HMimagebis[(int)(positionPerso[2]*2048/108)][(int)((positionPerso[0]+2)*2048/108)+1])/3+
    (HMimagebis[(int)(positionPerso[2]*2048/108)][(int)(positionPerso[0]*2048/108)-1]+HMimagebis[(int)(positionPerso[2]*2048/108)][(int)((positionPerso[0]+1)*2048/108)-1]+HMimagebis[(int)(positionPerso[2]*2048/108)][(int)((positionPerso[0]+2)*2048/108)-1])/3+

    (HMimagebis[(int)(positionPerso[2]*2048/108)+1][(int)(positionPerso[0]*2048/108)+1]+HMimagebis[(int)(positionPerso[2]*2048/108)+1][(int)((positionPerso[0]+1)*2048/108)+1]+HMimagebis[(int)(positionPerso[2]*2048/108)+1][(int)((positionPerso[0]+2)*2048/108)+1])/3+
    (HMimagebis[(int)(positionPerso[2]*2048/108)+1][(int)(positionPerso[0]*2048/108)-1]+HMimagebis[(int)(positionPerso[2]*2048/108)+1][(int)((positionPerso[0]+1)*2048/108)-1]+HMimagebis[(int)(positionPerso[2]*2048/108)+1][(int)((positionPerso[0]+2)*2048/108)-1])/3+
    (HMimagebis[(int)(positionPerso[2]*2048/108)-1][(int)(positionPerso[0]*2048/108)+1]+HMimagebis[(int)(positionPerso[2]*2048/108)-1][(int)((positionPerso[0]+1)*2048/108)+1]+HMimagebis[(int)(positionPerso[2]*2048/108)-1][(int)((positionPerso[0]+2)*2048/108)+1])/3+
    (HMimagebis[(int)(positionPerso[2]*2048/108)-1][(int)(positionPerso[0]*2048/108)-1]+HMimagebis[(int)(positionPerso[2]*2048/108)-1][(int)((positionPerso[0]+1)*2048/108)-1]+HMimagebis[(int)(positionPerso[2]*2048/108)-1][(int)((positionPerso[0]+2)*2048/108)-1])/3)/9;
// cout<<" x="<<positionPerso[0]<<" z="<<positionPerso[2]<<" color="<<(int)HMcolor<< " tab1="<<(int)(positionPerso[2]*2048/108)<< " tab1="<<(int)(positionPerso[0]*3*2048/108)<<endl;

// glActiveTexture(GL_TEXTURE0);
// glBindTexture(GL_TEXTURE_2D, tex[15]);  

// modelM=scale(108.0f,108.0f,108.0f);
// glUniformMatrix4fv(glGetUniformLocation(shamapID,"M"), 1, GL_FALSE, &modelM[0][0]);
// glBindVertexArray(vao[1]);
// glBindTexture(GL_TEXTURE_2D, tex[15]);
// glEnableVertexAttribArray(0);
// glDrawArrays(GL_TRIANGLES,0,6);
// glDisableVertexAttribArray(0);
// glBindVertexArray(0);

  static mat4  viewSwap;

  glUseProgram(shaderID);
  if(display_world)
    positionPerso[1]=(float)HMcolor/8-6;
  else
    positionPerso[1]=0;
//cout<<positionPerso[0]<<"  "<<positionPerso[2]<<endl;
  if(positionPerso[0]>103.5&&positionPerso[0]<106.5&&positionPerso[2]>14&&positionPerso[2]<16){
    positionPerso[0]=positionbis[0];
    positionPerso[1]=positionbis[1];
    positionPerso[2]=positionbis[2];
    viewSwap=viewOutM;
    viewOutM=viewM;
    viewM=viewSwap;
    fade=0.6;
    display_world=!display_world;
  }
  if(position[0]>103.5&&position[0]<106.5&&position[2]>14&&position[2]<16){
    position[0]=positionbis[0];
    position[1]=positionbis[1];
    position[2]=positionbis[2];
    //axe x a changer selon l endroit ou l on rentee ds le portail
    viewSwap=viewOutM;
    viewOutM=viewM;
    viewM=viewSwap;
    fade=0.6;
    display_world=!display_world;
}
static float rotateit=0.0;
if(deplacement)
	rotateit=horizontalAngleCam-M_PI/2;
translateM=translate(vec3(positionPerso[0],positionPerso[1],positionPerso[2]));
rotateM = rotate(rotateit+(float)M_PI*3/2+angle_key,vec3(0.0f,1.0f,0.0f));
modelM = translateM * rotateM * scaleM;


if(anim!=oldanim){
if(anim==0){          //standby
  a_last=1; 
  a_first=0;
}
else if(anim==1){    //dance 
  a_last=6.16;
  a_first=1.16;
}
else if(anim==2){    //happy
  a_last=8.06;
  a_first=6.33;
}
else if(anim==3){    //taking_box
  a_last=11.9;
  a_first=9.6;
}
else if(anim==4){    //put_down_box //OUI C EST PAREIL ET ALLOOOOOOOORRRSSSSS
  a_last=11.9;
  a_first=9.6;
}
else if(anim==5){    //carrying
  a_last=11.8;
  a_first=8.3;
}
else if(anim==6){    //angry
  a_last=14.4;
  a_first=11.96;
}
else if(anim==7){    //carry_box
  a_last=15.6;
  a_first=14.56;
}
else if(anim==8){    //put_box_up
  a_last=18.13;
  a_first=15.83;
}
else if(anim==9){    //sitting
  a_last=20.63;
  a_first=18.3;
}
else if(anim==10){    //run
  a_last=21.6;
  a_first=20.8;
}
else if(anim==11){    //walk
  a_last=23.1;
  a_first=21.76;
}
else if(anim==12){    //jump
  a_last=24.76;
  a_first=23.26;
}
else if(anim==13){    //walk_on_ledge
  a_last=30.86;
  a_first=24.96;
}
else if(anim==14){    //throwing
  a_last=33.16;
  a_first=31.03;
}
else if(anim==15){    //next_time
  a_last=36.3;
  a_first=33.33;
}
else if(anim==16){    //move_box
  a_last=37.53;
  a_first=36.46;
}
else if(anim==17){    //grasping_the_rope
  a_last=39.86;
  a_first=37.7;
}
else if(anim==18){    //move_right
  a_last=41.43;
  a_first=40.03;
}
else if(anim==19){    //move_left
  a_last=43;
  a_first=41.6;
}
else if(anim==20){    //leap
  a_last=48.06;
  a_first=43.16;
}
else if(anim==21){    //climb_the_ladder
  a_last=49.43;
  a_first=48.23;
}
else if(anim==22){    //climb_the_platform
  a_last=52.1;
  a_first=49.6;
}
else if(anim==23){    //hurt1
  a_last=54.33;
  a_first=53.33;
}
else if(anim==24){    //hurt2
  a_last=54.33;
  a_first=53.33;
}
else if(anim==25){    //death
  a_last=59.23;
  a_first=54.36;
}
oldanim=anim;
animating=a_first;
}


if(animating>a_last)
  animating=a_first;
if(anim==2)
	animating+=0.008;
else if(anim==1||anim==0)
	animating+=0.01;
else
	animating+=0.013;
bob.BoneTransform(animating,transformbob);
 
for(uint i = 0 ; i < transformbob.size() ; i++){
  bobeffect->SetBoneTransform(i, transformbob[i]);
}
if(portal==false)
  mvpM=projectionM*viewM*modelM; 

if(portal==true)
  mvpM=projectionM*viewOutM*modelM;

bobeffect->SetEyeWorldPos(position);
bobeffect->SetM(modelM);
if(!portal)
	bobeffect->SetV(viewM);
else
    bobeffect->SetV(viewOutM);
bobeffect->SetP(projectionM);
bobeffect->SetLightSpaceMatrix(lightSpaceMatrix);
glUniform1i(glGetUniformLocation(shaderID,"dID"),true);
if(viewType)
	glUniform1i(glGetUniformLocation(shaderID,"toon"),false);
else
	glUniform1i(glGetUniformLocation(shaderID,"toon"),true);

glUniform3fv(glGetUniformLocation(shaderID,"lightColor"), 1, &lColor[0]);
glUniform3fv(glGetUniformLocation(shaderID,"lightPos"), 1, &lPos[0]);
glUniform3fv(glGetUniformLocation(shaderID,"eyePos"), 1, &position[0]);
if(startcount)
	glUniform1f(glGetUniformLocation(shaderID,"light_force"),3.5f);
else
	glUniform1f(glGetUniformLocation(shaderID,"light_force"),1.0f);


glDisable(GL_CULL_FACE);
bob.Render();
glEnable(GL_CULL_FACE);
glCullFace(GL_FRONT); 
glUniform1i(glGetUniformLocation(shaderID,"dID"),false);

modelM = translateM * rotateM * translate(vec3(0.0f,-0.015f,0.0f)) * scaleM;
bobeffect->SetM(modelM);
bob.Render();
modelM = translateM * rotateM * translate(vec3(0.0f,0.015f,0.0f)) * scaleM;
bobeffect->SetM(modelM);
bob.Render();
modelM = translateM * rotateM * translate(vec3(-0.015f,0.0f,0.0f)) * scaleM;
bobeffect->SetM(modelM);
bob.Render();
modelM = translateM * rotateM * translate(vec3(0.015f,0.0f,0.0f)) * scaleM;
bobeffect->SetM(modelM);
bob.Render();

modelM = translateM * rotateM * translate(vec3(-0.015f,-0.015f,0.0f)) * scaleM;
bobeffect->SetM(modelM);
bob.Render();
modelM = translateM * rotateM * translate(vec3(0.015f,0.015f,0.0f)) * scaleM;
bobeffect->SetM(modelM);
bob.Render();
modelM = translateM * rotateM * translate(vec3(-0.015f,0.015f,0.0f)) * scaleM;
bobeffect->SetM(modelM);
bob.Render();
modelM = translateM * rotateM * translate(vec3(0.015f,-0.015f,0.0f)) * scaleM;
bobeffect->SetM(modelM);
bob.Render();

glCullFace(GL_BACK); 
}



void renderText(GLuint shaderID,string msg,vec3 text_pos,float text_rot){
  static Model font((GLchar*)"rsc/font/checked.obj",shaderID,1); 

  static mat4 scaleM=scale(vec3(0.6f,0.6f,0.6f));
  static mat4 translateM;  
  static mat4 rotateM=rotate((float)M_PI/2,vec3(1.0f,0.0f,0.0f));    
  static mat4 modelM;  
  static float angle=0.0;
  static float teta=0.0;
  static float inc=0.01;

  int nbcaracs=msg.size();
  int lettre;

  glUseProgram(shaderID);  
  glUniform1i(glGetUniformLocation(shaderID,"dID"),true);
  glUniform1i(glGetUniformLocation(shaderID,"gemColor"),4);





  for(int i=0;i<nbcaracs;i++){
    modelM=translate(text_pos)*rotate((float)M_PI/2*text_rot,vec3(0.0f,1.0f,0.0f))*rotateM*scaleM*rotate(cos(teta)/9,vec3(0.0f,0.0f,1.0f))*rotate(sin(teta)/9,vec3(0.0f,1.0f,0.0f))*translate(vec3(((float)i-(float)nbcaracs/2)*0.60,0.0f,0.0f))*rotate(angle,vec3(0.0f,0.0f,1.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
    lettre=msg[i]-'a';
    if(lettre>-1&&lettre<9)
      font.other(25-lettre);
    else if(lettre>=9&&lettre<18)
      font.other(23-lettre);
    else if(lettre==18)
      font.other(15);
    else if(lettre>18&&lettre<25)
      font.other(24-lettre);
    else if(lettre==25)
      font.other(16);
  }
  angle+=inc+inc*2*(M_PI/3-(angle>0?angle:-angle))*(M_PI/3-(angle>0?angle:-angle));
  if((angle>0?angle:-angle) > M_PI/4 && inc * angle > 0)
    inc=-inc;
  teta+=0.02;
}

void renderTextStatic(GLuint shaderID,string msg,vec3 text_pos,float text_rot){
  static Model font((GLchar*)"rsc/font/checked.obj",shaderID,1); 

  static mat4 scaleM=scale(vec3(0.6f,0.6f,0.6f));
  static mat4 translateM;  
  static mat4 rotateM=rotate((float)M_PI/2,vec3(1.0f,0.0f,0.0f));    
  static mat4 modelM;  

  int nbcaracs=msg.size();
  int lettre;

  glUseProgram(shaderID);  
  glUniform1i(glGetUniformLocation(shaderID,"dID"),true);
  glUniform1i(glGetUniformLocation(shaderID,"gemColor"),-4);


  for(int i=0;i<nbcaracs;i++){
    modelM=translate(text_pos)*rotate((float)M_PI/2*text_rot,vec3(0.0f,1.0f,0.0f))*rotateM*scaleM*translate(vec3(((float)i-(float)nbcaracs/2)*0.60,0.0f,0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
    lettre=msg[i]-'a';
    if(lettre>-1&&lettre<9)
      font.other(25-lettre);
    else if(lettre>=9&&lettre<18)
      font.other(23-lettre);
    else if(lettre==18)
      font.other(15);
    else if(lettre>18&&lettre<25)
      font.other(24-lettre);
    else if(lettre==25)
      font.other(16);
  }
}

void renderTexts(GLuint shaderID){
//mettre 1, 2, 3 ou 4 pour l angle pour les 4 inclinaisons de base
  if(display_world)
    renderText(shaderID,"nulle part",vec3(104.0f,5.0f,15.15f),3);
  else
    renderText(shaderID,"quelquepart",vec3(104.0f,5.0f,15.15f),3);

}


#define NB_GEMS 300
void renderGem(int gemMeshe,int type,vec3 initialPos,vec3 initialScale,GLuint shaderID,int gemID,int gemColor){

  static Model gem((GLchar*)"rsc/3Dgems/gemstonecomp.obj",shaderID,1); 

  static mat4 scaleM;
  static mat4 translateM;  
  static mat4 rotateM;    
  static mat4 rotateM2;    
  static mat4 modelM;    

  static bool first=true;  
  static vec2 lPosbis;
  static float avancement[NB_GEMS];
  static float avancementdelavancement[NB_GEMS];
  static float alpha[NB_GEMS];
  static bool catchedtest[NB_GEMS]; 
  static int sparklingstart[NB_GEMS];
  static int sparklingnow[NB_GEMS];
  static pair<vec3,vec3> catched[NB_GEMS];
  static bool gem_played[NB_GEMS];
  static int channel=0;


  glUseProgram(shaderID); 

  if(shaderID!=shadowDepthID){
    if(type==0){
      if(first==true){
        for(int i=0;i<NB_GEMS;++i){
          avancement[i]=0;
          avancementdelavancement[i]=0;
          alpha[i]=rand()/(float)RAND_MAX*M_PI;
          catchedtest[i]=false;
          sparklingstart[i]=(int)(rand()/(float)RAND_MAX*400); 
          sparklingnow[i]=0;
          gem_played[i]=false;
        }
        first=false;
      } 
 

  
      alpha[gemID]+=0.02;
      lPosbis=vec2(cos(alpha[gemID]),sin(alpha[gemID]));
      if(avancement[gemID]<1.0f){
        if((initialPos[0]-positionPerso[0])*(initialPos[0]-positionPerso[0])+(initialPos[2]-positionPerso[2])*(initialPos[2]-positionPerso[2])<16.0f&&display_world)
          catchedtest[gemID]=true;
        if(catchedtest[gemID]==true)
        {    
          catched[gemID]=bezierdegre2(initialPos,vec3(positionPerso[0]+0.2,positionPerso[1],positionPerso[2]+0.8),initialScale,vec3(initialScale[0]/3,initialScale[1],initialScale[2]/3),avancement[gemID]+=0.016+avancementdelavancement[gemID],4.5f); 
          avancementdelavancement[gemID]+=(avancementdelavancement[gemID]+0.006)/100;
          translateM=translate(vec3(catched[gemID].first[0],catched[gemID].first[1],catched[gemID].first[2])); 
          scaleM=scale(vec3(catched[gemID].second[0],catched[gemID].second[1],catched[gemID].second[2]));
        }
        else{  
          translateM=translate(vec3(initialPos[0],initialPos[1],initialPos[2]));
          scaleM=scale(vec3(initialScale[0],initialScale[1],initialScale[2]));
        }
        rotateM=rotate(lPosbis[0]/1.3f,vec3(1.0f,0.0f,0.0f));
        rotateM2=rotate(lPosbis[1]/1.3f,vec3(0.0f,0.0f,1.0f));
        modelM = translateM *  rotateM  * rotateM2 * scaleM;

        glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
        glUniform1i(glGetUniformLocation(shaderID,"dID"),true);
        glUniform1i(glGetUniformLocation(shaderID,"gemColor"),gemColor);

        switch(gemMeshe){
          case 1:gem.other(0);break; 
          case 2:gem.other(1);break; 
          case 3:gem.other(2);break; 
          case 4:gem.other(3);break; 
          case 5:gem.other(4);break;  
          case 6:gem.other(5);break;  
          case 7:gem.other(6);break; 
          case 8:gem.other(7);break; 
          case 9:gem.other(8);break; 
          case 10:gem.other(9);break; 
          case 11:gem.other(10);break; 
          case 12:gem.other(11);break; 
          case 13:gem.other(12);break; 
          case 14:gem.other(13);break;   
          case 15:gem.other(14);break; 
          case 16:gem.other(15);break; 
        }
      }
      else{
        if(gem_played[gemID]==false){
          gem_played[gemID]=true;
          Mix_PlayChannel(channel++%5,_gem_took_, 0);
        }
      } 
    }
    else if(type==1){
      if(avancement[gemID]<1.0){
        if(catchedtest[gemID]==true){
//catched[gemID]=bezierdegre2(initialPos,positionPerso,initialScale,vec3(initialScale[0]/3,initialScale[1],initialScale[2]/3),avancement[gemID]+=0.008+avancementdelavancement[gemID],4.5f); 
          translateM=translate(vec3(catched[gemID].first[0],catched[gemID].first[1],catched[gemID].first[2]));
          scaleM=scale(vec3(catched[gemID].second[0],catched[gemID].second[1],catched[gemID].second[2]));
        }
        else{
          translateM=translate(vec3(initialPos[0],initialPos[1],initialPos[2]));
          scaleM=scale(vec3(initialScale[0]/2.0f,initialScale[1]/2.8f,initialScale[2]/2.0f));
        }
        rotateM = rotate(horizontalAngleCam+(float)M_PI,vec3(0.0f,1.0f,0.0f));
        rotateM2 = rotate((float)M_PI/4.0f,vec3(1.0f,0.0f,1.0f));
        modelM = translateM * rotateM2 * rotateM * scaleM;
        glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
        glUniform1i(glGetUniformLocation(shaderID,"dID"),false);
        glUniform1i(glGetUniformLocation(shaderID,"gemColor"),gemColor);
//glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindVertexArray(vao[0]);
//glActiveTexture(GL_TEXTURE0);
        if(sparklingstart[gemID]++==399){
          sparklingstart[gemID]=0;
          sparklingnow[gemID]=1;
        }
        if(sparklingnow[gemID]>0){
          sparklingnow[gemID]++;
          if(sparklingnow[gemID]==64)
            sparklingnow[gemID]=0;
        }
        glBindTexture(GL_TEXTURE_2D, sparkle_sprites[sparklingnow[gemID]/2]);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(2);
        glDrawArrays(GL_TRIANGLES,0,6);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0); 
        glDisable(GL_BLEND);
//glDepthMask(GL_TRUE);
      }
    }
  }
}

void render2(GLuint shaderID){
  static float alphaGlass=0.00;
  static int rotasmall=0;
  static int rotasbig=0;
  static int rotasbigbig=0;
  static int compteur=0;
  static float compteurf=0.0;
  static float push=0.0;
  static float push_time=0.0;
  static float clock_back=0.0;
  static float clock_backbis=0.0;
  static int canal=7;

  vec3 vibrate=vibrator(vec3(0.0f,0.0f,1.0f),25);
  static mat4 scaleM;
  static mat4 translateM;  
  static mat4 translateMbis;  
  static mat4 rotateM;    
  static mat4 rotateM2;    
  static mat4 modelM;    
  static Model rock((GLchar*)"rsc/rock/rock.obj",shaderID,1);
  static Model lego((GLchar*)"rsc/rock/lego.obj",shaderID,1);
  static Model map2((GLchar*)"rsc/impossible/Clock.obj",shaderID,1);
  static Model button((GLchar*)"rsc/button/buttons.obj",shaderID,1);
  static Model invader((GLchar*)"rsc/button/invader.obj",shaderID,1);
  static Model cat((GLchar*)"rsc/models3d/cat/cat.obj",shaderID,1);



  static bool first=true;

  glUseProgram(shaderID);
  scaleM=scale(vec3(25.0f,25.0f,25.0f));
  translateM=translate(vec3(100.5f,0.0f,15.0f));
  translateMbis=translate(vec3(163.5f,0.0f,15.0f));
  glUniform1i(glGetUniformLocation(shaderID,"dID"),true);
  if(startcount&&clock_back<58){
    modelM = translateM * translate(vec3(-0.5f,clock_back,0.0f)) * translate(vibrate) * rotate(alphaGlass,vec3(1.0f,0.0f,0.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scaleM;
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(shaderID,"light_force"),3.5f);
  }
  else if(startcount){
    modelM = translateM * translate(vec3(-0.5f,clock_back,0.0f)) * rotate(alphaGlass,vec3(1.0f,0.0f,0.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scaleM;
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(shaderID,"light_force"),3.5f);
  }
  else{
    modelM = translateM * translate(vec3(-0.5f,clock_back,0.0f)) * rotate(alphaGlass,vec3(1.0f,0.0f,0.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scaleM;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glUniform1f(glGetUniformLocation(shaderID,"light_force"),1.0f);
  }
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  glUniform1i(glGetUniformLocation(shaderID,"gemColor"),-3);
  glBindTexture(GL_TEXTURE_2D, tex[9]);
  map2.other(6);
  map2.other(7);
  map2.other(9);
  
  glUniform1i(glGetUniformLocation(shaderID,"gemColor"),-1);
  map2.other(2);
  map2.other(4);
  map2.other(5);

  if(startcount&&clock_back<58){
  	modelM = translate(vec3(105.5f,0.0f,15.0f)) * translate(vec3(-vibrate[2],vibrate[2]/2.0f,0.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(3.3f,3.3f,0.5f));
  }
  else{
  	modelM = translate(vec3(105.5f,0.0f,15.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(3.3f,3.3f,0.5f));
  }
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  lego.draw();

  if(startcount&&clock_back<58){
    modelM = translateMbis * translate(vec3(0.0f,clock_back,0.0f)) * translate(vec3(-vibrate[2],vibrate[2]/2.0f,0.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(400.0f,400.0f,400.0f));
  }
  else{
    modelM = translateMbis * translate(vec3(0.0f,clock_back,0.0f))  * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(400.0f,400.0f,400.0f));
  }
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  rock.draw();

  if(startcount&&clock_back<58){
    modelM = translate(vec3(163.5f,0.0f,-50.0f)) * translate(vec3(vibrate[2],0.0f,-vibrate[2]/2)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(150.0f,600.0f,500.0f));
  }
  else{
    modelM = translate(vec3(163.5f,0.0f,-50.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(150.0f,600.0f,500.0f));
  }
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  rock.draw();

  if(startcount&&clock_back<58){
    modelM = translate(vec3(163.5f,0.0f,80.0f)) * translate(vec3(vibrate[2],0.0f,vibrate[2]/2)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(150.0f,600.0f,500.0f));
  }
  else{
    modelM = translate(vec3(163.5f,0.0f,80.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(150.0f,600.0f,500.0f));
  }
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  rock.draw();

  if(startcount&&clock_back<58){
    modelM = translate(vec3(250.0f-clock_back*1.5f,-28.0f+clock_back/7,15.0f)) * translate(vibrate*1.2f) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(450.0f,150.0f,450.0f));
  }
  else{
    modelM = translate(vec3(250.0f-clock_back*1.5f,-28.0f+clock_back/7,15.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(450.0f,150.0f,450.0f));
  }
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  rock.draw();
  if(startcount&&clock_back<58){
    modelM = translate(vec3(250.0f-clock_back*1.9f,-48.0f+clock_back/7,15.0f)) * translate(vibrate*1.2f) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(450.0f,150.0f,450.0f));
  }
  else{
    modelM = translate(vec3(250.0f-clock_back*1.9f,-48.0f+clock_back/7,15.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(450.0f,150.0f,450.0f));
  }
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  rock.draw();
  if(startcount&&clock_back<58){
    modelM = translate(vec3(250.0f-clock_back*2.3f,-68.0f+clock_back/7,15.0f)) * translate(vibrate*1.2f) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(450.0f,150.0f,450.0f));
  }
  else{
    modelM = translate(vec3(250.0f-clock_back*2.3f,-68.0f+clock_back/7,15.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(450.0f,150.0f,450.0f));
  }
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  rock.draw();
  if(startcount&&clock_back<58){
    modelM = translate(vec3(250.0f-clock_back*1.1f,-08.0f+clock_back/7,15.0f)) * translate(vibrate*1.2f) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(450.0f,150.0f,450.0f));
  }
  else{
    modelM = translate(vec3(250.0f-clock_back*1.1f,-08.0f+clock_back/7,15.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(450.0f,150.0f,450.0f));
  }
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  rock.draw();


  if(startcount&&clock_backbis<35){
    modelM = translate(vec3(150.0f,0.0f,312.0f-clock_back*2.0f-clock_backbis*1.5f)) * translate(vibrate*1.5f) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(800.0f,1600.0f,800.0f));
  }
  if(startcount){
    modelM = translate(vec3(150.0f,0.0f,312.0f-clock_back*2.0f-clock_backbis*1.5f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(800.0f,1600.0f,800.0f));
  }
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  rock.draw();
  if(startcount&&clock_backbis<35){
    modelM = translate(vec3(150.0f,0.0f,-290.0f+clock_back*2.0f+clock_backbis*1.5f)) * translate(vibrate*1.5f) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(800.0f,1600.0f,800.0f));
  }
  if(startcount){
    modelM = translate(vec3(150.0f,0.0f,-290.0f+clock_back*2.0f+clock_backbis*1.5f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(800.0f,1600.0f,800.0f));
  }
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  rock.draw();
  if(startcount&&clock_backbis<35){
    modelM = translate(vec3(-50.0f+clock_back*2.0f+clock_backbis*1.5f,0.0f,15.0f)) * translate(vibrate*1.5f) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(800.0f,1600.0f,10.0f));
  }
  if(startcount){
    modelM = translate(vec3(-72.0f+clock_back*2.0f+clock_backbis*1.5f,0.0f,15.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scale(vec3(800.0f,1600.0f,10.0f));
  }
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  rock.draw();




  static int end_quake=0;
  if(startcount){
    compteur+=compteur_inc;
    compteurf=(float)compteur;
    if(clock_back<58){
    	clock_back+=0.05;
    }
    else if(clock_back>=58&&clock_backbis<35){
    	clock_backbis+=0.05;
    }
    else if(end_quake<120){
    	end_quake++;
    	if(end_quake==1)
    		Mix_PlayChannel(5,_earthquake_end_, 0);
    }
    else{
    	Mix_HaltChannel(5);
    }
   // alphaGlass+=0.003;
  }
  if(!startcount){
    Mix_HaltChannel(7);
    Mix_HaltChannel(8);
    Mix_HaltChannel(9);
    Mix_HaltChannel(10);
    Mix_HaltChannel(11);
    Mix_HaltChannel(12);
    Mix_HaltChannel(13);
  }
  static float small=1;
  static float smallbis=0;
  Mix_VolumeChunk(_earthquake_,(float)MIX_MAX_VOLUME*5.0/4.0-sqrt((100.5-(float)position[0])*(100.5-(float)position[0])+(15.0-(float)position[2])*(15.0-(float)position[2]))/100.0*(float)MIX_MAX_VOLUME);
  Mix_VolumeChunk(_earthquake_end_,(float)MIX_MAX_VOLUME*6.5/4.0-sqrt((100.5-(float)position[0])*(100.5-(float)position[0])+(15.0-(float)position[2])*(15.0-(float)position[2]))/100.0*(float)MIX_MAX_VOLUME);
  Mix_VolumeChunk(_clock_tick_,(float)MIX_MAX_VOLUME*5.0/4.0-sqrt((100.5-(float)position[0])*(100.5-(float)position[0])+(15.0-(float)position[2])*(15.0-(float)position[2]))/100.0*(float)MIX_MAX_VOLUME);
  Mix_VolumeChunk(_clock_tick_max_,(float)MIX_MAX_VOLUME*5.0/4.0-sqrt((100.5-(float)position[0])*(100.5-(float)position[0])+(15.0-(float)position[2])*(15.0-(float)position[2]))/100.0*(float)MIX_MAX_VOLUME);
  if(startcount&&(float)compteur/200>smallbis){
  	smallbis++;
    if(clock_back<58||clock_backbis<35)
    	Mix_PlayChannel(5,_earthquake_, 0);
  }
  if(startcount&&(float)compteur/60>small){
  	//cout<<compteur_inc<<endl;
  	if(compteur_inc<60){
    	Mix_PlayChannel(canal++,_clock_tick_, 0);
    }
    else{
    	Mix_PlayChannel(canal++,_clock_tick_max_, 0);
    }
    if(canal>13)
    	canal=7;
  	small++; 
    rotasmall++;
  }
  static float big=1;
  if(startcount&&(float)compteur/3600>big){
    big++;
    rotasbig++;
  }
  static float bigbig=1;
  if(startcount&&(float)compteur/43200>bigbig){
    bigbig++;
    rotasbigbig++;
  }
  if(startcount&&clock_back<58){
  	modelM = translateM * translate(vec3(-0.5f,clock_back,0.0f)) * translate(vec3(-vibrate[2],vibrate[2]/2.0f,0.0f)) * rotate((float)((float)rotasmall/60*2*M_PI+alphaGlass),vec3(1.0f,0.0f,0.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scaleM;
  	glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  	map2.other(1);
  	modelM = translateM * translate(vec3(-0.5f,clock_back,0.0f)) * translate(vec3(-vibrate[2],vibrate[2]/2.0f,0.0f)) * rotate((float)((float)rotasbig/60*2*M_PI+alphaGlass),vec3(1.0f,0.0f,0.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scaleM;
  	glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  	map2.other(0);
  	modelM = translateM * translate(vec3(-0.5f,clock_back,0.0f)) * translate(vec3(-vibrate[2],vibrate[2]/2.0f,0.0f)) * rotate((float)((float)rotasbigbig/60*2*M_PI+alphaGlass),vec3(1.0f,0.0f,0.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scaleM;
  	glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  	map2.other(3);
  }
  else{
  	modelM = translateM * translate(vec3(-0.5f,clock_back,0.0f)) * rotate((float)((float)rotasmall/60*2*M_PI+alphaGlass),vec3(1.0f,0.0f,0.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scaleM;
  	glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  	map2.other(1);
  	modelM = translateM * translate(vec3(-0.5f,clock_back,0.0f)) * rotate((float)((float)rotasbig/60*2*M_PI+alphaGlass),vec3(1.0f,0.0f,0.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scaleM;
  	glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  	map2.other(0);
  	modelM = translateM * translate(vec3(-0.5f,clock_back,0.0f)) * rotate((float)((float)rotasbigbig/60*2*M_PI+alphaGlass),vec3(1.0f,0.0f,0.0f)) * rotate((float)-M_PI/2.0f,vec3(0.0f,1.0f,0.0f)) * scaleM;
  	glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  	map2.other(3);
  }

  if(push_b1){
    startcount=!startcountbefore;
    push=bezierdegre2simple(0.0,0.3,0.0,push_time+=0.03);
  }
  else{
    startcountbefore=startcount;
  }
  if(push_time>1.0){
    push_b1=false;
    push_time=0.0;
  }

  glDisable(GL_CULL_FACE);
  modelM = translate(vec3(50.0f+push,2.0f,15.0f))*rotate((float)(M_PI/2),vec3(0.0f,0.0f,1.0f)) ;
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  glUniform1i(glGetUniformLocation(shaderID,"gemColor"),-3);
  button.other(6);
  button.other(7);
  button.other(8);
  //SOCLE
  modelM = translate(vec3(50.0f,2.0f,15.0f))*rotate((float)(M_PI/2),vec3(0.0f,0.0f,1.0f)) ;
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  glUniform1i(glGetUniformLocation(shaderID,"gemColor"),0);
  button.other(9);

  modelM = translate(vec3(49.36f+push,2.02f,15.0f))*rotate((float)(-M_PI/2),vec3(1.0f,0.0f,0.0f))*rotate((float)(M_PI/2),vec3(0.0f,0.0f,1.0f))*scale(vec3(0.07f,0.07f,0.07f));
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  invader.draw();
  glEnable(GL_CULL_FACE);

  modelM = mat4(1.0f);
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  glUniform1i(glGetUniformLocation(shaderID,"gemColor"),0);
  cat.draw();

}

void render(GLuint shaderID){
  static float alphaSun=0.00;

  static mat4 scaleM;
  static mat4 translateM;  
  static mat4 translateMbis;  
  static mat4 rotateM;    
  static mat4 rotateM2;    
  static mat4 modelM;    

  static Model map1((GLchar*)"rsc/map/level1down.obj",shaderID,1);
  static Model portal((GLchar*)"rsc/map/portal2.obj",shaderID,1);
  static Model map1cantcull((GLchar*)"rsc/map/level1rest.obj",shaderID,1);
  static Model sun((GLchar*)"rsc/Sun/Termanation.obj",shaderID,1);


  static int gemID[NB_GEMS];
  static int gemColor[NB_GEMS];
  static float HMcolor[NB_GEMS];
  static float xGem[NB_GEMS];
  static float yGem[NB_GEMS];
  static bool first=true;


// cout<<shaderID<<endl;    
  glUseProgram(shaderID);
  scaleM=scale(vec3(4.0f,4.0f,4.0f));
  translateM=translate(vec3(105.0f,0.0f,15.0f));
  modelM = translateM * rotate((float)M_PI,vec3(0.0f,1.0f,0.0f)) *scaleM;
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  glUniform1i(glGetUniformLocation(shaderID,"dID"),true);
  glUniform1i(glGetUniformLocation(shaderID,"gemColor"),5);

  portal.draw();

  modelM=translate(vec3(106.0f,3.0f,15.0f))*rotate((float)-M_PI/2,vec3(0.0f,1.0f,0.0f))*scale(vec3(6.0f,6.0f,6.0f));
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);

  glBindVertexArray(vao[0]);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(2);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);



  scaleM=scale(vec3(100.0f,100.0f,100.0f));
  translateM=translate(vec3(54.0f,0.0f,54.0f));
  modelM = translateM * scaleM;
  glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
  glUniform1i(glGetUniformLocation(shaderID,"dID"),true);
  glUniform1i(glGetUniformLocation(shaderID,"gemColor"),0);


  map1.draw();
  glDisable(GL_CULL_FACE);
  map1cantcull.draw();
  glEnable(GL_CULL_FACE); 

  if(shaderID==programID||shaderID==shadowID||shaderID==shamapID){
    scaleM=scale(vec3(10.0f,10.0f,10.0f));
    translateM=translate(vec3(lPos[0],lPos[1],lPos[2]));
    modelM=rotate(alphaSun+=0.01f,vec3(0.0f,(float)M_PI,0.0f));
    modelM = translateM * modelM * scaleM ;
    glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
    glUniform1i(glGetUniformLocation(shaderID,"follow"), false);
    glUniform1i(glGetUniformLocation(shaderID,"dID"),false);
    sun.other(1);
    sun.other(0);
  }

  if(shaderID==programID||shaderID==shadowID||shaderID==shamapID){
    scaleM=scale(vec3(10.0f,10.0f,10.0f));
    translateM=translate(vec3(lPos[0],lPos[1],lPos[2]));
    modelM=rotate(alphaSun+=0.005f,vec3(0.0f,(float)M_PI,0.0f));
    modelM = translateM * modelM * scaleM ;
    glUniformMatrix4fv(glGetUniformLocation(shaderID,"M"), 1, GL_FALSE, &modelM[0][0]);
    glUniform1i(glGetUniformLocation(shaderID,"follow"), false);
    glUniform1i(glGetUniformLocation(shaderID,"dID"),false);
    sun.other(2);
  }


  renderwater();

  if(first==true){
    for(int i=0;i<NB_GEMS;++i){
      xGem[i]=104*(float)rand()/(float)RAND_MAX+2;
      yGem[i]=104*(float)rand()/(float)RAND_MAX+2;
      HMcolor[i]=((HMimagebis[(int)(yGem[i]*2048/108)][(int)(xGem[i]*2048/108)]+HMimagebis[(int)(yGem[i]*2048/108)][(int)((yGem[i]+1)*2048/108)]+HMimagebis[(int)(yGem[i]*2048/108)][(int)((xGem[i]+2)*2048/108)])/3+
        (HMimagebis[(int)(yGem[i]*2048/108)+1][(int)(xGem[i]*2048/108)]+HMimagebis[(int)(yGem[i]*2048/108)+1][(int)((xGem[i]+1)*2048/108)]+HMimagebis[(int)(yGem[i]*2048/108)+1][(int)((xGem[i]+2)*2048/108)])/3+
        (HMimagebis[(int)(yGem[i]*2048/108)-1][(int)(xGem[i]*2048/108)]+HMimagebis[(int)(yGem[i]*2048/108)-1][(int)((xGem[i]+1)*2048/108)]+HMimagebis[(int)(yGem[i]*2048/108)-1][(int)((xGem[i]+2)*2048/108)])/3+
        (HMimagebis[(int)(yGem[i]*2048/108)][(int)(xGem[i]*2048/108)+1]+HMimagebis[(int)(yGem[i]*2048/108)][(int)((xGem[i]+1)*2048/108)+1]+HMimagebis[(int)(yGem[i]*2048/108)][(int)((xGem[i]+2)*2048/108)+1])/3+
        (HMimagebis[(int)(yGem[i]*2048/108)][(int)(xGem[i]*2048/108)-1]+HMimagebis[(int)(yGem[i]*2048/108)][(int)((xGem[i]+1)*2048/108)-1]+HMimagebis[(int)(yGem[i]*2048/108)][(int)((xGem[i]+2)*2048/108)-1])/3+

        (HMimagebis[(int)(yGem[i]*2048/108)+1][(int)(xGem[i]*2048/108)+1]+HMimagebis[(int)(yGem[i]*2048/108)+1][(int)((xGem[i]+1)*2048/108)+1]+HMimagebis[(int)(yGem[i]*2048/108)+1][(int)((xGem[i]+2)*2048/108)+1])/3+
        (HMimagebis[(int)(yGem[i]*2048/108)+1][(int)(xGem[i]*2048/108)-1]+HMimagebis[(int)(yGem[i]*2048/108)+1][(int)((xGem[i]+1)*2048/108)-1]+HMimagebis[(int)(yGem[i]*2048/108)+1][(int)((xGem[i]+2)*2048/108)-1])/3+
        (HMimagebis[(int)(yGem[i]*2048/108)-1][(int)(xGem[i]*2048/108)+1]+HMimagebis[(int)(yGem[i]*2048/108)-1][(int)((xGem[i]+1)*2048/108)+1]+HMimagebis[(int)(yGem[i]*2048/108)-1][(int)((xGem[i]+2)*2048/108)+1])/3+
        (HMimagebis[(int)(yGem[i]*2048/108)-1][(int)(xGem[i]*2048/108)-1]+HMimagebis[(int)(yGem[i]*2048/108)-1][(int)((xGem[i]+1)*2048/108)-1]+HMimagebis[(int)(yGem[i]*2048/108)-1][(int)((xGem[i]+2)*2048/108)-1])/3)/9;
      HMcolor[i]=HMcolor[i]/8-6;
      gemID[i]=i%16+1;
      gemColor[i]=i%3+1;
    }
    first=false;
  }
  for(int i=0;i<NB_GEMS;++i){
    renderGem(gemID[i],0,vec3(xGem[i],HMcolor[i],yGem[i]),vec3(1.2f,1.6f,1.2f),shaderID,i,gemColor[i]);
  }  
  for(int i=0;i<NB_GEMS;++i){
    renderGem(gemID[i],1,vec3(xGem[i],HMcolor[i],yGem[i]),vec3(1.8f,1.8f,1.8f),shaderID,i,-gemColor[i]);
  }

}


int main(void){

  if(!initAll())return -1;
  if(!initTex())return -1;

  ps1ID = loadShaders( "shaders/ps1.vs", "shaders/ps1.fs" ,false,"");
  programID = loadShaders( "shaders/all.vs", "shaders/all.fs" ,false,"");
  skyboxID = loadShaders( "shaders/skybox.vs", "shaders/skybox.fs" ,false,""); 
  shadowID = loadShaders( "shaders/shadow.vs", "shaders/shadow.fs" ,false,"");
  shamapID = loadShaders( "shaders/shadow_normal.vs", "shaders/shadow_normal.fs" ,false,"");
  shadowDepthID = loadShaders( "shaders/shadowDepth.vs", "shaders/shadowDepth.fs" ,false,"");
  portalID = loadShaders( "shaders/portal.vs", "shaders/portal.fs" ,false,"");
  debugID = loadShaders( "shaders/debug.vs", "shaders/debug.fs" ,false,"");
  fadeID = loadShaders( "shaders/fade.vs", "shaders/fade.fs" ,false,"");
  waterID = loadShaders( "shaders/water.vs", "shaders/water.fs", false, "shaders/water.gs");
//grassID = loadShaders( "shaders/grass.vs", "shaders/grass.fs", false, "shaders/grass.gs");

GLuint tempID=shamapID;  //DEFINIT LE SHADER PRINCIPAL QU ON UTILISE


glUseProgram(tempID);
glUniform1i(glGetUniformLocation(tempID, "diffuseTexture"), 0);
glUniform1i(glGetUniformLocation(tempID, "shadowMap"), 1);
glUniform1i(glGetUniformLocation(tempID, "normalMap"), 1);

////////
//TEST//
////////
// for(int aze=0;aze<Twidth*3*Theight;aze+=3){
//   cout<<" r="<<(int)Timage[aze]<<" g="<<(int)Timage[aze+1]<<" b="<<(int)Timage[aze+2]<<endl;
// }



//LUMIERE -> OMBRE 
GLfloat near_plane=10.0f,far_plane=150.0f;
//GLfloat near_plane=-10.0f,far_plane=20.0f;
mat4 lightProjection,lightView;

double sleepTime(0);
glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
do{


  clk=glfwGetTime();
  delta=clk-last_clk;
  last_clk=clk;   

  sleepTime+=delta;
  if(sleepTime>=1.0/60.0){
//60 fps limit, trust me i am an engineer


      changeView(delta);

      glUniformMatrix4fv(glGetUniformLocation(tempID,"V"), 1, GL_FALSE, &viewOutM[0][0]);
      glUniformMatrix4fv(glGetUniformLocation(tempID,"P"), 1, GL_FALSE, &projectionM[0][0]);
      glUniformMatrix4fv(glGetUniformLocation(tempID,"lightSpaceMatrix"), 1, GL_FALSE, value_ptr(lightSpaceMatrix));
      glUniform3fv(glGetUniformLocation(tempID,"lightColor"), 1, &lColor[0]);
      glUniform3fv(glGetUniformLocation(tempID,"lightPos"), 1, &lPos[0]);
      glUniform3fv(glGetUniformLocation(tempID,"eyePos"), 1, &position[0]);
      glUniform1i(glGetUniformLocation(tempID, "shadows"), true);
	  glUniform1i(glGetUniformLocation(tempID,"display_world"),display_world);


    lightProjection = perspective((float)M_PI/2.0f, (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); 
    lInvDir=normalize(lPos-vec3(0.0f,0.0f,0.0f));
    lightView = lookAt(lPos,lPos-lInvDir,vec3(0.0f,1.0f,0.0f));
//lightProjection = ortho<float>(-10,10,-10,10,near_plane,far_plane);
//lInvDir=normalize(lPos-vec3(0.0f,0.0f,0.0f));
//lightView = lookAt(lInvDir,vec3(0,0,0),vec3(0.0f,1.0f,0.0f));
    lightSpaceMatrix = lightProjection * lightView * mat4(1.0f);



    if(timeline<300){
      glUseProgram(ps1ID);
      glViewport(0, 0, w, h);
      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
      renderps1();
      if(timeline==260)
      	fadeinv=-2;
      if(timeline==299)
      	fadeinv=1;
      renderfade();
    }
    else{
      glUseProgram(shadowDepthID); 
      glCullFace(GL_FRONT);  
      glUniformMatrix4fv(glGetUniformLocation(shadowDepthID,"lightSpaceMatrix"), 1, GL_FALSE, value_ptr(lightSpaceMatrix));
      glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
      glBindFramebuffer(GL_FRAMEBUFFER, fbo[0]);
      glClear(GL_DEPTH_BUFFER_BIT);
      render(shadowDepthID);
      renderTexts(shadowDepthID);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);


//un jour j arriverais a faire des portails magiques avec ca...
      glUseProgram(waterID);
      glUniformMatrix4fv(glGetUniformLocation(waterID,"V"), 1, GL_FALSE, &viewOutM[0][0]);
      glUseProgram(tempID);
      glUniformMatrix4fv(glGetUniformLocation(tempID,"V"), 1, GL_FALSE, &viewOutM[0][0]);
      glViewport(0, 0, w, h);
      glBindFramebuffer(GL_FRAMEBUFFER, fbo[1]);
      // if(display_world)
      //   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      // else
      //   glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
      glCullFace(GL_BACK); 

      
      if(display_world){ 
	    glUniform1i(glGetUniformLocation(tempID,"toon"),false);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex[0]);
        renderskinned(programID,true,true);
        render2(tempID);
      }
      else{
	    glUniform1i(glGetUniformLocation(tempID,"toon"),true);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex[1]);
        renderskinned(programID,true,false);
        renderBox();
        render(tempID);
      }
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      glUseProgram(portalID);
      glViewport(0, 0, w, h);
      // if(display_world)
      //   glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      // else
      //   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
      glDisable(GL_CULL_FACE);
      static mat4 portalModelM=translate(vec3(105.0f,3.0f,15.0f))*rotate((float)-M_PI/2,vec3(0.0f,1.0f,0.0f))*scale(vec3(6.0f,6.0f,6.0f));
      glUniformMatrix4fv(glGetUniformLocation(portalID,"M"), 1, GL_FALSE, &portalModelM[0][0]);
      glUniformMatrix4fv(glGetUniformLocation(portalID,"V"), 1, GL_FALSE, &viewM[0][0]);
      glUniformMatrix4fv(glGetUniformLocation(portalID,"P"), 1, GL_FALSE, &projectionM[0][0]);
      glUniform1i(glGetUniformLocation(portalID,"gemColor"),-20);
      glBindVertexArray(vao[0]);
      glBindTexture(GL_TEXTURE_2D, tex[17]);
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(2);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glDisableVertexAttribArray(2);
      glDisableVertexAttribArray(0);
      glBindVertexArray(0);
      glEnable(GL_CULL_FACE);



      glUseProgram(waterID);
      glUniformMatrix4fv(glGetUniformLocation(waterID,"V"), 1, GL_FALSE, &viewM[0][0]);
      glUseProgram(tempID);
      glCullFace(GL_BACK);   
      glUniformMatrix4fv(glGetUniformLocation(tempID,"V"), 1, GL_FALSE, &viewM[0][0]);
      glUniformMatrix4fv(glGetUniformLocation(tempID,"P"), 1, GL_FALSE, &projectionM[0][0]);
      glUniformMatrix4fv(glGetUniformLocation(tempID,"lightSpaceMatrix"), 1, GL_FALSE, value_ptr(lightSpaceMatrix));
      glUniform3fv(glGetUniformLocation(tempID,"lightColor"), 1, &lColor[0]);
      glUniform3fv(glGetUniformLocation(tempID,"lightPos"), 1, &lPos[0]);
      glUniform3fv(glGetUniformLocation(tempID,"eyePos"), 1, &position[0]);
      glUniform1i(glGetUniformLocation(tempID, "shadows"), true);


      if(display_world){
	    glUniform1i(glGetUniformLocation(tempID,"toon"),true);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex[1]);
        renderskinned(programID,false,false);
        renderBox();
        renderfadebehind(); 
        render(tempID);
        renderTexts(tempID);
        renderfade();
      }
      else{
	    glUniform1i(glGetUniformLocation(tempID,"toon"),false);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex[0]);
        renderskinned(programID,false,true);
        renderfadebehind();
        render2(tempID); 
        renderTexts(tempID);
        renderfade();
      }
// A FAIRE EN DERNIER AVEC TOUT CE QUI EST U.I.


    }





    glfwSwapBuffers(window);
    glfwGetWindowSize(window,&w,&h);
    printFps();
    timeline++;



    sleepTime=0;
  }
}
while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );


glfwTerminate();
return 0;
}

