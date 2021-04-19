//
// platform.cpp : This file contains the 'main' function. Program execution begins and ends there.
// The platform layer is in charge to create the environment necessary so the engine disposes of what
// it needs in order to create the application (e.g. window, graphics context, I/O, allocators, etc).
//


#include "pch.h"
#include "engine.h"
#include "Application.h"
#include "Modules/ModuleWindow.h"


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
			App->deltaTime = 1.0f / 60.0f;
			state = MAIN_INIT;
			break;

		case MAIN_INIT:
			LOG_TRACE("Application Initialization --------------");
			if (App->Init() == false) {
				LOG_ERROR("Application Init exits with ERROR");
				state = MAIN_EXIT;
			}
			else {
				Init();
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




std::string GetDirectoryPart(std::string path)
{
    size_t pos = path.rfind('/');
    if (pos == std::string::npos)
        pos = path.rfind('\\');

    if (pos != std::string::npos)
        return path.substr(0, path.length() - pos);

    LOG_ERROR("Could not get directory part from {0}", path);
    return path; // not found
}


std::string ReadTextFile(const char* filepath)
{
    std::ifstream ifs(filepath);

    if (ifs)
    {
        std::string fileText((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        return  fileText;
    }

    LOG_ERROR("fopen() failed reading file {0}", filepath);
    return "ERROR";
}

u64 GetFileLastWriteTimestamp(const char* filepath)
{
#ifdef _WIN32
    union Filetime2u64 {
        FILETIME filetime;
        u64      u64time;
    } conversor;

    WIN32_FILE_ATTRIBUTE_DATA Data;
    if (GetFileAttributesExA(filepath, GetFileExInfoStandard, &Data)) {
        conversor.filetime = Data.ftLastWriteTime;
        return(conversor.u64time);
    }
#else
    // NOTE: This has not been tested in unix-like systems
    struct stat attrib;
    if (stat(filepath, &attrib) == 0) {
        return attrib.st_mtime;
    }
#endif

    return 0;
}

void LogString(const char* str)
{
#ifdef _WIN32
    OutputDebugStringA(str);
    OutputDebugStringA("\n");
#else
    fprintf(stderr, "%s\n", str);
#endif
}
