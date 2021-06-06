#include "pch.h"
#include "PanelInfo.h"

PanelInfo::PanelInfo() : Panel()
{
	this->name = "Info";
	
	GLint numExtensions;
	glGetIntegerv(GL_CONTEXT_FLAGS, &numExtensions);

	std::string context = "";
	switch (numExtensions)
	{
	case GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT:
		context = "GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT";
		break;
	case GL_CONTEXT_FLAG_DEBUG_BIT:
		context = "GL_CONTEXT_FLAG_DEBUG_BIT";
		break;
	default:
		context = "UNDEFINED";
		break;
	}

	glStrInfo = "OpenGL version: " + std::string((const char*)glGetString(GL_VERSION)) + '\n'
		+ "OpenGL renderer: " + std::string((const char*)glGetString(GL_RENDERER)) + '\n'
		+ "OpenGL vendor: " + std::string((const char*)glGetString(GL_VENDOR)) + '\n'
		+ "OpenGL GLSL version: " + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)) + '\n'
		+ "OpenGL Context: " + context + '\n' + '\n'
		+ "Extensions ---------" + '\n';

		// Extensions
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	for (int i = 0; i < numExtensions; i++)
	{
		glStrInfo += std::string((const char*)glGetStringi(GL_EXTENSIONS, GLuint(i))) + '\n';
	}
}

PanelInfo::~PanelInfo()
{
}

void PanelInfo::Draw()
{
	ImGui::Begin(name.c_str());

	ImGui::Text(glStrInfo.c_str());

	ImGui::End();
}
