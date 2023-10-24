#pragma once

#include "container/seadBuffer.h"
#include "engine/actor/IActor.h"
#include "engine/component/IActorComponent.h"
#include <sead/container/seadBuffer.h>
#include <sead/math/seadMatrix.h>
#include <sead/math/seadVector.h>

namespace engine::actor {

class ActorBase : public IActor {
private:
    sead::SafeString mActorArchiveName;
    sead::Buffer<component::IActorComponent*> mActorComponents;
    u8 _210[0x84];

public:
    sead::Vector3f mTrans;
    sead::Matrix33f _2A0;
    sead::Vector3f mScale;
    u8 _2D0[0x30];
    sead::Vector3f mVelocity1;
    sead::Vector3f _30C;
    sead::Vector3f mVelocity2;
    u8 _324[0xdc];
    nn::os::MutexType _400;
    u8 _420[0x18];
    nn::os::MutexType _438;
};

} // namespace engine::actor
