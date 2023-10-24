#include "pe/DbgGui/MenuBar.h"
#include "imgui.h"
#include "lib/util/modules.hpp"

namespace pe {
namespace gui {

    void MenuBar::update()
    {
    }

    void MenuBar::draw()
    {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Windows")) {
                ImGui::Checkbox("Demo Window", &getDbgGuiSharedData().showDemoWindow);
                ImGui::Checkbox("Log", &getDbgGuiSharedData().showLog);
                ImGui::Checkbox("HeapViewer", &getDbgGuiSharedData().showHeapViewer);
                ImGui::Checkbox("ActorBrowser", &getDbgGuiSharedData().showActorBrowser);
                ImGui::Checkbox("Camera", &getDbgGuiSharedData().showCamera);
                ImGui::Checkbox("MemoryEditor", &getDbgGuiSharedData().showMemoryEditor);
                ImGui::Checkbox("DebugInfo", &getDbgGuiSharedData().showDebugInfo);
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

} // namespace gui
} // namespace pe
