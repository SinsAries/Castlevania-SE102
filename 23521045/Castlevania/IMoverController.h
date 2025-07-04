// IMoverController.h
#pragma once
#include "IComponent.h"

class IMoverController : public IComponent
{
public:
    // Hàm Update của Controller sẽ chịu trách nhiệm tính toán vận tốc cho RigidBody.
    // Nó thường được gọi TRƯỚC khi RigidBody::Update được gọi.
    virtual void Update(DWORD dt) = 0;

    // Controller thường không cần render, nên ta cung cấp một hàm Render rỗng.
    virtual void Render() override {}

    virtual ~IMoverController() {}
};