#pragma once
#include "PCH.h"
using namespace std::literals;

class ViewProjection
{
private:

	struct Eye
	{
		float x = 1.0f, y = 0.5f, z = 0.0f;
	} m_Eye;

	struct Centre
	{
		float x = 0.0f, y = 0.0f, z = 0.0f;
	} m_Centre;

	struct Up
	{
		float x = 0.0f, y = 1.0f, z = 0.0f;
	} m_Up;

	glm::mat4* m_Projection, *m_View;

	//MOVE FUNCTIONS//
	void moveEyeXPosition(const float& _x);
	void moveEyeYPosition(const float& _y);
	void moveEyeZPosition(const float& _z);

	void moveCentreXPosition(const float& _x);
	void moveCentreYPosition(const float& _y);
	void moveCentreZPosition(const float& _z);

	void moveUpXPosition(const float& _x);
	void moveUpYPosition(const float& _y);
	void moveUpZPosition(const float& _z);

public:

	//CONSTRUCTORS//
	ViewProjection();
	ViewProjection(glm::mat4* _projection, glm::mat4* _view);
	~ViewProjection();

	//MANAGMENT FUNCTIONS//
	int updateViewProjection(const char& _userInput);


};

