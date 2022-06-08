#include "ViewProjection.h"

#pragma region Move Functions

void ViewProjection::moveEyeXPosition(const float& _x)
{
	m_Eye.x += _x;

	(*m_View) = glm::lookAt
	(
		glm::vec3(m_Eye.x, m_Eye.y, m_Eye.z),
		glm::vec3(m_Centre.x, m_Centre.y, m_Centre.z),
		glm::vec3(m_Up.x, m_Up.y, m_Up.z)
	);
}

void ViewProjection::moveEyeYPosition(const float& _y)
{
	m_Eye.y += _y;

	(*m_View) = glm::lookAt
	(
		glm::vec3(m_Eye.x, m_Eye.y, m_Eye.z),
		glm::vec3(m_Centre.x, m_Centre.y, m_Centre.z),
		glm::vec3(m_Up.x, m_Up.y, m_Up.z)
	);
}

void ViewProjection::moveEyeZPosition(const float& _z)
{
	m_Eye.z += _z;

	(*m_View) = glm::lookAt
	(
		glm::vec3(m_Eye.x, m_Eye.y, m_Eye.z),
		glm::vec3(m_Centre.x, m_Centre.y, m_Centre.z),
		glm::vec3(m_Up.x, m_Up.y, m_Up.z)
	);
}

void ViewProjection::moveCentreXPosition(const float& _x)
{
	m_Centre.x += _x;

	(*m_View) = glm::lookAt
	(
		glm::vec3(m_Eye.x, m_Eye.y, m_Eye.z),
		glm::vec3(m_Centre.x, m_Centre.y, m_Centre.z),
		glm::vec3(m_Up.x, m_Up.y, m_Up.z)
	);
}

void ViewProjection::moveCentreYPosition(const float& _y)
{
	m_Centre.y += _y;

	(*m_View) = glm::lookAt
	(
		glm::vec3(m_Eye.x, m_Eye.y, m_Eye.z),
		glm::vec3(m_Centre.x, m_Centre.y, m_Centre.z),
		glm::vec3(m_Up.x, m_Up.y, m_Up.z)
	);
}

void ViewProjection::moveCentreZPosition(const float& _z)
{
	m_Centre.z += _z;

	(*m_View) = glm::lookAt
	(
		glm::vec3(m_Eye.x, m_Eye.y, m_Eye.z),
		glm::vec3(m_Centre.x, m_Centre.y, m_Centre.z),
		glm::vec3(m_Up.x, m_Up.y, m_Up.z)
	);
}

void ViewProjection::moveUpXPosition(const float& _x)
{
	m_Up.x += _x;

	(*m_View) = glm::lookAt
	(
		glm::vec3(m_Eye.x, m_Eye.y, m_Eye.z),
		glm::vec3(m_Centre.x, m_Centre.y, m_Centre.z),
		glm::vec3(m_Up.x, m_Up.y, m_Up.z)
	);
}

void ViewProjection::moveUpYPosition(const float& _y)
{
	m_Up.y += _y;

	(*m_View) = glm::lookAt
	(
		glm::vec3(m_Eye.x, m_Eye.y, m_Eye.z),
		glm::vec3(m_Centre.x, m_Centre.y, m_Centre.z),
		glm::vec3(m_Up.x, m_Up.y, m_Up.z)
	);
}

void ViewProjection::moveUpZPosition(const float& _z)
{
	m_Up.z += _z;

	(*m_View) = glm::lookAt
	(
		glm::vec3(m_Eye.x, m_Eye.y, m_Eye.z),
		glm::vec3(m_Centre.x, m_Centre.y, m_Centre.z),
		glm::vec3(m_Up.x, m_Up.y, m_Up.z)
	);
}

#pragma endregion

#pragma region Constructors

ViewProjection::ViewProjection()
{
}

ViewProjection::ViewProjection(glm::mat4* _projection, glm::mat4* _view)
	: m_Projection(_projection), m_View(_view)
{
}

ViewProjection::~ViewProjection()
{
}

#pragma endregion

#pragma region Management Functions

int ViewProjection::updateViewProjection(const char& _userInput)
{
	switch (_userInput)
	{
	case 't':
		moveEyeXPosition(0.1f);
		DN_CORE_INFO_OUTPUT("View port: Eye X postion moved");
		return 1;

	case 'y':
		moveEyeYPosition(0.1f);
		DN_CORE_INFO_OUTPUT("View port: Eye Y postion moved");
		return 1;

	case 'u':
		moveEyeZPosition(0.1f);
		DN_CORE_INFO_OUTPUT("View port: Eye Z postion moved");
		return 1;

	case 'g':
		moveCentreXPosition(0.1f);
		DN_CORE_INFO_OUTPUT("View port: Centre X postion moved");
		return 1;

	case 'h':
		moveCentreYPosition(0.1f);
		DN_CORE_INFO_OUTPUT("View port: Centre Y postion moved");
		return 1;

	case 'j':
		moveCentreZPosition(0.1f);
		DN_CORE_INFO_OUTPUT("View port: Centre Z postion moved");
		return 1;

	case 'b':
		moveUpXPosition(0.1f);
		DN_CORE_INFO_OUTPUT("View port: Up X postion moved");
		return 1;

	case 'n':
		moveUpYPosition(0.1f);
		DN_CORE_INFO_OUTPUT("View port: Up Y postion moved");
		return 1;

	case 'm':
		moveUpZPosition(0.1f);
		DN_CORE_INFO_OUTPUT("View port: Up Z postion moved");
		return 1;

	case 'T':
		moveEyeXPosition(-0.1f);
		DN_CORE_INFO_OUTPUT("View port: Eye X postion moved");
		return 1;

	case 'Y':
		moveEyeYPosition(-0.1f);
		DN_CORE_INFO_OUTPUT("View port: Eye Y postion moved");
		return 1;

	case 'U':
		moveEyeZPosition(-0.1f);
		DN_CORE_INFO_OUTPUT("View port: Eye Z postion moved");
		return 1;

	case 'G':
		moveCentreXPosition(-0.1f);
		DN_CORE_INFO_OUTPUT("View port: Centre X postion moved");
		return 1;

	case 'H':
		moveCentreYPosition(-0.1f);
		DN_CORE_INFO_OUTPUT("View port: Centre Y postion moved");
		return 1;

	case 'J':
		moveCentreZPosition(-0.1f);
		DN_CORE_INFO_OUTPUT("View port: Centre Z postion moved");
		return 1;

	case 'B':
		moveUpXPosition(-0.1f);
		DN_CORE_INFO_OUTPUT("View port: Up X postion moved");
		return 1;

	case 'N':
		moveUpYPosition(-0.1f);
		DN_CORE_INFO_OUTPUT("View port: Up Y postion moved");
		return 1;

	case 'M':
		moveUpZPosition(-0.1f);
		DN_CORE_INFO_OUTPUT("View port: Up Z postion moved");
		return 1;

	default:
		DN_CORE_INFO_OUTPUT("View port: No movement update");
		break;
	}

	return 0;
}

#pragma endregion
