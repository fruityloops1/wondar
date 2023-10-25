#include "pe/DbgGui/Windows/ActorBrowser.h"
#include "container/seadPtrArray.h"
#include "engine/actor/ActorBase.h"
#include "engine/actor/ActorMgr.h"
#include "engine/actor/IActor.h"
#include "game/actor/component/ItemGetRef.h"
#include "game/actor/component/PlayerControlRef.h"
#include "hook/trampoline.hpp"
#include "imgui.h"
#include "pe/Util/Log.h"
#include "util/modules.hpp"
#include <sead/heap/seadHeap.h>

namespace pe {
namespace gui {

    sead::PtrArray<engine::actor::ActorBase> sActorList;

    HOOK_DEFINE_TRAMPOLINE(ActorBaseCtor) { static void Callback(engine::actor::ActorBase * thisPtr, void* x1); };
    HOOK_DEFINE_TRAMPOLINE(ActorBaseDtor) { static void Callback(engine::actor::ActorBase * thisPtr); };

    void ActorBaseCtor::Callback(engine::actor::ActorBase* thisPtr, void* x1)
    {
        Orig(thisPtr, x1);
        sActorList.pushBack(thisPtr);
    }

    void ActorBaseDtor::Callback(engine::actor::ActorBase* thisPtr)
    {
        Orig(thisPtr);
        sActorList.clear();
    }

    ActorBrowser::ActorBrowser()
    {
        ActorBaseCtor::InstallAtOffset(0x00231204);
        ActorBaseDtor::InstallAtOffset(0x0041dfac);
        sActorList.allocBuffer(1024, nullptr);
    }

    void ActorBrowser::update()
    {
    }

    void ActorBrowser::draw()
    {
        if (getDbgGuiSharedData().showActorBrowser && ImGui::Begin("ActorBrowser", &getDbgGuiSharedData().showActorBrowser)) {

            for (int i = 0; i < sActorList.size(); i++) {
                engine::actor::IActor* actor = sActorList[i];

                char buf[256] { 0 };
                snprintf(buf, 256, "%s (%p)", actor->getActorName().cstr(), actor);

                ImGui::PushID(actor);
                bool expanded = ImGui::TreeNode(buf);
                ImGui::PopID();

                if (expanded) {
                    auto& sharedData = getDbgGuiSharedData();
                    if (ImGui::Button("Goto")) {
                        sharedData.memoryEditorGotoAddr = uintptr_t(actor);
                        sharedData.showMemoryEditor = true;
                    }
                    engine::actor::ActorBase* base = reinterpret_cast<engine::actor::ActorBase*>(actor);
                    ImGui::DragFloat3("Trans", base->mTrans.e.data());
                    ImGui::DragFloat3("Scale", base->mScale.e.data());
                    ImGui::DragFloat3("Velocity1", base->mVelocity1.e.data());
                    ImGui::DragFloat3("Velocity2", base->mVelocity2.e.data());

                    for (int i = 0; i < base->mActorComponents.size(); i++) {
                        engine::component::IActorComponent* component = base->mActorComponents[i];
                        if (component != nullptr) {
                            ImGui::PushID(component);
                            if (ImGui::Button("Goto")) {
                                sharedData.memoryEditorGotoAddr = uintptr_t(component);
                                sharedData.showMemoryEditor = true;
                            }
                            ImGui::PopID();
                            ImGui::SameLine();
                            char buf[128] { 0 };
                            snprintf(buf, 128, "%s (%d)", component->mGyamlPath, i);
                            if (ImGui::CollapsingHeader(buf)) {
                                ImGui::Text("vtable: %p", *reinterpret_cast<uintptr_t*>(component) - exl::util::modules::GetTargetStart() - 16 + 0x7100000000);
                                if (i >= 25 && i <= 28) {
                                    game::actor::component::PlayerControlRef* controlComponent = static_cast<game::actor::component::PlayerControlRef*>(component);
                                    if (ImGui::SliderInt("Item", reinterpret_cast<int*>(&controlComponent->mItemType1), 0, 10)) {
                                        controlComponent->mItemType2 = controlComponent->mItemType1;
                                        game::actor::component::ItemGetRef* itemComponent = static_cast<game::actor::component::ItemGetRef*>(base->mActorComponents[40]);
                                        if (itemComponent) {
                                            itemComponent->mItemType1 = controlComponent->mItemType1;
                                            itemComponent->mItemType2 = controlComponent->mItemType1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    ImGui::TreePop();
                }
            }

            ImGui::End();
        }
    }

} // namespace gui
} // namespace pe
