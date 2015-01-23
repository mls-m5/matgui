#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "linearlayout.h"
using namespace std;



#include "button.h"
// If using gl3.h
/* Ensure we are using opengl's core profile only */
#define GL3_PROTOTYPES 1
#include <GL/gl.h>
#include "draw.h"
#include "knobview.h"

#include <SDL2/SDL.h>
#define PROGRAM_NAME "sdl-gui"

// A simple function that prints a message, the error code returned by SDL,
// and quits the application
void sdldie(const char *msg)
{
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}


void checkSDLError(int line = -1)
{
#ifndef NDEBUG
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		printf("SDL Error: %s\n", error);
		if (line != -1)
			printf(" + line: %i\n", line);
		SDL_ClearError();
	}
#endif
}



class Program {
public:


    SDL_Window *mainwindow; /* Our window handle */
    SDL_GLContext maincontext; /* Our opengl context handle */
    KnobView knob;
    Button button;
    Button button2;
    LinearLayout layout;

    bool running = false;

    Program() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
            sdldie("Unable to initialize SDL"); /* Or die on error */

        /* Request opengl 3.2 context.
         * SDL doesn't have the ability to choose which profile at this time of writing,
         * but it should default to the core profile */
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2); //This prevents the rendering function from rendering anything of some reason

    //    Turn on double buffering with a 24bit Z buffer.
    //    You may need to change this to 16 or 32 for your system
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        // Create our window centered at 512x512 resolution
        mainwindow = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (!mainwindow) /* Die if creation failed */
            sdldie("Unable to create window");

        checkSDLError(__LINE__);

        // Create our opengl context and attach it to our window
        maincontext = SDL_GL_CreateContext(mainwindow);
        checkSDLError(__LINE__);

        initDrawModule(512, 512);

        // This makes our buffer swap syncronized with the monitor's vertical refresh
        SDL_GL_SetSwapInterval(1);

        layout.setLocation(0,0, 200,200);
        layout.addChild(&knob);
        layout.addChild(&button);
        layout.addChild(&button2);

        button.clicked.connect(this, &Program::onClicked);
        button.pointerMoved.connect(this, &Program::onClicked); //just tetsing
        button.pointerMoved.disconnect(this);

    }

    //callback function for the button
    void onClicked(View::pointerArgument event) {
    	cout << "hej " << event.x << endl;
    }

    //Rendering function
    void render(){
//    	drawSquare(vec(1, 1, 1), 0, 100,100, DRAW_STYLE_FILLED);
    	layout.draw();
//    	knob.draw();
//    	button.draw();
    	button2.draw();
    }

    void mainLoop() {
    	running = true;
        while (running) {
        	// Clear our buffer with a red background
//        	glClearColor ( 0.1 * i, 0.0, 0.0, 1.0 );
        	glClear ( GL_COLOR_BUFFER_BIT );
        	render();
        	//Swap our back buffer to the front
        	SDL_GL_SwapWindow(mainwindow);
        	// Wait
//        	SDL_Delay(200);


        	if (handleEvents()) {
        		break;
        	}

        	SignalBase::flushAll();
        }
    }

    bool handleEvents() {
    	SDL_Event event;
    	while (SDL_PollEvent(&event)) {
    		if (event.type == SDL_QUIT) {
    			return true;
    		}

    		switch (event.type) {
    		case SDL_MOUSEMOTION:
    		{
    			auto &e = event.motion;
    			layout.onPointerMove(0, (double)e.x, (double)e.y, 1);
    		}
    		break;
    		case SDL_MOUSEBUTTONDOWN:
    		{
    			auto &e = event.motion;
    			layout.onPointerDown(0, (double)e.x, (double)e.y);
    		}
    		break;

    		case SDL_MOUSEBUTTONUP:
    		{
    			auto &e = event.motion;
    			layout.onPointerUp(0, (double)e.x, (double)e.y);
    		}
    		break;
    		default:
    			break;
    		}
    	}
    	return false;
    }

    ~Program() {
        QuitDrawModule();

        // Delete our opengl context, destroy our window, and shutdown SDL
        SDL_GL_DeleteContext(maincontext);
        SDL_DestroyWindow(mainwindow);
        SDL_Quit();
    }

};



/* Our program's entry point */
int main(int argc, char *argv[])
{

	{
		Program program;
		program.mainLoop();
	}

    return 0;
}
