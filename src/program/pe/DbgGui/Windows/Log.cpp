#include "pe/DbgGui/Windows/Log.h"
#include "heap/seadHeapMgr.h"
#include "imgui.h"
#include "pe/Util/Log.h"
#include <sead/heap/seadHeap.h>

namespace pe {
namespace gui {

    void Log::update()
    {
    }

    void Log::draw()
    {
        if (getDbgGuiSharedData().showLog && ImGui::Begin("Log", &getDbgGuiSharedData().showLog)) {
            ImGui::InputText("Filter", mFilter, sizeof(mFilter));
            ImGui::SetNextWindowSizeConstraints(ImVec2(400, 150), ImVec2(FLT_MAX, FLT_MAX));
            if (ImGui::BeginChild("logthing", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
                if (shouldLogWindowScrollDown()) {
                    mScrollRefreshCounter = 10;
                    shouldLogWindowScrollDown() = false;
                }

                if (mScrollRefreshCounter > 0) {
                    mScrollRefreshCounter--;
                    ImGui::SetScrollY(ImGui::GetScrollMaxY());
                }
                for (LogType& type : pe::getLogLines()) {
                    const char* msg = reinterpret_cast<const char*>(&type) + 1;

                    if (*mFilter && !strstr(msg, mFilter))
                        continue;

                    switch (type) {
                    case LogType::Log:
                        ImGui::Text("%s", msg);
                        break;
                    case LogType::Warning:
                        ImGui::TextColored(ImVec4(0.8, 0.19, 0.76, 1.0), "%s", msg);
                        break;
                    case LogType::Error:
                        ImGui::TextColored(ImVec4(0.8, 0.19, 0.19, 1.0), "%s", msg);
                        break;
                    }
                }
                ImGui::EndChild();
            }
            ImGui::End();
        }
    }

} // namespace gui
} // namespace pe
