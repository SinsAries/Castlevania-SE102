#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include "GameObject.h"

class CSimon;

class ISimonState
{
public:
	virtual void Enter(CSimon* simon) = 0;
	virtual void HandleInput(CSimon* simon, BYTE* states) = 0;
	virtual void Update(CSimon* simon, DWORD dt, vector<LPGAMEOBJECT>* coObjects) = 0;
	virtual void Render(CSimon* simon) = 0;
	virtual ~ISimonState() {};
	virtual const wchar_t* GetStateName() = 0;
};

