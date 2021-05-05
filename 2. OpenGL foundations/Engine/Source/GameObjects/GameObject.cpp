#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
	for (int i = components.size() - 1; i >= 0; --i)
	{
		delete(components[i]);
	}
	components.clear();
}

void GameObject::DrawInspector()
{
	for (Component* component : components)
	{
		component->DrawInspector();
	}
}
