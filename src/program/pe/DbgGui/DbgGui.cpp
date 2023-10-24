#include "pe/DbgGui/DbgGui.h"
#include "helpers/InputHelper.h"
#include "imgui.h"
#include "pe/DbgGui/MenuBar.h"
#include "pe/DbgGui/Windows/ActorBrowser.h"
#include "pe/DbgGui/Windows/DebugInfo.h"
#include "pe/DbgGui/Windows/HeapViewer.h"
#include "pe/DbgGui/Windows/ImGuiDemo.h"
#include "pe/DbgGui/Windows/Log.h"
#include "pe/DbgGui/Windows/MemoryEditor.h"
#include "program/ExceptionHandler.h"
#include "program/imgui_nvn.h"
#include "util/modules.hpp"

namespace pe {
namespace gui {

    SEAD_SINGLETON_DISPOSER_IMPL(DbgGui)

    sead::ExpHeap*& getPeepaHeap()
    {
        static sead::ExpHeap* heap { nullptr };
        return heap;
    }

    static DbgGui::SharedData* sSharedDataPtrForExceptionHandler = nullptr;

    DbgGui::DbgGui()
    {
        sSharedDataPtrForExceptionHandler = &mSharedData;
        mComponents.allocBuffer(16, nullptr);
        mComponents.pushBack(new MenuBar);
        mComponents.pushBack(new ImGuiDemo);
        mComponents.pushBack(new Log);
        mComponents.pushBack(new HeapViewer);
        mComponents.pushBack(new ActorBrowser);
        mComponents.pushBack(new MemoryEditor);
        mComponents.pushBack(new DebugInfo);

        handler::installExceptionHandler([](handler::ExceptionInfo& info) {
            info.pc += 4;
            handler::printCrashReport(info);

            sSharedDataPtrForExceptionHandler->showLog = true;
            sSharedDataPtrForExceptionHandler->hideGui = false;

            return true;
        });
    }

    void DbgGui::draw()
    {
        if (InputHelper::isButtonHold(nn::hid::NpadButton::ZR) && InputHelper::isButtonPress(nn::hid::NpadButton::Plus))
            mSharedData.hideGui = !mSharedData.hideGui;
        ImGui::GetIO().MouseDrawCursor = !mSharedData.hideGui;
        if (mSharedData.hideGui)
            return;

        for (IComponent& c : mComponents)
            c.draw();
    }

    void DbgGui::update()
    {
        for (IComponent& c : mComponents)
            c.update();
    }

} // namespace gui
} // namespace pe
