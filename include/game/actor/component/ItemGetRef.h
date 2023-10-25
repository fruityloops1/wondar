#pragma once

#include "engine/component/IActorComponent.h"
#include <sead/basis/seadTypes.h>

namespace game::actor::component {

enum class ItemGetType {
    None,
    Super,
    Elephant = 3,
    Drill = 6,
    Bubble = 9
};

class ItemGetRef : public engine::component::IActorComponent {
private:
    void* _10;
    void* _18;
    ItemGetType mItemType1, mItemType2;
    u32 _28;
    u8 _2C[0x7C];
    u64 _A8;
    u32 _B0;

public:
};

} // namespace game::actor::component
