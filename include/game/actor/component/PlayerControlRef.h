#pragma once

#include "engine/component/IActorComponent.h"
#include "game/actor/component/ItemGetRef.h"
#include <sead/basis/seadTypes.h>

namespace game::actor::component {

class PlayerControlRef : public engine::component::IActorComponent {
private:
    u8 _10[0xA4];
    ItemGetType mItemType1, mItemType2;
    u8 _BC[0x1228];

public:
};

} // namespace game::actor::component
