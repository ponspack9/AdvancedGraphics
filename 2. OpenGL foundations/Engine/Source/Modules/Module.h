#pragma once

#include <string>

class Module
{
private:
	bool enabled;

protected:
	std::string name = "Module";

public:
	Module(bool enabled = true) : enabled(enabled) {}
	virtual ~Module() {}

	virtual bool Init() { return true; }

	virtual bool Start() { return true; }

	virtual bool Stop() { return true; }

	virtual bool PreUpdate(float dt) { return true; }

	virtual bool Update(float dt) { return true; }

	virtual bool PostUpdate(float dt) { return true; }

	virtual bool CleanUp() { return true; }

	// Setters
	virtual void SetEnabled(bool _enabled) { enabled = _enabled; }

	// Getters
	const char* GetName() const { return name.c_str(); }

	// Queries
	virtual bool IsEnabled() const { return enabled; }


};