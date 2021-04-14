#pragma once

#ifdef _WIN32
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>

#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <fstream>
#include <streambuf>
#include <assert.h>
#include <math.h>

#include <vector>
#include <string>

#include <stb_image.h>
#include <stb_image_write.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Log.h"