#pragma once
#include <string>

class Panel 
{
public:
	Panel();
	virtual ~Panel() = 0;

	virtual void Draw() = 0;
	bool ToggleVisibility() { enabled = !enabled; return enabled; };

protected:
	std::string name = "Panel";

private:
	bool enabled = true;
};