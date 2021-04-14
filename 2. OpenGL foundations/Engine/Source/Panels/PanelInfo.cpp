#include "pch.h"
#include "PanelInfo.h"

PanelInfo::PanelInfo() : Panel()
{
	this->name = "Info";
	
	glStrInfo = "OpenGL version: " + std::string((const char*)glGetString(GL_VERSION)) + '\n'
		+ "OpenGL renderer: " + std::string((const char*)glGetString(GL_RENDERER)) + '\n'
		+ "OpenGL vendor: " + std::string((const char*)glGetString(GL_VENDOR)) + '\n'
		+ "OpenGL GLSL version: " + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)) + '\n'
		+ '\n' + "Extensions ---------" + '\n';
		// Extensions
	GLint numExtensions;
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
