#pragma once

#include "pe/DbgGui/IComponent.h"

namespace pe {
namespace gui {

    class DebugInfo : public IComponent {
    public:
        void update() override;
        void draw() override;
    };

} // namespace gui
} // namespace pe
