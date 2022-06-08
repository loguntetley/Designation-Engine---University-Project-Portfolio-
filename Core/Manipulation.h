#pragma once
#include "PCH.h"

#include "Renderer.h"

class Manipulation
{
private:
	Renderer* m_Renderer;

public:
	Manipulation();
	Manipulation(Renderer* _renderer);
	~Manipulation();

	void Instanciate();
	void Update();

};