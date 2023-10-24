#include "pe/DbgGui/Windows/MemoryEditor.h"
#include "heap/seadHeapMgr.h"
#include "imgui.h"
#include "nn/diag.h"
#include "nx/kernel/svc.h"
#include <sead/heap/seadHeap.h>

namespace pe {
namespace gui {

    static ImU8 ReadHandler(const ImU8* data, size_t off)
    {
        MemoryInfo outInfo;
        u32 pageInfo;

        uintptr_t at(uintptr_t(data) + off);
        const ImU8* ptr(reinterpret_cast<const ImU8*>(at));

        svcQueryMemory(&outInfo, &pageInfo, at);
        if (outInfo.perm & Perm_R)
            return *ptr;
        else
            return 0xFC;
    }

    static void WriteHandler(ImU8* data, size_t off, ImU8 d)
    {
        MemoryInfo outInfo;
        u32 pageInfo;

        uintptr_t at(uintptr_t(data) + off);
        ImU8* ptr(reinterpret_cast<ImU8*>(at));

        svcQueryMemory(&outInfo, &pageInfo, at);
        if (outInfo.perm & Perm_W)
            *ptr = d;
    }

    MemoryEditor::MemoryEditor()
    {
        mEditor.ReadFn = ReadHandler;
        mEditor.WriteFn = WriteHandler;
    }

    void MemoryEditor::update()
    {
    }

    void MemoryEditor::draw()
    {
        if (getDbgGuiSharedData().showMemoryEditor && ImGui::Begin("MemoryEditor", &getDbgGuiSharedData().showMemoryEditor)) {
            sead::Heap* rootHeap = sead::HeapMgr::getRootHeap(0);

            uintptr_t& gotoAddr = getDbgGuiSharedData().memoryEditorGotoAddr;
            if (gotoAddr != 0) {
                uintptr_t realAddr = gotoAddr - rootHeap->getStartAddress();
                mEditor.GotoAddrAndHighlight(realAddr, realAddr);

                gotoAddr = 0;
            }

            sead::Heap* currentHeap = sead::HeapMgr::instance()->findContainHeap(reinterpret_cast<const void*>(rootHeap->getStartAddress() + mEditor.DataPreviewAddr));
            if (currentHeap) {
                ImGui::Text("Current Heap: %s", currentHeap->getName().cstr());
            }
            mEditor.DrawContents(reinterpret_cast<void*>(rootHeap->getStartAddress()), rootHeap->getSize(), rootHeap->getStartAddress());
            ImGui::End();
        }
    }

} // namespace gui
} // namespace pe
