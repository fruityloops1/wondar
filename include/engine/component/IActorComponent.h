#pragma once

namespace engine::component {

class IActorComponent {
    virtual void v0();

public:
    const char* mGyamlPath = nullptr;
};

} // namespace engine::component
