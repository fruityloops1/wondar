#include "thread/seadThread.h"
#include <sead/heap/seadHeapMgr.h>

namespace sead {

sead::Heap* HeapMgr::setCurrentHeap_(sead::Heap* heap)
{
    uintptr_t storage = sead::ThreadMgr::instance()->mThreadPtrTLS.getValue();
    sead::Heap*& currentHeap = *reinterpret_cast<sead::Heap**>(storage + 0x88);
    sead::Heap* prev = currentHeap;
    currentHeap = heap;
    return prev;
}

} // namespace sead
