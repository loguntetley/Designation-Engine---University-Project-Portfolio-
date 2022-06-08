#include "Manipulation.h"

Manipulation::Manipulation()
{
}

Manipulation::Manipulation(Renderer* _renderer)
	: m_Renderer(_renderer)
{
}

Manipulation::~Manipulation()
{
}

//User call that only get called once//
void Manipulation::Instanciate()
{
	int helicopter = m_Renderer->CreateMeshModel("VulkanPrototype/Models/Seahawk.obj");
	int cat = m_Renderer->CreateMeshModel("VulkanPrototype/Models/12221_Cat_v1_l3.obj");
	//int interior = m_Renderer->CreateMeshModel("VulkanPrototype/Models/Cottage_FREE.obj");

	glm::mat4 helicopterModel(1.0f);
	helicopterModel = glm::translate(helicopterModel, glm::vec3(-100.0f, -20.0f, -10.0f));
	helicopterModel = glm::rotate(helicopterModel, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	m_Renderer->UpdateModel(helicopter, helicopterModel);

	glm::mat4 catModel(1.0f);
	catModel = glm::translate(catModel, glm::vec3(-45.0f, -7.5f, 23.0f));
	catModel = glm::rotate(catModel, 90.0f, glm::vec3(-1.0f, 1.0f, 0.75f));
	catModel = glm::scale(catModel, glm::vec3(0.5f));
	m_Renderer->UpdateModel(cat, catModel);

	/*glm::mat4 interiorModel(1.0f);
	interiorModel = glm::translate(interiorModel, glm::vec3(0.0f, 0.0f, 2.0f));
	m_Renderer->UpdateModel(interior, interiorModel);*/
}

//User call that Require updating//
void Manipulation::Update()
{




}


