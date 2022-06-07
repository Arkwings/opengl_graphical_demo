#include "../headers/objloader.h"

void initSprites(void);
 
void initPortals(void);


vec3 beziertest(vec3 depart,vec3 middle,vec3 arrivee,float t){
	vec3 bob;
	bob[0]=(1-t)*(1-t)*depart[0]+2*t*(1-t)*middle[0]+t*t*arrivee[0];
	bob[1]=(1-t)*(1-t)*depart[1]+2*t*(1-t)*middle[1]+t*t*arrivee[1];
	bob[2]=(1-t)*(1-t)*depart[2]+2*t*(1-t)*middle[2]+t*t*arrivee[2];
	return bob;
}


float bezierdegre2simple(float depart,float middle,float arrivee,float t){
	return (1-t)*(1-t)*depart+2*t*(1-t)*middle+t*t*arrivee;
}

vec3 vibrator(vec3 wheretovibrate,float intens){
	static float t=0;
	float vib=bezierdegre2simple(-0.15,0.15,-0.15,t+=intens*0.01);
	if(t>1)
		t=0;
	return wheretovibrate*vib;
}

pair<vec3,vec3> bezierdegre2(vec3 depart,vec3 arrivee,vec3 initialScale,vec3 endScale,float t,float hauteur){

arrivee[2]-=0.7; // pour mettre au milieu du eprso sur Z
arrivee[1]+=1.2; // pour mettre au milieu du perso sur Y

vec3 pos1;
pos1[0]=(arrivee[0]+depart[0])/2;
pos1[1]=(arrivee[1]+depart[1])/2+hauteur;
pos1[2]=(arrivee[2]+depart[2])/2; 

vec3 scale1;
scale1[0]=(endScale[0]+initialScale[0])*4/5;
scale1[1]=(endScale[1]+initialScale[1])*4/5;
scale1[2]=(endScale[2]+initialScale[2])*4/5;

pair<vec3,vec3> posAndScale; 
posAndScale.first[0] = (1-t)*(1-t)*depart[0] + 2*t*(1-t)*pos1[0] + t*t*arrivee[0];
posAndScale.first[1] = (1-t)*(1-t)*depart[1] + 2*t*(1-t)*pos1[1] + t*t*arrivee[1];
posAndScale.first[2] = (1-t)*(1-t)*depart[2] + 2*t*(1-t)*pos1[2] + t*t*arrivee[2];
posAndScale.second[0] = (1-t)*(1-t)*initialScale[0] + 2*t*(1-t)*scale1[0] + t*t*endScale[0];
posAndScale.second[1] = (1-t)*(1-t)*initialScale[0] + 2*t*(1-t)*scale1[0] + t*t*endScale[0];
posAndScale.second[2] = (1-t)*(1-t)*initialScale[0] + 2*t*(1-t)*scale1[0] + t*t*endScale[0];



return posAndScale;
}



void v_recurences(float verticesentree[18],float * &verticessortie,int nb_width,int nb_height){
	int acc_width=0,acc_height=0;
	verticessortie=new float [nb_width*nb_height*18];
	for(int j=0;j<nb_height;++j){
		for(int i=0;i<nb_width;++i){
			for(unsigned int z=0;z<18;z+=3){
				verticessortie[z+j*18*nb_width+i*18]  =verticesentree[z]+acc_width;
				verticessortie[z+1+j*18*nb_width+i*18]=verticesentree[z+1];
				verticessortie[z+2+j*18*nb_width+i*18]=verticesentree[z+2]+acc_height;
//cout<<"ababa "<<verticesentree[z]<<verticesentree[z+1]<<verticesentree[z+2]<<endl;
//cout<<"babab "<<verticessortie[z]<<verticessortie[z+1]<<verticessortie[z+2]<<endl;
			}
			acc_width++;
		}
		acc_width=0;
		acc_height++;
	}
}






bool initTex(void){     
	float vertices[]={
		-1.0f,-1.0f, 0.0f, 
		1.0f,-1.0f, 0.0f, 
		-1.0f, 1.0f, 0.0f, 
		-1.0f, 1.0f, 0.0f, 
		1.0f,-1.0f, 0.0f, 
		1.0f, 1.0f, 0.0f 
	};
	float vtex[]={
		0.0f , 0.0f,
		1.0f , 0.0f,
		0.0f , 1.0f,
		0.0f , 1.0f,
		1.0f , 0.0f,
		1.0f , 1.0f
	};

	glGenVertexArrays(1, &vao[0]);
	glGenBuffers(1,&vbo[0]);
	glGenBuffers(1,&vbo[10]);
	glBindVertexArray(vao[0]);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
	glDisableVertexAttribArray(0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtex), vtex, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (GLvoid*)0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	float vertices2[]={
		0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 1.0f, 
		1.0f, 0.0f, 0.0f, 
		1.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 1.0f, 
		1.0f, 0.0f, 1.0f 
	};

	glGenVertexArrays(1, &vao[1]);
	glGenBuffers(1,&vbo[1]);
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);


	float * verticesrecur;
	int nb_width=32,nb_height=32;
	v_recurences(vertices2,verticesrecur,nb_width,nb_height);

	glGenVertexArrays(1, &vao[3]);
	glGenBuffers(1,&vbo[3]);
	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesrecur)*18*nb_width*nb_height, &verticesrecur[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	delete[] verticesrecur;


 






//loadTexture(tex[9],(GLchar*)"rsc/vater/vater.png\0",true); 
//cette ligne va causer des segfault pour des raisons non apparentes
	loadTexture(tex[10],(GLchar*)"rsc/black/black.png\0",false);




	initSprites();

	initPortals();


 //L OMBRE
	glGenFramebuffers(1, &fbo[0]);
	glGenTextures(1, &tex[1]);
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
	glGenerateMipmap(GL_TEXTURE_2D);    
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER,fbo[0]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex[1], 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


//LE SOIT DISANT PORTAIL MAIS IL MARCHERA SUREMENT JAMAIS
	//le fbo dans lequel on va soit disant mettre la camera de l autre
	glGenFramebuffers(1, &fbo[1]);
	glBindFramebuffer(GL_FRAMEBUFFER,fbo[1]);

    tex[17] = generateAttachmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,tex[17],0);

	glGenRenderbuffers(1, &rbo[1]);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo[1]);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,1920,1920);
	glBindRenderbuffer(GL_RENDERBUFFER,0);
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo[1]);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);






	HMimage = SOIL_load_image("rsc/map/heightmaprotatebis.png", &HMwidth, &HMheight, 0,SOIL_LOAD_RGB);
	HMimagebis=new unsigned char* [HMheight];
	for(int j=0;j<HMheight;++j){
		HMimagebis[j]=new unsigned char [HMwidth*3];
	}
	for(int j=0;j<HMheight;++j){
		for(int i=0;i<HMwidth*3;i+=3){
			HMimagebis[j][i]=HMimage[i*3+j*HMwidth*3];
			HMimagebis[j][i+1]=HMimage[i*3+1+j*HMwidth*3];
			HMimagebis[j][i+2]=HMimage[i*3+2+j*HMwidth*3];
//cout<<(int)HMimagebis[j][i]<<"  "<<(int)HMimagebis[j][i+1]<<"  "<<(int)HMimagebis[j][i+2]<<endl;
		}
	}
	glGenTextures(1, &tex[15]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex[15]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, HMwidth, HMheight, 0, GL_RGB, GL_UNSIGNED_BYTE, HMimage);
 // Parameters
	glGenerateMipmap(GL_TEXTURE_2D); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);  
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);





	return true;
}










void loadTexture(GLuint &ID,char* path,GLboolean alpha){
	static int i=0;
 //Generate texture ID and load texture data 
	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0+i++);
	int width,height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
 // Assign texture to ID
	cout<<path<<'='<<width<<" x "<<height<<endl<<endl;
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);

 // Parameters
	glGenerateMipmap(GL_TEXTURE_2D); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT );  
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
}



void initSprites(void){
	int width,height;
	unsigned char* image;
	GLchar* path=(GLchar*)"rsc/3Dgems/sparkling/sparkle\0";
	GLchar* c_path=new GLchar[35];
	for(int i=0;i<32;++i){
		for(unsigned int j=0;j<28;++j){
			c_path[j]=path[j];
		}
		if(i<10){
			c_path[28]='0';
			c_path[29]='0'+i;
		}
		else{
			c_path[28]='0'+i/10;
			c_path[29]='0'+i-i/10*10;
		}
		c_path[30]='.';
		c_path[31]='p';
		c_path[32]='n';
		c_path[33]='g';
		c_path[34]='\0';
		image = SOIL_load_image(c_path, &width, &height, 0,SOIL_LOAD_RGBA);
		cout<<c_path<<'='<<width<<" x "<<height<<endl;
		glGenTextures(1, &sparkle_sprites[i]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,sparkle_sprites[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );  
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap( GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
	}
	delete[] c_path;
	cout<<endl;
}




void initPortals(void){


}











GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil){
 // What enum to use?
	GLenum attachment_type;
	if(!depth && !stencil)
		attachment_type = GL_RGB;
	else if(depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if(!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;

 //Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if(!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, w, h, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
// Using both a stencil and depth test, needs special format arguments
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, w, h, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}
