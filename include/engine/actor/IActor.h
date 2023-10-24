#pragma once

#include <sead/heap/seadHeap.h>
#include <sead/prim/seadRuntimeTypeInfo.h>
#include <sead/prim/seadSafeString.h>

namespace engine::actor {

class IActor {
    SEAD_RTTI_BASE(IActor)

    sead::Heap* mActorHeap = nullptr;
    u8 _10[0x1e4];

public:
    virtual void field_10();
    virtual void* getComponentPropMap();
    virtual const sead::SafeString& getActorName();
    virtual const sead::SafeString& getActorName2();
    virtual const char* getHostIOIcon();
    virtual ~IActor();
};

} // namespace engine::actor
