#include "../headers/init.h"

bool initAll(void){ 
	srand(time(0));

	if(!glfwInit()){
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return false;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(w,h, "ondedaydirectx", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE; 
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return false;
	}
//glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//cacher le pointeur a faire a la fin
	glfwSetTime(0.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	SDL_Init(SDL_INIT_AUDIO);
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) 
//Initialisation de l'API Mixer
	{
		printf("%s", Mix_GetError());
	}

	if(!(_psx_ = Mix_LoadMUS("rsc/music/psx.mp3"))) {
		fprintf(stderr, "Erreur lors du Mix_LoadMUS: %s\n", Mix_GetError());
		exit(-5);
	}
	if(!(_wizard_ = Mix_LoadMUS("rsc/music/wizard.mp3"))) {
		fprintf(stderr, "Erreur lors du Mix_LoadMUS: %s\n", Mix_GetError());
		exit(-5);
	}
	Mix_VolumeMusic(MIX_MAX_VOLUME/4);
	Mix_AllocateChannels(14);
//0 to 4 dedicated to gems
//5 to earthquake
//6 to button
//7 to 13 clock
	if(!(_gem_took_ = Mix_LoadWAV("rsc/music/gem.ogg"))) {
		fprintf(stderr, "Erreur lors du Mix_LoadWAV: %s\n", Mix_GetError());
		exit(-5);
	}
	Mix_VolumeChunk(_gem_took_, MIX_MAX_VOLUME*1/8);

	if(!(_clock_tick_ = Mix_LoadWAV("rsc/music/clock.ogg"))) {
		fprintf(stderr, "Erreur lors du Mix_LoadWAV: %s\n", Mix_GetError());
		exit(-5);
	}
	Mix_VolumeChunk(_clock_tick_, MIX_MAX_VOLUME*4/8);
    if(!(_clock_tick_max_ = Mix_LoadWAV("rsc/music/clock_max.ogg"))) {
		fprintf(stderr, "Erreur lors du Mix_LoadWAV: %s\n", Mix_GetError());
		exit(-5);
	}
	Mix_VolumeChunk(_clock_tick_max_, MIX_MAX_VOLUME*4/8);

	if(!(_button_push_ = Mix_LoadWAV("rsc/music/button.ogg"))) {
		fprintf(stderr, "Erreur lors du Mix_LoadWAV: %s\n", Mix_GetError());
		exit(-5);
	}
	Mix_VolumeChunk(_button_push_, MIX_MAX_VOLUME);

    if(!(_earthquake_ = Mix_LoadWAV("rsc/music/earthquake.ogg"))) {
		fprintf(stderr, "Erreur lors du Mix_LoadWAV: %s\n", Mix_GetError());
		exit(-5);
	}
	Mix_VolumeChunk(_earthquake_, MIX_MAX_VOLUME);

    if(!(_earthquake_end_ = Mix_LoadWAV("rsc/music/earthquake_end.ogg"))) {
		fprintf(stderr, "Erreur lors du Mix_LoadWAV: %s\n", Mix_GetError());
		exit(-5);
	}
	Mix_VolumeChunk(_earthquake_end_, MIX_MAX_VOLUME);

	glPolygonOffset(0.0,0.0);
	glEnable(GL_DEPTH_TEST);
//glEnable(GL_CULL_FACE); //a mettre sans alpha a la fin des test 
	glDepthFunc(GL_LESS);

//glDepthRange(0.0f,1.0f);
//glEnable(GL_BLEND);
//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//or glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA)
//glBlendFunc(GL_ONE, GL_ONE);

glEnable(GL_CULL_FACE); //A ENABLE SUR LES NON-TRANSPARENTS -> NE DESSINE QUE D UN COTE PAR RAPPORT A LA CAM
glCullFace(GL_BACK);  
return true;
}


void printFps(void)
{ float t;
	static float t0=0,f= 0;
	f++;
	t = glfwGetTime();
	if(t - t0 > 2.000) { 
		printf("%f\n", (f / (t - t0)));
		t0 = t;
		f  = 0; 
	}
}
