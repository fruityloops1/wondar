#pragma once

#include <sead/heap/seadDisposer.h>

namespace engine::actor {
class IActor;

class ActorMgr {
    SEAD_SINGLETON_DISPOSER(ActorMgr);

public:
    virtual ~ActorMgr();

    void addActorToList(IActor* actor);
};

} // namespace engine::actor
