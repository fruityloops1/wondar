#pragma once

#include <sead/container/seadPtrArray.h>
#include <sead/heap/seadDisposer.h>
#include <sead/heap/seadExpHeap.h>

namespace pe {
namespace gui {

    class IComponent;
    class ExceptionPopup;

    sead::ExpHeap*& getPeepaHeap();

    class DbgGui {
        SEAD_SINGLETON_DISPOSER(DbgGui);

        sead::PtrArray<IComponent> mComponents;

    public:
        struct SharedData {
            bool hideGui = true;
            bool showDemoWindow = false;
            bool showLog = true;
            bool showHeapViewer = false;
            bool showActorBrowser = false;
            bool showCamera = false;
            bool showMemoryEditor = false;
            bool showDebugInfo = false;

            uintptr_t memoryEditorGotoAddr = 0;
        };
        SharedData mSharedData;

        DbgGui();

        void update();
        void draw();

        auto& getSharedData() { return mSharedData; }

        friend class IComponent;
    };

} // namespace gui
} // namespace pe
