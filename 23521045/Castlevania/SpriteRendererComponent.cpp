// SpriteRendererComponent.cpp

#include "SpriteRendererComponent.h"
#include "GameObject.h"
#include "debug.h"

void SpriteRendererComponent::Render()
{
    if (!this->isVisible || owner == nullptr)
    {
        return;
    }
    float x, y;
    owner->GetPosition(x, y);
    int nx = owner->getNx();

    if (this->animation != nullptr)
    {
        this->animation->Render(x, y);
    }
}

void SpriteRendererComponent::Update(DWORD dt)
{
}