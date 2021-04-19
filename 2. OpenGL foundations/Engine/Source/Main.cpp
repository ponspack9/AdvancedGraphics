//
// platform.cpp : This file contains the 'main' function. Program execution begins and ends there.
// The platform layer is in charge to create the environment necessary so the engine disposes of what
// it needs in order to create the application (e.g. window, graphics context, I/O, allocators, etc).
//


#include "pch.h"

enum main_states {
    MAIN_CREATION,
    MAIN_INIT,
    MAIN_START,
    MAIN_UPDATE,
    MAIN_FINISH,
    MAIN_EXIT
};

int main()
{
    Log::Init();
    LOG_TRACE("Entering application");

	int state = MAIN_CREATION;
	int ret = 0;

	while (state != MAIN_EXIT) {
		switch (state) {
		case MAIN_CREATION:

			LOG_TRACE("Application Creation --------------");
			App = new Application();
			state = MAIN_INIT;
			break;

		case MAIN_INIT:
			LOG_TRACE("Application Initialization --------------");
			if (App->Init() == false) {
				LOG_ERROR("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else {
				state = MAIN_START;
			}

			break;

		case MAIN_START:

			LOG_TRACE("Application Start --------------");
			if (App->Start() == false) {
				LOG_ERROR("Application Start exits with ERROR");
				state = MAIN_EXIT;
			}
			else {
				state = MAIN_UPDATE;
				LOG_TRACE("Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			if (App->MainUpdate() == false) {
				LOG_ERROR("Application Update exits with ERROR");
				state = MAIN_FINISH;
			}
		}
		break;

		case MAIN_FINISH:

			LOG_TRACE("Application Cleaning --------------");
			if (App->CleanUp() == false) {
				LOG_ERROR("Application Cleaning exits with ERROR");
			}
			else
				ret = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}
	}

	delete App;
	return ret;
}




