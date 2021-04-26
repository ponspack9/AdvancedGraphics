#pragma once

class Component;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void DrawInspector();

public:

	std::vector<Component*> components;
};

