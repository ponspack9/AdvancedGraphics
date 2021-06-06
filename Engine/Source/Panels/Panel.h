#pragma once

class Panel 
{
public:
	Panel() {};
	virtual ~Panel() {};

	virtual void Draw() = 0;
	bool ToggleVisibility() { enabled = !enabled; return enabled; };
	bool IsEnabled() { return enabled; };

protected:
	std::string name = "Panel";

private:
	bool enabled = true;
};