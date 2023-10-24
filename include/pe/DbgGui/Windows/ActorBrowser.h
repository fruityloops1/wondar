#pragma once

#include "pe/DbgGui/IComponent.h"

namespace pe {
namespace gui {

    class ActorBrowser : public IComponent {
    public:
        ActorBrowser();
        void update() override;
        void draw() override;
    };

} // namespace gui
} // namespace pe
