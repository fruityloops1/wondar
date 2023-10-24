#pragma once

#include <sead/math/seadVector.h>

namespace game::actor {

struct ActorPlacementInfo {
    sead::Vector3f mTrans;
    sead::Vector3f mRotate;
    sead::Vector3f mScale;
    const char* mActorArchiveName;
    const char* mName;
    uint64_t mHash;
    uint64_t mSRTHash;
    uint64_t mGroupGUID;
    void* mDynamicParam[2];
    void* mPlacementIter[2];
    u32 _70;
    bool mIsWorkDirectory;
};

} // namespace game::actor
