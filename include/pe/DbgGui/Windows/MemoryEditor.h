#pragma once

#include "imgui.h"
#include "imgui_memory_editor.h"
#include "pe/DbgGui/IComponent.h"

namespace pe {
namespace gui {

    class MemoryEditor : public IComponent {
        ::MemoryEditor mEditor;
        u8 balls[0x100];

    public:
        MemoryEditor();

        void update() override;
        void draw() override;
    };

} // namespace gui
} // namespace pe
