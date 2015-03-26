#include <SDL.h>
#include <stdio.h>
#include "Application.h"

const int SCREEN_W = 640;
const int SCREEN_H = 480;

int main(int argc, char* argv[]) {
	// The application manages the display window
	Application app;
	SDL_Event e;
	bool exit = false;

	if (app.Initialize()) {
		// Main application loop - poll events until exit
		while (!exit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					SDL_Log("Exit signal recieved");
					exit = true;
				}
				else {
					app.HandleEvent(e);
				}
			}

			// Paint the window
			app.PaintWindow();
			// Don't hog the CPU
			SDL_Delay(1);
		}
	}

	// Cleanup and exit
	app.Cleanup();
	return 0;
}