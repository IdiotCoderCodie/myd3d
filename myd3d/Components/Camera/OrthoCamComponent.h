#pragma once
#include "CameraComponent.h"

class OrthoCamComponent : public CameraComponent
{
public:
	OrthoCamComponent();
	OrthoCamComponent(float left, float right, float bottom, float top);
	~OrthoCamComponent();

	void ComponentID(componentId_t& out) const;

private:

};

