#include "pe/DbgGui/Windows/HeapViewer.h"
#include "heap/seadHeapMgr.h"
#include "imgui.h"
#include "pe/Util/Log.h"
#include <cstdio>
#include <sead/heap/seadHeap.h>

namespace pe {
namespace gui {

    static float toMegabytes(size_t bytes) { return float(bytes) / (1024 * 1024); }
    static float toKilobytes(size_t bytes) { return float(bytes) / 1024; }

    void HeapViewer::update()
    {
    }

    static void showHeap(sead::Heap* heap, DbgGui::SharedData& sharedData)
    {
        if (heap == nullptr)
            return;

        if (ImGui::Button("Goto")) {
            sharedData.memoryEditorGotoAddr = heap->getStartAddress();
            sharedData.showMemoryEditor = true;

            pe::log("Going to %p", heap->getStartAddress());
        }
        ImGui::SameLine();

        ImGui::PushID(heap);
        bool expanded = ImGui::TreeNode(heap->getName().cstr());
        ImGui::PopID();

        float mbUsed = toMegabytes(heap->getSize() - heap->getFreeSize());
        float mbSize = toMegabytes(heap->getSize());
        float kbUsed = toKilobytes(heap->getSize() - heap->getFreeSize());
        float kbSize = toKilobytes(heap->getSize());
        float percentUsed = (heap->getSize() - heap->getFreeSize()) / (float(heap->getSize()) / 100);

        bool useKb = mbSize < 1;
        const char* const unit = useKb ? "KB" : "MB";
        char buf[64];
        snprintf(buf, sizeof(buf), "%.2f%s/%.2f%s (%.2f%%) (%p-%p)", useKb ? kbUsed : mbUsed, unit, useKb ? kbSize : mbSize, unit, percentUsed, heap->getStartAddress(), heap->getEndAddress());
        ImGui::SameLine();
        ImGui::ProgressBar(percentUsed / 100, ImVec2(-FLT_MIN, 0), buf);

        if (expanded) {
            for (sead::Heap& childRef : *reinterpret_cast<sead::Heap::HeapList*>(uintptr_t(heap) + 0x40)) {
                sead::Heap* child = &childRef;
                if (child)
                    showHeap(child, sharedData);
            }
            ImGui::TreePop();
        }
    }

    void HeapViewer::draw()
    {
        if (getDbgGuiSharedData().showHeapViewer && ImGui::Begin("HeapViewer", &getDbgGuiSharedData().showHeapViewer)) {
            showHeap(sead::HeapMgr::instance()->getRootHeap(0), getDbgGuiSharedData());
            ImGui::End();
        }
    }

} // namespace gui
} // namespace pe
