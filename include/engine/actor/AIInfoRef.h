#pragma once

#include "engine/component/IActorComponent.h"
#include <sead/basis/seadTypes.h>

namespace engine::actor {

class ActorBase;

class AIInfoRef : public component::IActorComponent {
private:
    u8 _10[0x20];
    ActorBase* mParent = nullptr;

public:
};

} // namespace engine::actor
