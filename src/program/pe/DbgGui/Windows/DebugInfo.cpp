#include "pe/DbgGui/Windows/DebugInfo.h"
#include "imgui.h"
#include "util/sys/mem_layout.hpp"
#include <sead/heap/seadHeap.h>

namespace pe {
namespace gui {

    void DebugInfo::update()
    {
    }

    void DebugInfo::draw()
    {
        if (getDbgGuiSharedData().showDebugInfo && ImGui::Begin("DebugInfo", &getDbgGuiSharedData().showDebugInfo)) {
            if (ImGui::CollapsingHeader("Modules")) {
                auto& rtld = exl::util::GetRtldModuleInfo();
                auto& main = exl::util::GetMainModuleInfo();
                auto& self = exl::util::GetSelfModuleInfo();
                auto& nnsdk = exl::util::GetSdkModuleInfo();

                const struct {
                    const exl::util::ModuleInfo& info;
                    const char name[8];
                } modules[] { { rtld, "rtld" }, { main, "main" }, { self, "self" }, { nnsdk, "nnsdk" } };

                for (auto& module : modules) {
                    ImGui::Text("%s: %p-%p", module.name, module.info.m_Total.m_Start, module.info.m_Total.GetEnd());
                }
            }
            if (ImGui::Button("Abort")) {
                EXL_ABORT(42);
            }
            ImGui::End();
        }
    }

} // namespace gui
} // namespace pe
