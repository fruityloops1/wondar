#include "ExceptionHandler.h"
#include "framework/seadGameFramework.h"
#include "game/actor/ActorPlacementInfo.h"
#include "heap/seadHeapMgr.h"
#include "hook/replace.hpp"
#include "hook/trampoline.hpp"
#include "imgui.h"
#include "imgui_nvn.h"
#include "lib.hpp"
#include "nn/fs.h"
#include "pe/DbgGui/DbgGui.h"
#include "pe/Hacks/FSHacks.h"
#include "pe/Util/Log.h"
#include "util/modules.hpp"
#include "util/sys/mem_layout.hpp"
#include "util/sys/rw_pages.hpp"
#include <sead/filedevice/seadFileDeviceMgr.h>
#include <sead/framework/seadGameFramework.h>
#include <sead/math/seadVector.h>

static void initHeap()
{
    pe::gui::getPeepaHeap() = sead::ExpHeap::create(1024 * 1024 * 16, "PeepaHeap", sead::HeapMgr::getRootHeap(0), 8, sead::ExpHeap::cHeapDirection_Forward, false);
    pe::initializeLog(pe::gui::getPeepaHeap());
}

static void initDbgGui()
{
    sead::ScopedCurrentHeapSetter heapSetter(pe::gui::getPeepaHeap());
    pe::gui::DbgGui::createInstance(nullptr);
}

HOOK_DEFINE_TRAMPOLINE(CreateFileDeviceMgr) { static void Callback(sead::FileDeviceMgr * thisPtr); };
void CreateFileDeviceMgr::Callback(sead::FileDeviceMgr* thisPtr)
{
    Orig(thisPtr);
    nn::fs::MountSdCardForDebug("sd");
}

HOOK_DEFINE_TRAMPOLINE(GameFrameworkInitialize) { static void Callback(sead::GameFramework * thisPtr, const sead::Framework::InitializeArg& arg); };

void GameFrameworkInitialize::Callback(sead::GameFramework* thisPtr, const sead::Framework::InitializeArg& arg)
{
    Orig(thisPtr, arg);
    initDbgGui();
}

void drawDbgGui()
{
    sead::ScopedCurrentHeapSetter heapSetter(pe::gui::getPeepaHeap());

    auto* dbgGui = pe::gui::DbgGui::instance();
    if (dbgGui)
        dbgGui->draw();
}

HOOK_DEFINE_TRAMPOLINE(InitActorPlacementInfo) { static void Callback(game::actor::ActorPlacementInfo * info, void* byamlIter); };

void InitActorPlacementInfo::Callback(game::actor::ActorPlacementInfo* info, void* byamlIter)
{
    Orig(info, byamlIter);
    pe::log("%s", info->mActorArchiveName);
}

HOOK_DEFINE_TRAMPOLINE(CreateRootHeap) { static void Callback(sead::HeapMgr * thisPtr); };

void CreateRootHeap::Callback(sead::HeapMgr* thisPtr)
{
    Orig(thisPtr);
    initHeap();
}

extern "C" void exl_main(void* x0, void* x1)
{
    exl::hook::Initialize();

    using Patcher = exl::patch::CodePatcher;
    using namespace exl::patch::inst;

    InitActorPlacementInfo::InstallAtOffset(0x0005815c);
    CreateRootHeap::InstallAtOffset(0x005a66f8);
    CreateFileDeviceMgr::InstallAtOffset(0x005a6110);
    GameFrameworkInitialize::InstallAtOffset(0x005a5cfc);

    /*{
        constexpr size_t poolSize = 0xC0000;
        void* pool = malloc(poolSize);
        nn::socket::Initialize(pool, poolSize, 0x4000, 0xe);
    }*/

    nvnImGui::InstallHooks();
    nvnImGui::addDrawFunc(drawDbgGui);

    pe::installFSHacks();

    exl::util::RwPages a(exl::util::GetSdkModuleInfo().m_Total.m_Start + 0x00399790, 4);
    *reinterpret_cast<u32*>(a.GetRw()) = 0xD65F03C0; // just dont crash, idiot
}

extern "C" NORETURN void exl_exception_entry()
{
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}
