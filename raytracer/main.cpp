#include <SDL.h>
#include "Application.h"

const int SCREEN_W = 640;
const int SCREEN_H = 480;

int main(int argc, char* argv[]) {
	// The application manages the display window
	Application app;
	SDL_Event e;
	bool exit = false;


	if (app.Initialize()) {
		//const char *out = tinyfd_saveFileDialog("Hello", "", 0, NULL);
		//SDL_Log("%s", out);


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