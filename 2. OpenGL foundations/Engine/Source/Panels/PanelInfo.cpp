#include "pch.h"
#include "PanelInfo.h"

PanelInfo::PanelInfo() : Panel()
{
	this->name = "Info";
	
	glStrInfo = "OpenGL version: " + (char)glGetString(GL_VERSION)
		+ '\n';
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
