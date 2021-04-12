#pragma once
#include "Panel.h"

class PanelInfo : public Panel
{
public:
	PanelInfo();
	~PanelInfo();

	void Draw() override;

	std::string glStrInfo;
};

