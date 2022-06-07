#include "../headers/init.h"




float wave(float min,float max){
	static float i=max-(max-min)/2;
	static int up=1;
	float inc=(max-min)/100;

	i+=inc*up;

	if(i>max)
		up=-1;

	if(i<min)
		up=1;

	return i;
}


void lockCamera(vec3 personnage){
	position[0]=positionPerso[0]+(cos(horizontalAngleCam+M_PI/2))*3;		
	position[2]=positionPerso[2]-(sin(horizontalAngleCam+M_PI/2))*3;		
	position[1]=positionPerso[1]+4.0-(sin(verticalAngleCam))*7.5;		
}

void lockCameraBis(vec3 personnage,float longueur){
	position[0]=positionPerso[0]+(cos(horizontalAngleCam+M_PI/2))*longueur;		
	position[2]=positionPerso[2]-(sin(horizontalAngleCam+M_PI/2))*longueur;		
	position[1]=positionPerso[1]+4.0-(sin(verticalAngleCam))*7.5;		
}


void changeView(float deltaTime){

	static float initialFov = M_PI/2.0f;
static float speedh = 0.1f; // 3 units / second
static float speedv = 0.08f;
static float mouseSpeed = 0.0015f;
static double xpos=w/2, ypos=h/2;
static float teta=0.0f,alpha=(float)M_PI/4.0f; //pour la lumiere
static bool lock=false;
static int poschange=1;

deplacement=false;
static bool up_key=false;

//CAMERA
glfwSetCursorPos(window,w/2, h/2);
glfwPollEvents();
glfwGetCursorPos(window,&xpos,&ypos); // a mettre seulement en control manuel
//cout<<"xpos="<<xpos<<endl;
//cout<<"ypos="<<ypos<<endl;
glfwGetWindowSize(window,&w,&h); //a mettre seulement si pas en plein ecra

//DEMO
static bool demo=false;
if(demo){
xpos=w/2;
ypos=h/2;
if(timeline>300){
	if(timeline==301){
		positionbis=vec3(30.0f,5.0f,20.0f);
		ypos=h*2/3;
		lock=!lock;
	}
	if(timeline<540)
		ypos=h/2*wave(0.9835,1.013);
	if(timeline<700&&horizontalAngleCam<M_PI*3)
		xpos=w/2*0.985;
	if(timeline==554)
		anim=1;
	if(timeline==820)
		anim=11;
	if(timeline>820&&timeline<1420)
		positionPerso += vec3(1.0,0.0,0.0) * speedv * 0.8f;
	if(timeline==1120)
		lock=!lock;
	if(timeline>1020&&timeline<1520&&horizontalAngleCam>M_PI*3-M_PI/2){
		xpos=w/2*1.010;
		ypos=h/2*1.006;
	}
	if(timeline>1120&&timeline<1420){
		ypos+=h/2*0.001;
		position[1]+=0.25;
	}
	if(timeline==1420){
		anim=10;
	}
	if(timeline>1420&&timeline<1525){
		positionPerso += vec3(1.0,0.0,0.0) * speedv * 0.7f;
		positionPerso += vec3(0.0,0.0,1.0) * speedh * 0.7f;
		angle_key=-M_PI/4;
	}
	if(timeline>1525&&timeline<1574){		
		positionPerso += vec3(0.0,0.0,1.0) * speedh * 1.5f;
		angle_key=-M_PI/2;
	}
	static vec3 pos1565;
	static vec3 posmiddle1565;
	if(timeline==1574){
		anim=0;
		angle_key=0;
		pos1565=position;
		posmiddle1565=vec3((pos1565[0]+positionPerso[0]+(cos(horizontalAngleCam+M_PI/2))*3)/2,(pos1565[1]+positionPerso[2]-(sin(horizontalAngleCam+M_PI/2))*3)/2-20,(pos1565[2]+positionPerso[1]+4.0-(sin(verticalAngleCam))*7.5)/2);
	}
	static float time1565=0;
	if(timeline>1574&&time1565<1&&timeline<1900){
		time1565+=0.01;
		position=beziertest(pos1565,posmiddle1565,vec3(positionPerso[0]+(cos(horizontalAngleCam+M_PI/2))*3,positionPerso[1]+4.0-(sin(verticalAngleCam))*7.5,positionPerso[2]-(sin(horizontalAngleCam+M_PI/2))*3),time1565);
	}
	if(timeline==1700)
		anim=11;
	if(timeline>1700&&timeline<1760){
		lock=true;
		positionPerso += vec3(1.0,0.0,0.0) * speedv * 0.7f;
	}
	if(timeline>1700&&timeline<1760){
		ypos=h/2*1.006;
	}
	if(timeline==1750){
		anim=14;
	}
	if(timeline>1760&&timeline<1970){
		ypos=h/2*0.995;
	}
	static vec3 pos1800;
	static vec3 pos1800bis;
	static vec3 posmiddle1800;
	static vec3 posmiddle1800bis;
	if(timeline==1800){
		Mix_PlayChannel(6,_button_push_, 0);
		push_b1=true;
		pos1800=position;
		posmiddle1800=vec3(pos1800[0]/2,(pos1800[1]+10)/2,pos1800[2]);
	}
	static float time1800=0;
	if(timeline>1810&&timeline<1970&&time1800<1){
		time1800+=0.008;
		lock=false;
		position=beziertest(pos1800,posmiddle1800,vec3(-40,20,pos1800[2]),time1800);
	}
	if(timeline==1820)
		anim=0;
	if(timeline==1970){
		pos1800bis=position;
		posmiddle1800bis=vec3(pos1800bis[0]/2,80,pos1800bis[2]);
	}
	if(timeline>1970&&time1800>=1&&time1800<2){
		time1800+=0.007;
		position=beziertest(pos1800bis,posmiddle1800bis,vec3(positionPerso[0]+(cos(horizontalAngleCam+M_PI/2))*3,positionPerso[1]+4.0-(sin(verticalAngleCam))*7.5,positionPerso[2]-(sin(horizontalAngleCam+M_PI/2))*3),time1800-1);
	}
	if(timeline==2170){
		lock=true;
	}
	if(timeline>2170&&timeline<3000){
		deplacement=true;
		anim=11;
		if(timeline<2240)
			xpos=w/2*1.009;	
		if(timeline>2240&&timeline<2340){
			up_key=true;
			xpos=w/2*0.993;	
		}
		if(timeline>2340&&timeline<2690){
			up_key=true;
			xpos=w/2*1.015;
		}
		if(timeline>2690&&timeline<3000){
			up_key=true;
			ypos=h/2*1.0015;
		}
	}
	static float accel3000=0;
	if(timeline>3000&&timeline<3150){
		anim=0;
		xpos=w/2*(0.990+accel3000);
		accel3000+=0.000148;
	}
	static float accel3100=0;
	if(timeline>3150&&timeline<3400){
		xpos=w/2*(1.008-accel3100);
		accel3100+=0.000096;
		if(timeline>3200)
			ypos=h/2*0.998;
	}
	static float accel3300=0;
	if(timeline>3400&&timeline<3566){
		xpos=w/2*(0.992+accel3300);
		accel3300+=0.000156;
	}
	if(timeline>3566&&timeline<3790){
		up_key=true;
	}
	static float a=3;
	if(timeline>3790&&timeline<4350){
		lock=false;
		anim=2;
		xpos=w/2*0.99;
		lockCameraBis(positionPerso,a);
		if(timeline<4300)
			a+=0.01;
	}
	static float ababab=0;
	if(timeline>4350&&timeline<7500){  // ON LE DEPLACE DANS LA MAP
		lock=false;
		up_key=true;
		lockCameraBis(positionPerso,a);
		if(timeline<4450)
			xpos=w/2*1.008;
		else if(timeline<4600)
		 	xpos=w/2*0.998;
		else if(timeline<4850)
		 	xpos=w/2*1.006;
		else if(timeline<4980){
		}
		else if(timeline<5050)
		 	xpos=w/2*0.997;
		else if(timeline<5250)
		 	xpos=w/2*1.0063;
		else if(timeline<5450){
		}
		else if(timeline<5650)
		 	xpos=w/2*1.006;
		else if(timeline<5755){
		}
		else if(timeline<5873)
		 	xpos=w/2*1.014;
		else if(timeline<5973){
			ypos=h/2*1.0006;
		}
		else if(timeline<7500){
			if(timeline<6400){
				anim=0;
				a+=0.015;
		 		xpos=w/2*1.014;
				lockCameraBis(positionPerso,a);
			}
			else if(timeline<7200){
				anim=0;
				a+=0.015;
		 		xpos=w/2*1.014;
				lockCameraBis(vec3(positionPerso[0],positionPerso[1]+ababab,positionPerso[2]),a);
				ababab+=0.02;
			}
			else if(timeline<7300){
				anim=25;
				a+=0.015;
		 		xpos=w/2*1.014;
				lockCameraBis(vec3(positionPerso[0],positionPerso[1]+ababab,positionPerso[2]),a);
				ababab+=0.02;
			}
			up_key=false;
		}

	}
	if(timeline>7390)
		exit(0);

	//	cout<<horizontalAngleCam<<endl;
}
}

//FIN DEMO

if(xpos){
	horizontalAngleCam += mouseSpeed * float(w/2-xpos);
}
if(ypos){
	verticalAngleCam += mouseSpeed  * float(h/2-ypos);
} 
if(verticalAngleCam<-M_PI/2+M_PI/8){
	verticalAngleCam=-M_PI/2+M_PI/8;
}
if(verticalAngleCam>M_PI/2-M_PI/8){
	verticalAngleCam=M_PI/2-M_PI/8;
}
//cout<<verticalAngleCam<<endl;
direction = vec3(cos(verticalAngleCam) * sin(horizontalAngleCam),sin(verticalAngleCam),cos(verticalAngleCam) * cos(horizontalAngleCam));
vec3 right = vec3(sin((horizontalAngleCam - M_PI/2.0f)),0,cos((horizontalAngleCam - M_PI/2.0f)));
vec3 up = cross( right, direction );

//decide de quelle camera on bouge
static bool c=false;
if(glfwGetKey(window,GLFW_KEY_C) == GLFW_PRESS) c=true;
if (c==true && glfwGetKey( window, GLFW_KEY_C ) == GLFW_RELEASE){
	poschange=-poschange;
	c=false;
}

if(poschange==1){
	if (glfwGetKey( window, GLFW_KEY_T ) == GLFW_PRESS){
		position += direction * speedv * 5.0f;
	}
	if (glfwGetKey( window, GLFW_KEY_V ) == GLFW_PRESS){
		position -= direction * speedv * 5.0f;
	}
	if (glfwGetKey( window, GLFW_KEY_G ) == GLFW_PRESS){
		position += right * speedh * 2.5f;
	}
	if (glfwGetKey( window, GLFW_KEY_F ) == GLFW_PRESS){
		position -= right * speedh * 2.5f;
	}
}
else if(poschange==-1){
	if (glfwGetKey( window, GLFW_KEY_T ) == GLFW_PRESS){
		positionbis += direction * speedv * 5.0f;
	}
	if (glfwGetKey( window, GLFW_KEY_V ) == GLFW_PRESS){
		positionbis -= direction * speedv * 5.0f;
	}
	if (glfwGetKey( window, GLFW_KEY_G ) == GLFW_PRESS){
		positionbis += right * speedh * 2.5f;
	}
	if (glfwGetKey( window, GLFW_KEY_F ) == GLFW_PRESS){
		positionbis -= right * speedh * 2.5f;
	}
}

projectionM = perspective(initialFov, (float)(w / h), 0.01f, 250.0f);
//projectionM = ortho<float>(-10,10,-10,10,0.01f,250.0f);
if(lock)
	lockCamera(positionPerso);
viewM = lookAt(position,position+direction,up);
viewOutM = lookAt(positionbis,positionbis+direction,up);

viewBoxM = lookAt(vec3(0.0f),direction,up);



//PERSONNAGE
static bool olddeplacement;
static bool front;
static bool back;
olddeplacement=deplacement;
//deplacement=false;
front=false;
back=false;
if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS || up_key==true){
	positionPerso += direction * speedv * 1.5f;
	up_key=false;
	anim=10;
	deplacement=true;
	front=true;
	angle_key=0;
}
if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
	positionPerso -= direction * speedv * 1.5f;
	anim=10;
	deplacement=true;
	back=true;
	angle_key=(float)M_PI;
}
if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
	positionPerso += right * speedh*0.8f;
	deplacement=true;
	if(front==true&&back==false)
		angle_key-=(float)M_PI/4;
	else if(back==true&&front==false)
		angle_key+=(float)M_PI/4;
	else{
		angle_key=(float)-M_PI/2;
		anim=11;
	}
}
if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
	positionPerso -= right * speedh*0.8f;
	deplacement=true;
	if(front==true&&back==false)
		angle_key+=(float)M_PI/4;
	else if(back==true&&front==false)
		angle_key-=(float)M_PI/4;
	else{
		angle_key=(float)M_PI/2;
		anim=11;
	}
}
if(deplacement==false&&olddeplacement==true){
	olddeplacement=false;
	anim=0;
}



//LUMIERE
if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
	teta += 0.02;
}
if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
	teta -= 0.02;
}
if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
	alpha += 0.02;
}
if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
	alpha -= 0.02;
}
lPos=vec3(cos(alpha)*sin(teta)*100+64,sin(alpha)*100,cos(alpha)*cos(teta)*100+64);





//ANIMATION
static bool space=false;
if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS) space=true;
if (space==true && glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_RELEASE){
	space=false;
	anim++;
	if(anim>25) anim=0;
}


//FADE
static bool b=false;
if(glfwGetKey(window,GLFW_KEY_B) == GLFW_PRESS) b=true;
if (b==true && glfwGetKey( window, GLFW_KEY_B ) == GLFW_RELEASE){
	b=false;
	fade=1.0;
}
if(fadeinv<0||fade>0)fade-=0.015*fadeinv;

//FADE BEHIND
static bool n=false;
if(glfwGetKey(window,GLFW_KEY_N) == GLFW_PRESS) n=true;
if (n==true && glfwGetKey( window, GLFW_KEY_N ) == GLFW_RELEASE){
	n=false;
	fadeb=1.0;
}
if(fadeinv<0||fadeb>0)fadeb-=0.015*fadeinv;





//LOCK
static bool l=false;
if(glfwGetKey(window,GLFW_KEY_L) == GLFW_PRESS) l=true;
if (l==true && glfwGetKey( window, GLFW_KEY_L ) == GLFW_RELEASE){
	l=false;
	lock=!lock;
}


//ECLAIRAGE BAS
static bool kp0=false;
if(glfwGetKey(window,GLFW_KEY_KP_0) == GLFW_PRESS) kp0=true;
if (kp0==true && glfwGetKey( window, GLFW_KEY_KP_0 ) == GLFW_RELEASE){
	kp0=false;
	if(!push_b1)
		Mix_PlayChannel(6,_button_push_, 0);
	push_b1=true;
}

//VITESSE TEMPS
//static bool kp1=false;
//if(glfwGetKey(window,GLFW_KEY_KP_1) == GLFW_PRESS) kp1=true;
if (glfwGetKey( window, GLFW_KEY_KP_1 ) == GLFW_PRESS){
//	kp1=false;
	compteur_inc--;
}
//static bool kp3=false;
//if(glfwGetKey(window,GLFW_KEY_KP_3) == GLFW_PRESS) kp3=true;
if (glfwGetKey( window, GLFW_KEY_KP_3 ) == GLFW_PRESS){
//	kp3=false;
	compteur_inc++;
}





//if(timeline==301)
//	lock=!lock;



}

